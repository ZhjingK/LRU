#include<iostream>
#include<stdlib.h>
using namespace std;

//����ڵ���
template<class K, class T>
struct LRUnode
{
	K _key;
	T _data;

	LRUnode<K, T>* hash_prev;//ָ��hash�����ǰһ���ڵ�
	LRUnode<K, T>* hash_next;//ָ��hash����ĺ�һ���ڵ�
	LRUnode<K, T>* list_prev;//ָ��˫�������ǰһ���ڵ�
	LRUnode<K, T>* list_next;//ָ��˫������ĺ�һ���ڵ�

	LRUnode<K, T>(const K key, const T data)
		: _key(key)
		, _data(data)
		, hash_prev(NULL)
		, hash_next(NULL)
		, list_prev(NULL)
		, list_next(NULL)
	{}
};

//�º���
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

//������
template<class K, class T, class Compare = Isequal<K>>
class LRUcache
{
	typedef LRUnode<K, T> Node;
public:
	LRUcache(size_t capacity);
	~LRUcache();

	//�򻺴����������
	void LRUcacheSet(K key, T data);
	//�ӻ�����ȡ����
	bool LRUcacheGet(K key, T& data);

private:
	//��˫��������ɾ��ָ���ڵ�
	void RemoveFromList(Node* node);

	//�ӹ�ϣ������ɾ��ָ���ڵ�
	void RemoveFromHash(Node* node);

	//��˫�������ͷ����ڵ�
	LRUnode<K, T>* InsertValueToList(Node* node);

	//���ϣ�����в���һ�����浥Ԫ
	void InsertValueToHash(Node* node);

	//����˫������
	void UpdatecacheList(Node* node);

	//ɾ������˫������
	void DeleteList(Node* head);

	//���ù�ϣ����ӳ�䣬��ȡhashֵ
	int HashFunc(K key);

	//��hashmap�л�ȡһ�����浥Ԫ
	LRUnode<K, T>* GetValueFromHashmap(K key);

private:
	size_t _capacity;//���������
	Node** _Hashmap;//��ϣ����--������в���
	Node* _cachelistHead;//˫�򻺴������ͷ��
	Node* _cachelistTail;//˫�򻺴������β��
	size_t _cachelistsize;//˫�򻺴������нڵ�ĸ���
};