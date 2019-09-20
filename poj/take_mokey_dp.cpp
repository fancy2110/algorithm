#include <iostream>
#include <stdlib.h>

int choose[] = {1, 3, 4};
int position[10000] = {-1};
using namespace std;

int is_win(int n)
{
    int ret = position[n];
    if (ret >= 0)
        return ret;

    int result = 0;
    for (int i = 0; i < 3; i++)
    {
        int m = n - choose[i];
        if (m < 0)
            continue;

        int ret = is_win(m);
        if (ret == 0)
        {
            result = 1;
            break;
        }
    }
    position[n] = result;
    return result;
}

int main(int argc, char const **argv)
{
    int n = 25;

    memset(position, -1, sizeof(position));
    position[0] = 0;
    position[1] = 1;
    position[2] = 0;
    position[3] = 1;
    position[4] = 1;
    position[n] = is_win(n);

    for (int i = 0; i <= n; i++)
    {
        cout << i << ":" << position[i] << endl;
    }
}
