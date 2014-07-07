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
    public Byte btConType;	//服务器或玩家的类型
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


public class StCharVerifyVersionReq
{
	public UInt32 dwClientVersion;
	public void Read(ReadBufferHelper bh)
	{
		dwClientVersion = bh.ReadUint32();
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwClientVersion);
	}
};
public class StCharVerifyVersionAck
{
	public UInt32 dwVerifyCode;
	public void Read(ReadBufferHelper bh)
	{
		dwVerifyCode = bh.ReadUint32();
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwVerifyCode);
	}
};
public class StCharNewAccountReq
{
	public string szAccountName;
	public string szPassword;
	public void Read(ReadBufferHelper bh)
	{
		szAccountName = bh.ReadFixString(32);
		szPassword = bh.ReadFixString(32);
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteFixString(szAccountName, 32);
		bh.WriteFixString(szPassword, 32);
	}
};
public class StCharNewAccountAck
{
	public UInt16 nRetCode;
	public void Read(ReadBufferHelper bh)
	{
		nRetCode = bh.ReadUint16();
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint16(nRetCode);
	}
};
public class StCharNewCharReq
{
	public UInt32 dwAccountID;
	public string szCharName;
	public UInt32 dwFeature;
	public void Read(ReadBufferHelper bh)
	{
		dwAccountID = bh.ReadUint32();
		szCharName = bh.ReadFixString(32);
		dwFeature = bh.ReadUint32();
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwAccountID);
		bh.WriteFixString(szCharName, 32);
		bh.WriteUint32(dwFeature);
	}
};
public class StCharDelCharReq
{
	public UInt32 dwAccountID;
	public UInt64 u64CharID;
	public void Read(ReadBufferHelper bh)
	{
		dwAccountID = bh.ReadUint32();
		u64CharID = bh.ReadUint64();
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwAccountID);
		bh.WriteUint64(u64CharID);
	}
};
public class StCharDelCharAck
{
	public UInt16 nRetCode;
	public UInt32 dwAccountID;
	public UInt64 u64CharID;
	public void Read(ReadBufferHelper bh)
	{
		nRetCode = bh.ReadUint16();
		dwAccountID = bh.ReadUint32();
		u64CharID = bh.ReadUint64();
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint16(nRetCode);
		bh.WriteUint32(dwAccountID);
		bh.WriteUint64(u64CharID);
	}
};
public class StCharPickInfo
{
	public UInt64 u64CharID;
	public string szCharName;
	public UInt32 dwLevel;
	public UInt32 dwFeature;
	public void Read(ReadBufferHelper bh)
	{
		u64CharID = bh.ReadUint64();
		szCharName = bh.ReadFixString(32);
		dwLevel = bh.ReadUint32();
		dwFeature = bh.ReadUint32();
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint64(u64CharID);
		bh.WriteFixString(szCharName, 32);
		bh.WriteUint32(dwLevel);
		bh.WriteUint32(dwFeature);
	}
};
public class StCharNewCharAck
{
	public UInt16 nRetCode;
	public StCharPickInfo CharPickInfo;
	public void Read(ReadBufferHelper bh)
	{
		nRetCode = bh.ReadUint16();
		CharPickInfo.Read(bh);
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint16(nRetCode);
		CharPickInfo.Write(bh);
	}
};
public class StCharLoginReq
{
	public string szAccountName;
	public string szPassword;
	public void Read(ReadBufferHelper bh)
	{
		szAccountName = bh.ReadFixString(32);
		szPassword = bh.ReadFixString(32);
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteFixString(szAccountName, 32);
		bh.WriteFixString(szPassword, 32);
	}
};
public class StCharLoginAck
{
	public UInt16 nRetCode;
	public UInt32 dwAccountID;
	public Byte nCount;
	public StCharPickInfo[] CharPickInfo = new StCharPickInfo[4];
	public void Read(ReadBufferHelper bh)
	{
		nRetCode = bh.ReadUint16();
		dwAccountID = bh.ReadUint32();
		nCount = bh.ReadUint8();
		for(int i = 0; i < 4; i++)
		{
            CharPickInfo[i] = new StCharPickInfo();
			CharPickInfo[i].Read(bh);
		}
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint16(nRetCode);
		bh.WriteUint32(dwAccountID);
		bh.WriteUint8(nCount);
        for (int i = 0; i < 4; i++)
		{
			CharPickInfo[i].Write(bh);
		}
	}
};
public class StCharPickCharReq
{
	public UInt64 u64CharID;
	public void Read(ReadBufferHelper bh)
	{
		u64CharID = bh.ReadUint64();
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint64(u64CharID);
	}
};
public class StCharPickCharAck
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
	public UInt64 u64CharID;
	public UInt32 dwIdentifyCode;
	public void Read(ReadBufferHelper bh)
	{
		u64CharID = bh.ReadUint64();
		dwIdentifyCode = bh.ReadUint32();
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint64(u64CharID);
		bh.WriteUint32(dwIdentifyCode);
	
	}
};
public class StCharEnterGameAck
{
	public UInt32 dwIdentifyCode;
	public UInt32 dwSceneID;
	public void Read(ReadBufferHelper bh)
	{
		dwIdentifyCode = bh.ReadUint32();
		dwSceneID = bh.ReadUint32();
	
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwIdentifyCode);
		bh.WriteUint32(dwSceneID);
	
	}
};
public class StCharLeaveGameReq
{
	public UInt32 dwLeaveReason;
	public void Read(ReadBufferHelper bh)
	{
		dwLeaveReason = bh.ReadUint32();
		
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint32(dwLeaveReason);
	}
};
public class StCharLeaveGameAck
{
	public void Read(ReadBufferHelper bh)
	{
		
	}
	public void Write(WriteBufferHelper bh)
	{
		
	}
};
public class StCharMoveReq
{
	public UInt16 sDir;
    public float x;
    public float y;
    public float z;
	public void Read(ReadBufferHelper bh)
	{
		sDir = bh.ReadUint16();
		x = bh.ReadFloat();
		y = bh.ReadFloat();
		z = bh.ReadFloat();
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUint16(sDir);
		bh.WriteFloat(x);
		bh.WriteFloat(y);
		bh.WriteFloat(z);
	}
};
