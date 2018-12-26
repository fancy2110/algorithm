/**
 * http://acm.hdu.edu.cn/showproblem.php?pid=1005
*/

#include <iostream>

void version1()
{
    using namespace std;
    int a, b, t = 0;
    do
    {
        cin >> a >> b >> t;
        if (a == 0 && b == 0 && t == 0)
            break;

        int n_1 = 1;
        int n_2 = 1;
        int n = 0;

        for (int i = 3; i <= t; i++)
        {
            n = (a * n_1 + b * n_2) % 7;
            n_2 = n_1;
            n_1 = n;
        }
        cout << n << endl;
    } while (true);
}

int main(int argc, char const *argv[])
{
    using namespace std;
    int a, b, t = 0;
    int arr[49] = {1, 1};
    do
    {
        cin >> a >> b >> t;
        if (a == 0 && b == 0 && t == 0)
            break;

        //构建所有可能的组合
        for (int i = 2; i < 49; i++)
        {
            arr[i] = (a * arr[i - 1] + b * arr[i - 2]) % 7;
        }

        //获取对应的结果
        int n = arr[(t - 1) % 49];
        cout << n << endl;
    } while (true);
    return 0;
}
