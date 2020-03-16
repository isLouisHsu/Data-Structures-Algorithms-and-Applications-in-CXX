#pragma once
#include <iostream>
#include "linkedstack.h"
#include "binarysearchtree.h"

template<typename K, typename V>
class AVLTree :
	public BinarySearchTree<K, V>
{
public:
	AVLTree() : BinarySearchTree<K, V>() {}
	~AVLTree() {}

	void insert(const K&, const V&);
	void erase(const K&);

protected:
	static int balancedFactor(BinaryTreeNode< Pair<K, V>*>*);
	static BinaryTreeNode< Pair<K, V>*>* findImbalancedAncestor(
		BinaryTreeNode< Pair<K, V>*>*, LinkedStack<bool>**);
};

template<typename K, typename V>
int AVLTree<K, V>::balancedFactor(BinaryTreeNode< Pair<K, V>*>* node)
{
	return AVLTree<K, V>::heightofNode(node->left) - \
		AVLTree<K, V>::heightofNode(node->right);
}

template<typename K, typename V>
BinaryTreeNode< Pair<K, V>*>* AVLTree<K, V>::\
	findImbalancedAncestor(BinaryTreeNode< Pair<K, V>*>* node, 
							LinkedStack<bool>** routine)
{
	// 向上查找节点A，并记录路径
	BinaryTreeNode< Pair<K, V>*>* A = node;
	int bf = balancedFactor(A);
	while (bf > -2 && bf < 2) {
		if (!A->parent) return nullptr;

		if (routine) {
			if (A == A->parent->left)
				(*routine)->push(false);
			else
				(*routine)->push(true);
		}
		
		A = A->parent;
		bf = balancedFactor(A);
	}
	return A;
}

template<typename K, typename V>
void AVLTree<K, V>::insert(const K& key, const V& value)
{
	// 查找插入位置
	BinaryTreeNode< Pair<K, V>*>* node = this->find(key, true);
	Pair<K, V>* p = node->get();
	if (p->getKey() != key)
		p->setKey(key);
	p->setVal(value);

	// 查找最近的不平衡祖先，并记录路径
	LinkedStack<bool>* routine = new LinkedStack<bool>;	// 向左为false
	BinaryTreeNode< Pair<K, V>*>* A = findImbalancedAncestor(node, &routine);
	if (!A) {
		delete routine;
		return;
	}

	// 判断不平衡类型，并相应旋转
	int type = (routine->pop() << 1) + routine->pop();
	switch (type)
	{
	case 0:			// LL
		this->leftRotate(A);
		break;
	case 1:			// LR
		this->rightRotate(A->left);
		this->leftRotate(A);
		break;
	case 2:			// RL
		this->leftRotate(A->right);
		this->rightRotate(A);
		break;
	case 3:			// RR
		this->rightRotate(A);
		break;
	default:
		break;
	}

	// 若A为根节点，旋转后需改变根节点指针
	if (!A->parent->parent) this->m_tnRoot = A->parent;

	delete routine;
}

template<typename K, typename V>
void AVLTree<K, V>::erase(const K& key)
{
	BinaryTreeNode< Pair<K, V>*>* node = this->find(key, false);
	if (!node) return;
	
	// 记录父节点与其兄弟节点
	BinaryTreeNode< Pair<K, V>*>* P = node->parent;

	// ------------------ 重新组织二叉树 ------------------
	// 查找左子树的最大值，或右子树的最小值
	BinaryTreeNode< Pair<K, V>*>* replace = nullptr;
	while (!node->isLeaf()) {			// 直到搜索到叶节点为止
		if (node->left) {
			replace = this->max(node->left);
		}
		else if (node->right) {
			replace = this->min(node->right);
		}

		if (replace) {					// 找到可替换子节点
			Pair<K, V>* np = node->get();
			Pair<K, V>* rp = replace->get();
			np->setKey(rp->getKey());
			np->setVal(rp->getVal());
			node = replace;
		}
	}

	if (node->parent) {				// 修改父节点信息
		if (node == node->parent->left)
			node->parent->left = nullptr;
		else
			node->parent->right = nullptr;
	}
	else {						// 无父节点，即整棵树只有一个根节点，则修改根节点为空
		this->m_tnRoot = nullptr;
	}
	
	// ------------------ 维持平衡二叉树 ------------------
	// 查找最近的不平衡祖先，并记录路径
	LinkedStack<bool>* routine = new LinkedStack<bool>;	// 向左为false
	BinaryTreeNode< Pair<K, V>*>* A = findImbalancedAncestor(P, &routine);

	// 向根节点进行
	while (A) {

		// 判断类型
		bool deleteRight = false;	// 若为`true`，表示R类型。否则L类型
		if (routine->empty()) {		// P即为A，此时必有一子节点为空
			if (!A->left) 			// 左节点为空
				deleteRight = false;
			else if (!A->right) 	// 右节点为空
				deleteRight = true;
		}
		else {
			deleteRight = routine->pop();
		}
		BinaryTreeNode< Pair<K, V>*>* B = \
			deleteRight ? A->left : A->right;		// 不平衡节点的另一侧子树
		int type = (deleteRight << 2) + balancedFactor(B);

		// 矫正
		switch (type)
		{
		case -1:		// L-1
			this->rightRotate(A);
			break;
		case 0:			// L0
			this->rightRotate(A);
			break;
		case 1:			// L1
			this->leftRotate(A->right);
			this->rightRotate(A);
			break;
		case 3:			// R-1
			this->rightRotate(A->left);
			this->leftRotate(A);
			break;
		case 4:			// R0
			this->leftRotate(A);
			break;
		case 5:			// R1
			this->leftRotate(A);
			break;
		default:
			break;
		}

		// 修改整棵树的根节点
		if (!A->parent->parent) this->m_tnRoot = A->parent;

		// 向根节点
		for (int i = 0; i < routine->size(); i++) routine->pop();	// 清除
		A = findImbalancedAncestor(A, &routine);
	}

	delete routine;
	Pair<K, V>* pair = node->get();
	delete pair; delete node;
}

///////////////////////////////////////////////////////////////////////////////////
template<typename K, typename V>
class RBTree : public BinarySearchTree<K, V>
{
	// TODO:
};

///////////////////////////////////////////////////////////////////////////////////
template<typename K, typename V>
class SplayTree : public BinarySearchTree<K, V>
{
public:
	SplayTree() : BinarySearchTree<K, V>() {}
	~SplayTree(){}

	void insert(const K&, const V&);
	void erase(const K&);

protected:
	void slay(BinaryTreeNode< Pair<K, V>*>*);

};

template<typename K, typename V>
void SplayTree<K, V>::slay(BinaryTreeNode< Pair<K, V>*>* node)
{
	// 分裂操作
	while (node->parent) {
		int type = node->parent->isRoot() << 2;
		if (type) {	// 无祖父节点，即父节点为根节点
			type += node->isLeft();
			this->m_tnRoot = node;		// 修改根节点
		}
		else {		// 有祖父节点
			type += (node->parent->isLeft() << 1) + node->isLeft();
			if (node->parent->parent->isRoot())
				this->m_tnRoot = node;	// 修改根节点
		}

		switch (type)
		{
		case 0b0101:	// L
			SplayTree<K, V>::leftRotate(node->parent);
			break;
		case 0b0100:	// R
			SplayTree<K, V>::rightRotate(node->parent);
			break;
		case 0b0011:	// LL
			SplayTree<K, V>::leftRotate(node->parent->parent);
			SplayTree<K, V>::leftRotate(node->parent);
			break;
		case 0b0010:	// LR
			SplayTree<K, V>::rightRotate(node->parent);
			SplayTree<K, V>::leftRotate (node->parent);
			break;
		case 0b0000:	// RR
			SplayTree<K, V>::rightRotate(node->parent->parent);
			SplayTree<K, V>::rightRotate(node->parent);
			break;
		case 0b0001:	// RL
			SplayTree<K, V>::leftRotate(node->parent);
			SplayTree<K, V>::rightRotate(node->parent);
			break;
		default:
			break;
		}
	}

}

template<typename K, typename V>
void SplayTree<K, V>::insert(const K& key, const V& value)
{
	BinaryTreeNode< Pair<K, V>*>* node = this->find(key, true);
	Pair<K, V>* p = node->get();
	if (p->getKey() != key)
		p->setKey(key);
	p->setVal(value);
	
	slay(node);
}

template<typename K, typename V>
void SplayTree<K, V>::erase(const K& key)
{
	BinaryTreeNode< Pair<K, V>*>* node = this->find(key, false);
	if (!node) return;

	BinaryTreeNode< Pair<K, V>*>* slayNode = node->parent;

	// ------------------ 重新组织二叉树 ------------------
	// 查找左子树的最大值，或右子树的最小值
	BinaryTreeNode< Pair<K, V>*>* replace = nullptr;
	while (!node->isLeaf()) {			// 直到搜索到叶节点为止
		if (node->left) {
			replace = this->max(node->left);
		}
		else if (node->right) {
			replace = this->min(node->right);
		}

		if (replace) {					// 找到可替换子节点
			Pair<K, V>* np = node->get();
			Pair<K, V>* rp = replace->get();
			np->setKey(rp->getKey());
			np->setVal(rp->getVal());
			node = replace;
		}
	}

	if (node->parent) {				// 修改父节点信息
		if (node == node->parent->left)
			node->parent->left = nullptr;
		else
			node->parent->right = nullptr;
	}
	else {						// 无父节点，即整棵树只有一个根节点，则修改根节点为空
		this->m_tnRoot = nullptr;
	}

	// ------------------ 释放资源 ------------------
	Pair<K, V>* pair = node->get();
	delete pair; delete node;

	slay(slayNode);
}
