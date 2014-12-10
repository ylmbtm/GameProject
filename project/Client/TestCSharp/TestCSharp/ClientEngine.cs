using System; 
using System.Collections.Generic;
using System.Net.Sockets;
using System.Net;
using System.Diagnostics;

public enum ConnectState
{
    Not_Connect,
    Start_Connect,
    Raw_Connect,
    Succ_Connect
};


public abstract class MessageHandler 
{
    public abstract Boolean OnCommandHandle(Command_ID wCommandID, UInt64 u64ConnID, ReadBufferHelper ReadHelper);
} 


public class ClientConnector
{
    UInt32              m_dwServerTime;
    UInt64				m_u64ClientID;    //客户端ID
    UInt32              m_dwIdentifyCode; //登录识别码

    // Socket客户端对象
    private Socket m_ClientSocket = null;
    public ConnectState m_ConnectState;

    public Int32 mTimeOut = 500;
    private int PacketHeaderSize = 18;


    public const int BUFFER_MAX = 8096;
    private int m_RecvLen = 0;
    private int m_DataLen = 0;

    private byte[] m_RecvBuffer = new byte[BUFFER_MAX];
    private byte[] m_DataBuffer = new byte[BUFFER_MAX];


    private ReadBufferHelper m_ReadHelper = new ReadBufferHelper();

    public WriteBufferHelper m_WriteHelper = new WriteBufferHelper();

    private IList<MessageHandler> m_MsghandlerList = new List<MessageHandler>();

    public ClientConnector()
    {
        
    }

    public Boolean InitConnector() { return true; }

    public Boolean CloseConnector() { return true; }

    public Boolean SetClientID(UInt64 u64ClientID) 
    {
        m_u64ClientID = u64ClientID; //一般是角色ID

        m_WriteHelper.m_u64ClientID = u64ClientID;

        return true; 
    }

    public Boolean ConnectToServer(string strIpAddr, UInt16 sPort)
    {
        IPAddress ipAddress = IPAddress.Parse(strIpAddr);

        IPEndPoint ipEndpoint = new IPEndPoint(ipAddress, sPort);

        m_ClientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        m_ClientSocket.SendTimeout = mTimeOut;

        try
        {
            // 这是一个异步的建立连接，当连接建立成功时调用connectCallback方法
            m_ClientSocket.BeginConnect(ipEndpoint, new AsyncCallback(ConnectCallBack), m_ClientSocket);
        }
        catch (System.Exception ex)
        {
            Debug.Print("Connect Exception: " + ex.Message);
        }

        SetConnectState(ConnectState.Start_Connect);

        return true;
    }

    public Boolean DisConnect()
    {
        try
        {
            if (m_ClientSocket != null)
            {
                m_ClientSocket.Shutdown(SocketShutdown.Both);
                m_ClientSocket.Disconnect(true);
                m_ClientSocket.Close(10);
                m_ClientSocket = null;
            }
        }
        catch (System.Exception ex)
        {
            Debug.Print("SocketObject Closed Exception: " + ex.Message);
        }

        finally
        {
            SetConnectState(ConnectState.Not_Connect);
        }

        return true;
    }

    public Boolean SendData(byte[] pData, int dwLen)
    {
        if ((m_ClientSocket == null) || (!m_ClientSocket.Connected))
        {
            return false;
        }

        try
        {
            m_ClientSocket.Send(pData, dwLen, SocketFlags.None);
        }
        catch (System.Exception ex)
        {
            Debug.Print("SocketObject SendMessage Exception: " + ex.Message);
        }

        return true;
    }

    public Boolean Login(string strAccountName, string strPassword)
    {
        m_WriteHelper.BeginWrite((UInt16)Command_ID.CMD_CHAR_LOGIN_REQ, 0);

        m_WriteHelper.WriteFixString(strAccountName, 32);

        m_WriteHelper.WriteFixString(strPassword, 32);

        m_WriteHelper.EndWrite();

        SendData(m_WriteHelper.GetData(), (int)m_WriteHelper.GetDataLen());

        return true;
    }

    public Boolean RegisterMsgHandler(MessageHandler MsgHandler) 
    {
        if (m_MsghandlerList.Contains(MsgHandler))
        {
            return false;
        }

        m_MsghandlerList.Add(MsgHandler);

        return true; 
    }

    public Boolean UnregisterMsgHandler(MessageHandler MsgHandler) 
    {
        if (m_MsghandlerList.Contains(MsgHandler))
        {
            m_MsghandlerList.Remove(MsgHandler);

            return true;
        }

        return false; 
    }

    public Boolean Render()
    {
        if (m_ClientSocket == null || !m_ClientSocket.Connected)
        {
            return false;
        }

        if (m_ConnectState == ConnectState.Not_Connect)
        {
            return false;
        }

        if (m_ConnectState == ConnectState.Start_Connect)
        {
            return false;
        }

        if (ReceiveData())
        {
            ProcessData();
        }

        return true;
    }

    UInt32 GetServerTime()
    {
        return m_dwServerTime;
    }

	//以下是内部的消息处理
    public Boolean OnCommandHandle(Command_ID wCommandID, UInt64 u64ConnID, ReadBufferHelper ReadHelper) 
    {
        Boolean bHandled = false;

        switch (wCommandID)
        {
            case Command_ID.CMD_CONNECT_NOTIFY:
                {
                    OnCmdConnectNotify(wCommandID, u64ConnID, ReadHelper);

                    bHandled = true;
                }
                break;
            case Command_ID.CMD_CHAR_PICK_CHAR_ACK:
                {
                    OnCmdPickCharAck(wCommandID, u64ConnID, ReadHelper);

                    bHandled = true;
                }
                break;

            default:
                {
                    foreach (MessageHandler h in m_MsghandlerList)
                    {
                        if (h.OnCommandHandle(wCommandID, u64ConnID, ReadHelper))
                        {
                            bHandled = true;
                            break;
                        }
                    }

                }
                break;
        }
       
        if (!bHandled)
        {
            Debug.Print("Message %d is not by handled!", wCommandID);
        }

        return true; 
    }

    public Boolean OnCmdConnectNotify(Command_ID wCommandID, UInt64 u64ConnID, ReadBufferHelper ReadHelper)
    {
        StConnectNotify ConnectNotify = new StConnectNotify();
        
        ConnectNotify.Read(ReadHelper);

	    Byte ConType = ConnectNotify.btConType;

	    ConnectNotify.btConType =  (Byte)ConnectionType.TYPE_CLT_PLAYER;

	    ConnectNotify.u64ConnID = m_u64ClientID;

	    m_WriteHelper.BeginWrite((UInt16)Command_ID.CMD_CONNECT_NOTIFY, (Byte)CmdHandler_ID.CMDH_SVR_CON);

        ConnectNotify.Write(m_WriteHelper);

        m_WriteHelper.EndWrite();

        SendData(m_WriteHelper.GetData(), (int)m_WriteHelper.GetDataLen());

        if (ConType == (Byte)ConnectionType.TYPE_SVR_PROXY)
	    {
            StCharEnterGameReq CharEnterGameReq = new StCharEnterGameReq();

		    CharEnterGameReq.u64CharID = m_u64ClientID;
            CharEnterGameReq.dwIdentifyCode = m_dwIdentifyCode;

            m_WriteHelper.BeginWrite((UInt16)Command_ID.CMD_CHAR_ENTER_GAME_REQ, (Byte)CmdHandler_ID.CMDH_SENCE);

            CharEnterGameReq.Write(m_WriteHelper);

            m_WriteHelper.EndWrite();

            SendData(m_WriteHelper.GetData(), (int)m_WriteHelper.GetDataLen());
	    }
        else if (ConType == (Byte)ConnectionType.TYPE_SVR_LOGIN)
	    {
            SetConnectState(ConnectState.Succ_Connect);
	    }

        return true;
     }

    public Boolean OnCmdPickCharAck(Command_ID wCommandID, UInt64 u64ConnID, ReadBufferHelper ReadHelper)
    {
        StCharPickCharAck CharPickCharAck = new StCharPickCharAck();
        CharPickCharAck.Read(ReadHelper);

        if (CharPickCharAck.nRetCode == 0)
        {
            DisConnect();
            m_u64ClientID = CharPickCharAck.u64CharID;
            m_dwIdentifyCode = CharPickCharAck.dwIdentifyCode;
            m_WriteHelper.m_u64ClientID = CharPickCharAck.u64CharID;
            ConnectToServer(CharPickCharAck.szIpAddr, CharPickCharAck.sPort);
        }

        return true; 
    }

    public Boolean OnCmdHeartBeatAck(Command_ID wCommandID, UInt64 u64ConnID, ReadBufferHelper ReadHelper)
    {
        StCharHeartBeatAck CharHeartBeatAck = new StCharHeartBeatAck();
        CharHeartBeatAck.Read(ReadHelper);

        return true;
    }

    //以下都是内部网络实现

    bool Valid()
    {
        return ((m_ClientSocket != null) && (m_ClientSocket.Connected == true));
    }

    void SetConnectState(ConnectState conState)
    {
        m_ConnectState = conState;
    }

    void ConnectCallBack(IAsyncResult asyncConnect)
    {
        Socket ClientSocket = (Socket)asyncConnect.AsyncState;
        try
        {
            ClientSocket.EndConnect(asyncConnect);

            // 与socket建立连接成功，开启线程接受服务端数据
            SetConnectState(ConnectState.Raw_Connect);

            Debug.Print("SocketObject Connect Success.");
        }
        catch (Exception e)
        {
            Debug.Print("SocketObject Connect Failed: " + e.ToString());
        }
    }

    public bool IsConnectSuccessed()
    {
        return (m_ConnectState == ConnectState.Succ_Connect);
    }

   void ProcessData()
    {
        if (m_DataLen < PacketHeaderSize)
        {
            Array.Copy(m_RecvBuffer, 0, m_DataBuffer, m_DataLen, m_RecvLen);

            m_DataLen += m_RecvLen;
        }

        if (m_DataLen < PacketHeaderSize)
       {
           return ;
       }

       Byte CheckCode = m_DataBuffer[0];

       int nPacketSize = BitConverter.ToUInt16(m_DataBuffer, 1);

       if(nPacketSize <= m_DataLen)
       {
           //说明一个包的数据是足够了
           Array.Copy(m_DataBuffer, 0, m_ReadHelper.GetData(), 0, nPacketSize);

           Array.Copy(m_DataBuffer, nPacketSize, m_DataBuffer, 0, m_DataLen - nPacketSize);

           m_DataLen = m_DataLen - nPacketSize;
       }
       else
       {
           return ;
       }

       if (!m_ReadHelper.BeginRead())
       {
           return;
       }

       OnCommandHandle((Command_ID)m_ReadHelper.m_CommandHeader.wCommandID, 0, m_ReadHelper);

       return;
    }

   bool ReceiveData()
    {
        if (!Valid())
        {
            return false;
        }

        if (!m_ClientSocket.Poll(3, SelectMode.SelectRead))
        {
            return false;
        }

        if (!Valid())
        {
            Debug.Print("invalid socket!");

            return false;
        }

        try
        {
            m_RecvLen = m_ClientSocket.Receive(m_RecvBuffer, BUFFER_MAX, 0);
        }
        catch (SocketException err)
        {
            if (err.ErrorCode == 10054 || err.ErrorCode == 10053)
            {
                Debug.Print(string.Format("NetworkInterface::recv(): disable connect!"));
                if (m_ClientSocket != null && m_ClientSocket.Connected)
                {
                    m_ClientSocket.Close();
                }

                m_ClientSocket = null;
            }
            else
            {
                Debug.Print(string.Format("NetworkInterface::recv(): socket error(" + err.ErrorCode + ")!"));
            }

            return false;
        }


        if (m_RecvLen <= 0)
        {
            Debug.Print(string.Format("NetworkInterface::recv(): disable connect!"));
            if (m_ClientSocket != null && m_ClientSocket.Connected)
            {
                m_ClientSocket.Close();
            }

            m_ClientSocket = null;

            m_RecvLen = 0;

            return false;
        }

        return true;
    }
}
