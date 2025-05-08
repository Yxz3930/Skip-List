#include <iostream>
#include "SkipList_Base.h"

using namespace skip_list_base;

int main()
{
    SkipList sl;
    sl.display();

    // 插入
    std::cout << std::endl
              << "===== insert operator =====" << std::endl;
    sl.insert(1, 10);
    sl.insert(2, 20);
    sl.insert(3, 30);
    sl.insert(4, 40);
    sl.insert(5, 50);
    sl.insert(6, 60);
    sl.insert(7, 70);
    sl.insert(8, 80);
    sl.display();

    // 插入已存在的键
    std::cout << std::endl
              << "===== insert operator (exist key) =====" << std::endl;
    sl.insert(1, 10);
    sl.insert(2, 20);
    sl.display();

    // 查找
    std::cout << std::endl
              << "===== search operator =====" << std::endl;

    Node *node = sl.search(1);
    std::cout << "key: " << node->key << ", value: " << node->value << std::endl;
    node = sl.search(2);
    std::cout << "key: " << node->key << ", value: " << node->value << std::endl;
    node = sl.search(3);
    std::cout << "key: " << node->key << ", value: " << node->value << std::endl;

    // 查找不存在的键
    std::cout << std::endl
              << "===== search operator (non-exist key) =====" << std::endl;
    sl.search(100);
    sl.search(200);

    // 删除
    std::cout << std::endl
              << "===== delete operator =====" << std::endl;
    sl.earse(1);
    sl.earse(2);
    sl.display();

    sl.insert(1, 100);
    sl.insert(11, 110);
    sl.display();


    // 删除不存在的键
    std::cout << std::endl
              << "===== delete operator (non-exist key) =====" << std::endl;
    sl.earse(100);
    sl.earse(200);
    sl.display();

    return 0;
}