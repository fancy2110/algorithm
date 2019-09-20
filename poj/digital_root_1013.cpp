/**
 * 
 * http://acm.hdu.edu.cn/showproblem.php?pid=1013
 * 
*/
#include <iostream>
#include <stdlib.h>
#include <cstring>

int get_digital_root(int n)
{
    int new_n = n;
    do
    {
        int tmp = new_n;
        new_n = 0;
        while (tmp > 0)
        {
            new_n += tmp % 10;
            tmp /= 10;
        }
    } while (new_n > 10);

    return new_n;
}

int main(int argc, char const **argv)
{
    char n = '0';
    int root = 0;

    using namespace std;

    while (true)
    {
        n = getchar();
        if (n == '\n')
        {
            if (root == 0)
            {
                return 1;
            }

            int ret = get_digital_root(root);
            cout << ret << endl;
            root = 0;
        }
        else
        {
            root += n - '0';
        }
    }

    return 0;
}