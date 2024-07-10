//
// Created by Joho on 2023/11/21.
//
#include <stdio.h>
void radixsort(int *a, int n)

{
    int b[n], m = a[0], exp = 1, i;
    const int BASE = 10;

    //寻找最大值
    for (i = 1; i < n; i++)
        if (a[i] > m)
            m = a[i];

    while (m / exp > 0)
    {
        int bucket[BASE] = {0};
        //按照exp所在的位将所有元素放入桶中
        for (i = 0; i < n; i++)
            bucket[(a[i] / exp) % BASE]++;
        //做前缀和-以<=i结尾的数的个数-以i结尾的数的最大索引+1
        for (i = 1; i < BASE; i++)
            bucket[i] += bucket[i - 1];
        //依据前缀和将原数组中每一个元素放置在基于技术排列后的位置 并将前缀和减1
        for (i = n - 1; i >= 0; i--)
            b[--bucket[(a[i] / exp) % BASE]] = a[i];
        //复制回原数组
        for (i = 0; i < n; i++)
            a[i] = b[i];

        exp *= BASE;
    }
}

int main()
{
    int arr[10] = {1, 35, 98, 256, 789, 47, 4, 956, 64, 222};
    int len = sizeof(arr) / sizeof(arr[0]);

    radixsort(&arr[0], len);

    for (int i = 0; i < len; i++)
        printf("%d\t", arr[i]);

    return 0;
}
