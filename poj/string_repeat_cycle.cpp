/**
 * 如果 一个 字符串 可以 由 某个 长度 为 k 的 字符串 重复 多次 得到， 我们 说 该 串 以 k 为 周期。
 * 例如， abcabcabcabc 以 3 为 周期（ 注意， 它 也 以 6 和 12 为 周期）。
 *
 * 输入 一个 长度 不超过 80 的 串， 输出 它的 最小 周期。
 * 样 例 输入： HoHoHo 样 例 输出： 2
 *
 *
 * Created by 樊溪 on 2017/7/4.
 */

#include <iostream>

int main() {

    char word[100];
    std::cin >> word ;

    size_t len = strlen(word);


    for(int i = 1; i < len; i++)
    {
        if (len % i != 0) {
            continue;
        }

        int ok = 1;

        for(int j = i; j < len; j++)
        {
            if (word[j] != word[j % i])
            {
                ok = 0 ;
                break;
            }
        }

        if (ok) {
            std::cout << i << std::endl;
        }
    }

    return 0;
}

