// Data_Structures_Algorithms_and_Applications_in_CXX.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <cmath>
#include <functional>
#include <algorithm>
#include "balancedbinarysearchtree.h"

using namespace std;

int main(int argc, char** argv)
{
	string names[6] = {
		"甲", "乙", "丙", "丁", "戊", "己" };
	int numbers[6] = { 2, 3, 4, 1, 5, 6 };

	SplayTree<int, string> tree;

	// 插入键值对
	cout << endl << "插入" << endl;
	for (int i = 0; i < 6; i++) {
		tree.insert(numbers[i], names[i]);
	}
	tree.print();

	// 查询2
	tree.insert(6, "乙");
	tree.print();

	// 查询3
	tree.insert(3, "丙");
	tree.print();

	// 删除2
	tree.erase(2);
	tree.print();

	system("pause");
}

