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
#include <stdlib.h>
#include <cstring>

struct room
{
    //bug count
    int count = 0;
    //brain possiblity
    int percent = 0;
    //
    bool isOcupy = false;
};

#define DEBUG false

#define EMPTY 0
#define OCUPY 1
#define MAX_TROOPERS = 100
#define RATIO = 20
#define MAX_ROOM = 100

int m, n = 0;
struct room nodes[101];
int tunnels[101][101] = {0};
int p[20000 + 1] = {0};

using namespace std;

void get_posibility(int start, int troopers)
{
    if (troopers == 0)
        return;

    struct room &item = nodes[start];
    if (item.isOcupy) //已经被占领了
        return;

    if (DEBUG)
        cout << "start:" << start << ", troopers:" << troopers << endl;
    item.isOcupy = true;
    int cost = 20;
    if (item.count != 0)
        cost = item.count;
    int left_troopers = troopers - cost;
    int last_p = p[left_troopers];
    int new_p = p[troopers] + item.percent;
    p[left_troopers] = new_p; //非负数，可以直接覆盖，一定比旧值要大

    for (int i = 1; i <= n; i++)
    {
        if (DEBUG)
            cout << "tunnel(" << start << "," << i << "): " << tunnels[start][i] << endl;
        if (tunnels[start][i] == 0) //没有通道
            continue;

        if (item.count > troopers) //剩余的兵力已经不够
            continue;

        if (DEBUG)
            cout << "room[" << i << "] isOcupy:" << item.isOcupy << ", count:" << item.count << ", posibility:" << item.percent << endl;

        get_posibility(i, left_troopers);
    }
    item.isOcupy = false;
}

int main(int argc, char const *argv[])
{

    while (true)
    {
        cin >> n >> m;
        if (DEBUG)
            cout << "n:" << n << ", m:" << m << endl;

        if (n < 0 && m < 0)
            break;

        int i = 1;
        while (i <= n)
        {
            cin >> nodes[i].count >> nodes[i].percent;
            nodes[i].isOcupy = false;
            i++;
        }

        memset(tunnels, 0, sizeof(tunnels));
        i = 1;
        while (i <= n - 1)
        {
            int x, y = 0;
            cin >> x >> y;
            tunnels[x][y] = 1;
            tunnels[y][x] = 1;
            i++;
        }

        memset(p, 0, sizeof(p));

        int total_trooper = m * 20;
        get_posibility(1, total_trooper);

        for (size_t i = 0; i < 20 * m + 1; i++)
        {
            if (p[i] > 0)
            {
                if (DEBUG)
                {
                    cout << "i:" << i << ", percent:" << p[i] << endl;
                }
                else
                {
                    cout << p[i] << endl;
                    break;
                }
            }
        }
    }
}
