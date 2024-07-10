//
// Created by Joho on 2023/11/21.
//
#include <stdio.h>

void insertionSort()
{
    int arr[] = {1, 5, 3, 9, 7};
    int len = sizeof(arr) / sizeof(arr[0]);

    int i, j, key;
    for (i = 1; i < len; i++) //从1开始是因为默认第一个元素是最小的有序表
    {
        key = arr[i];
        j = i - 1; //a[j]是有序表最后一个元素
        while ((j >= 0) && (arr[j] > key)) //从后往前遍历并且判断arr[j]是否大于key
            //j>=0防止数组越界
        {
            arr[j + 1] = arr[j];//后移
            j--;//j前移
        }
        arr[j + 1] = key; //arr[j]是第一个比key小的元素,将key置于其后(比key大的有序表元素都已经后移一个位置了)
    }

    //打印数组
    for (int i = 0; i < len; i++)
        printf("%d\t", arr[i]);
}

int main()
{
    insertionSort();
}
