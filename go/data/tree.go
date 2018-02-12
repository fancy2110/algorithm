package data

//Tree 树的抽象接口
type Tree interface {
	Find(key Node) Node
	Delete(ket Node) bool
	Inert(key Node, value Node) bool
	Count() int
	Height() int
	Show()
}
