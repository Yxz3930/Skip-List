#ifndef SKIPLIST_TEMPLATE_H
#define SKIPLIST_TEMPLATE_H

#include <atomic>
#include <random>
#include <iostream>
#include <cmath>
#include <mutex>

namespace skip_list_template
{

    template <typename key_t, typename value_t>
    struct Node
    {
        key_t key;
        value_t value;
        int level; // 该节点对应的level层级（注意是索引 而不是真实的第几层）
        Node<key_t, value_t> **forward;

        Node() {}
        Node(key_t _key, value_t _value, int _level);
        ~Node();
    };

    template <typename key_t, typename value_t>
    inline Node<key_t, value_t>::Node(key_t _key, value_t _value, int _level)
    {
        key = _key;
        value = _value;
        level = _level;

        forward = new Node *[level + 1]; // 析构中释放
        for (int i = 0; i <= level; ++i)
            forward[i] = nullptr;
    }

    template <typename key_t, typename value_t>
    inline Node<key_t, value_t>::~Node()
    {
        delete[] forward;
    }

    /**
     * ****************************************
     * 下面进行跳表类的设计
     * ****************************************
     */

    // constexpr int MAX_LEVEL = 16; // 设定跳表最高层级为16层(总共17层) 对应着2^(MAX_LEVEL + 1)个节点
    constexpr float P = 0.5f;

    template <typename key_t, typename value_t>
    class SkipList
    {
    private:
        int level = 0;                     // 当前跳表的层级
        Node<key_t, value_t> *dummyHeader; // 虚拟头节点
        size_t size = 0;                        // 跳表中的节点数量
        int MAX_LEVEL = 16;

        // 随机引擎和分布
        std::mt19937 gen;
        std::uniform_real_distribution<float> distribution;

        // 互斥锁
        std::mutex skipListMtx;

    public:
        SkipList(int _MAX_LEVEL = 16);

        ~SkipList();

        /// @brief 随机生成层级
        /// @return
        int randomLevel();

        /// @brief 查找key
        /// @param key 键
        /// @return 找到则返回该节点地址 否则返回nullptr
        Node<key_t, value_t> *search(key_t key);

        /// @brief 插入节点
        /// @param key 键
        /// @param value 值
        /// @return 成功插入返回true 否则返回false
        bool insert(key_t key, value_t value);

        /// @brief 删除节点
        /// @param key 键
        /// @return 成功删除返回true 否则返回false
        bool erase(key_t key);

        /// @brief 打印跳表
        void display();

        /// @brief 获取跳表中节点的个数 实际上只有第零层有节点
        /// @return 
        size_t getSize(){
            return this->size;
        }
    };

    template <typename key_t, typename value_t>
    inline SkipList<key_t, value_t>::SkipList(int _MAX_LEVEL) : level(0), size(0), MAX_LEVEL(_MAX_LEVEL), gen(std::random_device{}()), distribution(0.0f, 1.0f) // 构造函数的初始化列表顺序需要和类中变量的声明顺序一致才可以
    {
        // 创建头节点 头节点需要和MAX_LEVEL的层级对应
        // 由于类型未知 并且头节点对于键值对不关心 所以可以直接初始化进行操作
        key_t key = key_t();
        value_t value = value_t();
        this->dummyHeader = new Node<key_t, value_t>(key, value, MAX_LEVEL);
    }

    template <typename key_t, typename value_t>
    inline SkipList<key_t, value_t>::~SkipList()
    {
        // 迭代删除第一个节点
        Node<key_t, value_t> *curNode = this->dummyHeader;
        while (curNode->forward[0])
        {
            Node<key_t, value_t> *delNode = curNode->forward[0];
            curNode->forward[0] = delNode->forward[0];
            delete delNode;
        }
        delete curNode;

        // // 迭代删除下一个节点
        // Node<key_t, value_t>* curNode = this->dummyHeader->forward[0];
        // while(curNode)
        // {
        //     Node<key_t, value_t>* nextNode = curNode->forward[0];
        //     delete curNode;
        //     curNode = nextNode;
        // }
        // delete headerNode;
    }

    template <typename key_t, typename value_t>
    inline int SkipList<key_t, value_t>::randomLevel()
    {
        // 根据节点数量动态确定当前的最大层级
        int maxLevel = static_cast<int>(log2(this->size)) - 1;
        if(maxLevel > MAX_LEVEL)
            maxLevel = MAX_LEVEL;
        // std::cout << "maxLevel: " << maxLevel << std::endl;
        int level = 0;

        // level最大可用达到maxLevel 刚好不超过最大层级
        while (level < maxLevel && this->distribution(this->gen) < P)
            ++level;

        return level;
    }

    template <typename key_t, typename value_t>
    inline Node<key_t, value_t> *SkipList<key_t, value_t>::search(key_t key)
    {
        std::lock_guard<std::mutex> lock(this->skipListMtx);
        Node<key_t, value_t> *curNode = this->dummyHeader;
        for (int i = this->level; i >= 0; --i)
        {
            while (curNode->forward[i] && curNode->forward[i]->key < key)
                curNode = curNode->forward[i];
        }

        curNode = curNode->forward[0];
        if (curNode && curNode->key == key)
        {
            return curNode;
        }
        else
        {
            // std::cout << "search: no key (" << key << ")" << std::endl;
            return nullptr;
        }

        return nullptr;
    }

    template <typename key_t, typename value_t>
    inline bool SkipList<key_t, value_t>::insert(key_t key, value_t value)
    {
        std::lock_guard<std::mutex> lock(this->skipListMtx);

        // 记录需要插入节点位置的前一个节点
        // 数组大小为 MAX_LEVEL + 1 因为层级可能超过当前跳表的层级
        Node<key_t, value_t> *recordPreNode[MAX_LEVEL + 1];
        Node<key_t, value_t> *curNode = this->dummyHeader;

        // 遍历每个层级
        // 在每个层级中查找
        for (int i = this->level; i >= 0; --i)
        {
            while (curNode->forward[i] && curNode->forward[i]->key < key)
                curNode = curNode->forward[i];
            recordPreNode[i] = curNode;
        }
        curNode = curNode->forward[0];

        if (!curNode || curNode->key != key)
        {
            this->size++;
            // 为空或者不等于key 说明不存在这个节点 需要插入
            int newLevel = this->randomLevel();
            Node<key_t, value_t> *node = new Node<key_t, value_t>(key, value, newLevel);
            if (newLevel > this->level)
            {
                // 如果新的level大于跳表的level 说明需要提高跳表层级
                for(int i = this->level +1 ; i<=newLevel; ++i)
                    recordPreNode[i] = this->dummyHeader;
                this->level = newLevel;            
            }

            // 开始在每个层级上插入节点
            for (int i = 0; i <= newLevel; ++i)
            {
                node->forward[i] = recordPreNode[i]->forward[i];
                recordPreNode[i]->forward[i] = node;
            }

            // std::cout << "insert: {" << key << ", " << value << "}, level: " << newLevel << std::endl;
            return true;
        }
        else
        {
            // 如果下一个节点的键与插入的键一致 则更新 或者报错
            // std::cout << "exist same key" << std::endl;
            // curNode->value = value;
            return false;
        }

        return false;
    }

    template <typename key_t, typename value_t>
    inline bool SkipList<key_t, value_t>::erase(key_t key)
    {
        Node<key_t, value_t>* recordPreNode[this->level + 1]; 
        Node<key_t, value_t>* curNode = this->dummyHeader;
        for(int i = this->level; i>=0; --i)
        {
            while(curNode->forward[i] && curNode->forward[i]->key < key)
                curNode = curNode->forward[i];
            recordPreNode[i] = curNode;
        }
        curNode = curNode->forward[0];

        if(curNode && curNode->key == key)
        {
            // 如果节点存在且节点的key等于要删除的key 说明找到需要删除的节点了
            // value_t value = curNode->value;

            // 删除该节点在各层级上的位置
            for(int i = 0; i<=curNode->level; ++i)
            {
                recordPreNode[i]->forward[i] = curNode->forward[i];
                curNode->forward[i] = 0;
            }
            // 因为实际上只有一个节点 所以需要将所有的层级删除完成之后在删除该节点
            delete curNode;


            // 尝试一下看看是否需要降低层级
            // 从最高层级往下判断 如果当前层级上只有一个头节点 下一个节点为nullptr 则需要删除这个层级
            while(this->level > 0 && this->dummyHeader->forward[this->level] == nullptr)
                this->level--;

            // std::cout << "delete: {" << key << ", " << value << "}" << std::endl;
            this->size--;
            return true;
        }
        else
        {
            // std::cout << "key not exist" << std::endl;
            return false;
        }
        return false;
    }

    template <typename key_t, typename value_t>
    inline void SkipList<key_t, value_t>::display()
    {
        std::cout << "\n===== SkipList =====" << std::endl;
        for (int i = this->level; i >= 0; --i)
        {
            Node<key_t, value_t> *curNode = this->dummyHeader;
            std::cout << "Level " << i << " ";
            while (curNode)
            {
                std::cout << "{" << curNode->key << ", " << curNode->value << "}" << "; ";
                curNode = curNode->forward[i];
            }
            std::cout << std::endl;
        }
    }
}

#endif // SKIPLIST_TEMPLATE_H