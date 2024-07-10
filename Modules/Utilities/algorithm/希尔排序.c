//
// Created by Joho on 2023/11/21.
//
#include <stdio.h>

void shellSort()
{
    int arr[] = {1, 12, 13, 19, 26, 7, 8, 15, 3, 23, 99, 8, 35, 27, 34, 5};
    int len = sizeof(arr) / sizeof(arr[0]);  //16

    int gap, i, j;
    int temp;

    for (gap = len / 2; gap >= 1; gap /= 2) //第一个间隔为len/2,然后不断除2缩小
    {
        for (i = gap; i < len; i++) //对每一个下标大于gap的元素进行遍历
            //arr[gap]是第一组最后一个元素
        {
            temp = arr[i]; //将要插入的值赋值给temp,因为它所处的位置可能被覆盖
            for (j = i - gap; arr[j] > temp && j >= 0; j -= gap) { //i所处的子序列:i i-gap i-2gap i-n*gap( i-n*gap >= 0)
                arr[j + gap] = arr[j]; //arr[j]若大于要插入的值则将位置后移
            }
            arr[j + gap] = temp; //无论是arr[j]<temp还是j<0了,都将temp插入到arr[j]这一个子序列的后一个位置(j+gap)
        }
    }

    //打印数组
    for (int i = 0; i < len; i++)
        printf("%d\t", arr[i]);
}

int main()
{
    shellSort();
}
