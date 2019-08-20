/***
 * http://acm.hdu.edu.cn/showproblem.php?pid=1010
 */

/*
test case 
4 4 9
S.X.
..X.
..XD
....
3 4 6
S.X.
..X.
...D
7 4 17
Sx.D
....
X..X
X...
X.X.
X.X.
X...
0 0 0

*/

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <math.h>
#include <stdio.h>
#include <float.h>

struct item
{
    int x;
    int y;
};

#define DOOR 'D'
#define EMPTY '.'
#define DESTORY '-'
#define WALL 'X'
#define START 'S'

char maze[10][10] = {'X'};
struct item test[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
struct item door = {10, 10};

using namespace std;

int distance(int x, int y)
{
    return abs(x - door.x) + abs(y - door.y);
}

bool find_path(int x, int y, int time)
{

    if (maze[x][y] == DOOR)
    {
        return time == 0;
    }

    int min_length = distance(x, y);
    // cout << "move to x:" << x << ", y:" << y << ", t:" << time << ", min_length:" << min_length << ", holder:" << maze[x][y] << endl;
    if (time < 0 || min_length > time)
    {
        return false;
    }

    bool ret = false;
    for (size_t i = 0; i < 4; i++)
    {
        struct item &op = test[i];
        int new_x = x + op.x;
        int new_y = y + op.y;
        if (maze[new_x][new_y] == EMPTY)
        { //move to next
            maze[x][y] = DESTORY;
            ret = find_path(new_x, new_y, time - 1);
            maze[x][y] = EMPTY;
        }
        else if (maze[new_x][new_y] == DOOR)
        {
            ret = find_path(new_x, new_y, time - 1);
        }
        else if (maze[new_x][new_y] == WALL)
        {
            continue;
        }

        if (ret)
        {
            break;
        }
    }

    return ret;
}

int main(int argc, char const *argv[])
{
    int n, m, t = 0;
    cout.setf(ios::fixed);
    do
    {
        cin >> n >> m >> t;
        if (n == 0 && m == 0 && t == 0)
            break;

        // cout << "n:" << n << ", m:" << m << ", t:" << t << endl;

        for (size_t i = 0; i < 10; i++)
        {
            for (size_t j = 0; j < 10; j++)
            {
                maze[i][j] = WALL;
            }
        }

        int start_x, start_y = 0;

        int line = 1;
        while (line <= n)
        {
            int row = 1;
            while (row <= m)
            {
                cin >> maze[row][line];
                if (maze[row][line] == START)
                {
                    start_x = row;
                    start_y = line;
                }

                if (maze[row][line] == DOOR)
                {
                    door.x = row;
                    door.y = line;
                }

                row++;
            }
            line++;
        }

        int min_length = distance(start_x, start_y);
        bool ret = false;
        if (t >= min_length && (t - min_length) % 2 == 0)
        {
            maze[start_x][start_y] = EMPTY;
            ret = find_path(start_x, start_y, t);
        }

        if (ret)
        {
            cout << "YES" << endl;
        }
        else
        {
            cout << "No" << endl;
        }
    } while (true);
    /* code */
    return 0;
}
