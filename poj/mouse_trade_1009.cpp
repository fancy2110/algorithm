/***
 * http://acm.hdu.edu.cn/showproblem.php?pid=1009
 */

/*
test case 
5 3
7 2
4 3
5 2
20 3
25 18
24 15
15 10
0 1
1 0
1 0
5 4
10000 5
2000 2
100 0
300 0
-1 -1
*/

#include <iostream>
#include <iomanip>
#include <math.h>
#include <float.h>

struct node
{
    int food;
    int money;
    float price;
};

struct node rooms[1001];
int items[1001];

using namespace std;

int compare_price(int left_index, int right_index)
{
    struct node &left = rooms[left_index];
    struct node &right = rooms[right_index];

    if (left.price < right.price)
        return -1;
    if (left.price == right.price)
        return 0;
    return 1;
}

void sort(int *items, int start, int end)
{
    if (start >= end)
        return;

    int mid = items[start];
    int i = start;
    int j = end;

    while (i < j)
    {
        while (i < j && compare_price(items[j], mid) < 0)
            j--;
        items[i] = items[j];

        while (i < j && compare_price(items[i], mid) >= 0)
            i++;
        items[j] = items[i];
    }

    items[i] = mid;
    sort(items, start, i);
    sort(items, i + 1, end);
}

int main(int argc, char const *argv[])
{
    int n = 0;
    int sum = 0;
    do
    {
        cin >> sum >> n;
        if (n == -1 && sum == -1)
            break;

        // cout << "sum:" << sum << ", n:" << n << endl;

        float total = 0.0f;
        int index = 0;
        int count = 0;

        while (count < n)
        {
            struct node &room = rooms[index];
            cin >> room.food >> room.money;
            if (room.money <= 0)
            {
                total += room.food;
            }
            else
            {
                room.price = room.food * 1.0f / room.money;
                items[index] = index;
                index++;
            }

            count++;
        }

        // cout << "sum:" << sum << ", total:" << total << endl;
        sort(items, 0, index - 1);

        int item_index = 0;
        while (sum > 0 && index > 0 && index < item_index)
        {
            struct node &room = rooms[items[item_index++]];
            if (sum >= room.money)
            {
                total += room.food;
                sum -= room.money;
            }
            else
            {
                total += sum * room.price;
                break;
            }

            // cout << "sum:" << sum << ", total:" << total << endl;
        }

        cout << fixed << setprecision(3) << total << endl;
    } while (true);
    /* code */
    return 0;
}
