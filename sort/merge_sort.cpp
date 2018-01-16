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

void merge(vector<int>& data, int min, int max, vector<int>& temp) 
{
    int mid = (min + max) / 2;  
    int j = min;
    int i = mid+1;

    int k = 0;
    while (j <= mid && i <= max) 
    {
        if (data[j] < data[i]) 
        {
            temp[k++] = data[j++];
        }  else {
            temp[k++] = data[i++];
        }
    }

    while(j <= mid) {
        temp[k++] = data[j++];
    }

    while(i <= max) {
        temp[k++] = data[i++];
    }

    for(int i = 0 ; i < k; i++) 
    {
        data[i+min] = temp[i];
    } 
}

/**
 * 
*/
void sort(vector<int> & data,int min, int max,  vector<int>& temp) 
{
    if (max - min < 1) {
        return;
    }

    int mid = (min + max) / 2;

    sort(data, min, mid, temp);
    sort(data, mid+1, max, temp);
    merge(data, min, max, temp);

    // cout << "min:" << min << ", max:" << max << endl;
    // for (vector<int>::iterator it = data.begin(); it != data.end(); ++it)
    // {
    //     cout << *it <<"," ;
    // }
    // cout << endl;
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

    vector<int> temp(items);
     sort(items, 0, items.size() - 1, temp);

    ofstream out_data("out.txt");
    for (vector<int>::iterator it = items.begin(); it != items.end(); ++it)
    {
        out_data << *it << endl;
    }

    out_data.close();
    return 1;
}