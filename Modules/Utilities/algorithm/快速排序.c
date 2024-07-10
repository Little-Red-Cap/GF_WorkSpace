//
// Created by Joho on 2023/11/21.
//
#include <stdio.h>

void quick_sort(int arr[], int start, int end);

void quickSort()
{
    int arr[10] = {11, 7, 9, 3, 4, 6, 2, 8, 5, 3};
    quick_sort(arr, 0, 9);
    for (int i = 0; i < 10; i++)
        printf("%d\t", arr[i]);
}

int partition(int arr[], int start, int end)
{
    int temp = arr[start];//以arr[start]作为基准,将被放置在数组中间
    //同时将start位置作为交换元素的缓冲点-类比交换两个数的第三个数
    int li = start, ri = end;//li->left index 左索引 li==start 所以初始li是第一个缓冲点
    while (li < ri) {
        while (li < ri && arr[ri] > temp)//找到我们右起第一个小于基准值的元素索引ri
            ri--;
        if (li < ri) {
            arr[li] = arr[ri];//将右起第一个小于基准值的元素索引放置在缓冲点(li)
            //同时此时的ri成为新的缓冲点
            li++;
        }
        while (li < ri && arr[li] < temp)//找到我们右起第一个小于基准值的元素索引li
            li++;
        if (li < ri) {
            arr[ri] = arr[li];//将左起第一个大于基准值的元素索引放置在缓冲点 (ri)
            //同时此时的li成为新的缓冲点
            ri--;
        }
        //结束上述操作后li和ri分别是左右已排序部分(置于两端)的后面一个和前面一个元素(不包含在其中)
        //明显若li==ri则只剩下最后一个位置
    }
    arr[li] = temp;
    return li;//返回的是基准值最终的索引
}

void quick_sort(int arr[], int start, int end)
{
    if (start < end) {
        int index = partition(arr, start, end);//依照基准值分区
        quick_sort(arr, start, index - 1);//基准值之左再排序
        quick_sort(arr, index + 1, end);//基准值之右再排序
    }
}

int main()
{
    quickSort();
}
