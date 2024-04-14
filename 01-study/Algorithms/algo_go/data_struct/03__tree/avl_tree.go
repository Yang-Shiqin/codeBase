// TODO
/* avl树(平衡二叉搜索树)的常用操作:
    -   : 查找(bfs、dfs)
    -    : 遍历(层序、前中后序)
    -   : 插入删除？
*************************/
package main

import "fmt"

type TreeNode struct{
    Val int
    Height int  // avl相关操作需要获取节点高度(该节点到其最远叶节点距离, 叶节点高度0, 空节点高度-1)
    Left *TreeNode
    Right *TreeNode
}

func CreateNode(v int) *TreeNode{
    return &TreeNode{
        Val: v,
        Height: 0,
        Left: nil,
        Right: nil,
    }
}

func (t )

// 根据nodelist层序排(完美二叉树，不考虑nil没孩子), 构建二叉树
func createTree(nodeList []*TreeNode) *TreeNode{
    maxi := len(nodeList)
    for i, v := range nodeList{
        if v == nil{    // 没父节点的会被抛弃
            continue
        }
        if (i+1)*2-1>=maxi{ // 左节点
            break
        }
        v.left = nodeList[(i+1)*2-1]
        if (i+1)*2>=maxi{   // 右节点
            break
        }
        v.right = nodeList[(i+1)*2]
    }
    return nodeList[0]
}

// 遍历都测试过了，应该没问题
// 层序遍历
func (t *TreeNode) levelOrder() []*TreeNode{
    ret := []*TreeNode{t}
    head := 0
    for head < len(ret){
        if ret[head] == nil{
            head++
            continue
        }
        ret = append(ret, ret[head].left)
        ret = append(ret, ret[head].right)
        head++
    }
    return ret
}

// 前序遍历, 返回是靠出参返回
func (t *TreeNode) preOrder(ret *[]*TreeNode){
    if t == nil{
        return
    }
    *ret = append(*ret, t)
    t.left.preOrder(ret) 
    t.right.preOrder(ret)
}

// 中序遍历
func (t *TreeNode) inOrder(ret *[]*TreeNode){
    if t == nil{
        return
    }
    t.left.inOrder(ret) 
    *ret = append(*ret, t)
    t.right.inOrder(ret)
}

// 后序遍历
func (t *TreeNode) postOrder(ret *[]*TreeNode){
    if t == nil{
        return
    }
    t.left.postOrder(ret) 
    t.right.postOrder(ret)
    *ret = append(*ret, t)
}

// dfs、bfs和前序遍历、层序遍历很像就不写了

// 打印
func (t *TreeNode) printTree(){
    l := t.levelOrder()
    line_num := 1
    head := 0
    tail := head+line_num
    for (head<tail) && (tail<len(l)){
        line_num = 0
        for ; head<tail; head++{
            if l[head] != nil{
                line_num++
                fmt.Printf("%d ", l[head].val)
            }else{
                fmt.Printf("N ")
            }
        }
        line_num *= 2
        tail += line_num
        fmt.Println()
    }
}

func main(){
    var nodeList = make([]*TreeNode, 15)
    for i:=0; i<10; i++{
        nodeList[i] = &TreeNode{
            val:i,
            left:nil,
            right:nil,
        }
    }
    treeList := createTree(nodeList)
    treeList.printTree()
    //nodeList[2] = nil
    //createTree(nodeList).printTree()
    var ret []*TreeNode
    treeList.postOrder(&ret)
    fmt.Println(nodeList)
    fmt.Println(ret)
}

