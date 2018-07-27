#include"LRU.h"

//构造函数
template<class K,class T,class Compare=Isequal<K>>
LRUcache<K, T, Compare>::LRUcache(size_t capacity)
:_capacity(capacity)
, _Hashmap(new LRUnode<K, T>*[capacity])
, _cachelistHead(NULL)
, _cachelistTail(NULL)
, _cachelistsize(0)
{
	memset(_Hashmap, 0, sizeof(LRUnode<K, T>*)*_capacity);
}

//析构函数
template<class K,class T,class Compare=Isequal<K>>
LRUcache<K, T, Compare>::~LRUcache()
{
	DeleteList(_cachelistHead);
	delete[] _Hashmap;
	_capacity = 0;
	_Hashmap = NULL;
	_cachelistHead = NULL;
	_cachelistTail = NULL;
	_cachelistsize = 0;
}

//向缓存中添加数据
template<class K,class T,class Compare=Isequal<K>>
void LRUcache<K, T, Compare>::LRUcacheSet(K key, T data)
{
	Node* cur = GetValueFromHashmap(key);
	if (cur == NULL)
	{
		cur = new Node(key, data);
		InsertValueToHash(cur);
		Node* LastNode = InsertValueToList(cur);
		if (NULL != LastNode)
		{
			RemoveFromHash(LastNode);
		}
	}
	else
	{
		UpdatecacheList(cur);
	}
}

//从缓冲中获取数据
template<class K,class T,class Compare=Isequal<K>>
bool LRUcache<K, T, Compare>::LRUcacheGet(K key, T& data)
{
	Node* cur = GetValueFromHashmap(key);
	if (cur != NULL)
	{
		UpdatecacheList(cur);
		data = cur->_data;
		return true;
	}
	return false;
}

//hash映射函数
template<class K,class T,class Compare=Isequal<K>>
int LRUcache<K, T, Compare>::HashFunc(K key)
{
	return key%_capacity;
}

//从Hashmap中获得一个缓存单元
template<class K,class T,class Compare=Isequal<K>>
LRUnode<K, T>* LRUcache<K, T, Compare>::GetValueFromHashmap(K key)
{
	Node* node = _Hashmap[HashFunc(key)];
	while (NULL != node)
	{
		if (Compare()(node->_key, key))
		{
			return node;
		}
		node = node->hash_next;
	}
	return NULL;
}

//向双向链表中中插入一个缓存单元
template<class K,class T,class Compare=Isequal<K>>
LRUnode<K, T>* LRUcache<K, T, Compare>::InsertValueToList(Node* node)
{
	Node* LastNode = NULL;
	if (++_cachelistsize > _capacity)
	{
		LastNode = _cachelistTail;
		RemoveFromList(LastNode);
	}

	if (_cachelistHead == NULL)
	{
		_cachelistHead = node;
		node->list_next = NULL;
		node->list_prev = NULL;
	}
	else
	{
		node->list_next = _cachelistHead;
		_cachelistHead->list_prev = node;
		_cachelistHead = node;
		node->list_prev = NULL;
	}
	return LastNode;
}

//向Hashmap中插入一个缓存单元
template<class K,class T,class Compare=Isequal<K>>
void LRUcache<K, T, Compare>::InsertValueToHash(Node* node)
{
	Node* cur = _Hashmap[HashFunc(node->_key)];
	if (NULL == cur)
	{
		_Hashmap[HashFunc(node->_key)] = node;
		node->hash_next = NULL;
		node->hash_prev = NULL;
	}
	else
	{
		node->hash_next = cur;
		cur->hash_prev = node;
		_Hashmap[HashFunc(node->_key)] = node;
		node->hash_prev = NULL;
	}
}

//从Hashmap中移除一个缓存单元
template<class K,class T,class Compare=Isequal<K>>
void LRUcache<K, T, Compare>::RemoveFromHash(Node* node)
{
	if (node != NULL)
	{
		if (node->hash_prev == NULL)
		{
			_Hashmap[HashFunc(node->_key)] = node->hash_next;
			if (node->hash_next)
			{
				node->hash_next->hash_prev = NULL;
			}
		}
		else
		{
			node->hash_prev->hash_next = node->hash_next;
			if (node->hash_next)
			{
				node->hash_next->hash_prev = node->hash_prev;
			}
		}
		delete node;
		node = NULL;
	}
}

//从双向链表中删除一个节点
template<class K,class T,class Compare=Isequal<K>>
void LRUcache<K, T, Compare>::RemoveFromList(Node* node)
{
	if (_cachelistHead == NULL)
	{
		return;
	}
	if (_cachelistHead == _cachelistTail)
	{
		_cachelistHead = _cachelistTail = NULL;
	}
	else if (_cachelistHead = node)
	{
		_cachelistHead = node->list_next;
		_cachelistHead->list_prev = NULL;
	}
	else if (_cachelistTail = node)
	{
		_cachelistTail = _cachelistTail->list_prev;
		_cachelistTail->list_next = NULL;
	}
	else
	{
		node->list_prev->list_next = node->list_next;
		node->list_next->list_prev = node->list_prev;
	}
	--_cachelistsize;
}

//更新双向链表
template<class K,class T,class Compare=Isequal<K>>
void LRUcache<K, T, Compare>::UpdatecacheList(Node* node)
{
	RemoveFromList(node);
	InsertValueToList(node);
}

//删除整个链表
template<class K,class T,class Compare=Isequal<K>>
void LRUcache<K, T, Compare>::DeleteList(Node* head)
{
	Node* cur = head;
	while (cur)
	{
		head = head->list_next;
		delete cur;
		cur = head;
	}
}

int main()
{
	LRUcache<int, char> lru(9);
	lru.LRUcacheSet(0, 'a');
	lru.LRUcacheSet(1, 'b');
	lru.LRUcacheSet(2, 'c');
	lru.LRUcacheSet(3, 'd');

	char c = 0;
	cout << lru.LRUcacheGet(3, c) << endl;
	system("pause");
}