//                  CAS lock_free_queue实现
//              var->compare_exchange_strong(expected, new_val)
//              表示： var == expected ? var = new_val : swap(expected, var)
#include <atomic>
#include <iostream>

namespace CAS
{

    template<typename T>
    struct Node
    {
        T data_;
        std::atomic<Node<T>> *next_;
    };
    
    template<typename T>
    class CAS_queue
    {
        public:
            explicit CAS_queue();
            virtual ~CAS_queue(); 

            void Enqueue(T insert_val);
            CAS_queue* Dequeue();

        private:
            std::atomic<Node<T>> *head;
            std::atomic<Node<T>> *tail;
            std::atomic<int> element_num;
    };
    
    
    template<typename T>
    CAS_queue<T>::CAS_queue() : head(nullptr), tail(new Node<T>), element_num(0)
    {
        // init status: head == tail
        head = tail;
        tail->next_ = nullptr;
    }

    template<typename T>
    void CAS_queue<T>::Enqueue(T insert_val)
    {   
        // create new node
        auto node = new Node<T>;
        node->data_ = insert_val;
        node->next_ = nullptr;

        Node<T> *p;
        do
        {
            p = tail;
        } while ( !(p->next_)->compare_exchange_strong(nullptr, node) ); 
        // 在这里，可能已经有thread写成功了，而当前thread还在执行 p=tail
        // false: p->next != nullptr 也就是说已经更改了
    
        tail->compare_exchange_strong(tail, node);
    }

// namespace CAS
}

int main()
{
    CAS::CAS_queue<int> instance();

    return 0;
}