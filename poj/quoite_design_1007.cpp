/***
 * http://acm.hdu.edu.cn/showproblem.php?pid=1007
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
int nodes[100001];
int temp[100001];
/**
 * 按照维度比较节点大小
*/
bool littler(int left_index, int right_index, bool isX)
{
    struct node &left = items[left_index];
    struct node &right = items[right_index];
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
bool bigger(int left_index, int right_index, bool isX)
{
    struct node &left = items[left_index];
    struct node &right = items[right_index];
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
float distance(int first_index, int second_index)
{
    struct node &first = items[first_index];
    struct node &second = items[second_index];

    float x2 = first.x - second.x;
    float y2 = first.y - second.y;
    float d = sqrt(x2 * x2 + y2 * y2);
    return d;
}

void show_items(int *ref_items, int length)
{
    for (int i = 0; i < length; i++)
    {
        struct node &item = items[ref_items[i]];
        cout << "sort: " << i << "; P(" << item.x << "," << item.y << ")" << endl;
    }

    cout << endl;
}

/**
 * 按照x轴排序
 */
void sort_point(int *ref_items, int start, int end, bool isX)
{
    if (start >= end)
        return;

    int mid_node = ref_items[start];
    int i = start;
    int j = end;

    while (i < j)
    {
        //find the less item before j
        while (i < j && bigger(ref_items[j], mid_node, isX))
            j--;
        ref_items[i] = ref_items[j];

        // cout << "inner , i:" << i << ", j:" << j << endl;
        //find the bigger item after i
        while (i < j && littler(ref_items[i], mid_node, isX))
            i++;
        ref_items[j] = ref_items[i];
    }
    ref_items[i] = mid_node;
    sort_point(ref_items, start, i, isX);
    sort_point(ref_items, i + 1, end, isX);
}

float find_min_distance(int start, int end)
{
    int total = end - start + 1;

    if (total == 1) //一个点，直接返回0.0f
    {
        return FLT_MAX;
    }

    if (total == 2) //两个点，直接两个点的距离
    {
        return distance(nodes[start], nodes[end]);
    }

    //至少三个点
    int mid = (start + end) / 2;

    float ld = find_min_distance(start, mid);
    float rd = find_min_distance(mid + 1, end);
    float d = ld < rd ? ld : rd;

    struct node &mid_node = items[nodes[mid]];
    int k = 0;
    for (int i = start; i < end; i++)
    {
        struct node &item = items[nodes[i]];
        // cout << "d:" << d << " | total:" << total << ", index:" << i << "; base p(" << item.x << "," << item.y << ")" << endl;
        if (fabs(item.x - mid_node.x) <= d)
        {
            temp[k++] = nodes[i];
        }
    }

    //排序
    // show_items(temp, k);
    sort_point(temp, 0, k - 1, false);
    // show_items(temp, k);

    //遍历每两个点之间的距离
    for (int i = 0; i < k; i++)
    {
        struct node &base = items[temp[i]];
        for (int j = i + 1; j < k; j++)
        {
            struct node &check_node = items[temp[j]];
            if (fabs(base.y - check_node.y) < d)
            {
                // cout << "base p(" << base.x << "," << base.y << ")" << endl;
                // cout << "check_node p(" << check_node.x << "," << check_node.y << ")" << endl;
                float tmp_d = distance(temp[i], temp[j]);
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
    while (cin >> total && total > 0)
    {
        if (total <= 0)
        {
            // cout << "total:" << total << endl;
            break;
        }

        float x = 0.0f, y = 0.0f;
        int index = 0;
        while (index < total)
        {
            cin >> x >> y;
            items[index].x = x;
            items[index].y = y;
            nodes[index] = index;
            index++;
        }

        // show_items(nodes, total);
        sort_point(nodes, 0, total - 1, true);
        // show_items(nodes, total);

        cout.setf(ios::fixed);
        float min_r = find_min_distance(0, total - 1) / 2.0f;
        float min_r = 0.0f;
        cout << fixed << setprecision(2) << min_r << endl;
    }
    /* code */
    return 0;
}
