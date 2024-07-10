//
// Created by Joho on 2023/11/21.
//
#include <stdio.h>
#include <malloc.h>

int min(int x, int y)
{
    return x < y ? x : y;
}

void merge_sort(int arr[], int len)
{
    int *a = arr;                              //左指针->首元素
    int *b = (int *) malloc(len * sizeof(int)); //右指针->尾元素
    int seg, start;
    for (seg = 1; seg < len; seg += seg) {
        for (start = 0; start < len; start += seg * 2) {
            int low = start;
            int mid = min(start + seg, len);
            int high = min(start + seg * 2, len);

            int k = low;
            int start1 = low, end1 = mid;
            int start2 = mid, end2 = high;
            while (start1 < end1 && start2 < end2)
                b[k++] = a[start1] < a[start2] ? a[start1++] : a[start2++];
            while (start1 < end1)//将左边剩下的元素放置到数组b中
                b[k++] = a[start1++];
            while (start2 < end2)//将左边剩下的元素放置到数组b中
                b[k++] = a[start2++];
        }
        int *temp = a;
        a = b;
        b = temp;
    }
    if (a != arr) {
        int i;
        for (i = 0; i < len; i++)
            b[i] = a[i];
        b = a;
    }
    free(b);
}

int main()
{
    int arr[] = {1, 12, 13, 19, 26, 7, 8, 15, 3, 23, 99, 8, 35, 27, 34, 5};
    merge_sort(arr, 30);
}
