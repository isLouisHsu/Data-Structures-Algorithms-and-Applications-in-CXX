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
	string names[8] = {
		"甲", "乙", "丙", "丁", "戊", "己", "庚", "辛" };
	int numbers[8] = { 20, 15, 25, 12, 10, 23, 24, 19 };

	AVLTree<int, string> tree;

	// 插入键值对
	cout << endl << "插入" << endl;
	for (int i = 0; i < 8; i++) {
		cout << "插入" << numbers[i] << ": ";
		tree.insert(numbers[i], names[i]);
		tree.print();
	}

	// 删除键值对
	cout << endl << "删除" << endl;
	tree.erase(10); 
	tree.print();	// 12 L-1不平衡

	tree.erase(12); 
	tree.erase(15);
	tree.erase(19); 
	tree.print();	// L0不平衡


	// 插入键值对
	cout << endl << "重新插入" << endl;
	for (int i = 0; i < 8; i++) {
		tree.insert(numbers[i], names[i]);
	}
	tree.print();

	tree.erase(25); 
	tree.erase(12); 
	tree.erase(10); 
	tree.erase(15); 
	tree.erase(19); 
	tree.print();	// L1不平衡

	system("pause");
}

