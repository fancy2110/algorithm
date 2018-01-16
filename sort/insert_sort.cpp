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
int sort(vector<int> & data, bool reverse) 
{
    int size = data.size();
    int min = 0;
    int min_pos = 0;
    for(int i=0; i < size; i++) 
    {
        min = data[i];
        for (int j = size - 1; j > i; --j) 
        {
            if (min < data[j])
            {
                continue;
            } 

            min = data[j];
            min_pos = j;
        }

        int num = data[i];
        data[i] = min;
        data[min_pos] = num;
    }
    return 0;
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

    sort(items, true);

    ofstream out_data("out.txt");
    for (vector<int>::iterator it = items.begin(); it != items.end(); ++it)
    {
        out_data << *it << endl;
    }

    out_data.close();
    return 1;
}