using System;
public class TransferHeader
{
    public Byte CheckCode;
    public UInt16 dwSize;

    public static int GetSize()
    {
        return 5;
    }

    public void Read(ReadBufferHelper bh)
    {
        CheckCode = bh.ReadUint8();
        dwSize = bh.ReadUint16();
    }

    public void Write(WriteBufferHelper bh)
    {
        bh.WriteUint8(CheckCode);
        bh.WriteUint16(dwSize);
    
    }
};

public class CommandHeader
{
    public Byte CmdHandleID;
    public UInt16 wCommandID;
    public UInt32 dwSceneID;
    public UInt64 u64CharID;		//角色ID

    public static int GetSize()
    {
        return 15;
    }

    public void Read(ReadBufferHelper bh)
    {
        CmdHandleID = bh.ReadUint8();
        wCommandID = bh.ReadUint16();
        dwSceneID = bh.ReadUint32();
        u64CharID = bh.ReadUint64();
    }

    public void Write(WriteBufferHelper bh)
    {
        bh.WriteUint8(CmdHandleID);
        bh.WriteUint16(wCommandID);
        bh.WriteUint32(dwSceneID);
        bh.WriteUint64(u64CharID);
    }
};


public class StConnectNotify
{
    public Byte   btConType;	//服务器或玩家的类型
    public UInt64 u64ConnID;	//服务器ID或玩家ID

    public void Read(ReadBufferHelper bh)
    {
        btConType = bh.ReadUint8();
        u64ConnID = bh.ReadUint64();
    }

    public void Write(WriteBufferHelper bh)
    {
        bh.WriteUint8(btConType);
        bh.WriteUint64(u64ConnID);
    }
};

public class StCharPickCharAck	//选择角色请求回复
{
    public UInt16 nRetCode;
    public UInt64 u64CharID;
    public UInt32 dwIdentifyCode;
    public UInt16 sPort;
    public string szIpAddr;
    public void Read(ReadBufferHelper bh)
    {
        nRetCode = bh.ReadUint16(); 
        u64CharID = bh.ReadUint64();
        dwIdentifyCode = bh.ReadUint32();
        sPort = bh.ReadUint16();
        szIpAddr = bh.ReadFixString(32);
    }

    public void Write(WriteBufferHelper bh)
    {
        bh.WriteUint16(nRetCode);
        bh.WriteUint64(u64CharID);
        bh.WriteUint32(dwIdentifyCode);
        bh.WriteUint16(sPort);
        bh.WriteFixString(szIpAddr, 32);

    }
};

public class StCharEnterGameReq
{
    public UInt64 u64CharID;		//进入游戏请求
    public UInt32 dwIndentifyCode;		//进入游戏请求码

    public void Read(ReadBufferHelper bh)
    {
        u64CharID = bh.ReadUint64();
        dwIndentifyCode = bh.ReadUint32();
    }

    public void Write(WriteBufferHelper bh)
    {
       bh.WriteUint64(u64CharID);
       bh.WriteUint32(dwIndentifyCode);
    }
};

