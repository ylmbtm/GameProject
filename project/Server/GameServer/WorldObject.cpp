#include "stdafx.h"
#include "WorldObject.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "DataBuffer/BufferHelper.h"
#include "Scene.h"


CWorldObject::CWorldObject()
{
	m_pOwnerScene = NULL;

	m_pGridNext   = NULL;

	m_pGridPrev   = NULL;
}

CWorldObject::~CWorldObject()
{
	m_pOwnerScene = NULL;

	m_pGridNext   = NULL;

	m_pGridPrev   = NULL;
}

CPosition& CWorldObject::GetPosition()
{
	return m_ObjectPos;
}

VOID CWorldObject::SetPosition( FLOAT x, FLOAT y, FLOAT z )
{
	m_ObjectPos.x = x;

	m_ObjectPos.y = y;

	m_ObjectPos.z = z;

	return ;
}

BOOL CWorldObject::IsChanged()
{

	return TRUE;
}

BOOL CWorldObject::ClearChangeFlag()
{

	return TRUE;
}

BOOL CWorldObject::SetUpdate()
{
	if(m_pOwnerScene != NULL)
	{
		m_pOwnerScene->AddToUpdateList(this);
	}
	
	return TRUE;
}

UINT32 CWorldObject::WriteToBuffer( CBufferHelper *pBufHelper, UINT32 dwChangeFlag, UINT32 dwDest )
{
	UINT32 dwSize = 0;

	dwSize += CObject::WriteToBuffer(pBufHelper, dwChangeFlag, dwDest);

	dwSize += pBufHelper->Write(m_ObjectPos);

	return dwSize;
}

UINT32 CWorldObject::ReadFromBuffer( CBufferHelper *pBufHelper )
{
	UINT32 dwSize = 0;	
	
	dwSize += CObject::ReadFromBuffer(pBufHelper);

	dwSize += pBufHelper->Read(m_ObjectPos);

	return dwSize;
}

CScene* CWorldObject::GetOwnerScene()
{
	return m_pOwnerScene;
}

VOID CWorldObject::SetOwnerScene( CScene *pScene )
{
	m_pOwnerScene = pScene;

	return ;
}
