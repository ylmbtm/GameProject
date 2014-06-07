#ifndef __RANDOM_VALUE__
#define __RANDOM_VALUE__

class CRandonValue
{
private:
	CRandonValue()
	{
		for(INT32 i = 0; i < 100; i++)
		{
			m_Values[i] = i;
		}

		Disorder(m_Values, 100);

		m_nCurIndex		= 1;

		m_nStartIndex	= 0;
	}

	~CRandonValue()
	{


	}

	void Disorder(INT32 Arr[], INT32 nCount)
	{
		INT32 nIndex, nTemp;
		rand(time(NULL));
		for (INT32 i = 0; i <n; i++)
		{
			nIndex = rand() % (n - i) + i;
			if (nIndex != i)
			{
				nTemp		= Arr[i];
				Arr[i]		= Arr[nIndex];
				Arr[nIndex]	= nTemp;
			}
		}
	}


public:
	static CRandonValue& GetInstance()
	{
		static CRandonValue _Value;

		return _Value;
	}


	INT32 Random()
	{
		m_nCurIndex = (m_nCurIndex+1)%100;
		if(m_nCurIndex != m_nStartIndex)
		{
			return m_Values[m_nCurIndex];
		}
		else
		{
			m_nStartIndex = m_nCurIndex = rand()%100;

			return m_Values[m_nCurIndex];
		}
	}

	INT32  m_Values[100];

	INT32  m_nCurIndex;

	INT32  m_nStartIndex;
}


#endif /* __RANDOM_VALUE__*/