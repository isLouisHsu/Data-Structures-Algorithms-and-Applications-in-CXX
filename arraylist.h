s#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>

template<typename T> T f1(T x) { return x; }
template<typename T> T f2(T x) { return x % 10; }

template<typename T>
class ArrayList
{
public:
	// ���졢�������졢����
	ArrayList(int initSize = 10);
	ArrayList(const ArrayList<T>&);
	~ArrayList() { delete[] m_TElements;}

	// ������Ϣ
	bool empty() const { return m_iCount == 0; }
	int  size()  const { return m_iCount; }

	// Ԫ�ػ�ȡ����ѯ
	T&   get    (const int index)   const;
	int  indexOf(const T&  element) const;

	// �������
	void insert(const int index, const T& e);
	void erase (const int index);

	// ���
	void print() const;

protected:
	void checkIndex(int index) const { 
		if (index < 0 || index > m_iCount - 1) throw "��Ч������"; }
	void expend();

	T*  m_TElements;
	int m_iSize;
	int m_iCount;
};

template<typename T>
ArrayList<T>::ArrayList(int initSize) :
	m_iSize(initSize), m_iCount(0)
{
	if (m_iSize <= 0) throw "��Ч�����鳤��";
	m_TElements = new T[m_iSize];
}

template<typename T>
ArrayList<T>::ArrayList(const ArrayList<T>& a)
{
	m_iSize = a.m_iSize;
	m_iCount = a.m_iCount;
	m_TElements = new T[m_iSize];
	std::copy(a.m_TElements, a.m_TElements + m_iSize, m_TElements);
}

template<typename T>
T& ArrayList<T>::get(const int index) const
{
	checkIndex(index);
	return m_TElements[index];
}

template<typename T>
int ArrayList<T>::indexOf(const T& element) const
{
	int index = (int)(std::find(m_TElements, m_TElements + m_iSize, element) - m_TElements);
	// ��δ�ҵ�������-1
	return index == m_iSize - 1 ? -1 : index;
}

template<typename T>
void ArrayList<T>::insert(const int index, const T& e)
{
	if (m_iCount == 0 && index == 0) {
		m_TElements[0] = e; m_iCount++;
		return;
	}

	checkIndex(index);

	// ����������������
	if (m_iCount >= m_iSize - 1) 
		expend();

	// Ԫ�غ���
	std::copy(m_TElements + index, m_TElements + m_iCount, m_TElements + index + 1);

	// ����Ԫ�أ���������
	m_TElements[index] = e; m_iCount++;
}

template<typename T>
void ArrayList<T>::erase(const int index)
{
	checkIndex(index);

	if (index > m_iCount - 1) return;

	// ǰ��Ԫ��
	std::copy(m_TElements + index + 1, m_TElements + m_iCount, m_TElements + index);

	// �����Լ�
	m_iCount--;
}

template<typename T>
void ArrayList<T>::print() const
{
	for (int i = 0; i < m_iCount; i++)
		std::cout << m_TElements[i] << " ";
	std::cout << std::endl;
}

template<typename T>
void ArrayList<T>::expend()
{
	int size = 2 * m_iSize;

	// �±��ڴ�ռ䣬������
	T* elements = new T[size];
	std::copy(m_TElements, m_TElements + m_iSize, elements);

	// �ͷ�ԭ�ڴ棬����ָ��
	delete [] m_TElements;
	m_TElements = elements;

	// ������Ϣ
	m_iSize = size;
}

/***************************************************/
template<typename T>
class ChainNode
{
public:
	ChainNode() { ptr = nullptr; prev = nullptr; next = nullptr; }
	ChainNode(const T val) { ptr = new T; *ptr = val; prev = nullptr; next = nullptr; }
	~ChainNode()	 { if (ptr) delete ptr; }

	void setVal(T val)  { if (!ptr) ptr = new T;  *ptr = val;}
	T& getVal()       { if (!ptr) throw "invalid node"; return *ptr; }

	ChainNode* prev;
	ChainNode* next;
	T* ptr;
};

template<typename T>
class ChainList
{
public:
	// ���졢��������������
	ChainList() { m_cnHead = new ChainNode<T>; m_iCount = 0; }
	ChainList(const ChainList<T>&);
	~ChainList();

	// ������Ϣ
	bool empty() const { return !m_cnHead->ptr; }
	int  size()  const { return m_iCount; }

	// Ԫ�ػ�ȡ����ѯ
	ChainNode<T>* getNode(int index) const;
	T&   get(const int index)   const;
	int  indexOf(const T& value) const;

	// �������
	void insert(const int index, const T& value);
	void erase(const int index);

	// ���
	void print() const;

	// ����
	ChainList<int> binsort(int range, T(*pFunc)(T));
	ChainList<int> radixsort();
	ChainList<T> ordered(const ChainList<int> order);

private:
	void checkIndex(int index) const {
		if (index < 0 || index > m_iCount) throw "��Ч������";}

	ChainNode<T>* m_cnHead;
	int m_iCount;
};

template<typename T>
ChainList<T>::ChainList(const ChainList<T>& list)
{
	m_cnHead = new ChainNode<T>;
	m_iCount = 0;

	ChainNode<T>* node = list.m_cnHead;
	for (int i = 0; i < list.m_iCount && node; i++) {
		insert(i, node->getVal());
		node = node->next;
	}
}

template<typename T>
ChainList<T>::~ChainList()
{
	ChainNode<T>* node = m_cnHead;

	while (!node->ptr && !node) {
		ChainNode<T>* tmp = node->next;

		// �ͷŽڵ�ռ�
		node->~ChainNode();

		delete node;
		node = tmp;
	}
}

template<typename T>
ChainNode<T>* ChainList<T>::getNode(const int index) const
{
	checkIndex(index);

	// ������������ʽѰ�ҽڵ�
	ChainNode<T>* node = m_cnHead;
	for (int i = 0; i < index; i++)node = node->next;

	return node;
}

template<typename T>
T& ChainList<T>::get(const int index) const
{
	return getNode(index)->getVal();
}

template<typename T>
int ChainList<T>::indexOf(const T& value) const
{
	ChainNode<T>* node = m_cnHead;
	// ����
	int index = 0;
	while (node->ptr && node->getVal()!=value) {
		node = node->next;
		index++;
	}
	return index == m_iCount ? -1 : index;
}

template<typename T>
void ChainList<T>::insert(const int index, const T& value)
{
	// �����ڵ�
	ChainNode<T>* tmp = new ChainNode<T>(value);

	// ��ȡ�ڵ�
	ChainNode<T>* node = getNode(index);

	// ���ӣ�ע��ͷ�ڵ�Ĵ���
	if (node->prev) {
		node->prev->next = tmp; 
		tmp->prev = node->prev;
	}
	tmp->next = node; 
	node->prev = tmp;

	if (index == 0) m_cnHead = tmp;

	m_iCount++;
}


template<typename T>
void ChainList<T>::erase(const int index)
{
	// ��ȡ�ڵ�
	ChainNode<T>* node = getNode(index);
	if (index == 0) {
		m_cnHead = node->next; m_cnHead->prev = nullptr;}

	// ���ӣ��ͷţ�ע��ͷ�ڵ�Ĵ���
	if(node->prev) node->prev->next = node->next;
	node->~ChainNode();

	m_iCount--;
}

template<typename T>
void ChainList<T>::print() const
{
	ChainNode<T>* node = m_cnHead;

	while (node->ptr && node->next) {
		std::cout << node->getVal() << " ";
		node = node->next;
	}
	std::cout << std::endl;
}

template<typename T>
ChainList<int> ChainList<T>::binsort(int range, T (*pFunc)(T))
{
	// ��������(����±꣬��ʡ�ڴ棬��ʱ������ڲ�ѯ)
	ChainList<int>** bins = new ChainList<int>* [range];
	for (int i = 0; i < range; i++){
		bins[i] = new ChainList<int>;
	}

	// �����ݻ�������
	for (ChainNode<T>* node = m_cnHead; node->ptr; node = node->next) {
		T val = node->getVal();
		int index = pFunc? (*pFunc)(val): val;
		bins[index]->insert(0, indexOf(val));
	}

	for (int i = 0; i < range; i++) {
		bins[i]->print();
	}

	// ��������˳�򣬴洢������
	ChainList<int> order;
	for (int i = 0; i < range; i++) {
		for (ChainNode<int>* node = bins[i]->m_cnHead; node->ptr; node = node->next) {
			order.insert(order.size(), node->getVal());
		}
		delete bins[i];
	}
	delete [] bins;

	return order;
}

template<typename T>
ChainList<int> ChainList<T>::radixsort()
{
	// TODO:
	return;

	ChainList<int> order;

	bool isdone = false; T r = 1;
	while (!isdone) {

		ChainList<T> tmp;
		if (order.empty())
			tmp = ChainList<T>(*this);					// ���ÿ������캯�������
		else
			tmp = ChainList<T>(*this).ordered(order);	// ���ÿ������캯�������

		// ȡ��
		for (ChainNode<T>* node = tmp.m_cnHead; node->ptr; node = node->next)
			node->setVal(node->getVal() / r);
		r *= 10;
		tmp.print();

		// ���ݸ�λ����������
		order = tmp.binsort(10, f2);
		
		order.print();
		std::cout << std::endl;
		ChainList<T>(*this).ordered(order).print();
	}

	return order;
}

template<typename T>
ChainList<T> ChainList<T>::ordered(const ChainList<int> order)
{
	ChainList<T> output; int cnt = 0;
	for (ChainNode<int>* node = order.m_cnHead; node->ptr; node = node->next) {
		output.insert(cnt++, get(node->getVal()));
	}
	return output;
}
