#include "SkipList_Base.h"
#include <cmath>

namespace skip_list_base
{

    int SkipList::randomLevel()
    {
        // 根据跳表中节点的个数确定最大层级
        int maxLevel = (int)log2(this->size) - 1;
        std::cout << "maxLevel: " << maxLevel << std::endl;
        int level = 0;
        
        while(level < maxLevel && this->distr(gen) < P)
            ++level;

        return level;
    }

    Node *SkipList::search(int key)
    {
        Node *curNode = this->header;
        // 从上往下每个层级的遍历
        for (int i = this->level; i >= 0; --i)
        {
            // 每个层级中链表进行查找操作
            while (curNode->forward[i] && curNode->forward[i]->key < key)
                curNode = curNode->forward[i];
        }
        // 因为上面for循环和while循环跳出的条件是i=0且curNode->forward[0]->key > key 或者 curNode->forward[0]为nullptr
        // 所以这里需要往前走一步 获取等于key的那个节点
        curNode = curNode->forward[0];
        if (curNode && curNode->key == key)
            return curNode;
        else
        {
            std::cout << "no key: " << key << std::endl;
        }

        return nullptr;
    }

    void SkipList::insert(int key, int value)
    {
        // 用于记录每个层级上(包括层级0)需要添加节点的前一个节点 这样才可以进行插入
        Node *recordPreNode[MAX_LEVEL + 1];
        Node *curNode = this->header;

        // 从最高层级往下查找 记录应该插入的位置前一个节点
        for (int i = this->level; i >= 0; --i)
        {
            while (curNode->forward[i] && curNode->forward[i]->key < key)
                curNode = curNode->forward[i];

            // 在每个层级上记录一下插入位置的前一个节点
            recordPreNode[i] = curNode;
        }

        // 找到层级0的下一个节点位置 用这个判断一下是否存在相同key的节点
        curNode = curNode->forward[0];

        if (!curNode || curNode->key != key)
        {
            // 已经确定要添加这个节点了 所以这里先将size++ 后续计算层级的时候需要已经加1的size
            this->size++;

            // 下一个节点不存在或者节点的键不一样
            int newLevel = this->randomLevel();
            if (newLevel > this->level)
            {
                // 如果新生成的层级高于目前跳表的层级 则说明超过的层级上前一个节点是header
                for (int i = level + 1; i <= newLevel; ++i)
                {
                    recordPreNode[i] = header;
                }
                this->level = newLevel;
            }

            // 更新节点的前一个节点
            Node *newNode = new Node(key, value, newLevel);
            for (int i = 0; i <= newLevel; ++i)
            {
                newNode->forward[i] = recordPreNode[i]->forward[i];
                recordPreNode[i]->forward[i] = newNode;
            }

            std::cout << "insert: {" << key << ", " << value << "}, level: " << newLevel << std::endl;

            // std::cout << "Inserted key " << key << " with level " << newLevel << std::endl;
        }
        else if (curNode->key == key)
        {
            // 如果下一个节点的键与插入的键一致 则更新 或者报错
            std::cout << "exist same key" << std::endl;

            curNode->value = value;
        }
    }

    void SkipList::erase(int key)
    {
        Node *recordPreNode[MAX_LEVEL + 1];
        Node *curNode = this->header;
        for (int i = this->level; i >= 0; --i)
        {
            // 如果当前层级中 下一个节点存在且节点的键小于查找的键 则需要当前链表继续往前找
            while(curNode->forward[i] && curNode->forward[i]->key < key)
                curNode = curNode->forward[i];
            recordPreNode[i] = curNode;
        }

        // 找到对应key的节点 但也有可能不存在
        curNode = curNode->forward[0];
        if(curNode && curNode->key == key)
        {
            int value = curNode->value;
            // 如果存在 则进行删除
            for(int i = 0; i<=curNode->level; ++i)
            {
                Node* nextNode = recordPreNode[i]->forward[i];
                recordPreNode[i]->forward[i] = nextNode->forward[i];
                
            }
            // 不能在for循环里面进行删除 因为本质上需要删除的就只有一个节点而已
            delete curNode;

            // 跳表的层级可能需要降层
            // 如果level大于0 并且 从上往下头节点的下一个节点为nullptr 说明该层已经没有了 需要降一级
            while(this->level > 0 && this->header->forward[this->level] == nullptr)
                this->level--;

            std::cout << "delete: {" << key << ", " << value << "}" << std::endl;
            this->size--;
        }
        else
        {
            std::cout << "key not exist" << std::endl;
            
        }
    }



    void SkipList::display()
    {
        std::cout << "\n===== Skip List =====\n";
        for(int i=this->level; i>=0; --i)
        {
            std::cout << "Level: " << i << ": "; 
            Node* curNode = this->header;
            while(curNode)
            {
                std::cout << "{" << curNode->key << ", " << curNode->value << "}" << "; ";
                // std::cout << "key: " << curNode->key << ", value: " << curNode->value << " ";
                curNode = curNode->forward[i];
            }
            std::cout << std::endl;
        }
    }
}
