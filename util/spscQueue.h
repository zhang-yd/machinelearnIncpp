
#ifndef _SPSCQUEUE_H_ 
#define _SPSCQUEUE_H_ 

#include <cassert> 
#include <cstddef> 
#include <atomic> 
#include <new>   // std::hardware_destructive_interference_size 
#include <memory>  // std::allocator   
#include <stdexcept> 
#include <type_traits>   // std::enable_if, std::is_*_des 


template <typename T, typename Allocator=std::allocator<T>>
class SPSCQueue 
{
#if defined(__cpp_if_constexpr) && defined(__cpp_lib_void_t)
    template <typename Alloc2, typename = void>
    struct has_allocate_at_least : std::false_type {} ; 

    template <typename Alloc2>
    struct has_allocate_at_least<Alloc2, std::void_t<typename Alloc2::value_type, 
        decltype(std::declval<Alloc2 &>().allocate_at_least(size_t{}))>>: std::true_type {}; 
    // This metafunction is used to detect ill-formed types in context 
#endif 


public:
    explicit SPSCQueue(const size_t capacity, const Allocator &allocator = Allocator()): 
        m_capacity(capacity), m_allocator(allocator), m_head(0), m_tail(0) 
    {
        if(m_capacity < 1)
        {
            m_capacity = 1; 
        }

        if(m_capacity > SIZE_MAX - 2 * kPadding)
        {
            m_capacity = SIZE_MAX - 2 * kPadding; 
        }

    #if defined(__cpp_if_constexpr) && defined(__cpp_lib_void_t)
        if constexpr (has_allocate_at_least<Allocator>::value)
        {
            auto res = m_allocator.allocate_at_least(m_capacity + 2 * kPadding); 
            m_slots =  res.ptr; 
            m_capacity = res.count - 2 * kPadding; 
        }
        else
        {
            m_slots = std::allocator_traits<Allocator>::allocate(m_allocator, m_capacity + 2*kPadding)
        }
    #else
        m_slots = std::allocator_traits<Allocator>::allocate(m_allocator, m_capacity + 2*kPadding);  
    #endif 

        static_assert(alignof(SPSCQueue<T>) == kCacheLineSize, "");  
        static_assert(sizeof(SPSCQueue<T>) >= 3 * kCacheLineSize, ""); 
        assert(reinterpret_cast<char *>(&m_tail) - reinterpret_cast<char *>(&m_head) >=
            static_cast<std::ptrdiff_t>(kCacheLineSize)); 
    }


    ~SPSCQueue()
    {
        while(front())
        {
            pop(); 
        }
        std::allocator_traits<Allocator>::deallocate(m_allocator, m_slots, m_capacity + 2*kPadding); 
    }

    SPSCQueue(const SPSCQueue &) = delete; 
    SPSCQueue &operator=(const SPSCQueue &) = delete; 

    template <typename... Args>
    void emplace(Args &&... args) noexcept(std::is_nothrow_constructible<T, Args &&...>::value)
    {
        static_assert(std::is_constructible<T, Args &&...>::value, "T must be constructible with args"); 
        auto const head = m_head.load(std::memory_order_relaxed); 
        auto nexthead = head + 1; 

        if(nexthead == m_capacity)
        {
            nexthead = 0; 
        }
        while(nexthead == m_tail.load(std::memory_order_acquire))
        { 
        }
        new (&m_slots[head + kPadding]) T(std::forward<Args>(args)...); 
        m_head.store(nexthead, std::memory_order_release); 
    }

    template <typename... Args>
    bool try_emplace(Args &&...args) noexcept(std::is_nothrow_constructible<T, Args &&...>::value)
    {
        static_assert(std::is_constructible<T, Args &&...>::value, "T must be constructible with ");
        auto const head = m_head.load(std::memory_order_relaxed);  
        auto nexthead = head + 1; 
        if(nexthead == m_capacity)
        {
            nexthead = 0; 
        }
        if(nexthead == m_tail.load(std::memory_order_acquire))
        {
            return false;  
        }

        new (&m_slots[head + kPadding]) T(std::forward<Args>(args)...); 
        m_head.store(nexthead, std::memory_order_release); 
        return true; 
    }

    void push(const T& v) noexcept(std::is_nothrow_copy_constructible<T>::value)
    {
        static_assert(std::is_copy_constructible<T>::value, "T must be copy constructible "); 
        emplace(v); 
    }

    template <typename P, typename = typename std::enable_if<std::is_constructible<T, P &&>::value>::tpye>
    void push(P &&v) noexcept(std::is_nothrow_constructible<T, P &&>::value) 
    {
        emplace(std::forward<P>(v)); 
    }

    bool try_push(const T& v) noexcept(std::is_nothrow_copy_constructible<T>::value)
    {
        static_assert(std::is_copy_constructible<T>::value, "T must be copy constructible "); 
        return try_push(v);     
    }

    template <typename P, typename = typename std::enable_if<std::is_constructible<T, P &&>::value>::type>
    bool try_push(P&& v) noexcept(std::is_nothrow_constructible<T, P &&>::value) 
    {
        return try_push(std::forward<P>(v)); 
    }

    T* front() noexcept 
    {
        auto const tail = m_tail.load(std::memory_order_relaxed);
        if(m_head.load(std::memory_order_acquire) == tail)
        {
            return nullptr; 
        } 
        return &m_slots[tail + kPadding]; 
    }

    void pop() noexcept 
    {
        static_assert(std::is_nothrow_destructible<T>::value, "T must be nothrow destructible"); 

        auto const tail = m_tail.load(std::memory_order_relaxed); 
        assert(m_head.load(std::memory_order_acquire) != m_tail); 
        m_slots[tail + kPadding].~T(); 

        auto nextTail = tail + 1; 
        if(nextTail == m_capacity)
        {
            nextTail = 0; 
        }
        m_tail.store(nextTail, std::memory_order_release); 
    }

    size_t size() const noexcept 
    {
        std::ptrdiff_t ptrdiff = m_head.load(std::memory_order_acquire) - m_tail.load(std::memory_order_acquire); 
        if(ptrdiff < 0)
        {
            ptrdiff += m_capacity; 
        }
        return static_cast<size_t>(ptrdiff); 
    }

    bool empty() const noexcept { return size() == 0; } 

    size_t capacity() const noexcept 
    {
        return m_capacity - 1; 
    }


private:
#ifdef __cpp_lib_hardware_interference_size 
    static constexpr size_t kCacheLineSize = std::hardware_destructive_interference_size; 
#else 
    static constexpr size_t kCacheLineSize = 64; 
#endif 
    static constexpr size_t kPadding = (kCacheLineSize - 1)/sizeof(T) + 1; 

private:
    size_t m_capacity; 
    T *m_slots; 

#if defined(__has_cpp_attribute) && __has_cpp_attribute(no_unique_address)   // __has_cpp_attribute test whether the attribute referenced by its operand is recognized by GCC 
    Allocator m_allocator[[no_unique_address]]; 
#else 
    Allocator m_allocator; 
#endif 

    alignas(kCacheLineSize) std::atomic<size_t> m_head; 
    alignas(kCacheLineSize) std::atomic<size_t> m_tail; 

    char m_padding[kCacheLineSize - sizeof(m_tail)]; 
}; 

#endif // _SPSCQUEUE_H_ 
