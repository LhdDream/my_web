//
// Created by kiosk on 2020/1/18.
//

#ifndef MY_WEB_PRIORITY_QUEUE_H
#define MY_WEB_PRIORITY_QUEUE_H
//
//使用一个hash map 和一个vector来进行实现
#include <utility>
#include <unordered_map>
#include <vector>


class Priority_queue {
public:
    using pair  = std::pair<size_t, uint64_t>;

    Priority_queue() = default;

    Priority_queue &operator=(const Priority_queue &other) = delete;

    bool Contains(size_t key) const {
        return m_pointer.find(key) != m_pointer.end();
    }

   size_t GetSize () const {
        return m_heap.size();
    }
    const pair &Top() const {
        return m_heap.front();
    }


    void Push(size_t key, uint64_t value) {
        if (Contains(key)) {
            const size_t index = m_pointer.at(key);
            m_heap.at(index).second = value;
            Heap_Up(index);
            //可能比之前结点小　／／那么进行下滤
            Heap_Down(index);
        } else {
            const size_t index = m_heap.size();
            m_pointer[key] = index;
            m_heap.emplace_back(key, value);
            Heap_Up(index);
        }

    }

    void Pop() {
        m_pointer.erase(m_heap.front().first);
        m_heap.front() = m_heap.back();
        m_heap.resize(m_heap.size() - 1);
        constexpr size_t index = 0;
        m_pointer[m_heap.front().first] = index;
        Heap_Down(index);
    }
private:
    //根据时间排序
    void Heap_Up(size_t index) {
        while (index != m_root) {
            auto &&parent = Parent(index);
            auto &&value = m_heap.at(index).second;
            auto &&parent_value = m_heap.at(parent).second;
            if (parent_value > value)
                break;
            Swap(index, parent);
            index = parent;
        }
    }

    void Heap_Down(size_t index) {
        while (index < m_heap.size()) {
            auto &&child = Leftchild(index);
            //超出堆的范围
            if (child >= m_heap.size()) {
                break;
            }
            //找不到相应的孩子
            if (child + 1 < m_heap.size()) {
                child = (m_heap.at(child + 1).second > m_heap.at(child).second) ? child + 1 : child;
            }
            if (m_heap.at(index).second > m_heap.at(child).second)
                break;
            Swap(index, child);
            index = child;
        }
    }

    //堆中元素的交换
    void Swap(size_t first, size_t second) {
        auto &&pair_first = m_heap.at(first);
        auto &&pair_second = m_heap.at(second);
        m_pointer[pair_first.first] = second;
        m_pointer[pair_second.first] = first;
        std::swap(m_heap[first], m_heap[second]);
    }

    inline size_t Parent(size_t index) const {
        return (index - 1) / 2;
    }

    inline size_t Leftchild(size_t index) const {
        return index * 2 + 1;
    }

    std::vector<pair> m_heap;
    std::unordered_map<size_t, size_t> m_pointer;
    static constexpr size_t m_root = 0;// 堆的根为0
};

#endif //MY_WEB_PRIORITY_QUEUE_H
