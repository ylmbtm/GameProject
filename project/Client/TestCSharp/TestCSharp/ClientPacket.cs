using System;
public class StConnectNotify
{
	public Byte btConType;
	public UInt64 u64ConnID;
	public void Read(ReadBufferHelper bh)
	{
		btConType = bh.ReadUInt8();
		u64ConnID = bh.ReadUInt64();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt8(btConType);
		bh.WriteUInt64(u64ConnID);
		return ;
	}
};
public class StDisConnectNotify
{
	public Byte btConType;
	public UInt64 u64ConnID;
	public void Read(ReadBufferHelper bh)
	{
		btConType = bh.ReadUInt8();
		u64ConnID = bh.ReadUInt64();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt8(btConType);
		bh.WriteUInt64(u64ConnID);
		return ;
	}
};
public class StCharVerifyVersionReq
{
	public UInt32 dwClientVersion;
	public void Read(ReadBufferHelper bh)
	{
		dwClientVersion = bh.ReadUInt32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt32(dwClientVersion);
		return ;
	}
};
public class StCharVerifyVersionAck
{
	public UInt32 dwVerifyCode;
	public void Read(ReadBufferHelper bh)
	{
		dwVerifyCode = bh.ReadUInt32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt32(dwVerifyCode);
		return ;
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
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteFixString(szAccountName);
		bh.WriteFixString(szPassword);
		return ;
	}
};
public class StCharNewAccountAck
{
	public UInt16 nRetCode;
	public void Read(ReadBufferHelper bh)
	{
		nRetCode = bh.ReadUInt16();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt16(nRetCode);
		return ;
	}
};
public class StCharNewCharReq
{
	public UInt32 dwAccountID;
	public string szCharName;
	public UInt32 dwFeature;
	public void Read(ReadBufferHelper bh)
	{
		dwAccountID = bh.ReadUInt32();
		szCharName = bh.ReadFixString(32);
		dwFeature = bh.ReadUInt32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt32(dwAccountID);
		bh.WriteFixString(szCharName);
		bh.WriteUInt32(dwFeature);
		return ;
	}
};
public class StCharDelCharReq
{
	public UInt32 dwAccountID;
	public UInt64 u64CharID;
	public void Read(ReadBufferHelper bh)
	{
		dwAccountID = bh.ReadUInt32();
		u64CharID = bh.ReadUInt64();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt32(dwAccountID);
		bh.WriteUInt64(u64CharID);
		return ;
	}
};
public class StCharDelCharAck
{
	public UInt16 nRetCode;
	public UInt32 dwAccountID;
	public UInt64 u64CharID;
	public void Read(ReadBufferHelper bh)
	{
		nRetCode = bh.ReadUInt16();
		dwAccountID = bh.ReadUInt32();
		u64CharID = bh.ReadUInt64();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt16(nRetCode);
		bh.WriteUInt32(dwAccountID);
		bh.WriteUInt64(u64CharID);
		return ;
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
		u64CharID = bh.ReadUInt64();
		szCharName = bh.ReadFixString(32);
		dwLevel = bh.ReadUInt32();
		dwFeature = bh.ReadUInt32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt64(u64CharID);
		bh.WriteFixString(szCharName);
		bh.WriteUInt32(dwLevel);
		bh.WriteUInt32(dwFeature);
		return ;
	}
};
public class StCharNewCharAck
{
	public UInt16 nRetCode;
	public StCharPickInfo CharPickInfo= new StCharPickInfo();
	public void Read(ReadBufferHelper bh)
	{
		nRetCode = bh.ReadUInt16();
		CharPickInfo.Read(bh);
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt16(nRetCode);
		CharPickInfo.Write(bh);
		return ;
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
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteFixString(szAccountName);
		bh.WriteFixString(szPassword);
		return ;
	}
};
public class StCharLoginAck
{
	public UInt16 nRetCode;
	public UInt32 dwAccountID;
	public Byte nCount;
	public StCharPickInfo CharPickInfo = new StCharPickInfo[4];
	public void Read(ReadBufferHelper bh)
	{
		nRetCode = bh.ReadUInt16();
		dwAccountID = bh.ReadUInt32();
		nCount = bh.ReadUInt8();
		for(size_t i = 0; i < 4; i++)
		{
			CharPickInfo[i].Read(bh);
		}
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt16(nRetCode);
		bh.WriteUInt32(dwAccountID);
		bh.WriteUInt8(nCount);
		for(size_t i = 0; i < 4; i++)
		{
			CharPickInfo[i].Write(bh);
		}
		return ;
	}
};
public class StCharPickCharReq
{
	public UInt64 u64CharID;
	public void Read(ReadBufferHelper bh)
	{
		u64CharID = bh.ReadUInt64();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt64(u64CharID);
		return ;
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
		nRetCode = bh.ReadUInt16();
		u64CharID = bh.ReadUInt64();
		dwIdentifyCode = bh.ReadUInt32();
		sPort = bh.ReadUInt16();
		szIpAddr = bh.ReadFixString(32);
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt16(nRetCode);
		bh.WriteUInt64(u64CharID);
		bh.WriteUInt32(dwIdentifyCode);
		bh.WriteUInt16(sPort);
		bh.WriteFixString(szIpAddr);
		return ;
	}
};
public class StCharEnterGameReq
{
	public UInt64 u64CharID;
	public UInt32 dwIdentifyCode;
	public void Read(ReadBufferHelper bh)
	{
		u64CharID = bh.ReadUInt64();
		dwIdentifyCode = bh.ReadUInt32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt64(u64CharID);
		bh.WriteUInt32(dwIdentifyCode);
		return ;
	}
};
public class StCharEnterGameAck
{
	public UInt32 dwIdentifyCode;
	public UInt32 dwSceneID;
	public void Read(ReadBufferHelper bh)
	{
		dwIdentifyCode = bh.ReadUInt32();
		dwSceneID = bh.ReadUInt32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt32(dwIdentifyCode);
		bh.WriteUInt32(dwSceneID);
		return ;
	}
};
public class StCharLeaveGameReq
{
	public UInt32 dwLeaveReason;
	public void Read(ReadBufferHelper bh)
	{
		dwLeaveReason = bh.ReadUInt32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt32(dwLeaveReason);
		return ;
	}
};
public class StCharLeaveGameAck
{
	public void Read(ReadBufferHelper bh)
	{
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		return ;
	}
};
public class StCharGmCmdReq
{
	public string szGMCommand;
	public void Read(ReadBufferHelper bh)
	{
		szGMCommand = bh.ReadFixString(32);
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteFixString(szGMCommand);
		return ;
	}
};
public class StCharMoveReq
{
	public UInt16 sDir;
	public Single x;
	public Single y;
	public Single z;
	public void Read(ReadBufferHelper bh)
	{
		sDir = bh.ReadUInt16();
		x = bh.ReadFloat();
		y = bh.ReadFloat();
		z = bh.ReadFloat();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt16(sDir);
		bh.WriteFloat(x);
		bh.WriteFloat(y);
		bh.WriteFloat(z);
		return ;
	}
};
public class StCharHeartBeatReq
{
	public UInt32 dwReqTimestamp;
	public void Read(ReadBufferHelper bh)
	{
		dwReqTimestamp = bh.ReadUInt32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt32(dwReqTimestamp);
		return ;
	}
};
public class StCharHeartBeatAck
{
	public UInt32 dwReqTimestamp;
	public UInt32 dwServerTime;
	public void Read(ReadBufferHelper bh)
	{
		dwReqTimestamp = bh.ReadUInt32();
		dwServerTime = bh.ReadUInt32();
		return ;
	}
	public void Write(WriteBufferHelper bh)
	{
		bh.WriteUInt32(dwReqTimestamp);
		bh.WriteUInt32(dwServerTime);
		return ;
	}
};
