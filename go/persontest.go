package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"

	"github.com/fancy2110/learn/data"
)

//IntNode 数字结点
type IntNode struct {
	value int
}

//PersonNode 个人结点
type PersonNode struct {
	properties []string
	phone      string
	id         int
}

//Compare 定义key
func (target *IntNode) Compare(b interface{}) int {
	nodeB, _ := b.(*IntNode)
	if target.value > nodeB.value {
		return 1
	} else if target.value == nodeB.value {
		return 0
	} else {
		return -1
	}
}

func (target *IntNode) String() string {
	return strconv.Itoa(target.value)
}

//Compare 定义person的数据结构
func (target *PersonNode) Compare(b interface{}) int {
	bValue, _ := b.(PersonNode)
	if target.id > bValue.id {
		return 1
	} else if target.id == bValue.id {
		return 0
	} else {
		return -1
	}
}

func (target *PersonNode) String() string {
	return fmt.Sprintf("ID:%d, Phone:%s", target.id, target.phone)
}

func main() {
	dataFile := "/Users/fanxi/Documents/vm/data/address.txt"

	reader, err := os.Open(dataFile)
	if err != nil {
		fmt.Println(dataFile, err)
		return
	}

	persons := data.NewTree()

	defer reader.Close()
	input := bufio.NewReader(reader)
	count := 20
	for count > 0 {
		line, err := input.ReadString('\n') //以'\n'为结束符读入一行

		if err != nil || io.EOF == err {
			fmt.Print(err)
			break
		}

		items := strings.Split(line, ",")

		id, err := strconv.Atoi(items[0])
		if err != nil {
			continue
		}

		node := new(IntNode)
		node.value = id

		person := new(PersonNode)
		person.id = id
		person.phone = items[5]
		person.properties = items

		persons.Insert(node, person)

		count--
	}

	persons.Show()
	node := new(IntNode)
	node.value = 3100

	data := persons.Find(node)
	fmt.Println(data)

	persons.Delete(node)
	node.value = 3103
	persons.Delete(node)
	node.value = 3106
	persons.Delete(node)

	node.value = 3093
	persons.Delete(node)
	node.value = 3095
	persons.Delete(node)
	persons.Show()
}
