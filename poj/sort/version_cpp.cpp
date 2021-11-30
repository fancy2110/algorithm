#include <iostream>

using namespace std;
int data[] = {0, 100, 1, 2, 8, 3, 1000, 50};
const int size = 8;

int *insert_sort()
{
    int *new_data = new int[size];
    for (int i = 0; i < size; i++)
    {
        new_data[i] = -1;
    }

    new_data[0] = data[0];

    int new_size = 1;
    for (int i = 1; i < size; i++)
    {
        int new_position = 0;
        for (; new_position < new_size; new_position++)
        {
            if (data[i] <= new_data[new_position])
            {
                break;
            }
        }

        new_size++;
        for (int t = new_size; t > new_position; t--)
        {
            new_data[t] = new_data[t - 1];
            /* code */
        }
        new_data[new_position] = data[i];
    }
    return new_data;
}

void quick_sort(int start, int end)
{
    if (end <= start)
        return;

    int mid = (end - start) / 2 + start;
    int i = start;
    int j = end;
    int flag = data[mid];
    cout << "i:" << i << ", j:" << j << endl;

    while (i < j)
    {
        while (i < j && flag > data[i])
            i++;
        while (i < j && flag < data[j])
            j--;

        if (i < j)
        {
            int tmp = data[i];
            data[i] = data[j];
            data[j] = tmp;
            i++;
            j--;
        }
    }

    int new_mid = (i + j) / 2;
    quick_sort(start, new_mid);
    quick_sort(new_mid + 1, end);
}

int main(int argc, char const *argv[])
{
    int *sorted_data = insert_sort();
    for (int i = 0; i < size; i++)
    {
        cout << sorted_data[i] << endl;
    }

    cout << "quick_sort" << endl;
    quick_sort(0, size - 1);
    for (int i = 0; i < size; i++)
    {
        cout << data[i] << endl;
    }

    delete[] sorted_data;
}
