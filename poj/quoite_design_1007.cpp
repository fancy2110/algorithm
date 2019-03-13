/***
 * 
 * Have you ever played quoit in a playground? Quoit is a game in which flat rings are pitched at some toys, with all the toys encircled awarded.
 * In the field of Cyberground, the position of each toy is fixed, and the ring is carefully designed so it can only encircle one toy at a time. On the other hand, to make the game look more attractive, the ring is designed to have the largest radius. Given a configuration of the field, you are supposed to find the radius of such a ring.
 * Assume that all the toys are points on a plane. A point is encircled by the ring if the distance between the point and the center of the ring is strictly less than the radius of the ring. If two toys are placed at the same point, the radius of the ring is considered to be 0.
 * test case :
 * 2
 * 0 0
 * 1 1
 * 2
 * 1 1
 * 1 1
 * 3
 * -1.5 0
 * 0 0
 * 0 1.5
 * 0
 * 
 * result:
 * 0.71
 * 0.00
 * 0.75
 */

#include <iostream>
#include <iomanip>
#include <math.h>
#include <float.h>

using namespace std;

struct node
{
    float x = 0.0f;
    float y = 0.0f;
};

struct node items[100001];
struct node temp[100001];
/**
 * 按照维度比较节点大小
*/
bool littler(struct node &left, struct node &right, bool isX)
{
    if (isX)
    {
        return left.x < right.x;
    }
    else
    {
        return left.y < right.y;
    }
}

/**
 * 按照维度比较节点大小
*/
bool bigger(struct node &left, struct node &right, bool isX)
{
    if (isX)
    {
        return left.x >= right.x;
    }
    else
    {
        return left.y >= right.y;
    }
}

/**
 * 计算两个点之间的距离
*/
float distance(struct node &first, struct node &second)
{
    float x2 = first.x - second.x;
    float y2 = first.y - second.y;
    float d = sqrt(x2 * x2 + y2 * y2);
    return d;
}

void show_items(int length)
{
    for (int i = 0; i < length; i++)
    {
        struct node &item = items[i];
        cout << "sort: " << i << "; P(" << item.x << "," << item.y << ")" << endl;
    }
}

void copy(struct node &left, struct node &right)
{
    left.x = right.x;
    left.y = right.y;
}
/**
 * 按照x轴排序
 */
void sort_point(int start, int end, bool isX)
{
    if (start >= end)
        return;

    struct node mid_node;
    copy(mid_node, items[start]);

    int i = start;
    int j = end;

    while (i < j)
    {
        //find the less item before j
        while (i < j && littler(items[i], mid_node, isX))
            ++i;
        copy(items[i], items[j]);

        // cout << "inner , i:" << i << ", j:" << j << endl;
        //find the bigger item after i
        while (i < j && bigger(items[j], mid_node, isX))
            --j;
        copy(items[j], items[i]);
    }

    copy(items[i], mid_node);
    sort_point(start, i, isX);
    sort_point(i + 1, end, isX);
}

float find_min_distance(int start, int end)
{
    int total = end - start + 1;

    if (start == end) //一个点，直接返回0.0f
    {
        return FLT_MAX;
    }

    if (total == 2) //两个点，直接两个点的距离
    {
        return distance(items[start], items[end]);
    }

    //至少三个点
    int mid = (start + end) / 2;

    float ld = find_min_distance(start, mid);
    float rd = find_min_distance(mid + 1, end);
    float d = ld < rd ? ld : rd;

    struct node &mid_node = items[mid];
    int k = 0;
    for (int i = 0; i < total; i++)
    {
        struct node &item = items[i];
        // cout << "d:" << d << " | total:" << total << ", index:" << i << "; base p(" << item.x << "," << item.y << ")" << endl;
        if (fabs(item.x - mid_node.x) <= d)
        {
            temp[k++] = item;
        }
    }

    //排序
    // show_items(k);
    sort_point(0, k - 1, false);
    // show_items(k);

    //遍历每两个点之间的距离
    for (int i = 0; i < k; i++)
    {
        struct node &base = temp[i];
        for (int j = i + 1; j < k; j++)
        {
            struct node &check_node = temp[j];
            if (fabs(base.y - check_node.y) < d)
            {
                // cout << "base p(" << base.x << "," << base.y << ")" << endl;
                // cout << "check_node p(" << check_node.x << "," << check_node.y << ")" << endl;
                float tmp_d = distance(base, check_node);
                d = tmp_d < d ? tmp_d : d;
                // cout << "find :" << i << ", " << j << fixed << setprecision(2) << " d:" << d << endl;
            }
        }
    }

    return d;
}

int main(int argc, char const *argv[])
{
    using namespace std;
    int total = 0;
    do
    {
        cin >> total;
        if (total <= 0)
            break;

        float x = 0.0f, y = 0.0f;
        int index = 0;
        while (index < total)
        {
            cin >> x >> y;
            items[index].x = x;
            items[index].y = y;
            index++;
        }

        sort_point(0, total - 1, true);
        // show_items(total);
        // cout << "total:" << total << endl;

        float min_r = find_min_distance(0, total - 1) / 2.0f;
        cout << fixed << setprecision(2) << min_r << endl;
    } while (true);
    /* code */
    return 0;
}
