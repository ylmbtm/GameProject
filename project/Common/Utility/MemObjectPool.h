#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

template<typename T>
class MemObjectNode
{
public:
	T m_Value;
	MemObjectNode<T> *m_pNext;
	MemObjectNode<T> *m_pPrev;
};


template <typename T>
class MemObjectPool
{
public:
	MemObjectPool(void){}
	~MemObjectPool(void){}
public:
	T*		CreateMemObject();

	void    FreeMemObject(T* pValue);


private:
	bool	AllocNodeBuffer(int nSize = 1024);

	bool	FreeNodeBuffer();

private:
	MemObjectNode<T> *m_pFreeHead;

	std::vector<MemObjectNode<T>*> m_NodeBuff;
};

template <typename T>
void MemObjectPool<T>::FreeMemObject( T* pValue )
{
	if(pValue == NULL)
	{
		return;
	}

	MemObjectNode<T> *pNode = (MemObjectNode<T> *)pValue;

	pNode->m_pNext = m_pFreeHead;
	m_pFreeHead->m_pPrev = pNode;
	m_pFreeHead = pNode;
	m_pFreeHead->m_pPrev = NULL;
	
	return ;
}

template <typename T>
T* MemObjectPool<T>::CreateMemObject()
{
	if(m_pFreeHead == NULL)
	{
		AllocNodeBuffer();
	}

	if(m_pFreeHead == NULL)
	{
		return NULL;
	}

	MemObjectNode<T> *pValidNode = m_pFreeHead;

	m_pFreeHead = m_pFreeHead->m_pNext;

	m_pFreeHead->m_pPrev	= NULL;

	pValidNode->m_pNext		= NULL;
	pValidNode->m_pPrev		= NULL;

	return &pValidNode->m_Value;
}



template <typename T>
bool MemObjectPool<T>::AllocNodeBuffer( int nSize /*= 1024*/ )
{
	MemObjectNode<T> *pNode = (MemObjectNode<T> *)malloc(sizeof(MemObjectNode<T>) * nSize);
	if(pNode == NULL)
	{
		return false;
	}

	memset(pNode, 0, sizeof(MemObjectNode<T>) * nSize);

	m_NodeBuff.push_back(pNode);

	for(int i = 0; i < nSize; i++)
	{
		if(m_pFreeHead == NULL)
		{
			m_pFreeHead = pNode;
		}
		else
		{
			pNode->m_pNext			= m_pFreeHead;
			m_pFreeHead->m_pPrev	= pNode;
			m_pFreeHead				= pNode;
			m_pFreeHead->m_pPrev	= NULL;
		}

		pNode ++;
	}

	return true;
}

template <typename T>
bool MemObjectPool<T>::FreeNodeBuffer()
{
	for(size_t i = 0; i < m_NodeBuff.size(); ++i)
	{
		MemObjectNode *pNode = m_NodeBuff.at(i);
		if(pNode != NULL)
		{
			free(pNode);
		}
	}

	return true;
}


#define MEM_OBJECT_POOL_SUPORT(ClassName) \
	typedef MemObjectPool<##ClassName##> PoolType; \
	static void* operator new(size_t size) \
	{ \
		if(size != sizeof(ClassName))
		{\
			ASSERT_FAIELD;\
		}\
		return GetObjectPool()->CreateMemObject(); \
	} \
	static void operator delete(void* pointee) \
	{ \
		GetObjectPool()->FreeMemObject((ClassName*)pointee); \
	} \
	static PoolType* GetObjectPool() \
	{ \
		static PoolType s_MemObjectPool; \
		return &s_MemObjectPool; \
	}

#endif //__OBJECT_POOL_H__