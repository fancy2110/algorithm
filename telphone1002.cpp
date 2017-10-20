#include <iostream>
using namespace std;

struct node {
    int value;
    int count;

    struct node * left;
    struct node * right;
};


//二叉树插入 todo 变化为平衡二叉树
void insert(struct node * root, int value) {
    if (root->value == -1) {
        root->value = value;  
        root->count = 1;
        return;
    }

    if (root->value == value) 
    {
        root->count ++;
        return;
    }

    if (root->value > value) {
        if (root->left != NULL) 
        {
            insert(root->left, value);
        } else {
            root->left = new struct node;
            root->left->value = value;
            root->left->count = 1;
        }
    } else {
        if (root->right != NULL)
        {
            insert(root->right, value);
        } else {
            root->right = new struct node;
            root->right->value = value;
            root->right->count = 1;
        }
    }
}


void show(struct node* root) 
{
    if (root == NULL) return;

    if (root->left != NULL) 
    {
        show(root->left);
    } 

    cout << root->value / 10000 << "-" << root->value % 10000 << " " << root->count << endl;
    
    if (root->right != NULL)
    {
        show(root->right);
    }
}

int main() {
    int number = 0;
    cin >> number;

    //根节点
    struct node * root = new struct node;
    root->value = -1;

    int map[256] =  {0};
    map['A'] = map['B'] = map['C'] = 2;
    map['D'] = map['E'] = map['F'] = 3;
    map['G'] = map['H'] = map['I'] = 4;
    map['J'] = map['K'] = map['L'] = 5;
    map['M'] = map['N'] = map['O'] = 6;
    map['P'] = map['R'] = map['S'] = 7;
    map['T'] = map['U'] = map['V'] = 8;
    map['W'] = map['X'] = map['Y'] = 9; 

    char phone[16] = {'\0'};

    while(number > 0) {
        for(int i=0; i< 16; i++) {
            phone[i] = '\0';
        }

        cin >> phone;
        int new_value = 0;
        for(int i = 0; i < 16 ; i ++)
        {
            if (phone[i] == '-')  
            {
                continue;
            }

            if (phone[i] == '\0') break;

            if (phone[i] >= 'A' && phone[i] <= 'Y') {
                new_value = map[phone[i]] + new_value * 10;
            } else {
                new_value = (phone[i] - '0') + new_value * 10;
            }
        }

        insert(root, new_value);
        number--;
    }

    show(root);
    return 0;
}
