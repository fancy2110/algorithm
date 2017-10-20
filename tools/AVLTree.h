//
// Created by 樊溪 on 2017/7/4.
//

#ifndef TOOL_AVLTREE_H
#define TOOL_AVLTREE_H

#include<iostream>

using  namespace std;

class AVLTree {
    struct node {
        struct node *left;
        struct node *right;
        struct node *parent;

        int key;
        int value;
        int height; 
    };

    struct node * watcher;
    struct node * root;

    void insert(struct node* root, int key , int value);
    void llRotate(struct node* root); 
    void lrRotate(struct node* root);
    void rrRotate(struct node* root);
    void rlRotate(struct node* root);

    //创建新节点
    struct node * create_node(int key, value)
    {
        struct node * leaf = new struct node();
        leaf->parent = leaf->left = leaf->right = watcher;
        leaf->key = key;
        leaf->value = value;
        leaf->height = 1;
        return leaf;
    }

    //获取左右子树中加大的高度
    int max_height(struct node* left, struct node* right) 
    {
        int max_left = left == NULL ? 0 : left->height;
        int max_right = right == NULL ? 0 : right->height;

        return max_left > max_right ? max_left : max_right;
    }

public:
    AVLTree();

    void show() const;
    void insert(int key, int value);
    void remove(int key);
    void get(int key) const;

    //析构函数
    ~AVLTree() ;
};


#endif //TOOL_AVL_H