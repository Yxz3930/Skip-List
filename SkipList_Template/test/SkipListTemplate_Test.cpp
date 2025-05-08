#include <iostream>
#include "SkipList_Template.h"

using namespace skip_list_template;

int main()
{

    SkipList<int, int> sl;
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
    Node<int, int> *node = sl.search(1);
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
    sl.erase(1);
    sl.erase(2);
    sl.erase(3);
    sl.erase(4);
    sl.erase(5);
    sl.display();

    // 删除不存在的键
    std::cout << std::endl
              << "===== delete operator (non-exist key) =====" << std::endl;
    sl.erase(100);
    sl.erase(200);


    return 0;
}