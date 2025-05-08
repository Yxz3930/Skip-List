#ifndef SKIPLIST_BASE_H
#define SKIPLIST_BASE_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <limits>
using namespace std;

namespace skip_list_base
{

constexpr int MAX_LEVEL = 16;
constexpr float P = 0.5;


struct Node
{
    int key; // 键值对中的键
    int value; // 键值对中的值
    Node** forward; // 不同层级上的下一个节点
    int level; // 当前节点拥有的层级(注意是层级 类似于索引的意思 实际的层数则是层级加一)

    Node(int _key, int _value, int _level): key(_key), value(_value), level(_level)
    {
        forward = new Node*[level + 1]; // 层级为零 但是实际上是第一层 依此类推 层级level实际上是第level+1层 所以需要加一
        for(int i=0; i<=level; ++i)
        {
            forward[i] = nullptr;
        }
    }
    ~Node(){
        delete[] forward; // 析构中delete
    }
};



class SkipList
{
private:
    int level; // 跳表的最大层级
    Node* header; // 头节点
    std::mt19937 gen;                          // 随机数引擎
    std::uniform_real_distribution<> distr;    // 均匀分布对象

    size_t size = 0; // 记录跳表中节点的个数 用于计算最高的层级 log_2 (size)

public:
    SkipList() : gen(std::random_device{}()), distr(0.0, 1.0), size(0), level(0)
    {
        // std::random_device{}() // 等价于std::random_device() 因为其内部重载了()运算符
        this->level = 0;
        this->header = new Node(-1, -1, MAX_LEVEL); // 需要析构中delete
    }

    ~SkipList(){
        Node* curNode = this->header->forward[0];
        while(curNode)
        {
            Node* nextNode = curNode->forward[0];
            delete curNode;
            curNode = nextNode;
        }
        delete this->header;
    }

    /// @brief 根据随机概率进行增加层级 层级索引
    /// @return 
    int randomLevel();


    /// @brief 根据key查找某个节点
    /// @param key 键
    /// @return 找到则返回节点地址 否则返回nullptr
    Node* search(int key);


    /// @brief 插入键值对
    /// @param key 键
    /// @param value 值
    void insert(int key, int value);


    /// @brief 删除键值对节点
    /// @param key 键
    void earse(int key);
    

    /// @brief 打印跳表
    void display();

    /// @brief 获取跳表中节点的个数
    /// @return 
    size_t getSize(){
        return this->size;
    }
};












    

}

#endif // SKIPLIST_BASE_H