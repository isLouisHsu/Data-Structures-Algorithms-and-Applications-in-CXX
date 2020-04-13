
#include <vector>

template<typename T>
int binarySearch(std::vector<T>& nums, T target) {
	// write your code here
	if (nums.size() == 0) return -1;

	int left = 0; int right = nums.size();
	while (left < right) {  // while (left != right) {
		int middle = (left + right) / 2;    // 向下取整，left和right必定能到达相等
		target > nums[middle] ? left = middle + 1 : right = middle;
		// target > nums[middle]时，middle必定不是；而target <= nums[middle]时，middle可能是(但可能不是第一次出现)
	}
	return nums[left] == target ? left : -1;
}

template<typename T>
int binarySearch(T* nums, int n, T target, bool leftFirst = true) {
	if (!nums || n == 0) return -1;

	int left = 0; int right = n;
	while (left < right) {
		if (leftFirst) {
			int middle = (left + right) / 2;		// 向下取整
			target > nums[middle] ? left = middle + 1 : right = middle;
		} else {
			int middle = (left + right) / 2 + 1;    // 向上取整
			target < nums[middle] ? right = middle - 1 : left = middle;
		}
	}
	return nums[left] == target ? left : -1;
}

int binary_search(std::vector<int> nums, int target) {
	int left = 0, right = nums.size() - 1;
	while (left <= right) {
		int mid = left + (right - left) / 2;
		if (nums[mid] < target) {
			left = mid + 1;
		}
		else if (nums[mid] > target) {
			right = mid - 1;
		}
		else if (nums[mid] == target) {
			// 直接返回
			return mid;
		}
	}
	// 直接返回
	return -1;
}

int binarysearch_bound(std::vector<int> nums, int target, bool mode=0) {
	// 注意左右闭区间
	int left = 0, right = nums.size() - 1;
	// 直至left > right终止
	while (left <= right) {
		int mid = left + (right - left) / 2;
		if (nums[mid] < target) {
			left = mid + 1;
		} else if (nums[mid] > target) {
			right = mid - 1;
		} else if (nums[mid] == target) {
			if (mode == 0) {
				return mid;
			}
			if (mode == 1) {
				// 别返回，锁定左侧边界
				right = mid - 1;
			} else if (mode == 2) {
				// 别返回，锁定右侧边界
				left = mid + 1;
			}
		}
	}
	// 越界检查
	if (mode == 1) {
		// 最后要检查 left 越界的情况
		if (left >= nums.size() || nums[left] != target) return -1;
		return left;
	} else if (mode == 2) {
		// 最后要检查 right 越界的情况
		if (right < 0 || nums[right] != target) return -1;
		return right;
	}
}

// ----------------------------------------------------------------------------------------------------
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
		findK(a, begin, left - 1, k);	// 在左侧寻找第`k`大的数，注意此时left忽略
	else
		findK(a, left, end, k);		// 在右侧寻找第`k-left`大的数
}

template<typename T>
int findK(T a[], int n, int k)
{
	return findK(a, 0, n, k);
}