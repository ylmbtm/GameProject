#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

template<typename T>
class CObjectItem
{
	T m_Value;

	COjbectItem *m_pNext;
};


template <typename T>
class CObjectPool
{
public:
	CObjectPool(void);
	~CObjectPool(void);
public:
	COjbectItem* Create



public:

	COjbectItem *m_pFree;


};

#endif //__OBJECT_POOL_H__