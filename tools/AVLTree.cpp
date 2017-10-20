#include "AVLTree.h"
#include <stack.h>

using namespace std;

AVLTree::AVLTree() {
    this->watcher = new struct node();
}

AVLTree::~AVLTree() {
    struct node * prt = new struct node();
}


AVLTree::show() 
{
    stack<struct node*> tracer();
    struct node * prt = this->root;


}

AVLTree::insert(struct node* root, int key, int value)
{
    if (key >= root->key) {
        if (root->right != watcher) {
            insert(root->right, key, value);    
        } else {
            root->right =  create_node(key, value);
            root->right->parent = root;
        }
    } else {
        if (root->left != watcher)
        {
            insert(root->left, key, value);
        } else {
            root->left = create_node(key, value);
            root->left->parent = root;
        }
    }

    int left_height = (left == NULL ? 0 : left->height);
    int right_height = (right == NULL ? 0 : right->height); 

    int sub_height = left_height - right_height;
    if (sub_height < -1)  //左子树比右子树高度要高
    {
        if (root->parent->left == root) { //该结点为父节点的左子树则
            llRotate(root);
        } else if (root->parent->right == root) {
            rrRotate(root);
        }
    }

    if (sub_height > 1) 
    {
        if (root->parent->left == root) 
        {
            lrRotate(root);
        } else if (root->parent->right == root) {
            rlRotate(root);
        }

    }
}

void AVLTree::llRotate(struct node * root)
{
    //转化父节点关系
    root->parent->left = root->left;
    root->left->parent = root->parent;
    root->parent = root->left;

    root->left = root->parent->right;
    root->parent->right = root;

    root->height = max(root->left, root->right);
    root->parent->height = max(root->parent->left, root->parent->right);

}

void AVLTree::lrRotate(struct node *root)
{
    root->parent->left = root->right;
    root->right->parent = root->parent;
    root->parent = root->right;

    root->right = root->parent->left;
    root->parent->left = root;

    root->height = max(root->left, root->right);
    root->parent->height = max(root->parent->left, root->parent->right);
}

void AVLTree::rrRotate(struct node *root)
{
     //转化父节点关系
    root->parent->right = root->right;
    root->right->parent = root->parent;
    root->parent = root->right;

    root->right = root->parent->left;
    root->parent->left = root;

    root->height = max(root->left, root->right);
    root->parent->height = max(root->parent->left, root->parent->right);
}

void AVLTree::rlRotate(struct node * root)
{
     //转化父节点关系
    root->parent->right = root->left;
    root->left->parent = root->parent;
    root->parent = root->left;

    root->left = root->parent->right;
    root->parent->right = root;

    root->height = max(root->left, root->right);
    root->parent->height = max(root->parent->left, root->parent->right);
}

AVLTree::insert(int key, int value) 
{
    if (root == NULL)
    {
        root = create_node(key, value);
        return;
    }

    insert(root, key, value);
}

AVLTree::get(int key) 
{
    struct node *iter = root;
    while(iter != watcher)
    {
        if (iter->key == key) 
        {
            return iter->value;
        }

        iter = (iter->key > key ? iter->left : iter->right);
    }

    return -1; //未找到
}