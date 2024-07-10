//
// Created by Joho on 2023/11/21.
//
#include <stdio.h>

void Sort()
{
    int arr[] = {100, 92, 5, 9, 3, 8, 23, 17, 50, 6};

    int len = sizeof(arr) / sizeof(arr[0]);
    int index = 0;                //待会用来存储未排序区最小元素的位置索引
    for (int i = 0; i < len; i++) //从小到大
    {
        index = i;
        for (int j = i + 1; j < len; j++) //用i之后的每一个元素去与i元素比较大小,若小于arr[i]则更新最小元素索引
        {
            if (arr[j] < arr[index])
                index = j;
        }
        //将i与index的元素调换位置
        //注意:此处不可将调换位置的函数写进第二层for循环即for(int j=i+1)中,因为交换后i与min指向的对象会交换,此后循环就可能出现仅仅小于arr[i](此时已经换到了min位置)但不小于arr[min](这时在i位置上)的元素也与初始位置上进行交换的情况,具体情况可以试验!
        if (i != index) //判断是否需要调换,将最小元素位置换至第一个未排序的位置
        {
            int temp = arr[i];
            arr[i] = arr[index];
            arr[index] = temp;
        }
    }

    //打印数组
    for (int i = 0; i < len; i++)
        printf("%d\t", arr[i]);
}

int main()
{
    Sort();
}
