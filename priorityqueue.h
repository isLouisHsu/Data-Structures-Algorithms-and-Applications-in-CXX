#pragma once
#include <iostream>
#include "linkedqueue.h"
#include "binarytree.h"

template<typename T>
class Element
{
public:
	Element(const T& value, const int priority) {
		v = new T; *v = value;
		p = new int; *p = priority;
	}
	~Element() { 
		delete p; delete v; }

	void setPriority(const int priority) { *p = priority; }
	void setValue(const T& value) { *v = value; }

	int getPriority() const { return *p; }
	T   getValue() const { return *v; }

private:
	T* v;
	int* p;
};

template<typename T>
class PriorityQueue
{
public:
	virtual bool empty() const = 0;
	virtual int size() const = 0;

	virtual void push(const T& value, const int priority) = 0;
	virtual T pop() = 0;
	virtual T top() const = 0;
};

////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class MaxHeap : public LinkedBinaryTree<Element<T>*>
{
public:
	MaxHeap() : LinkedBinaryTree<Element<T>*>() {}
	~MaxHeap() {}

	void print();

	void push(const T& value, const int priority);
	T pop();
	T top() const { return this->m_tnRoot->get()->getValue(); }
	int maxPriority() const { return this->m_tnRoot->get()->getPriority(); }

protected:
	static void insert(BinaryTreeNode<Element<T>*>* node, BinaryTreeNode<Element<T>*>** tree);
};

template<typename T>
void MaxHeap<T>::insert(BinaryTreeNode<Element<T>*>* node, BinaryTreeNode<Element<T>*>** tree)
{
	BinaryTreeNode<Element<T>*>* root = (*tree)->parent; (*tree)->parent = nullptr;				// 保存根节点信息，不将tree视作子树

	LinkedQueue<BinaryTreeNode<Element<T>*>**>* queue = MaxHeap<T>::tree2flattenQueue(tree);	// 调用继承的父类静态函数
	while (queue->size() > 0) {
		// 根据完全二叉树，寻找堆的最后一个位置
		BinaryTreeNode<Element<T>*>* parent = *queue->pop();

		/************************** 插入节点 **************************/
		// 无子树空，不进行节点插入
		if (parent->left && parent->right) continue;

		// 存在子树为空，优先对左子树进行插入
		if (!parent->left) {
			parent->left = node;
		}
		else if (!parent->right) {
			parent->right = node;
		}
		node->parent = parent;
		/************************** 插入节点 **************************/

		/************************ 调整节点顺序 ************************/
		while (node->get()->getPriority() > parent->get()->getPriority()) {

			// 交换父节点与子节点的值
			Element<T>* temp = node->get(); node->set(parent->get()); parent->set(temp);
			node = node->parent;

			///************ 修改节点间的链接关系 **************/
			//// 1. 修改插入节点的父节点信息
			//node->parent = parent->parent;
			//// 2. 修改父节点的父节点的子树信息
			//if (parent->parent) {
			//	// 2.1 父节点不为根节点，则将祖父节点作为父节点
			//	if (parent->parent->left == parent) {
			//		parent->parent->left = node;
			//	}
			//	else {
			//		parent->parent->right = node;
			//	}
			//}
			//else {
			//	// 2.2 若父节点是树的根节点，则修改为根节点
			//	*tree = node;
			//}
			//// 3. 修改插入节点的子树信息、父节点的子树信息
			//BinaryTreeNode<Element<T>*>* temp;
			//if (parent->left == node) {
			//	// 3.1 插入节点为父节点的左子节点
			//	// 3.1.1 左子树更新
			//	temp = node->left;
			//	node->left = parent;
			//	parent->left = temp;
			//	// 3.1.2 右子树互换
			//	temp = node->right;
			//	node->right = parent->right;
			//	parent->right = temp;
			//}
			//else {
			//	// 3.2 插入节点为父节点的右子节点
			//	// 3.2.1 右子树更新
			//	temp = node->right;
			//	node->right = parent;
			//	parent->right = temp;
			//	// 3.1.2 左子树互换
			//	temp = node->left;
			//	node->left = parent->left;
			//	parent->left = temp;
			//}
			//// 4. 修改父节点的父节点信息
			//parent->parent = node;
			///************ 修改节点间的链接关系 **************/

			/******* 更新node的父节点，多次“起泡”操作 *******/
			parent = node->parent;
			if (!parent) break;
			/******* 更新node的父节点，多次“起泡”操作 *******/
		}
		break;
	}
	/************************ 调整节点顺序 ************************/

	(*tree)->parent = root;	// 恢复子树身份
	delete queue;
}

template<typename T>
void MaxHeap<T>::print()
{
	if (this->empty()) return;

	// 根据优先级打印堆
	LinkedQueue<BinaryTreeNode<Element<T>*>**>* queue = this->tree2flattenQueue(&this->m_tnRoot);
	while (queue->size() > 0) {
		Element<T>* element = (*(queue->pop()))->get();
		std::cout << '[' << element->getPriority() << ']' \
			<< element->getValue() << " ";
	}
	std::cout << std::endl;
	delete queue;
}

template<typename T>
void MaxHeap<T>::push(const T& value, const int priority)
{
	// 创建一个节点
	Element<T>* element = new Element<T>(value, priority);
	BinaryTreeNode<Element<T>*>* node = this->createNode(); node->set(element);

	// 无元素时，直接放在根节点，退出函数
	if (this->empty()) { this->m_tnRoot = node; return; }

	insert(node, &this->m_tnRoot);
}

//template<typename T>
//T MaxHeap<T>::pop()
//{
//	if (this->empty()) throw "堆为空！";
//
//	/**************** 取出根节点与左右子树 ****************/
//	BinaryTreeNode<Element<T>*>* toDel = this->m_tnRoot;
//	T value = toDel->get()->getValue();
//
//	// 若根节点已经是叶节点，返回值并退出
//	if (this->m_tnRoot->isLeaf()) {
//		this->m_tnRoot = nullptr;	// 注意根节点的变更
//		delete toDel; return value;
//	}
//
//	BinaryTreeNode<Element<T>*>* left  = this->m_tnRoot->left ;
//	BinaryTreeNode<Element<T>*>* right = this->m_tnRoot->right;
//	/**************** 取出根节点与左右子树 ****************/
//
//	/********************* 选择枝干 **********************/
//	BinaryTreeNode<Element<T>*>* trunk = nullptr;
//	BinaryTreeNode<Element<T>*>* branch = nullptr;
//
//	if (!right) {
//		trunk = left;
//	} else {
//		// 将节点多的作为主枝
//		if (LinkedBinaryTree<Element<T>*>::sizeofNode(left) < \
//			LinkedBinaryTree<Element<T>*>::sizeofNode(right)) {
//			trunk = right; branch = left;
//		} else {
//			branch = right; trunk = left;
//		}
//	}
//	trunk->parent = nullptr; 
//	this->m_tnRoot = trunk;			// 注意根节点的变更
//
//	// 若只包含主枝干，则无需进行插入操作
//	if (!branch) {
//		delete toDel; return value;
//	}
//	/********************* 选择枝干 **********************/
//
//	/**************** 将branch插入trunk *****************/
//	// 创建节点链表
//	LinkedQueue<BinaryTreeNode<Element<T>*>**>* queue = \
//				LinkedBinaryTree<Element<T>*>::tree2flattenQueue(&branch);
//	LinkedQueue<BinaryTreeNode<Element<T>*>*>* nodes = \
//				new LinkedQueue<BinaryTreeNode<Element<T>*>*>;
//	while (queue->size() > 0) nodes->push(*queue->pop());
//
//	// 依次插入
//	while (nodes->size() > 0) {
//		BinaryTreeNode<Element<T>*>* node = nodes->pop();
//		// BinaryTreeNode<Element<T>*>* node = *queue->pop();
//		if (!node) continue;
//
//		node->parent = node->left = node->right = nullptr;
//		insert(node, &this->m_tnRoot);
//	}
//	delete nodes;
//
//	// 释放节点，返回值
//	delete queue; delete toDel; return value;
//	/**************** 将branch插入trunk *****************/
//}

template<typename T>
T MaxHeap<T>::pop()
{
	if (this->empty()) throw "堆为空！";

	// 取出堆顶元素
	T value = this->m_tnRoot->get()->getValue();

	// 最后一个叶子节点
	LinkedQueue<BinaryTreeNode<Element<T>*>**>* queue = MaxHeap<T>::tree2flattenQueue(&this->m_tnRoot);	// 调用继承的父类静态函数
	BinaryTreeNode<Element<T>*>* node = *(queue->back());
	delete queue;

	// 覆盖根节点的值
	delete this->m_tnRoot->get();
	this->m_tnRoot->set(node->get());

	// 释放叶子节点
	if (!node->isRoot()) {
		if (node->isLeft()) 
			node->parent->left = nullptr;
		else 
			node->parent->right = nullptr;
	} else {
		this->m_tnRoot = nullptr;
	}
	delete node;

	// 调整堆顶元素数据
	node = this->m_tnRoot;
	while (node) {
		if (node->left && node->get()->getPriority() < node->left->get()->getPriority()) {
			// 交换节点的值
			Element<T>* temp = node->get(); node->set(node->left->get()); node->left->set(temp);
			node = node->left; continue;
		} 
		if (node->right && node->get()->getPriority() < node->right->get()->getPriority()) {
			// 交换节点的值
			Element<T>* temp = node->get(); node->set(node->right->get()); node->right->set(temp);
			node = node->right; continue;
		}
		break;
	}

	return value;
}