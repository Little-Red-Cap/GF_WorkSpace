#include "TEA.h"
#include <stdio.h>
#include <stdint.h>

// https://blog.csdn.net/m0_73393932/article/details/130094306
//https://cloud.tencent.com/developer/article/1732280


//加密函数
void tea_encrypt (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=0, i;     //v0,v1分别为字符串的低字节高字节
    uint32_t delta=0x9e3779b9;
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];
    for (i=0; i < 32; i++) {               //加密32轮
        sum += delta;
        v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
    }
    v[0]=v0; v[1]=v1;//加密后再重新赋值
}
//解密函数
void tea_decrypt (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=0xC6EF3720, i;
    uint32_t delta=0x9e3779b9;
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];
    for (i=0; i<32; i++) {                        //解密时将加密算法的顺序倒过来，还有+=变为-=
        v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
        v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        sum -= delta;
    }
    v[0]=v0; v[1]=v1;//解密后再重新赋值
}



void xtea_encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}

void xtea_decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
    for (i=0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}



#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))

void btea(uint32_t *v, int n, uint32_t const key[4])
{
    uint32_t y, z, sum;
    unsigned p, rounds, e;
    if (n > 1)            /* Coding Part */
    {
        rounds = 6 + 52/n;
        sum = 0;
        z = v[n-1];
        do
        {
            sum += DELTA;
            e = (sum >> 2) & 3;
            for (p=0; p<n-1; p++)
            {
                y = v[p+1];
                z = v[p] += MX;
            }
            y = v[0];
            z = v[n-1] += MX;
        }
        while (--rounds);
    }
    else if (n < -1)      /* Decoding Part */
    {
        n = -n;
        rounds = 6 + 52/n;
        sum = rounds*DELTA;
        y = v[0];
        do
        {
            e = (sum >> 2) & 3;
            for (p=n-1; p>0; p--)
            {
                z = v[p-1];
                y = v[p] -= MX;
            }
            z = v[n-1];
            y = v[0] -= MX;
            sum -= DELTA;
        }
        while (--rounds);
    }
}














int tea_test()
{
    uint32_t v[2]={1,2},k[4]={2,2,3,4};
    // v为要加密的数据是两个32位无符号整数
    // k为加密解密密钥，为4个32位无符号整数，即密钥长度为128位
    printf("加密前原始数据：%u %u\n",v[0],v[1]);
    tea_encrypt(v, k);
    printf("加密后的数据：%u %u\n",v[0],v[1]);
    tea_decrypt(v, k);
    printf("解密后的数据：%u %u\n",v[0],v[1]);
    return 0;
}


int xtea_test()
{
    uint32_t v[2] = {1, 2};
    uint32_t const k[4] = {2, 2, 3, 4};
    unsigned int r = 32;//num_rounds建议取值为32
    // v为要加密的数据是两个32位无符号整数
    // k为加密解密密钥，为4个32位无符号整数，即密钥长度为128位
    printf("加密前原始数据：%u %u\n", v[0], v[1]);
    xtea_encipher(r, v, k);
    printf("加密后的数据：%u %u\n", v[0], v[1]);
    xtea_decipher(r, v, k);
    printf("解密后的数据：%u %u\n", v[0], v[1]);
    return 0;
}


int xxtea_test()
{
    uint32_t v[2]= {1,2};
    uint32_t const k[4]= {2,2,3,4};
    int n= 2; //n的绝对值表示v的长度，取正表示加密，取负表示解密
    // v为要加密的数据是两个32位无符号整数
    // k为加密解密密钥，为4个32位无符号整数，即密钥长度为128位
    printf("加密前原始数据：%u %u\n",v[0],v[1]);
    btea(v, n, k);
    printf("加密后的数据：%u %u\n",v[0],v[1]);
    btea(v, -n, k);
    printf("解密后的数据：%u %u\n",v[0],v[1]);
    return 0;
}





