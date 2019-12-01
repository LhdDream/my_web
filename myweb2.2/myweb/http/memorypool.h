//
// Created by kiosk on 19-10-15.
//

#ifndef MY_REDIES_MEMORYBLOCK_H
#define MY_REDIES_MEMORYBLOCK_H

#include <climits>
#include <cstddef>
#include <utility>
#include <cstdint>
#include <memory>


template<typename T, size_t BlockSize = 4096>
class MemoryPool {
public:
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef const T *const_pointer;
    typedef const T &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef std::false_type propagate_on_container_copy_assignment;
    typedef std::true_type propagate_on_container_move_assignment;
    typedef std::true_type propagate_on_container_swap;
    //编译期限中的true和false
    template<typename U>
    struct rebind {
        typedef MemoryPool<U> other;
    };

    MemoryPool() noexcept {
        currentBlock_ = nullptr;
        currentSlot_ = nullptr;
        lastSlot_ = nullptr;
        freeSlots_ = nullptr;
    };

    MemoryPool(const MemoryPool &memoryPool) noexcept = delete;

    MemoryPool(MemoryPool &&memoryPool) noexcept = delete;

    ~MemoryPool() {
        auto curr = currentBlock_;
        while (curr != nullptr) {
            auto prev = curr->next;
            delete curr;
            curr = prev;
        }
    };

    MemoryPool &operator=(const MemoryPool &memoryPool) = delete;

    MemoryPool &operator=(MemoryPool &&memoryPool) = delete;

    pointer allocate(size_t n = 1, const_pointer hint = nullptr);

    void deallocate(pointer p, size_t n = 1);

    template<class U, class... Args>
    void construct(U *p, Args &&... args);

    template<class U>
    void destroy(U *p);

private:
    union DATA_ {
        value_type element;
        DATA_ *next;
        //stl中union 和下一块的内容指向
    };
    DATA_ *currentBlock_;
    DATA_ *currentSlot_;
    DATA_ *lastSlot_;
    DATA_ *freeSlots_;

    void allocateBlock();
};

template<typename T, size_t BlockSize>
void
MemoryPool<T, BlockSize>::allocateBlock() {
    auto newBlock = static_cast<char *> (operator new(BlockSize));
    // new
    reinterpret_cast<DATA_ *>(newBlock)->next = currentBlock_;
    //使用尾插
    currentBlock_ = reinterpret_cast<DATA_ *>(newBlock);
    //创建出一个新块
    auto body_ = newBlock + sizeof(DATA_ *);

    auto res = reinterpret_cast<uintptr_t >(body_);
    // 为了类型安全
    size_t bodyoffest = ((alignof(DATA_) - res) % alignof(DATA_));
    //计算类型字节数超出指针多少? //方便内存对齐
    //alignof 表示内存对齐的大小结构体中
    currentSlot_ = reinterpret_cast<DATA_ *>(body_ + bodyoffest);
    lastSlot_ = reinterpret_cast<DATA_ *> (newBlock + BlockSize - sizeof(DATA_) + 1);
    //块的最后一块指针
}


template<typename T, size_t BlockSize>
typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::allocate(size_t n, const_pointer hint) {
    if (n <= 0)
        throw std::bad_alloc();
    if (freeSlots_ != nullptr) {
        auto result = reinterpret_cast<pointer>(freeSlots_);
        freeSlots_ = freeSlots_->next;
        return result;
        //找到一块空闲块并且返回出去
    } else {
        if (currentSlot_ >= lastSlot_) // 说明剩余下来的块不够使用
            allocateBlock(); // 重新创建一个块出来
        return reinterpret_cast<pointer>(currentSlot_++);
    }
}


template<typename T, size_t BlockSize>
inline void
MemoryPool<T, BlockSize>::deallocate(pointer p, size_t n) {
    if (p != nullptr) {
        reinterpret_cast<DATA_ *>(p)->next = freeSlots_;
        freeSlots_ = reinterpret_cast<DATA_ *>(p);
        //链表删除
    }
}


template<typename T, size_t BlockSize>
template<class U, class... Args>
inline void
MemoryPool<T, BlockSize>::construct(U *p, Args &&... args) {
    new(p) U(std::forward<Args>(args)...);
    //使用可变参数模板和完美转发转化为右值
}


template<typename T, size_t BlockSize>
template<class U>
inline void
MemoryPool<T, BlockSize>::destroy(U *p) {
    p->~U();
}

#endif //MY_REDIES_MEMORYBLOCK_H
