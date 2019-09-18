/**
 * http://acm.hdu.edu.cn/showproblem.php?pid=1011
 * 
 * input :
 *  
 5 10
 50 10
 40 10
 40 20
 65 30
 70 30
 1 2
 1 3
 2 4
 2 5
 1 1
 20 7
 8 2
 0 0
 0 9
 0 8
 0 4
 0 7
 0 3
 0 2
 0 1
 1 2
 1 3
 2 4
 2 5
 4 6
 6 7
 7 8
 5 2
 0 1
 0 1
 0 5
 0 1
 0 2
 1 2
 1 3
 2 4
 2 5
 -1 -1

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

#define DEBUG true

#define EMPTY 0
#define OCUPY 1
#define MAX_TROOPERS = 100
#define RATIO = 20
#define MAX_ROOM = 100

int m, n = 0;
struct room nodes[101];
int tunnels[101][101] = {0};
int p[100 + 1] = {0};

using namespace std;

void try_ocupy(int start, int troopers, int cost, int percent)
{
    int last_cost = m - troopers;
    int new_cost = last_cost + cost;
    if (cost > 0)
    {
        int last_p = p[last_cost];
        int new_p = last_cost + percent;
        if (new_p > last_p)
            p[new_cost] = new_p; //非负数，可以直接覆盖，一定比旧值要大

        if (DEBUG)
            cout << "start:" << start << ", last_cost:" << last_cost << ", last_p:" << last_p << endl;
    }

    for (int i = 1; i <= n; i++)
    {
        if (DEBUG)
            cout << "tunnel(" << start << "," << i << "): " << tunnels[start][i] << endl;
        if (tunnels[start][i] == 0) //没有通道
            continue;

        get_posibility(i, m - new_cost);
    }
}

void get_posibility(int start, int troopers)
{
    if (troopers == 0)
        return;

    struct room &item = nodes[start];
    if (DEBUG)
        cout << "room[" << start << "] isOcupy:" << item.isOcupy << ", count:" << item.count << ", posibility:" << item.percent << endl;
    if (item.isOcupy) //已经被占领了
        return;

    if (item.count > troopers) //已经没有足够兵力了
        return;

    item.isOcupy = true;
    int cost = 0;
    if (item.count != 0)
    {
        int mod_left = item.count % 20;
        cost = item.count / 20;
        if (mod_left > 0)
        {
            cost++;
        }
        try_ocupy(start, troopers, cost, item.percent);
    }
    else if (item.percent > 0)
    {
        try_ocupy(start, troopers, 0, item.percent);
        try_ocupy(start, troopers, 1, item.percent);
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

        int total_trooper = m;
        get_posibility(1, total_trooper);

        int max = 0;
        int position = 0;
        for (size_t i = 0; i < m + 1; i++)
        {
            if (DEBUG)
                cout << "i:" << i << ", percent:" << p[i] << endl;
            if (p[i] > max)
            {
                max = p[i];
                position = i;
            }
        }
        cout << max << endl;
    }
}
