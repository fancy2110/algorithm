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

#define DEBUG false

#define EMPTY 0
#define OCUPY 1
#define MAX_TROOPERS = 100
#define RATIO = 20
#define MAX_ROOM = 100

int m, n = 0;
struct room nodes[101];
int tunnels[101][101] = {0};
//为每个节点构建一个访问结果记录
int p[100 + 1][100 + 1] = {0};
int tmp[100 + 1] = {0};

using namespace std;
/**
 * 返回值为剩余的兵力
*/
bool get_posibility(int start, int troopers);

void print_result()
{
    if (!DEBUG)
        return;
    cout << "-------------------------------" << endl;
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= m; j++)
        {
            cout << p[i][j] << ",";
        }
        cout << endl;
    }
    cout << endl;
}

bool get_posibility(int start, int troopers)
{
    if (troopers == 0)
        return false;

    struct room &item = nodes[start];
    if (DEBUG)
        cout << "room[" << start << "] isOcupy:" << item.isOcupy << ", count:" << item.count << ", posibility:" << item.percent << endl;

    if (item.isOcupy) //已经被占领了
        return false;

    if (item.count > troopers * 20) //已经没有足够兵力了
        return false;

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
    }

    for (int i = cost; i <= m; i++)
    {
        p[start][i] = item.percent;
    }

    for (size_t i = 1; i <= n; i++)
    {
        if (DEBUG)
            cout << "tunnel(" << start << "," << i << "): " << tunnels[start][i] << endl;
        if (tunnels[start][i] == 0 || i == start) //没有通道
            continue;

        int left_troopers = troopers - cost;
        if (!get_posibility(i, left_troopers))
            continue;

        memset(tmp, 0, sizeof(tmp));
        for (int j = cost; j <= m; j++)
        {
            tmp[j] = p[start][j];
        }

        if (DEBUG)
        {
            cout << "from " << start << " ocupy(" << i << "), cost:" << cost << ", percent :" << item.percent << endl;
            cout << "base room: ";
            for (size_t j = 0; j <= m; j++)
            {
                cout << tmp[j] << ",";
            }
            cout << endl;
            cout << "copy room: ";
            for (size_t j = 0; j <= m; j++)
            {
                cout << p[i][j] << ",";
            }
            cout << endl;
        }

        for (size_t t = cost; t <= m; t++)
        {
            int old_p = tmp[t];
            for (size_t o = 1; o + t <= m; o++)
            {
                int new_p = old_p + p[i][o];
                if (new_p > p[start][t + o])
                {
                    p[start][t + o] = new_p;
                }
            }
        }

        print_result();
    }

    item.isOcupy = false;
    return true;
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
        if (DEBUG)
            print_result();
        cout << p[1][m] << endl;
    }
}
