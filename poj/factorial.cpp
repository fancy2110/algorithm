/**
 * Created by 樊溪 on 2017/7/4.
 *
 * 输入 不超过 1000 的 正 整数 n， 输出 n!= 1 × 2 × 3 ×… × n 的 精确 结果。
 *
 * 样 例 输入： 30 样 例 输出： 265252859812191058636308480000000
 */

#include<iostream>
#include "../tools/Integer.h"

int main() {
    Integer a;
    Integer b(1);

    int number = 10;

    try {
        std::cin >> number;
    } catch (std::exception) {
        std::cout << "Integer is error" << std::endl;
    }

    
    std::cout << b << std::endl;
    for(int i = 1; i <= number;  i++)
    {
        Integer c = a + b;
        a = b;
        b = c;
        std::cout << b << std::endl;
    }

    return  1;
}

