#ifndef __BATTLE_PROPERTY_MGR_H__
#define __BATTLE_PROPERTY_MGR_H__

#define MAX_PROPERTY_NUM 64

class CProperty
{
public:
	CProperty()
	{
		m_fBaseValue = 0;
		m_fCurValue  = 0;
		m_fValueChg  = 0;
		m_fPercentChg= 0;
		m_bLocked	 = FALSE;
	}

	~CProperty()
	{

	}

	FLOAT GetCurValue()
	{
		return m_fCurValue;
	}

	FLOAT SetCurValue(FLOAT fValue)
	{
		return m_fCurValue = fValue;
	}

	FLOAT GetBaseVaue()
	{
		return m_fCurValue;
	}

	BOOL UpdateValue()
	{
		m_fCurValue = m_fBaseValue + m_fValueChg;

		m_fCurValue = m_fCurValue * m_fPercentChg;

		return TRUE;
	}

	FLOAT m_fBaseValue;
	FLOAT m_fCurValue;

	FLOAT m_fValueChg;
	FLOAT m_fPercentChg;

	BOOL  m_bLocked;
};



class CPropertyMgr
{
public:
	CPropertyMgr();

	~CPropertyMgr();

public:
	virtual BOOL	IsChanged();

	virtual BOOL	ClearChangeFlag();

	virtual BOOL    SetUpdate();

	virtual UINT32	WriteToBuffer(CBufferHelper *pBufHelper, UINT32 dwChangeFlag, UINT32 dwDest);

	virtual UINT32  ReadFromBuffer(CBufferHelper *pBufHelper);

public:

	FLOAT		GetPropertyValue(UINT32 dwPropertyID);

	VOID		SetProPertyValue(UINT32 dwPropertyID, FLOAT fValue);

	CProperty	m_PropertyList[MAX_PROPERTY_NUM];

};

#endif //__BATTLE_PROPERTY_MGR_H__
