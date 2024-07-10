//
// Created by Joho on 2023/11/21.
//
#include <stdio.h>

void bucket_sort(int arr[], int LEN);

void bucketSort()
{
    int arr[] = {3, 5, 3, 0, 8, 6, 1, 5, 8, 6, 2, 4, 9, 4, 7, 0, 1, 8, 9, 7, 3, 1, 2, 5, 9, 7, 4, 0, 2, 6};
    int len = (int) sizeof(arr) / sizeof(arr[0]);//30
    bucket_sort(arr, len);

    for (int i = 0; i < len; i++)
        printf("%d   ", arr[i]);
}

void bucket_sort(int arr[], int LEN)
{
    int bucket[5][6] = {0}, i, j, k, temp; //初始化桶,每个桶存放6个数据
    //寻找最大最小值
    int min = arr[0], max = arr[0];
    for (i = 0; i < LEN; i++) {
        if (arr[i] < min)
            min = arr[i];
        if (arr[i] > max)
            max = arr[i];
    }
    //遍历数组,将元素放到对应桶中
    int index0 = 0, index1 = 0, index2 = 0, index3 = 0, index4 = 0;
    for (i = 0; i < LEN; i++) {
        if (arr[i] < min + (max - min + 1) / 5 * 1 && index0 < 7) {
            bucket[0][index0] = arr[i];
            index0++;
        } else if (arr[i] < min + (max - min + 1) / 5 * 2 && (index1 < 7 || index0 >= 7)) {
            bucket[1][index1] = arr[i];
            index1++;
        } else if (arr[i] < min + (max - min + 1) / 5 * 3 && (index2 < 7 || index1 >= 7)) {
            bucket[2][index2] = arr[i];
            index2++;
        } else if (arr[i] < min + (max - min + 1) / 5 * 4 && (index3 < 7 || index2 >= 7)) {
            bucket[3][index3] = arr[i];
            index3++;
        } else if (arr[i] < min + (max - min + 1) / 5 * 5 && (index4 < 7 || index3 >= 7)) {
            bucket[4][index4] = arr[i];
            index4++;
        }
    }

    //在每个桶中使用冒泡排序
    for (i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) //从小到大
        {                           // 外循环为排序趟数，len个数进行len-1趟
            for (int k = 0; k < 5 - i; k++) { // 内循环为每趟比较的次数，第i趟比较len-i次,因为第一次已经将最大的元素冒泡到最后一个位置了
                if (bucket[i][k] > bucket[i][k + 1]) { //相邻元素比较,逆序则将交换位置
                    temp = bucket[i][k];
                    bucket[i][k] = bucket[i][k + 1];
                    bucket[i][k + 1] = temp;
                }
            }
        }
    }

    //将桶中排序结果还原到原数组中
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 6; j++) {
            arr[i * 6 + j] = bucket[i][j];
        }
    }
}

int main()
{
    bucketSort();
}
