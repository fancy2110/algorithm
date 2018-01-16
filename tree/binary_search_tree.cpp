#include <stack>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct node {
    const static int size = 3;
    struct node * parent = NULL;
    int key[size] = {-1};
    int value[size] = {-1};
    struct node * branch[size+1] = { NULL };
};

struct node * create_node(int key , int value)
{
    struct node *item = new struct node();
    item->value[0] = value;
    item->key[0] = key;
    return item;
}

node* split_tree(node * iter) 
{

    int half = node::size / 2;

    if (iter->parent == NULL) {
        node* left = create_node(-1, -1);
        node* right = create_node(-1, -1);

        left->parent = iter;
        right->parent = iter;

        memcpy(left->key, iter->key, half * sizeof(int)); 
        memcpy(left->value, iter->value, half * sizeof(int)); 
        memcpy(left->branch, iter->branch, half * sizeof(struct node *)); 

        int left_number = (node::size - half - 1);
        memcpy(right->key, iter->key + half + 1,  left_number * sizeof(int)); 
        memcpy(right->value, iter->value + half + 1, left_number * sizeof(int)); 
        memcpy(right->branch, iter->branch + half + 1, left_number * sizeof(struct node *)); 

        iter->key[0] = iter->key[half];
        iter->value[0] = iter->value[half];
        iter->branch[0] = left;
        iter->branch[1] = right;

        memset(iter->key+1, -1, node::size - 1);
        memset(iter->value+1, -1, node::size - 1);
        memset(iter->branch+2, NULL, node::size - 1);
        return iter;
    } else {
        node* right = create_node(-1, -1);

        int left_number = (node::size - half - 1);
        memcpy(right->key, iter->key + half + 1,  left_number * sizeof(int)); 
        memcpy(right->value, iter->value + half + 1, left_number * sizeof(int)); 
        memcpy(right->branch, iter->branch + half + 1, left_number * sizeof(struct node *)); 

        memset(iter->key+half, -1, node::size - half);
        memset(iter->value+ half, -1, node::size - half);
        memset(iter->branch+ half, NULL, node::size - half);

        int key = iter->key[half];
        int value = iter->value[half];

        int i = node::size - 1;
        for(; i > 0; i--) {
            if (iter->parent->key[i] < key) {
                break;
            } 

            iter->parent->branch[i + 1] = iter->parent->branch[i];
            iter->parent->key[i] = iter->parent->key[i-1];
            iter->parent->value[i] = iter->parent->value[i-1];
        }

        iter->parent->key[i] = key;
        iter->parent->value[i] = value;
        iter->parent->branch[i+1] = right;

        return iter->parent;
    }
}

void insert(struct node* root, int key ,int value)
{
    node * iter = root;
    bool inserted = false;
    while (iter != NULL && !inserted) {
        for(int i=0; i < node::size; i++) {
            if (iter->key[i] == key){
                iter->value[i] = value;
                return;
            } 

            if (iter->key[i] > key) {
                if (iter->branch[i] != NULL) {
                    iter = iter->branch[i];
                    break;
                }

                int j = node::size - 1;
                while (j > i && iter->key[j] != -1) {
                    iter->branch[j+1] = iter->branch[j];
                    iter->key[j] = iter->key[j-1];
                    iter->value[j] = iter->value[j-1];

                    j --;
                }

                iter->branch[i] = NULL;
                iter->key[i] = key;
                iter->value[i] = value;

                inserted = true;
                break;
            }
        }
    }

    while (iter->key[node::size - 1] > 0) {
        iter = split_tree(iter); 
    }
}


void delete_node(struct node* root, int key) 
{
    node * iter = root;
    int i = 0;
    bool stopped = false;
    while (iter != NULL && !stopped) {
        for (; i < node::size; i++) {
            if (iter->key[i] >= key) {
                break;
            }
        }

        if (iter->key[i] == key) {
            break;
        }

        iter = iter->branch[i];
    }

    //find and delete
    if (iter != NULL) {

    }
}

int get(struct node * root, int key)
{
    struct node * iter = root;
    while (iter != NULL) {
        for (int i=0; i < node::size; i ++)
        {
            if (iter->key[i] == key) return;

            if (iter->key[i] > key) {
                iter = iter->branch[i];
                break;
            }
        }
        iter = iter->branch[node::size];
    }

    return -1;
}

int main(int argc, char ** argv) 
{
    struct node* root = create_node(-1,-1);

    ifstream in_data("in.dat");    
    if (! in_data.is_open()) 
    {
        std::cout << "File name error" << std::endl;
        return -1;
    }

    int i;
    while(in_data >> i)
    {
        insert(root, i, i);
    }

    return 1;
}