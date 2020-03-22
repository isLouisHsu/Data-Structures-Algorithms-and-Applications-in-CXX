#pragma once
#include <iostream>

template<typename T>
void selectionSort(T a[], int n) 
{
	// 当n-1个数排序后，最后一个数也排序完毕，故进行n-1次循环即可
	for (int i = n - 1; i >= 1; i--) {	

		// 将索引为i的数与索引为0 ~ i-1的数进行比较，找到最大
		int index = i;
		for (int j = i - 1; j >= 0; j--) {
			if (a[j] > a[index]) index = j;
		}

		// 交换
		T temp = a[i]; a[i] = a[index]; a[index] = temp;
	}
}

template<typename T>
void bubbleSort(T a[], int n)
{
	int cmpCnt = 0; int mvCnt = 0;

	// 当n-1个数排序后，最后一个数也排序完毕，故进行n-1次循环即可
	for (int i = n - 1; i >= 1; i--) {
		// 一次冒泡操作，将最大数存储至索引为`i`处
		bool moved = false;
		for (int j = 0; j < i; j++) {

			cmpCnt++;
			if (a[j] > a[j + 1]) {
				mvCnt++;
				T temp = a[j]; a[j] = a[j + 1]; a[j + 1] = temp;
				moved = true;
			}
		}
		if (!moved) break;	// 本次冒泡中无元素移动，提前结束
	}

	std::cout << "比较" << cmpCnt << "次 移动" << mvCnt << "次" << std::endl;
}

template<typename T>
void merge(T a[], int begin, int middle, int end)
{
	int cmpCnt = 0; int mvCnt = 0;

	T* b = new T[end];

	// 将a中两个有序数组排序到b
	int i = begin, j = middle, k = begin;
	while (k < end) {
		int idx = 0;
		if (i == middle)
			idx = j++;
		else if (j == end)
			idx = i++;
		else {
			idx = a[i] < a[j] ? i++ : j++;
			cmpCnt++;
		}
		b[k++] = a[idx]; mvCnt++;
	}

	// 拷贝回a
	for (i = begin; i < end; a[i] = b[i++]) mvCnt++;
	delete [] b;

	std::cout << "比较" << cmpCnt << "次 移动" << mvCnt << "次" << std::endl;
}

template<typename T>
void mergeSort(T a[], int begin, int end)
{
	if (end - begin < 2) return;
	
	// 递归
	int middle = (begin + end) / 2;
	mergeSort(a, begin, middle);
	mergeSort(a, middle, end);

	merge(a, begin, middle, end);
}

template<typename T>
void mergeSort(T a[], int n)
{
	mergeSort(a, 0, n);
}

template<typename T>
void quickSort(T a[], int begin, int end)	// a[begin] ~ a[end - 1]
{
	int cmpCnt = 0; int mvCnt = 0;

	if (end - begin < 2) return;	// 仅包含1个数，无需排序

	T pivot = a[begin];	// 当前子数组的头元素视作支点
	int left = begin, right = end - 1;	// 左右索引
	while (true) {
		while (a[left] <= pivot && left < end) left++;	// 搜索从左至右大于支点的元素a[left]
		while (a[right] >= pivot && right > 0) right--;	// 搜索从右至左小于支点的元素a[right]

		cmpCnt++;
		if (left < right) {								// 交换a[left]与a[right]
			mvCnt++;
			T temp = a[left];
			a[left] = a[right];
			a[right] = temp;
		}
		else {
			break;
		}
	}
	// 此时索引为`begin + 1 ~ left - 1`的数都小于支点，索引为`left ~ end - 1`的数都大于支点；
	// 将支点调整至left - 1处，使支点左侧元素都小于支点，右侧都大于支点
	mvCnt++;
	a[begin] = a[left - 1];
	a[left - 1] = pivot;

	// 划分子数组，递归
	quickSort(a, begin, left - 1);
	quickSort(a, left, end);

	std::cout << "比较" << cmpCnt << "次 移动" << mvCnt << "次" << std::endl;
}

template<typename T>
void quickSort(T a[], int n)
{
	quickSort(a, 0, n);
}

template<typename T>
int findK(T a[], int begin, int end, int k)	// a[begin] ~ a[end - 1]
{
	if (end - begin < 2) return a[begin];	// 仅包含1个数，一定为查找的数

	T pivot = a[begin];	// 当前子数组的头元素视作支点
	int left = begin, right = end - 1;	// 左右索引
	while (true) {
		while (a[left] <= pivot && left < end) left++;	// 搜索从左至右大于支点的元素a[left]
		while (a[right] >= pivot && right > 0) right--;	// 搜索从右至左小于支点的元素a[right]

		if (left < right) {								// 交换a[left]与a[right]
			T temp = a[left];
			a[left] = a[right];
			a[right] = temp;
		}
		else {
			break;
		}
	}
	// 此时索引为`begin + 1 ~ left - 1`的数都小于支点，索引为`left ~ end - 1`的数都大于支点；
	// 将支点调整至left - 1处，使支点左侧元素都小于支点，右侧都大于支点
	a[begin] = a[left - 1];
	a[left - 1] = pivot;

	// 划分子数组，递归
	if (k < left)
		findK(a, begin, left - 1, k);	// 在左侧寻找第`k`大的数
	else
		findK(a, left, end, k);		// 在右侧寻找第`k-left`大的数
}

template<typename T>
int findK(T a[], int n, int k)
{
	return findK(a, 0, n, k);
}