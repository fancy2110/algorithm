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
    if (min >= max) return;
    int mid = data[min]; 
    int i = min;
    int j = max;
    while (i < j) {
        while(data[j] >= mid && i < j) j--;
        data[i] = data[j];
        while(data[i] <= mid && i < j) i++;
        data[j] = data[i]; 
    }

    data[i] = mid;
    sort(data, min, i, reverse);
    sort(data, i+1, max, reverse);
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