
#ifndef _STACKALLOC_H_ 
#define _STACKALLOC_H_  


#include <memory> 

template <typename T> 
struct stackNode 
{
    T data; 
    stackNode *next; 
}; 


template <typename T, class Alloc=std::allocator<T> > 
class stackAlloc
{
public:
    typedef stackNode<T> node; 
    typedef typename Alloc::template rebind<node>::other allocator; 
    
public:
    stackAlloc() { m_head = 0;  } 
    ~stackAlloc() { clear(); } 

    bool empty() { return (m_head == 0); }

    void clear() 
    {
        node *cur = m_head; 
        while (cur)
        {
            node* tmp = cur->next; 
            m_alloctor.destroy(cur); 
            m_alloctor.deallocate(cur, 1); 
            cur = tmp; 
        }
        m_head = 0; 
    }

    void push(const T& element) 
    {
        node* tmp = m_alloctor.allocate(1); 
        m_alloctor.construct(tmp, node()); 
        tmp->data = element; 
        tmp->next = m_head; 
        m_head = tmp; 
    }

    T pop() 
    {
        T result = m_head->data; 
        node* tmp = m_head->next; 
        m_alloctor.destroy(m_head); 
        m_alloctor.deallocate(m_head, 1); 
        m_head = tmp; 
        return result; 
    }

    T top() { return (m_head->data);  } 

private:
    allocator m_alloctor; 
    node* m_head; 
}; 


#endif // _STACKALLOC_H_  
