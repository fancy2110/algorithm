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
#include <algorithm>
#include <iomanip>
#include <math.h>
#include <stdio.h>
#include <float.h>

struct node
{
    double food;
    double money;
    double price;
};

struct node rooms[1001];
int items[1001];

using namespace std;

void show_items(int *ref_items, int length)
{
    for (int i = 0; i < length; i++)
    {
        struct node &item = rooms[ref_items[i]];
        cout << "sort: " << i << "; P(" << item.food << "," << item.money << "," << item.price << ")" << endl;
    }

    cout << endl;
}

bool compare_price(int left_index, int right_index)
{
    struct node &left = rooms[left_index];
    struct node &right = rooms[right_index];

    return left.price > right.price;
}

int main(int argc, char const *argv[])
{
    int n = 0;
    double sum = 0.0;
    cout.setf(ios::fixed);
    do
    {
        cin >> sum >> n;
        if (n == -1 && sum == -1)
            break;

        // cout << "sum:" << sum << ", n:" << n << endl;

        double total = 0.0f;
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
                room.price = room.food / room.money;
                items[index] = index;
                index++;
            }

            count++;
        }

        // show_items(items, index);
        sort(items, items + index, compare_price);
        // show_items(items, index);

        int item_index = 0;
        while (sum > 0 && index > 0 && index > item_index)
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

        // cout << setprecision(3) << total << endl; 此输出不能满足需求，无法ac
        printf("%.3lf\n", total);
    } while (true);

    // double total = 12.3456789;
    // cout << setprecision(3) << total << endl;
    // printf("%.3lf\n", total);
    /* code */
    return 0;
}
