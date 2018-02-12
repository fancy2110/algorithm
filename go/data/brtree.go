package data

import (
	"fmt"
)

//RED 红色
const RED bool = true

//BLACK 黑色
const BLACK bool = false

//Element 数据结点
type Element struct {
	value  Node
	key    Node
	left   *Element
	right  *Element
	parent *Element
	color  bool //True red color , False black color
}

//BRTree 红黑树的数据结构
type BRTree struct {
	//红黑树根节点
	root *Element
	//结点数量
	count int
	//树高度
	height int
}

//NewTree 构造新树
func NewTree() *BRTree {
	tree := new(BRTree)
	return tree
}

//Insert 插入新的数据
func (tree *BRTree) Insert(key Node, value Node) bool {
	parent, node := search(tree, key)
	if tree.root == nil {
		elem := create(tree, key, value)
		tree.root = elem
		elem.parent = nil
		fix(tree, elem)
		return true
	}

	if node == nil {
		elem := create(tree, key, value)
		//为找到对应的key的节点，添加结点信息
		if key.Compare(parent.key) > 0 {
			parent.right = elem
		} else {
			parent.left = elem
		}
		elem.parent = parent
		fix(tree, elem)
		return false
	}

	node.value = value
	return true
}

//Find key Node 结点的键值;
//查找对应的key的值
func (tree *BRTree) Find(key Node) Node {
	_, node := search(tree, key)

	if node != nil {
		return node.value
	}

	return nil
}

//Delete key Node 删除结点
func (tree *BRTree) Delete(key Node) bool {
	_, node := search(tree, key)

	if node == nil {
		return false
	}

	pNode := node
	for node.left != nil || node.right != nil {
		if node.right != nil {
			pNode = node.right
			for pNode.left != nil {
				fmt.Printf("right key:%s\n", pNode.key)
				pNode = pNode.left
			}
		} else if node.left != nil {
			pNode = node.left
			for pNode.right != nil {
				fmt.Printf("left key:%s\n", pNode.key)
				pNode = pNode.right
			}
		}

		tmpKey := node.key
		node.key = pNode.key
		pNode.key = tmpKey

		tmpValue := node.value
		node.value = pNode.value
		pNode.value = tmpValue

		node = pNode
		fmt.Printf("pNode.key:%s\n", node.key)
	}

	//delete root node
	if node.parent == nil {
		tree.root = nil
		return true
	}

	parent := node.parent
	if node.parent.left == node {
		node.parent.left = nil
	} else {
		node.parent.right = nil
	}

	//delete red node
	if node.color == RED {
		node.parent = nil
		node.left = nil
		node.right = nil
		return true
	}

	if parent.left != nil || parent.right != nil {
		return true
	}

	if parent.color == RED {
		parent.color = BLACK
		return true
	}

	var anotherChild *Element
	for parent != tree.root {
		superParent := parent.parent

		if superParent.left == parent {
			anotherChild = superParent.right
			rotateLeft(tree, parent)
		} else {
			anotherChild = superParent.left
			rotateRight(tree, anotherChild)
		}

		if superParent.color == BLACK {
			if anotherChild.color == BLACK {
				//改分支减少了一个层级
				superParent.color = RED
				parent = parent.parent
				continue
			}
		}

		break
	}

	if tree.root.color == RED {
		tree.root.color = BLACK
	}

	return true
}

//fix 维持树的平衡性
func fix(tree *BRTree, node *Element) {
	for {
		//1、插入根节点
		if node.parent == nil {
			node.color = BLACK
			return
		}

		//插入的父节点为黑结点
		if node.parent.color == BLACK {
			return
		}

		//插入的叔父节点为红结点
		superParent := node.parent.parent
		if superParent.left != nil && superParent.left.color == RED && superParent.right != nil && superParent.right.color == RED {
			superParent.left.color = BLACK
			superParent.right.color = BLACK
			superParent.color = RED
			node = node.parent.parent
			continue
		}

		//插入的叔父节点为黑节点
		parent := node.parent
		if superParent.left == parent {
			if parent.left == node {
				superParent.color, parent.color = rotateRight(tree, parent)
			} else {
				parent.color, node.color = rotateLeft(tree, node)
				superParent.color, parent.color = rotateRight(tree, parent)
			}
		} else if superParent.right == parent {
			if parent.right == node {
				superParent.color, parent.color = rotateLeft(tree, node.parent)
			} else {
				parent.color, node.color = rotateRight(tree, node)
				superParent.color, parent.color = rotateLeft(tree, parent)
			}
		}
		//不改变父结点颜色，不用递归修改
		return
	}
}

func rotateRight(tree *BRTree, node *Element) (bool, bool) {
	parent := node.parent
	if parent == tree.root {
		tree.root = node
		node.parent = nil
	} else {
		base := node.parent.parent
		if base.left == parent {
			base.left = node
			node.parent = base
		} else {
			base.right = node
			node.parent = base
		}
	}

	parent.left = node.right
	if parent.left != nil {
		parent.left.parent = parent
	}
	parent.parent = node
	node.right = parent

	return node.color, parent.color
}

func rotateLeft(tree *BRTree, node *Element) (bool, bool) {
	parent := node.parent

	if node.parent == tree.root {
		tree.root = node
		node.parent = nil
	} else {
		base := node.parent.parent
		if base.left == parent {
			base.left = node
			node.parent = base
		} else {
			base.right = node
			node.parent = base
		}

	}

	parent.right = node.left
	if parent.right != nil {
		parent.right.parent = parent
	}

	parent.parent = node
	node.left = parent

	return node.color, parent.color
}

func swap(a bool, b bool) (bool, bool) {
	return b, a
}

func swapNode(a Node, b Node) (Node, Node) {
	return b, a
}

//create 创建新结点
func create(tree *BRTree, key Node, value Node) *Element {
	elem := new(Element)
	elem.key = key
	elem.value = value
	elem.left = nil
	elem.right = nil
	elem.color = RED
	return elem
}

//检索具体的结点是否存在
func search(tree *BRTree, key Node) (*Element, *Element) {
	if tree == nil {
		return nil, nil
	}

	var parent *Element
	node := tree.root

	for node != nil {
		ret := key.Compare(node.key)

		if ret == 0 {
			break
		}

		parent = node
		if ret > 0 {
			node = node.right
		} else {
			node = node.left
		}
	}

	return parent, node
}

//Count 返回数中的结点
func (tree *BRTree) Count() int {
	if tree == nil {
		return 0
	}

	return tree.count
}

//Height 返回树中的结点
func (tree *BRTree) Height() int {
	if tree == nil {
		return 0
	}

	return tree.height
}

//Show 打印树
func (tree *BRTree) Show() {
	fmt.Println("Print Tree")
	show(tree.root)
	fmt.Println("End Print Tree")
}

//Show tree
func show(node *Element) {
	if node == nil {
		return
	}

	if node.parent == nil {
		fmt.Printf("root:%s, color:%s\n", node.key, node.color)
	} else {
		fmt.Printf("key:%s, color:%s -> parent:%s\n", node.key, node.color, node.parent.key)
	}

	show(node.left)
	show(node.right)
}
