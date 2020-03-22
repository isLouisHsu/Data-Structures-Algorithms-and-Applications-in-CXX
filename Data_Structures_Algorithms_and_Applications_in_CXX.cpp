// Data_Structures_Algorithms_and_Applications_in_CXX.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <cmath>
#include <functional>
#include <algorithm>
#include <vector>
#include "sort.h"

using namespace std;

int main(int argc, char** argv)
{
    int numbers[10] = { 8, 4, 7, 3, 6, 1, 2, 9, 10, 5 };

    quickSort(numbers, 10);
    for (int i = 0; i < 10; i++)
        cout << numbers[i] << ' ';

	system("pause");
}

