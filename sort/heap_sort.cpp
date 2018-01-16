/**
 * 插入排序 
 * 
 * @author 樊溪
 * @date 2017/10/27
 */
#include<iostream>
#include<fstream>
#include<vector>
#include<stdint.h>

using namespace std;

// begin with 1
void fix_heap(vector<int> &data, int root, int size)
{
    int node = root;
    while(node < size) {
        int left = node * 2 ;  
        int right = node * 2 + 1;

        int next_node = size;
        if (right > size && left > size) {
            break;
        }
        
        if (right > size && left <= size) {
            if (data[node-1] > data[left - 1])
            {
                next_node = left;
            } else {
                break;
            }
        }
        
        if (right <= size && left > size) {
            if (data[node - 1] > data[right - 1]) {
                next_node = right;
            } else {
                break;
            }  
        }
        
        if (right <= size && left <= size) {
            next_node = data[left-1] > data[right-1] ? right : left;
            if (data[node - 1] < data[next_node - 1]) {
                break;
            }
        }

        int min = data[node-1];
        data[node-1] = data[next_node-1];
        data[next_node-1] = min;
        node = next_node;
    }
}

/**
 * 
*/
void sort(vector<int> & data,int min, int max,  bool reverse) 
{
    for(int i = max / 2; i > 0; i--) {
        fix_heap(data, i, max);
    }

    int i = max;
    while ( i > min ) 
    {
        int min_value = data[0];
        data[0] = data[i - 1];
        data[i-1] = min_value;

        i --;
        fix_heap(data, 1, i);
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

    sort(items, 0, items.size(), true);

    ofstream out_data("out.txt");
    for (vector<int>::iterator it = items.begin(); it != items.end(); ++it)
    {
        out_data << *it << endl;
    }

    out_data.close();
    return 1;
}