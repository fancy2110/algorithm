//
// Created by 樊溪 on 2017/7/4.
//

#ifndef TOOL_BRTREE_H
#define TOOL_BRTREE_H

#include<iostream>

using  namespace std;

class BRTree {
    struct node {
        struct node *left;
        struct node *right;
        struct node *parent;

        bool color; 
        int value;
        int key;
    };

    struct node * watcher;

public:
    Integer() {}

    void show() const;
    void insert(int key, int value);
    void get(int key);

    //析构函数
    ~Integer() ;
};


#endif //STUDY_INTEGER_H