#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

template<typename T>
void selectionSort(T a[], int n) 
{
	if (a == nullptr || n <= 0) return;

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

// -----------------------------------------------------------------------------------------------------
template<typename T>
void bubbleSort(T a[], int n)
{
	if (a == nullptr || n <= 0) return;

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

// -----------------------------------------------------------------------------------------------------
template<typename T>
void merge(T a[], int begin, int middle, int end)
{
	if (a == nullptr || begin > middle || middle > end) return;

	int cmpCnt = 0; int mvCnt = 0;

	// --------------- 借助辅助空间，将a数字拷入，再拷回来 ----------------
	//T* b = new T[end];

	//// 将a中两个有序数组排序到b
	//int i = begin, j = middle, k = begin;
	//while (k < end) {
	//	int idx = 0;
	//	if (i == middle)
	//		idx = j++;
	//	else if (j == end)
	//		idx = i++;
	//	else {
	//		idx = a[i] < a[j] ? i++ : j++;
	//		cmpCnt++;
	//	}
	//	b[k++] = a[idx]; mvCnt++;
	//}

	//// 拷贝回a
	//for (i = begin; i < end; a[i] = b[i++]) mvCnt++;
	//delete [] b;


	// ----------------- 改进：减少空间、时间复杂度 ---------------------
	int n = end - middle;
	T* b = new T[n];
	memcpy(b, a + middle, n * sizeof(T));

	T* ptr = a + end - 1;
	T* ptr1 = a + middle - 1;
	T* ptr2 = b + n - 1;

	// 从后往前覆盖a的值
	while (ptr >= a + begin) {

		T* p = nullptr;
		if (ptr1 < a + begin) {
			p = ptr2; ptr2--;
		} else if (ptr2 < b) {
			p = ptr1; ptr1--;
		} else {
			if (*ptr1 > * ptr2) {
				p = ptr1; ptr1--;
			} else {
				p = ptr2; ptr2--;
			}
		}

		*ptr = *p;
		ptr--;
	}
	// --------------------------------------------------------------

	std::cout << "比较" << cmpCnt << "次 移动" << mvCnt << "次" << std::endl;
	delete [] b;
}

template<typename T>
void mergeSort(T a[], int begin, int end)
{
	if (a == nullptr || begin <= end) return;
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

// -----------------------------------------------------------------------------------------------------
template<typename T>
int partition1(T a[], int begin, int end)
{
	if (a == nullptr || begin >= end) return -1;

	T pivot = a[begin];	// 当前子数组的头元素视作支点
	int left = begin, right = end - 1;	// 左右索引
	while (true) {
		while (a[left] <= pivot && left < end) left++;	// 搜索从左至右大于支点的元素a[left]
		while (a[right] >= pivot && right > begin) right--;	// 搜索从右至左小于支点的元素a[right]

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
	return left - 1;
}

template<typename T>
int partition2(T a[], int begin, int end)
{
	if (a == nullptr || begin >= end) return -1;

	// 当前子数组的头元素视作支点
	T pivot = a[begin];	
	// 快慢指针
	int slow = begin, fast = begin;
	while (true) {
		// 先走一步
		fast++;
		if (fast == end) break;

		// 快指针不停的走，直到值遇到不大于支点的节点
		if (a[fast] <= pivot) {
			// 慢指针跟上一步
			slow++;
			// 交换
			T temp = a[slow];
			a[slow] = a[fast];
			a[fast] = temp;
		}
	}
	// 支点放置到合适位置
	a[begin] = a[slow];
	a[slow] = pivot;
	return slow;
}

template<typename T>
int partition3(T a[], int begin, int end)
{
	if (a == nullptr || begin >= end) return -1;

	// 当前子数组的头元素视作支点
	T pivot = a[begin];
	// 左右指针
	bool flag = true;	// 指针移动标志
	int left = begin, right = end - 1;
	while (true) {
		// 指针顺序查找
		if (flag) {
			// 从右到左查找不小于pivot的元素
			while (right >= 0 && a[right] >= pivot) right--;
			if (left >= right) return left;
		} else {
			// 从左到右查找大于pivot的元素
			while (left < end && a[left] <= pivot) left++;
			if (left >= right) return right;
		}
		// 交换元素
		T temp = a[left];
		a[left] = a[right];
		a[right] = temp;
		// 交换方向
		flag = !flag;
	}
}

template<typename T>
void quickSort(T a[], int begin, int end)	// a[begin] ~ a[end - 1]
{
	if (a == nullptr || begin >= end) return;
	if (end - begin < 2) return;	// 终止条件：仅包含1个数，无需排序

	int pivot = partition2(a, begin, end);
	if (pivot == -1) return;
	// 划分子数组，递归
	quickSort(a, begin, pivot);
	quickSort(a, pivot + 1, end);

}

template<typename T>
void quickSort(T a[], int n)
{
	if (a == nullptr || n <= 0) return;
	quickSort(a, 0, n);
}

// -----------------------------------------------------------------------------------------------------
template<typename T>
void adjust(T a[], int n, int index)	// 调整元素a[index]在最大堆中的位置
{
	if (a == nullptr || n <= 0 || index < 0 || index >= n) return;

	// 判断元素与左右子节点元素的值大小关系，若不满足最大堆则递归调整位置
	for (int c = 1; c <= 2; c++) {
		int child = 2 * index + c;	// 左右子节点的索引
		if (child >= n) return;		// 无子节点，无需调整

		if (a[index] < a[child]) {	// 该节点不满足最大堆，进行调整
			T temp = a[index]; a[index] = a[child]; a[child] = temp;
			adjust(a, n, child);	// 继续调整直至叶子节点
		}
	}
}

template<typename T>
void heapSort(T a[], int n)
{
	if (a == nullptr || n <= 0) return;
	
	// 初始化最大堆(需从最后一个节点开始)
	for (int i = n - 1; i >= 0; adjust(a, n, i--));

	// 当n-1个数排序后，最后一个数也排序完毕，故进行n-1次循环即可
	for (int i = n - 1; i >= 1; i--) {
		// 堆顶元素依次出堆，与末尾元素交换
		T temp = a[i]; a[i] = a[0]; a[0] = temp;
		// 调整堆顶元素位置
		adjust(a, i, 0);
	}
}

template<typename T>
void heapSort(std::vector<T>& nums)
{
	if (nums.size() < 2) return;
	for (int i = nums.size() - 1; i >= 0; i--) {
		// 构筑最大堆
		std::make_heap(nums.begin(), nums.begin() + i + 1, std::less<T>());
		// 将堆顶元素放置到末尾
		std::pop_heap(nums.begin(), nums.begin() + i + 1, std::less<T>());
	}
}

// -----------------------------------------------------------------------------------------------------
template<typename T>
void binSort(std::vector<T>& nums)
{
	if (nums.size() < 2) return;
	// 寻找最小最大元素
	T minVal = *std::min_element(nums.begin(), nums.end());
	T maxVal = *std::max_element(nums.begin(), nums.end());
	// 构筑“箱子”
	std::vector<T> bins(maxVal - minVal + 1);
	for (int i = 0; i < nums.size(); i++) {
		bins[nums[i] - minVal]++;
	}
	// 收集元素
	int k = 0;
	for (int i = 0; i < bins.size(); i++) {
		if (bins[i] == 0) continue;
		for (int j = 0; j < bins[i]; j++) {
			nums[k++] = i + minVal;
		}
	}
}

// -----------------------------------------------------------------------------------------------------
template<typename T>
int digit(T num, int d)
{
	int base = (int)std::pow(10, d);
	return num / base % 10;
}

template<typename T>
void radixSort(std::vector<T>& nums)
{
	if (nums.size() < 2) return;
	// 构筑“桶”
	std::vector<std::vector<T> > bins(10);

	int d = 0;	// 位标
	while (true) {
		// 数字放入桶
		for (int i = 0; i < nums.size(); i++) {
			int index = digit(nums[i], d);
			bins[index].push_back(nums[i]);
		}
		// 数字排序完毕
		if (bins[0].size() == nums.size()) {
			break;
		}
		// 从桶中收集数字
		int k = 0;
		for (int i = 0; i < bins.size(); i++) {
			while (bins[i].size() > 0) {
				auto it = bins[i].begin();
				nums[k++] = *it;
				bins[i].erase(it);
			}
		}
		d++;
	}
}


// -----------------------------------------------------------------------------------------------------
template<typename T>
void insertSort(std::vector<T>& nums)
{
	if (nums.size() < 2) return;

	for (int i = 0; i < nums.size(); i++) {
		int num = nums[i];
		// 在`0, ..., i - 1`中寻找合适的插入位置
		for (int j = 0; j < i; j++) {
			// `j`处大于`num`，为插入位置
			if (nums[j] > num) {
				// 从后向前挪动
				for (int k = i; k > j; k--) {
					nums[k] = nums[k - 1];
				}
				// 存放`num`在`j`处
				nums[j] = num;
				break;
			}
		}
	}
}

// -----------------------------------------------------------------------------------------------------
template<typename T>
void shellInsertSort(std::vector<T>& nums, int index, int gk)
{
	for (int i = index; i < nums.size(); i += gk) {
		int num = nums[i];

		// 在`index, index + gk, index + 2 * gk, ...`中寻找合适的插入位置
		for (int j = index; j < i; j += gk) {

			// `j`处大于`num`，为插入位置
			if (nums[j] > num) {
				// 从后向前挪动
				for (int k = i; k > j; k -= gk) {
					nums[k] = nums[k - gk];
				}

				// 存放`num`在`j`处
				nums[j] = num;
				break;
			}
		}
	}
}

template<typename T>
void shellSort(std::vector<T>& nums)
{
	if (nums.size() < 2) return;

	// 希尔增量：`{n/2, n/4, ..., 4, 2, 1}`
	for (int gk = nums.size() / 2; gk > 0; gk /= 2) {

		// 划分为`gk`个组，每组`n/2-1 ~ n/2`个数
		for (int k = 0; k < gk; k++) {

			// 对分组数据`nums[k], nums[k + gk], nums[k + 2 * gk]... `进行选择排序
			shellInsertSort(nums, k, gk);
		}
	}
}

// -----------------------------------------------------------------------------------------------------
template<typename T>
void bucketSort(std::vector<T>& nums)
{
	if (nums.size() < 2) return;
	// 寻找最小最大元素
	T minVal = *std::min_element(nums.begin(), nums.end());
	T maxVal = *std::max_element(nums.begin(), nums.end());
	// 构筑“桶”，间隔为`10`
	std::vector<std::vector<T> > bins((maxVal - minVal) / 10 + 1);
	for (int i = 0; i < nums.size(); i++) {
		// 映射
		int index = (nums[i] - minVal) / 10;
		bins[index].push_back(nums[i]);
	}
	// 桶内排序，并取出
	int k = 0;
	for (int i = 0; i < bins.size(); i++) {
		std::sort(bins[i].begin(), bins[i].end());
		while (!bins[i].empty()) {
			auto it = bins[i].begin();
			nums[k++] = *it;
			bins[i].erase(it);
		}
	}
}