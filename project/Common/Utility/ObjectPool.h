#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

template<typename T>
class ObjectNode
{
public:
	T m_Value;
	ObjectNode<T> *m_pNext;
	ObjectNode<T> *m_pPrev;
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