#include<iostream>
#include<stdlib.h>
using namespace std;

//缓存节点类
template<class K, class T>
struct LRUnode
{
	K _key;
	T _data;

	LRUnode<K, T>* hash_prev;//指向hash链表的前一个节点
	LRUnode<K, T>* hash_next;//指向hash链表的后一个节点
	LRUnode<K, T>* list_prev;//指向双向链表的前一个节点
	LRUnode<K, T>* list_next;//指向双向链表的后一个节点

	LRUnode<K, T>(const K key, const T data)
		: _key(key)
		, _data(data)
		, hash_prev(NULL)
		, hash_next(NULL)
		, list_prev(NULL)
		, list_next(NULL)
	{}
};

//仿函数
template<class K>
struct Isequal
{
	bool operator()(K left, K right)
	{
		if (left == right)
		{
			return true;
		}
		return false;
	}
};

//缓存类
template<class K, class T, class Compare = Isequal<K>>
class LRUcache
{
	typedef LRUnode<K, T> Node;
public:
	LRUcache(size_t capacity);
	~LRUcache();

	//向缓存中添加数据
	void LRUcacheSet(K key, T data);
	//从缓存中取数据
	bool LRUcacheGet(K key, T& data);

private:
	//从双向链表中删除指定节点
	void RemoveFromList(Node* node);

	//从哈希数组中删除指定节点
	void RemoveFromHash(Node* node);

	//向双向链表表头插入节点
	LRUnode<K, T>* InsertValueToList(Node* node);

	//向哈希数组中插入一个缓存单元
	void InsertValueToHash(Node* node);

	//更新双向链表
	void UpdatecacheList(Node* node);

	//删除整个双向链表
	void DeleteList(Node* head);

	//利用哈希函数映射，获取hash值
	int HashFunc(K key);

	//从hashmap中获取一个缓存单元
	LRUnode<K, T>* GetValueFromHashmap(K key);

private:
	size_t _capacity;//缓存的容量
	Node** _Hashmap;//哈希数组--方便进行查找
	Node* _cachelistHead;//双向缓存链表的头部
	Node* _cachelistTail;//双向缓存链表的尾部
	size_t _cachelistsize;//双向缓存链表中节点的个数
};