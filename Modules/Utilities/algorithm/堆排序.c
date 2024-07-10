//
// Created by Joho on 2023/11/21.
//
#include <stdio.h>

void swap(int *a, int *b);
void heap_Sort(int arr[], int len);

int heapSort()
{
    int arr[] = {3, 5, 3, 0, 8, 6, 1, 5, 8, 6, 2, 4, 9, 4, 7, 0, 1, 8, 9, 7, 3, 1, 2, 5, 9, 7, 4, 0, 2, 6};
    int len = (int) sizeof(arr) / sizeof(*arr);
    for (int i = len; i > 1; i--)
        heap_Sort(arr, i); //建立堆 每次规模减1

    //打印结果
    for (int i = 0; i < len; i++)
        printf("%d ", arr[i]);
    return 0;
}

//构造一个大顶堆并将最大值换至最后一位
void heap_Sort(int arr[], int len)
{
    int dad = len / 2 - 1; //最后一个父节点
    int son = 2 * dad + 1; //该父节点下的首个子节点
    while (dad >= 0) {
        //判断是否有两个子节点若有则在其中寻找最大子节点
        if (son + 1 <= len - 1 && arr[son] < arr[son + 1])
            son++;
        if (arr[dad] < arr[son]) //若父节点小于子节点则交换位置
            swap(&arr[dad], &arr[son]);
        dad--;             //回退到上一个父节点
        son = 2 * dad + 1; //上一个父节点的首个子节点
    }
    swap(&arr[0], &arr[len - 1]);
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    heapSort();
}
