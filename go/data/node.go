package data

//Node 数据结点接口
type Node interface {
	//Compare 比较与目标节点的大小
	Compare(b interface{}) int
}
