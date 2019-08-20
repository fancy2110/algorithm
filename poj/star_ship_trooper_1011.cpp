/**
 * http://acm.hdu.edu.cn/showproblem.php?pid=1011
 * 
 * input :
 *  
 * 5 10
 * 50 10
 * 40 10
 * 40 20
 * 65 30
 * 70 30
 * 1 2
 * 1 3
 * 2 4
 * 2 5
 * 1 1
 * 20 7
 * -1 -1
 * 
 * output: 
 * 50
 * 7
*/
#include <iostream>

struct room
{
    int count = 0;
    int percent = 0;
};

#define EMPTY 0
#define OCUPY 1

int m, n = 0;
struct room nodes[101];
int tunnels[101][101] = {0};
int p[20000 + 1] = {0};
int r_flag[101] = {EMPTY};

using namespace std;

void get_posibility(int start, int troopers)
{
    cout << "start:" << start << ", troopers:" << troopers << endl;
    for (int i = 1; i <= 100; i++)
    {
        // cout << "point(" << start << "," << i << "): " << tunnels[start][i] << ", flag:" << r_flag[i] << endl;
        if (tunnels[start][i] == 1 && r_flag[i] == EMPTY)
        {
            struct room &item = nodes[i];
            if (item.count < troopers)
            {
                int last_p = p[troopers - item.count];
                int new_p = p[troopers] + item.percent;
                if (new_p > last_p)
                {
                    p[troopers - item.count] = new_p;
                }

                r_flag[i] = OCUPY;

                get_posibility(i, troopers - item.count);
            }
        }
    }
}

int main(int argc, char const *argv[])
{

    while (true)
    {
        cin >> n >> m;
        cout << "n:" << n << ", m:" << m << endl;
        if (n < 0 && m < 0)
            break;

        int i = 1;
        while (i <= n)
        {
            cin >> nodes[i].count >> nodes[i].percent;
            i++;
        }

        memset(tunnels, 0, sizeof(tunnels));
        i = 1;
        while (i <= n - 1)
        {
            int x, y = 0;
            cin >> x >> y;
            cout << "x:" << x << ", y:" << y << endl;
            tunnels[x][y] = 1;
            i++;
        }

        memset(r_flag, EMPTY, sizeof(r_flag));
        memset(p, 0, sizeof(p));

        get_posibility(1, m * 20);
        for (size_t i = 0; i < 100; i++)
        {
            if (p[i] > 0)
            {
                cout << p[i] << endl;
                break;
            }
        }
    }
}
