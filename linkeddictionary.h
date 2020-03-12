#pragma once
#include <cmath>
#include <ctime>

#include "arraylist.h"

// 定义键值对
template<typename K, typename V>
class Pair
{
public:
	Pair() { m_key = new K; m_value = new V; };
	Pair(const K k, const V v) { m_key = new K; *m_key = k;  m_value = new V; *m_value = v; };
	~Pair() { delete m_key; delete m_value; }

	void setKey(const K k) { *m_key = k; }
	void setVal(const V v) { *m_value = v; }
	K getKey() const { return *m_key; }
	V getVal() const { return *m_value; }

private:
	K* m_key;
	V* m_value;
};

// 跳表
template<typename K, typename V>
class SkipList
{
public:
	SkipList(int maxLevel=3, float p=0.5);
	~SkipList();

	bool empty() const { return size() == 0;  }
	int  size()  const { return m_kvp[0]->size(); }

	void insert(const K& k, const V& v);
	V get  (const K& k) const;
	void erase(const K& k);
	void print() const;

private:
	ChainNode<Pair<K, V>*>* find(const K& k) const;
	int level() const {
		int lv = 0;
		for (; std::rand() > m_iCutoff; lv++);
		return (lv > m_iMaxLevel - 1) ? (m_iMaxLevel - 1) : lv;
	}

	int m_iCutoff;			// 条件概率，截断
	int m_iMaxLevel;		// 最大链表层数
	ChainList<Pair<K, V>*>** m_kvp;	// 多层链表数组
};

template<typename K, typename V>
SkipList<K, V>::SkipList(int maxLevel, float p) : 
	m_iMaxLevel(maxLevel), m_iCutoff(p * RAND_MAX)
{
	m_kvp = new ChainList<Pair<K, V>*> * [maxLevel];
	for (int i = 0; i < m_iMaxLevel; i++)
		m_kvp[i] = new ChainList<Pair<K, V>*>;
}

template<typename K, typename V>
SkipList<K, V>::~SkipList()
{
	for (int i = 0; i < m_iMaxLevel; i++)
		delete m_kvp[i];
	delete[] m_kvp;
}

template<typename K, typename V>
ChainNode<Pair<K, V>*>* SkipList<K, V>::find(const K& k) const
{
	// 若字典内无数对，返回空
	if (empty()) return nullptr;
	
	ChainNode<Pair<K, V>*>* ret = m_kvp[0]->getNode(0);			// 初始化为0级链表首节点
	for (int i = m_iMaxLevel - 1; i >= 0; i--) {				// 由高级向低级搜索
		if (m_kvp[i]->empty()) continue;

		int index = m_kvp[i]->indexOf(ret->getVal());			// 查找当前节点
		ChainNode<Pair<K, V>*> * pn = (index == -1)? \
			m_kvp[i]->getNode(0): m_kvp[i]->getNode(index);		// 当前级链表开始搜索的位置

		while ((pn->getVal()->getKey() < k) && pn->next->ptr) {
			pn = pn->next; ret = pn;
		}

		if (!pn->next->ptr) continue;							// 已为尾节点
		if (pn->next->getVal()->getKey() == ret->getVal()->getKey()) 
			return pn->next;									// 后一节点是否符合
	}
	return ret;
}

template<typename K, typename V>
void SkipList<K, V>::insert(const K& k, const V& v)
{
	// 先进行查找，若找到节点则不添加键，仅修改值
	ChainNode<Pair<K, V>*> * found = find(k);

	// ----- 若字典为空，直接插入键值对 -----
	if (!found) {
		// 创建节点
		Pair<K, V>* pn = new Pair<K, V>(k, v);
		// 确定层级
		int lv = level();
		// 插入各级链表
		for (int i = 0; i <= lv; i++) 
			m_kvp[i]->insert(0, pn);
		return;
	}

	// ----- 若找到节点则不添加键，仅修改值 -----
	if (found->getVal()->getKey() == k) { found->getVal()->setVal(v); return; }

	// ----- 若未找到，在标志节点附近有序插入 -----
	Pair<K, V>* pn = new Pair<K, V>(k, v);
	// 确定层级
	int lv = level();
	// 插入各级链表
	for (int i = 0; i <= lv; i++) {
		// 当前链表空
		if (m_kvp[i]->empty()) { m_kvp[i]->insert(0, pn); continue; }

		int index = 0;
		ChainNode<Pair<K, V>*>* lvn = m_kvp[i]->getNode(0);		// 当前级链表的首节点
		while (lvn) {
			if (!lvn->ptr) {					// 尾节点
				m_kvp[i]->insert(index, pn);
				break;
			} else {
				if (lvn->ptr && lvn->getVal()->getKey() > k) {	// 当前节点的键大于k
					m_kvp[i]->insert(index, pn);
					break;
				} else {
					lvn = lvn->next; index++;
				}
			}
		}
	}
}

template<typename K, typename V>
V SkipList<K, V>::get(const K& k) const
{
	// 查找
	ChainNode<Pair<K, V>*>* found = find(k);
	if (found) return found->getVal()->getVal();
	return NULL;
}

template<typename K, typename V>
void SkipList<K, V>::erase(const K& k)
{
	// 先进行查找
	ChainNode<Pair<K, V>*>* found = find(k);

	// ----- 若未找到 ----
	if (!found) return;
	
	// ----- 若找到，删除各级链中的该数对-----
	for (int i = m_iMaxLevel - 1; i >= 0; i--) {
		ChainList<Pair<K, V>*>* pc = m_kvp[i];		// 当前级链表
		int index = pc->indexOf(found->getVal());
		if (index == -1) continue;
		pc->erase(index);
	}
}

template<typename K, typename V>
void SkipList<K, V>::print() const
{
	for (int i = 0; i < m_iMaxLevel; i++) {
		ChainNode<Pair<K, V>*>* pn = m_kvp[i]->getNode(0);	// 当前级链表的首节点

		std::cout << "Level [" << i << "]: ";
		while (pn->ptr && pn->next) {
			Pair<K, V>* kvp = pn->getVal();
			std::cout << kvp->getKey() << ": " << kvp->getVal() << " -> ";
			pn = pn->next;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//////////////////////////////////////////////////////////////////////////////////////
template<typename K, typename V>
class HashTabel
{
public:
	HashTabel(int length = 50, int d = 23);
	~HashTabel();

	bool empty() const { return size() == 0; }
	int  size()  const {
		int cnt = 0;
		for (int i = 0; i < m_iLength; i++)
			if (m_listIsEmpty[i]) cnt++;
		return cnt;
	}

	void insert(const K& k, const V& v);
	V get(const K& k) const;
	void erase(const K& k);
	void print() const;

private:
	size_t hash(const K k) const { return k % m_iD; }
	Pair<K, V>* find(const K& k, bool insert = true) const;
	int indexOf(const Pair<K, V>* p) const {
		for (int i = 0; i < m_iLength; i++)
			if (m_listTable[i] == p) return i;
	}

	int m_iD;
	Pair<K, V>** m_listTable;
	bool* m_listIsEmpty;
	int m_iLength;
};

template<typename K, typename V>
HashTabel<K, V>::HashTabel(int length, int d) :
	m_iLength(length), m_iD(d)
{
	m_listTable = new Pair<K, V>* [m_iLength];
	m_listIsEmpty = new bool [m_iLength];
	for (int i = 0; i < m_iLength; i++) {
		m_listTable[i] = new Pair<K, V>;
		m_listIsEmpty[i] = true;
	}
}

template<typename K, typename V>
HashTabel<K, V>::~HashTabel()
{
	for (int i = 0; i < m_iLength; i++) {
		delete m_listTable[i];
	}
	delete[] m_listTable;
	delete[] m_listIsEmpty;
}

template<typename K, typename V>
Pair<K, V>* HashTabel<K, V>::find(const K& k, bool insert) const
{
	int hv = hash(k);	// 求取hash值

	// 从初始桶开始查找
	for (int i = 0; i < m_iLength; i++) {
		int index = (hv + i) % m_iLength;

		// 回到初始位置，同时发生冲突与溢出，返回空
		if (i != 0 && index == hv) return nullptr;

		// 在插入操作时，可返回空桶
		if (insert && m_listIsEmpty[index]) return m_listTable[index];

		// 查找到相同键的桶
		if (m_listTable[index]->getKey() == k)
			return m_listTable[index];
	}

	return nullptr;
}

template<typename K, typename V>
void HashTabel<K, V>::insert(const K& k, const V& v)
{
	Pair<K, V>* found = find(k);
	if (!found) {
		std::cout << "哈希表已满" << std::endl;
		return;
	}

	// 反查位置
	int index = indexOf(found);
	// 若为空，设置键
	if (m_listIsEmpty[index]) found->setKey(k);
	// 设置值
	found->setVal(v); 
	m_listIsEmpty[index] = false;
}

template<typename K, typename V>
V HashTabel<K, V>::get(const K& k) const
{
	Pair<K, V>* found = find(k);
	if (found) return found->getVal();
	return NULL;
}

template<typename K, typename V>
void HashTabel<K, V>::erase(const K& k)
{
	Pair<K, V>* found = find(k, false);
	if (!found) return;

	// 反查位置
	int index = indexOf(found);
	// 设置为空
	m_listIsEmpty[index] = true;
}

template<typename K, typename V>
void HashTabel<K, V>::print() const
{
	for (int i = 0; i < m_iLength; i++) {
		std::cout << "|[" <<  i << "]";
		if (!m_listIsEmpty[i])
			std::cout <<  m_listTable[i]->getKey() << ": " << \
					      m_listTable[i]->getVal() << "| ->";
		else
			std::cout << " -> ";
	}
	std::cout << std::endl;
}

//////////////////////////////////////////////////////////////////////////////////////
template<typename K, typename V>
class HashChains
{
public:
	HashChains(int length = 50, int d = 23);
	~HashChains();

	bool empty() const { return size() == 0; }
	int  size()  const {
		int cnt = 0;
		for (int i = 0; i < m_iLength; i++)
			cnt += m_listChains[i]->size();
		return cnt;
	}

	void insert(const K& k, const V& v);
	V get(const K& k) const;
	void erase(const K& k);
	void print() const;

private:
	size_t hash(const K k) const { return k % m_iD; }
	Pair<K, V>* find(const K& k, bool insert = true) const;
	ChainList<Pair<K, V>*>* findChain(const K& k) const { return m_listChains[hash(k)]; }

	int m_iD;
	ChainList<Pair<K, V>*>** m_listChains;
	int m_iLength;
};

template<typename K, typename V>
HashChains<K, V>::HashChains(int length, int d) :
	m_iLength(length), m_iD(d)
{
	m_listChains = new ChainList<Pair<K, V>*>* [m_iLength];
	for (int i = 0; i < m_iLength; i++)
		m_listChains[i] = new ChainList<Pair<K, V>*>;
}

template<typename K, typename V>
HashChains<K, V>::~HashChains()
{
	for (int i = 0; i < m_iLength; i++)
		delete m_listChains[i];
	delete[] m_listChains;
}

template<typename K, typename V>
Pair<K, V>* HashChains<K, V>::find(const K& k, bool insert) const
{
	int hv = hash(k);	// 求取hash值

	// 获取哈希值对应的桶
	ChainList<Pair<K, V>*>* chain = m_listChains[hv];

	// 在桶内依次查找
	ChainNode<Pair<K, V>*>* node = chain->getNode(0);
	while (node) {
		// 已到达链尾
		if (!node->next) {
			if (!insert) return nullptr;
			// 插入操作时，创建节点
			Pair<K, V>* p = new Pair<K, V>;
			chain->insert(0, p);
			return p;
		}
		// 找到对应键
		if (node->getVal()->getKey() == k)
			return node->getVal();
		node = node->next;
	}
	return nullptr;
}

template<typename K, typename V>
void HashChains<K, V>::insert(const K& k, const V& v)
{
	Pair<K, V>* p = find(k);
	if (p->getKey() != k) p->setKey(k);
	p->setVal(v);
}

template<typename K, typename V>
V HashChains<K, V>::get(const K& k) const
{
	Pair<K, V>* p = find(k, false);
	if (p) return p->getVal();
	return NULL;
}

template<typename K, typename V>
void HashChains<K, V>::erase(const K& k)
{
	ChainList<Pair<K, V>*>* chain = findChain(k);
	Pair<K, V>* p = find(k, false);
	chain->erase(chain->indexOf(p));
	delete p;
}

template<typename K, typename V>
void HashChains<K, V>::print() const
{
	for (int i = 0; i < m_iLength; i++) {
		ChainList<Pair<K, V>*>* chain = m_listChains[i];
		ChainNode<Pair<K, V>*>* node = chain->getNode(0);
		std::cout << "[" << i << "]";
		while (node && node->ptr) {
			Pair<K, V>* p = node->getVal();
			std::cout << p->getKey() << ":" << p->getVal() << " -> ";
			node = node->next;
		}
		std::cout << " | ";
	}
	std::cout << std::endl;
}