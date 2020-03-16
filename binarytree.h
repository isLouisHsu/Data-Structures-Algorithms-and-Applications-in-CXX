#pragma once
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>
#include "arraylist.h"
#include "linkedqueue.h"

template<typename T>
class ArrayBinaryTree: public ArrayList<T>
{
public:
	ArrayBinaryTree() :ArrayList<T>() {}
	ArrayBinaryTree(T* set, int n) {	// 由给定集合生成二叉树，空叶节点用`#`表示，形如`{3,9,20,#,#,15,7}`
		this->m_TElements = new T[n];
		std::copy(set, set + n, this->m_TElements);
		this->m_iCount = this->m_iSize = n;
	}
	ArrayBinaryTree(const ArrayBinaryTree<T>& tree) {
		int n = tree.size();
		this->m_TElements = new T[n];
		std::copy(tree.m_TElements, tree.m_TElements + n, this->m_TElements);
		this->m_iCount = this->m_iSize = n;
	}
	~ArrayBinaryTree() {}
	
	int size() { return sizeOfNode(0); }		// 二叉树结点数
	int height() { return heightofNode(0); }	// 二叉树高度
	bool empty() { return size() == 0; }		// 二叉树是否为空
	void print() const {
		int H = 0;
		for (int i = 0; i < this->m_iCount; i++) {
			int h = std::floor(std::log2(i + 1));
			if (h > H) {
				H = h;
				std::cout << std::endl;
			}
			std::cout << this->m_TElements[i] << ' ';
		}
		std::cout << std::endl;
	}

	// ------------------ 深度优先搜索 ------------------ 
	void preOrder(void (*visit)(int node), int node = 0) {	// 前序遍历
		if (!checkNode(node)) return; visit(node); preOrder(visit, left(node)); preOrder(visit, right(node)); }
	void inOrder(void (*visit)(int node), int node = 0) {	// 中序遍历
		if (!checkNode(node)) return; inOrder(visit, left(node)); visit(node); inOrder(visit, right(node)); }
	void postOrder(void (*visit)(int node), int node = 0) {	// 后序遍历
		if (!checkNode(node)) return; postOrder(visit, left(node)); postOrder(visit, right(node)); visit(node); }
	// ------------------ 广度优先搜索 ------------------ 
	void levelOrder(void (*visit)(int node), int node = 0) {	// 层次遍历
		if (!checkNode(node)) return; for (int i = node; i < this->m_iCount; i++) visit(i); }

protected:
	bool checkNode(int node) { 
		if (node < 0 || node >= this->m_iCount) return false;
		if (this->m_TElements[node] == (T)'#') return false;
		return true;
	}
	static int parent(int node) { return (node - 1) / 2; }
	static int left(int node) { return 2 * (node + 1) - 1; }
	static int right(int node) { return 2 * (node + 1); }

	int sizeOfNode(int node) {
		if (!checkNode(node)) return 0;
		int sizeL = sizeOfNode(left(node));
		int sizeR = sizeOfNode(right(node));
		return 1 + sizeL + sizeR;
	}
	int heightofNode(int node) {
		if (!checkNode(node)) return 0;
		int heightL = heightofNode(left(node));
		int heightR = heightofNode(right(node));
		return 1 + (heightL > heightR ? heightL : heightR);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class BinaryTreeNode
{
	// 左右子树的维护，放在二叉树`LinkedBinaryTree`中处理
public:
	BinaryTreeNode(BinaryTreeNode<T>* p = nullptr) : parent(p) {
		left  = right = nullptr;  value = new T; }
	~BinaryTreeNode() { delete value; }

	bool isLeft() const { if (!parent) return false; return parent->left == this; }// 是否为左子节点
	bool isRoot() const { return !parent; }				// 是否为根节点
	bool isLeaf() const { return (!left) && (!right); }	// 是否为叶子节点

	T get() const { return *value; }
	void set(const T& v) { *value = v; }

	BinaryTreeNode* parent;	// 增加父母节点信息
	BinaryTreeNode* left, *right;

private:
	T* value;
};

template<typename T>
class LinkedBinaryTree
{
public:
	LinkedBinaryTree();
	LinkedBinaryTree(T*, int);			// 由给定集合生成二叉树，空叶节点用`#`表示，形如`{3,9,20,#,#,15,7}`
	LinkedBinaryTree(const LinkedBinaryTree<T>&);
	~LinkedBinaryTree();

	int size();						// 二叉树结点数
	int height();					// 二叉树高度
	bool empty();					// 二叉树是否为空
	void print(int mode = 3);
	void swap();					// 交换二叉树的左右子节点

	// ------------------ 深度优先搜索 ------------------ 
	void preOrder(void (*visit)(BinaryTreeNode<T>* node), BinaryTreeNode<T>* node = m_tnRoot);		// 前序遍历
	void inOrder(void (*visit)(BinaryTreeNode<T>* node), BinaryTreeNode<T>* node = m_tnRoot);		// 中序遍历
	void postOrder(void (*visit)(BinaryTreeNode<T>* node), BinaryTreeNode<T>* node = m_tnRoot);		// 后序遍历
	// ------------------ 广度优先搜索 ------------------ 
	void levelOrder(void (*visit)(BinaryTreeNode<T> * node), BinaryTreeNode<T> * node = m_tnRoot);	// 层次遍历

protected:
	// 节点操作，注意需要定义为static成员函数，才能取址供遍历函数调用
	static BinaryTreeNode<T>* createNode(BinaryTreeNode<T>* p = nullptr);
	static void deleteNode(BinaryTreeNode<T>* node);
	static LinkedQueue<BinaryTreeNode<T>**>* tree2flattenQueue(BinaryTreeNode<T>** ptr);
	static void printNode(BinaryTreeNode<T>* node);
	static void swapNode(BinaryTreeNode<T>* node);
	static int  sizeofNode(BinaryTreeNode<T>* node);
	static int heightofNode(BinaryTreeNode<T>* node);

	BinaryTreeNode<T>* m_tnRoot;	// 根节点
};

template<typename T>
LinkedBinaryTree<T>::LinkedBinaryTree() 
{ 
	m_tnRoot = nullptr; 
}

template<typename T>
LinkedBinaryTree<T>::LinkedBinaryTree(T* set, int n)
{
	// 初始化根节点
	m_tnRoot = createNode(); m_tnRoot->set(set[0]);

	// 初始化队列
	LinkedQueue<BinaryTreeNode<T>*> queueParent;		// 中间过程使用的队列
	queueParent.push(m_tnRoot);							// 放入根节点

	int cnt = 0;
	while (queueParent.size() > 0) {
		BinaryTreeNode<T>* parent = queueParent.pop();	// 弹出父节点

		// 左子树
		if ((++cnt) >= n) break;
		T val = set[cnt];
		if (val != (T)'#') {
			BinaryTreeNode<T>* left = createNode(parent);	// 创建左子树
			left->set(val);									// 赋值左子树节点
			parent->left = left;							// 子节点链接到父节点
			queueParent.push(left);							// 置入队列
		}
		
		// 右子树
		if ((++cnt) >= n) break;
		val = set[cnt];
		if (val != (T)'#') {
			BinaryTreeNode<T>* right = createNode(parent);	// 创建右子树
			right->set(val);								// 赋值左子树节点
			parent->right = right;							// 子节点链接到父节点
			queueParent.push(right);						// 置入队列
		}
	}
}

template<typename T>
LinkedBinaryTree<T>::LinkedBinaryTree(const LinkedBinaryTree<T>& tree)
{
	// 初始化待拷贝数组
	LinkedQueue<BinaryTreeNode<T>**>* queuePtrFrom = tree2flattenQueue(const_cast<BinaryTreeNode<T>**>(&tree.m_tnRoot));
	LinkedQueue< BinaryTreeNode<T>*>* queueFrom = new LinkedQueue< BinaryTreeNode<T>*>;
	while (queuePtrFrom->size() > 0) { queueFrom->push(*(queuePtrFrom->pop())); }
	delete queuePtrFrom;

	// 更改类型为列表，便于查找，实际上不修改可也
	ChainList<BinaryTreeNode<T>*>* listFrom = queueFrom;
	ChainList<BinaryTreeNode<T>*>* listTo = new ChainList<BinaryTreeNode<T>*>;
	
	// 复制数组的值
	for (int i = 0; i < listFrom->size(); i++) {
		BinaryTreeNode<T>* nodeFrom = listFrom->get(i);
		BinaryTreeNode<T>* nodeTo = new BinaryTreeNode <T>;	// 创建节点
		nodeTo->set(nodeFrom->get());						// 复制值
		listTo->insert(listTo->size(), nodeTo);
	}

	// 链接节点间关系
	for (int i = 0; i < listFrom->size(); i++) {
		BinaryTreeNode<T>* nodeFrom = listFrom->get(i);
		BinaryTreeNode<T>* nodeTo   = listTo  ->get(i);

		int index = -1;
		// 父节点
		if (nodeFrom->parent) {
			index = listFrom->indexOf(nodeFrom->parent);
			nodeTo->parent = listTo->get(index);
		} else {
			m_tnRoot = nodeTo;
		}
		// 左右子树节点
		if (nodeFrom->left) {
			index = listFrom->indexOf(nodeFrom->left);
			nodeTo->left = listTo->get(index);
		}
		if (nodeFrom->right) {
			index = listFrom->indexOf(nodeFrom->right);
			nodeTo->right = listTo->get(index);
		}
	}
	delete listFrom; delete listTo;
}

template<typename T>
LinkedBinaryTree<T>::~LinkedBinaryTree() 
{ 
	postOrder(&deleteNode, m_tnRoot); 
}

template<typename T>
int LinkedBinaryTree<T>::size() 
{ 
	return sizeofNode(m_tnRoot); 
}			

template<typename T>
int LinkedBinaryTree<T>::height() 
{ 
	return heightofNode(m_tnRoot); 
}

template<typename T>
bool LinkedBinaryTree<T>::empty() 
{ 
	return size() == 0; 
}

template<typename T>
void LinkedBinaryTree<T>::print(int mode) {
	switch (mode)
	{
	case 0:
		std::cout << "preOrder: ";
		preOrder(&printNode, m_tnRoot);
		break;
	case 1:
		std::cout << "inOrder: ";
		inOrder(&printNode, m_tnRoot);
		break;
	case 2:
		std::cout << "postOrder: ";
		postOrder(&printNode, m_tnRoot);
		break;
	case 3:
		std::cout << "levelOrder: ";
		levelOrder(&printNode, m_tnRoot);
		break;
	default:
		break;
	}
	std::cout << std::endl;
}

template<typename T>
void LinkedBinaryTree<T>::swap()
{
	preOrder(&swapNode, m_tnRoot);
}

template<typename T>
void LinkedBinaryTree<T>::preOrder(void (*visit)(BinaryTreeNode<T>* node), BinaryTreeNode<T>* node)
{
	if (!node) return; 
	visit(node); 
	preOrder(visit, node->left); 
	preOrder(visit, node->right);
}

template<typename T>
void LinkedBinaryTree<T>::inOrder(void (*visit)(BinaryTreeNode<T>* node), BinaryTreeNode<T>* node) {
	if (!node) return; 
	inOrder(visit, node->left); 
	visit(node); 
	inOrder(visit, node->right);
}

template<typename T>
void LinkedBinaryTree<T>::postOrder(void (*visit)(BinaryTreeNode<T>* node), BinaryTreeNode<T>* node) {
	if (!node) return;
	postOrder(visit, node->right);
	postOrder(visit, node->left); 
	visit(node);
}

template<typename T>
void LinkedBinaryTree<T>::levelOrder(void (*visit)(BinaryTreeNode<T>* node), BinaryTreeNode<T>* node) {
	if (!node) return;
	LinkedQueue<BinaryTreeNode<T>**>* ordered = tree2flattenQueue(&node);
	while (ordered->size() > 0) { 
		BinaryTreeNode<T>** n = ordered->pop();
		visit(*n); 
	}
	delete ordered;
}

template<typename T>
LinkedQueue<BinaryTreeNode<T>**>* LinkedBinaryTree<T>::tree2flattenQueue(BinaryTreeNode<T>** ptr) {
	// - 需要获取`m_tnRoot`的地址，不可定义形参`BinaryTreeNode<T>* node = m_tnRoot`再将`&node`存储，因为函数返回后，形参`node`将被释放
	// - 注意这里要定义二级指针，否则在出队列时，会将节点破坏
	LinkedQueue<BinaryTreeNode<T>**>* queue = new LinkedQueue<BinaryTreeNode<T>**>;		// 中间过程使用的队列
	LinkedQueue<BinaryTreeNode<T>**>* ordered = new LinkedQueue<BinaryTreeNode<T>**>;	// 已排序的队列

	queue->push(ptr);
	while (queue->size() > 0) {
		BinaryTreeNode<T>** p = queue->pop();	// 弹出队首元素
		ordered->push(p);						// 将该节点存入已排序队列

		// 将子节点存入队列
		if ((*p)->left) queue->push(&((*p)->left));
		if ((*p)->right) queue->push(&((*p)->right));
	}
	delete queue;
	return ordered;
}

template<typename T>
BinaryTreeNode<T>* LinkedBinaryTree<T>::createNode(BinaryTreeNode<T>* p) 
{ 
	return new BinaryTreeNode<T>(p); 
}

template<typename T>
void LinkedBinaryTree<T>::deleteNode(BinaryTreeNode<T>* node) 
{ 
	if (!node) return; 
	delete node; 
}

template<typename T>
void LinkedBinaryTree<T>::printNode(BinaryTreeNode<T>* node) 
{ 
	if (!node) return; 
	std::cout << node->get() << " "; 
}

template<typename T>
void LinkedBinaryTree<T>::swapNode(BinaryTreeNode<T>* node)
{
	if (!node) return;
	BinaryTreeNode<T>* temp = node->left;
	node->left = node->right;
	node->right = temp;
}

template<typename T>
int  LinkedBinaryTree<T>::sizeofNode(BinaryTreeNode<T>* node) {
	if (!node) return 0;
	// 采用后续遍历
	int sizeL = sizeofNode(node->left);
	int sizeR = sizeofNode(node->right);
	return 1 + sizeL + sizeR;
}

template<typename T>
int LinkedBinaryTree<T>::heightofNode(BinaryTreeNode<T>* node) {
	if (!node) return 0;
	// 采用后续遍历
	int heightL = heightofNode(node->left);
	int heightR = heightofNode(node->right);
	return 1 + (heightL > heightR ? heightL : heightR);
}

/////////////////////////////////////////////////////////////////////////////////////
class Expression : protected LinkedBinaryTree<char>
{
public:
	Expression(char* symbols, int n) : LinkedBinaryTree<char>(symbols, n) { translate(); }
	~Expression() { if (expression) delete expression; }

	int size()   { return LinkedBinaryTree<char>::size(); }
	int height() { return LinkedBinaryTree<char>::height(); }
	bool empty() { return LinkedBinaryTree<char>::empty(); }
	void print(int mode = 3) { LinkedBinaryTree<char>::print(mode); }

	float fCalculate() const;
	char* expression;

private:
	void translate();
	static void translateNode(BinaryTreeNode<char>* node, char* buff, int n);
	static float fCalculateNode(BinaryTreeNode<char>* node);

	static float char2float(char& c) { return (float)(c - '0'); }
};

void Expression::translate()
{
	if (!expression) {
		expression = new char[256]; expression[0] = '\0';
	}
	translateNode(this->m_tnRoot, expression, 256);
}

void Expression::translateNode(BinaryTreeNode<char>* node, char* buff, int n)
{
	// 嵌套中止条件：叶节点
	if (node->isLeaf()) {
		sprintf_s(buff, n, "%s%c%c%c", buff, '(', node->get(), ')');
		return;
	}

	translateNode(node->left, buff, n);		// 左子树
	sprintf_s(buff, n, "%c%s%c", '(', buff, node->get());
	translateNode(node->right, buff, n);		// 右子树
	sprintf_s(buff, n, "%s%c", buff, ')');
}

float Expression::fCalculate() const
{
	return fCalculateNode(this->m_tnRoot);	// 计算根节点的值
}

float Expression::fCalculateNode(BinaryTreeNode<char>* node)
{
	if (!node) return 0;

	// 若为叶子节点，返回操作树
	if (node->isLeaf()) {
		char c = node->get();
		return char2float(c);
	}

	// 不为叶子节点，获取左右子树的运算结果
	float x = fCalculateNode(node->left);
	float y = fCalculateNode(node->right);
	// 从自身读取操作符，并运算
	char op = node->get();
	switch (op)
	{
	case '+':
		return x + y;
	case '-':
		return x - y;
	case '*':
		return x * y;
	case '/':
		return x / y;
	default:
		throw "未定义的运算符";
	}
}