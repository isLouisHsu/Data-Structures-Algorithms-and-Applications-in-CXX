#pragma once
#include <iostream>
#include <algorithm>
#include "binarytree.h"

template<typename T>
class MaxWinnerTree : public ArrayBinaryTree<T>
{
public:
	MaxWinnerTree() : ArrayBinaryTree<T>() {}
	MaxWinnerTree(T* set, int n);
	~MaxWinnerTree() {}

	void set(const int i, const T& value) { this->m_TElements[i + offset()] = value; }
	T& get(const int i) const { return this->m_TElements[i + offset()]; }
	int index(const T& value) const;

	void play(const int i = -1);    // 在忽略选手i的情况下比赛，即始终被视作输者
	void replay(const int i);       // 在i的值修改后，对i一系列进行重赛
	T& winner() const { return this->m_TElements[0]; }

protected:
	int offset() const { return (this->m_iCount - 1) / 2; }
};

template<typename T>
MaxWinnerTree<T>::MaxWinnerTree(T* set, int n)
{
	int size = 2 * n - 1;		// 包含n-1个内部节点与n个外部节点
	this->m_TElements = new T[size];
	std::copy(set, set + n, this->m_TElements + n - 1);
	this->m_iCount = this->m_iSize = size;
	play();
}

template<typename T>
void MaxWinnerTree<T>::play(const int i) {
	// 从最后一个内部节点开始竞赛
	for (int index = this->parent(this->m_iCount - 1); index >= 0; index--) {
		int left = this->left(index); int right = this->right(index);
		// 包含忽略的选手
		if (left - offset() == i && i >= 0)
			this->m_TElements[index] = this->m_TElements[right];
		else if (right - offset() == i && i >= 0)
			this->m_TElements[index] = this->m_TElements[left];
		// 所有选手参加比赛
		else
			this->m_TElements[index] = this->m_TElements[left] >= this->m_TElements[right] ? \
			this->m_TElements[left]: this->m_TElements[right];
	}
}

template<typename T>
void MaxWinnerTree<T>::replay(const int i) {
	// 对选手i进行重赛
	int index = this->parent(i + offset());	// 寻找父节点
	while (true) {
		index = this->parent(index);
		int left = this->left(index); 
		int right = this->right(index);
		this->m_TElements[index] = this->m_TElements[left] > this->m_TElements[right] ? \
			this->m_TElements[left]: this->m_TElements[right];
		if (index == 0) break;
	}
}

template<typename T>
int MaxWinnerTree<T>::index(const T& value) const
{
	T* start = this->m_TElements + offset();
	T* end = this->m_TElements + this->m_iCount;
	return (int)(std::find(start, end, value) - start);
}

////////////////////////////////////////////////////////////////////////////////////////////////
class BinPackingBF : public MaxWinnerTree<int>
{
public:
	BinPackingBF(int n = 8, int capacity = 10);	// 箱子数与箱子容量
	~BinPackingBF() { if (result) delete result; }

	void solve(int* objs, int n);		// 求解
	void print() const;

protected:
	int n_bins; int capacity;
	int* result; int n_objs;		// 保存每个箱子所在的箱子索引
};

BinPackingBF::BinPackingBF(int n, int capacity)
{
	int size = 2 * n - 1;
	this->m_TElements = new int[size];
	this->m_iCount = this->m_iSize = size;
	this->n_bins = n; this->capacity = capacity;
}

void BinPackingBF::solve(int* objs, int n)
{
	result = new int[n];  n_objs = n;

	// 保存每次物品放置前的箱子使用情况
	int* bins = new int[n_bins];
	std::memset(bins, 0, sizeof(int) * n_bins);

	for (int i = 0; i < n; i++) {
		if (objs[n] > capacity)
			throw "物体太大！";

		// 将当前箱子使用情况赋值给外部节点
		memcpy(this->m_TElements + this->offset(), bins, sizeof(int) * n_bins);

		// 尝试性查找当前剩余容量最小的箱子
		this->play();
		int used = this->winner();		// 当前已使用容量最大的箱子容量
		int index = this->index(used);	// 当前已使用容量最大的箱子索引
		if (capacity - used >= objs[i]) {	// 剩余容量大于物品大小
			result[i] = index;
			bins[index] += objs[i];
			continue;
		}

		// 当前剩余容量最小的箱子无法装入物品i
		int choosen = -1;
		while (choosen < 0) {
			this->m_TElements[index + offset()] = -1;	// 该箱子始终无法获胜
			this->play();					// 重新竞赛
			used = this->winner();			// 当前已使用容量最大的箱子容量
			index = this->index(used);		// 当前已使用容量最大的箱子索引
			if (capacity - used >= objs[i])
				choosen = index;
		}
		result[i] = choosen;
		bins[choosen] += objs[i];
	}

	delete[] bins;
}

void BinPackingBF::print() const
{
	for (int i = 0; i < n_objs; i++)
		std::cout << result[i] << ' ';
	std::cout << std::endl;
}