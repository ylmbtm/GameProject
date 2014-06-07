#include "stdafx.h"
#include "DataBuffer.h"


CBufferManagerAll::CBufferManagerAll()
{

}

CBufferManagerAll::~CBufferManagerAll()
{

}

CBufferManagerAll* CBufferManagerAll::GetInstancePtr()
{
	static CBufferManagerAll BufferManagerAll;

	return &BufferManagerAll;
}

IDataBuffer* CBufferManagerAll::AllocDataBuff( int nSize )
{
	if(nSize < 512)
	{
		return g_BufferManagerHK.AllocDataBuff();
	}
	else if(nSize < 1024)
	{
		return g_BufferManager1K.AllocDataBuff();
	}
	else if(nSize <2048)
	{
		return g_BufferManager2K.AllocDataBuff();
	}
	else if(nSize < 4096)
	{
		return g_BufferManager4K.AllocDataBuff();
	}
	else if(nSize < 8192)
	{
		return g_BufferManager8K.AllocDataBuff();
	}

	return NULL;
}
