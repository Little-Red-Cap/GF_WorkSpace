//
// Created by Joho on 2023/11/21.
//
#include <stdio.h>
void bubbleSort()
{
    //C实现
    int arr[] = {5, 9, 3, 8, 6};
    int len = sizeof(arr)/sizeof(arr[0]);
    int temp;
    for (int i = 0; i < len - 1; i++) //从小到大
    {                                 // 外循环为排序趟数，len个数进行len-1趟
        for (int j = 0; j < len - 1 - i; j++)
        { // 内循环为每趟比较的次数，第i趟比较len-i次,因为第一次已经将最大的元素冒泡到最后一个位置了
            if (arr[j] > arr[j + 1])
            { //相邻元素比较,逆序则将交换位置
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    //打印数组
    for (int i = 0; i < len; i++)
        printf("%d\t", arr[i]);
}

int main()
{
    bubbleSort();
}
