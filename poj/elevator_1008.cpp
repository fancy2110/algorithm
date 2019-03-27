/***
 * http://acm.hdu.edu.cn/showproblem.php?pid=1008
 */

#include <iostream>

int main(int argc, char const *argv[])
{
    using namespace std;
    int n = 0;
    do
    {
        cin >> n;
        if (n <= 0)
            break;

        int total = 0;
        int last_level = 0;
        int target_level = 0;
        while (n > 0)
        {
            cin >> target_level;
            // cout << "target: " << target_level << endl;
            if (target_level < last_level)
            {
                total += 4 * (last_level - target_level) + 5;
            }
            else
            {
                total += 6 * (target_level - last_level) + 5;
            }
            last_level = target_level;
            n--;
        }

        cout << total << endl;
    } while (true);
    /* code */
    return 0;
}
