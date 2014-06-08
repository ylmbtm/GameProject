#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

template<typename T>
class COjbectItem
{
	T *m_pValue;

	COjbectItem *m_pNext;
};


template <typename ValueType>
class CObjectPool : public stdext::hash_map<UINT32, CObjectPool<ValueType>>
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