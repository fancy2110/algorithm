/**
 * 插入排序 
 * 
 * @author 樊溪
 * @date 2017/10/27
 */
#include<iostream>
#include<fstream>
#include<vector>

using namespace std;
/**
 * 
*/
void sort(vector<int> & data,int min, int max,  bool reverse) 
{
    int step = 0; 
    int size = data.size();
    while (step <= size / 3) {
       step = step * 3 + 1;
    }

    for(; step >= 1; )
    {
        for(int i = 0; i < size; i += step)
        {
            int min_position = i;
            for (int j = i; j < size; j += step) 
            {
                if (data[min_position] > data[j]) {
                    min_position = j;
                }
            }

            int tmp = data[i];
            data[i] = data[min_position];
            data[min_position] = tmp;

        }

        step = (step - 1) / 3;
    }
}

int main() 
{
    vector<int> items;

    ifstream in_data("in.dat");    
    if (! in_data.is_open()) 
    {
        std::cout << "File name error" << std::endl;
        return -1;
    }

    int i;
    while(in_data >> i)
    {
        items.push_back(i);
    }

    sort(items, 0, items.size() - 1, true);

    ofstream out_data("out.txt");
    for (vector<int>::iterator it = items.begin(); it != items.end(); ++it)
    {
        out_data << *it << endl;
    }

    out_data.close();
    return 1;
}