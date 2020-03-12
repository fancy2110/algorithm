#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <cstring>

int flag[100010] = {0};
int step, mod = 0;

/***
 * 通过比较生成对应各随机数的所需的次数得出
*/
bool solution_one()
{
    int n = 0;
    int count = 0;
    while (flag[n] != 1 && mod > 0)
    {
        flag[n] = 1;
        n = (n + step) % mod;
        count++;
        // cout << n << ",";
    }
    return count == mod;
}

/**
 * 通过计算两个随机数种子，最大公约数为1来判断
*/
bool solution_two()
{
    int n = step;
    int m = mod;

    while (n != m)
    {
        if (n > m)
        {
            n = n - m;
        }
        else
        {
            m = m - n;
        }
    }

    return n == 1;
}

int main(int argc, const char **argv)
{
    using namespace std;

    while (cin >> step >> mod)
    {
        memset(flag, 0, sizeof(flag));
        // cout << "------------" << step << ", " << mod << "------------" << endl;

        // cout << endl;
        bool isOk = solution_two();
        cout << setw(10) << step << setw(10) << mod << "    ";
        if (isOk)
            cout << "Good Choice" << endl;
        else
            cout << "Bad Choice" << endl;
        cout << endl;
    }
}