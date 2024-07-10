//
// Created by Joho on 2023/11/21.
//
#include <stdio.h>

void counting_Sort(int arr[], int LEN);

void countingSort()
{
    int arr[] = {3, 5, 3, 0, 8, 6, 1, 5, 8, 6, 2, 4, 9, 4, 7, 0, 1, 8, 9, 7, 3, 1, 2, 5, 9, 7, 4, 0, 2, 6};
    int len = (int) sizeof(arr) / sizeof(*arr);
    counting_Sort(arr, len);

    for (int i = 0; i < len; i++)
        printf("%d   ", arr[i]);
}

void counting_Sort(int arr[], int LEN)
{
    //寻找最大最小值
    int max = arr[0], min = arr[0], i, j = 0;
    for (i = 0; i < LEN; i++) {
        if (arr[i] < min)
            min = arr[i];
        if (arr[i] > max)
            max = arr[i];
    }

    //建立计数数组
    int new_len = max - min + 1;

    int conunting_arr[new_len];

    for (i = 0; i < new_len; i++) //初始化
        conunting_arr[i] = 0;

    for (i = 0; i < LEN; i++)//计数
        conunting_arr[arr[i] - min]++;

    //根据计数结果进行排序
    for (i = 0; i < new_len; i++) {
        int index = conunting_arr[i];
        while (index != 0) {
            arr[j] = i + min;
            index--;
            j++;
        }
    }
}

int main()
{
    countingSort();
}
