#pragma once
#include <iostream>
#include "linkeddictionary.h"
#include "binarytree.h"

template<typename K, typename V>
class BinarySearchTree : 
	public LinkedBinaryTree<Pair<K, V>*>
{
public:
	BinarySearchTree() : LinkedBinaryTree<Pair<K, V>*>() {}
	~BinarySearchTree() {}

	BinaryTreeNode< Pair<K, V>*>* find(const K&, bool insert=false);
	void insert(const K&, const V&);
	void erase(const K&);
	void print();	// 注意
					// - 不能声明为const，否则无法调用非const的printPairNode；
					// - 类的静态成员无法声明为const。

	void ascending();

protected:
	static void printPairNode(BinaryTreeNode< Pair<K, V>*>*);
	static BinaryTreeNode< Pair<K, V>*>* max(BinaryTreeNode< Pair<K, V>*>*);
	static BinaryTreeNode< Pair<K, V>*>* min(BinaryTreeNode< Pair<K, V>*>*);
};

template<typename K, typename V>
BinaryTreeNode< Pair<K, V>*>* BinarySearchTree<K, V>::find(const K& key, bool insert)
{
	// 若树为空
	if (!this->m_tnRoot) {
		if (!insert) return nullptr;

		Pair<K, V>* p = new Pair<K, V>;
		this->m_tnRoot = new BinaryTreeNode< Pair<K, V>*>;
		this->m_tnRoot->set(p); return this->m_tnRoot;
	}

	// 树不为空
	BinaryTreeNode< Pair<K, V>*>* node = this->m_tnRoot;
	bool toLeft = false;
	while (node) {
		// 找到对应键的数值对
		if (node->get()->getKey() == key)
			return node;
		// 当前节点的键比搜查键更大，则向左子树搜索
		else if (node->get()->getKey() > key) {
			if (!node->left) {		// 左子树为空
				toLeft = true;
				break;
			}
			node = node->left;
		}
		// 当前节点的键比搜查键更小，则向右子树搜索
		else {
			if (!node->right) {		// 右子树为空
				toLeft = false;
				break;
			}
			node = node->right;
		}
	}
	// 未找到
	if (!insert) return nullptr;

	// 创建新节点
	Pair<K, V>* p = new Pair<K, V>;
	BinaryTreeNode< Pair<K, V>*>* n = \
		new BinaryTreeNode< Pair<K, V>*>(node);
	n->set(p);
	
	// 将节点加入搜索树
	if (toLeft)
		node->left = n;
	else
		node->right = n;

	return n;
}

template<typename K, typename V>
void BinarySearchTree<K, V>::insert(const K& key, const V& value)
{
	BinaryTreeNode< Pair<K, V>*>* node = find(key, true);
	Pair<K, V>* p = node->get();
	if (p->getKey() != key)
		p->setKey(key);
	p->setVal(value);
}

template<typename K, typename V>
void BinarySearchTree<K, V>::erase(const K& key)
{
	BinaryTreeNode< Pair<K, V>*>* node = find(key, false);
	if (!node) return;

	// ------------------ 重新组织二叉树 ------------------
	// 查找左子树的最大值，或右子树的最小值
	BinaryTreeNode< Pair<K, V>*>* replace = nullptr;
	while (!node->isLeaf())	{			// 直到搜索到叶节点为止
		if (node->left) {
			replace = max(node->left);
		}
		else if (node->right) {
			replace = min(node->right);
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
	} else {						// 无父节点，即整棵树只有一个根节点，则修改根节点为空
		this->m_tnRoot = nullptr;
	}

	// ------------------ 释放资源 ------------------
	Pair<K, V>* pair = node->get();
	delete pair; delete node;
}

template<typename K, typename V>
BinaryTreeNode< Pair<K, V>*>* BinarySearchTree<K, V>::max(BinaryTreeNode< Pair<K, V>*>* node)
{
	while (node->right) 
		node = node->right;
	return node;
}

template<typename K, typename V>
BinaryTreeNode< Pair<K, V>*>* BinarySearchTree<K, V>::min(BinaryTreeNode< Pair<K, V>*>* node)
{
	while (node->left)
		node = node->left;
	return node;
}

template<typename K, typename V>
void BinarySearchTree<K, V>::ascending() 
{
	this->inOrder(&printPairNode, this->m_tnRoot);
	std::cout << std::endl;
}

template<typename K, typename V>
void BinarySearchTree<K, V>::print()
{
	this->levelOrder(&printPairNode, this->m_tnRoot);
	std::cout << std::endl;
}

template<typename K, typename V>
void BinarySearchTree<K, V>::printPairNode(BinaryTreeNode< Pair<K, V>*>* node)
{
	if (!node) return;
	Pair<K, V>* p = node->get();
	std::cout << '[' << p->getKey() << ']' << p->getVal() << ' ';
}

///////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename V>
class IndexedBinarySearchTree :
	public BinarySearchTree<K, V>
{
public:
	IndexedBinarySearchTree() : BinarySearchTree<K, V>() {}
	~IndexedBinarySearchTree() {}

	BinaryTreeNode< Pair<K, V>*>* operator[] (int index);
};

template<typename K, typename V>
BinaryTreeNode< Pair<K, V>*>* IndexedBinarySearchTree<K, V>::operator[] (int index)
{
	if (index < 0) throw "索引必须大于0！";
	
	int leftSize = -1; int offset = 0;
	BinaryTreeNode< Pair<K, V>*>* node = this->m_tnRoot;

	while (leftSize != index && node) {
		leftSize = IndexedBinarySearchTree<K, V>::sizeofNode(node->left);
		if (offset + leftSize == index) {
			IndexedBinarySearchTree<K, V>::printPairNode(node);
			return node;
		}

		else if (offset + leftSize > index)
			node = node->left;
		else {
			node = node->right;
			offset += (leftSize + 1);
		}
	}

}