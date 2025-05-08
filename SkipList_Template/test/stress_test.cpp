#include <iostream>
#include "SkipList_Template.h"
#include <chrono>
#include <vector>
#include <iomanip>
#include <thread>

using namespace skip_list_template;

// 进行压力测试

constexpr int TEST_COUNTS = 1000000; // 插入、查找、删除一百万次
constexpr int range = 100 * TEST_COUNTS;
std::mutex testMtx;

template <typename key_t, typename value_t>
void insertTest(int testCounts, int threadNums, SkipList<key_t, value_t> &sl, std::vector<key_t> &keys)
{
    std::lock_guard<std::mutex> lock(testMtx);
    int nums = 0;
    int count = (int)(testCounts / threadNums);
    for (int i = 0; i < count; ++i)
    {
        key_t key = rand();
        value_t value = rand();
        bool flag = sl.insert(key, value);
        if (flag)
            keys.emplace_back(key);
        else
        {
            // 如果插入失败说明key是重复的 重新生成继续插入
            i--;
            continue;
        }
    }
    // std::cout << "insert same key nums: " << nums << std::endl;
}

template <typename key_t, typename value_t>
void searchTest(int testCounts, int threadNums, SkipList<key_t, value_t> &sl, std::vector<key_t> &keys)
{
    std::lock_guard<std::mutex> lock(testMtx);
    int nums = 0;
    int count = (int)(testCounts / threadNums);
    for (int i = 0; i < count; ++i)
    {
        key_t key = keys[i];
        Node<key_t, value_t> *flag = sl.search(key);
        if (flag == nullptr)
            ++nums;
    }
    // std::cout << "search non-exist key nums: " << nums << std::endl;
}

template <typename key_t, typename value_t>
void eraseTest(int testCounts, int threadNums, SkipList<key_t, value_t> &sl, std::vector<key_t> &keys, int order = 0)
{
    std::lock_guard<std::mutex> lock(testMtx);
    int nums = 0;
    int count = (int)(testCounts / threadNums);

    for (int i = 0; i < count; ++i)
    {
        // key_t key = rand();
        key_t key = keys[i + count * order];
        bool flag = sl.erase(key);
        if (flag == false)
            ++nums;
    }
    // std::cout << "erase non-exist key nums: " << nums << std::endl;
}

int main()
{
    srand(time(nullptr));
    std::chrono::system_clock::time_point start, end;
    double duration;
    SkipList<int, int> sl(20);

    // for (int i = 0; i < 10; ++i)
    // {
    //     // 单线程插入、查找、删除
    //     std::vector<int> keys;
    //     keys.reserve(TEST_COUNTS);
    //     // 插入操作
    //     start = std::chrono::system_clock::now();
    //     insertTest<int, int>(TEST_COUNTS, 1, sl, keys);
    //     end = std::chrono::system_clock::now();
    //     auto duration_1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    //     // std::cout << "insert " << TEST_COUNTS << ", time: " << duration_1 << "ms" << std::endl;
    //     // std::cout << "SkipList size: " << sl.getSize() << std::endl
    //     //           << std::endl;

    //     // 查找操作
    //     start = std::chrono::system_clock::now();
    //     searchTest<int, int>(TEST_COUNTS, 1, sl, keys);
    //     end = std::chrono::system_clock::now();
    //     auto duration_2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    //     // std::cout << "search " << TEST_COUNTS << ", time: " << duration_2 << "ms" << std::endl;
    //     // std::cout << "SkipList size: " << sl.getSize() << std::endl
    //     //           << std::endl;

    //     // 删除操作
    //     start = std::chrono::system_clock::now();
    //     eraseTest<int, int>(TEST_COUNTS, 1, sl, keys);
    //     end = std::chrono::system_clock::now();
    //     auto duration_3 = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    //     // std::cout << "erase " << TEST_COUNTS << ", time: " << duration_3 << "ms" << std::endl;
    //     // std::cout << "SkipList size: " << sl.getSize() << std::endl
    //     //           << std::endl;

    //     std::cout << "insert times: " << std::fixed << std::setprecision(2) << duration_1 << "ms,   "
    //               << "search times: " << std::fixed << std::setprecision(2) << duration_2 << "ms,   "
    //               << "erase times: " << std::fixed << std::setprecision(2) << duration_3 << "ms"
    //               << std::endl;

    //     keys.clear();
    // }


    // 多线程测试 4个线程
    for (int i = 0; i < 10; ++i)
    {
        int threadNums = 4;
        std::vector<std::thread> threads;
        // 多线程插入、查找、删除
        std::vector<int> keys;
        keys.reserve(TEST_COUNTS);
        // 插入操作
        start = std::chrono::system_clock::now();
        for (int i = 0; i < threadNums; ++i)
        {
            std::thread t(insertTest<int, int>, TEST_COUNTS, threadNums, std::ref(sl), std::ref(keys));
            threads.push_back(std::move(t));
        }
        for (auto &t : threads)
        {
            if (t.joinable())
                t.join();
        }
        end = std::chrono::system_clock::now();
        auto duration_1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
        // std::cout << "insert " << TEST_COUNTS << ", time: " << duration_1 << "ms" << std::endl;
        // std::cout << "SkipList size: " << sl.getSize() << std::endl
        //           << std::endl;

        // 查找操作
        threads.clear();
        start = std::chrono::system_clock::now();
        for (int i = 0; i < threadNums; ++i)
        {
            std::thread t(searchTest<int, int>, TEST_COUNTS, threadNums, std::ref(sl), std::ref(keys));
            threads.push_back(std::move(t));
        }
        for (auto &t : threads)
        {
            if (t.joinable())
                t.join();
        }
        end = std::chrono::system_clock::now();
        auto duration_2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
        // std::cout << "search " << TEST_COUNTS << ", time: " << duration_2 << "ms" << std::endl;
        // std::cout << "SkipList size: " << sl.getSize() << std::endl
        //           << std::endl;

        // 删除操作
        threads.clear();
        start = std::chrono::system_clock::now();
        for (int i = 0; i < threadNums; ++i)
        {
            std::thread t(eraseTest<int, int>, TEST_COUNTS, threadNums, std::ref(sl), std::ref(keys), i);
            threads.push_back(std::move(t));
        }
        for (auto &t : threads)
        {
            if (t.joinable())
                t.join();
        }
        end = std::chrono::system_clock::now();
        auto duration_3 = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
        // std::cout << "erase " << TEST_COUNTS << ", time: " << duration_3 << "ms" << std::endl;
        // std::cout << "SkipList size: " << sl.getSize() << std::endl
        //           << std::endl;

        std::cout << "insert times: " << std::fixed << std::setprecision(2) << duration_1 << "ms,   "
                  << "search times: " << std::fixed << std::setprecision(2) << duration_2 << "ms,   "
                  << "erase times: " << std::fixed << std::setprecision(2) << duration_3 << "ms"
                  << std::endl;
    }
}