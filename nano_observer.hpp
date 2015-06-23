#ifndef NANO_OBSERVER_HPP
#define NANO_OBSERVER_HPP

#include "nano_function.hpp"

namespace Nano
{

class Observer
{
    template <typename T> friend class Signal;

    struct DelegateKeyObserver { DelegateKey delegate; Observer* observer; };
    struct Node { DelegateKeyObserver data; Node* next; } *head = nullptr;

    //--------------------------------------------------------------------------

    void insert(DelegateKey const& key, Observer* ptr)
    {
        head = new Node { { key, ptr }, head };
    }
    void insert(DelegateKey const& key)
    {
        this->insert(key, this);
    }

    //--------------------------------------------------------------------------

    void remove(DelegateKey const& key)
    {
        Node* node = head;
        Node* prev = nullptr;
        // Only delete the first occurrence
        for ( ; node; prev = node, node = node->next)
        {
            if (node->data.delegate == key)
            {
                if (prev)
                {
                    prev->next = node->next;
                }
                else
                {
                    head = head->next;
                }
                delete node;
                break;
            }
        }
    }

    //--------------------------------------------------------------------------

    template <typename Delegate, typename... Uref>
    void onEach (Uref &&... args)
    {
        for (auto node = head, next = head; node; node = next)
        {
            next = node->next;
            // Perfect forward and emit
            Delegate(node->data.delegate)(std::forward<Uref>(args)...);
        }
    }

    template <typename Delegate, typename Accumulate, typename... Uref>
    void onEach_Accumulate (Accumulate && accumulate, Uref &&... args)
    {
        for (auto node = head, next = head; node; node = next)
        {
            next = node->next;
            // Perfect forward, emit, and accumulate the return value
            accumulate(Delegate(node->data.delegate)(std::forward<Uref>(args)...));
        }
    }

    //--------------------------------------------------------------------------

    protected:

    ~Observer()
    {
        for (auto node = head; node;)
        {
            auto temp = node;
            // If this is us we only need to delete
            if (this != node->data.observer)
            {
                // Remove this slot from this listening Observer
                node->data.observer->remove(node->data.delegate);
            }  
            node = node->next;
            delete temp;       
        }
    }

};

} // namespace Nano ------------------------------------------------------------

//#include <atomic>
//#include <memory>
//
//namespace Nano
//{
//
//class Observer
//{
//    template <typename T> friend class Signal;
//
//    struct DelegateKeyObserver
//    {
//        DelegateKey delegate;
//        Observer* observer;
//    };
//    struct Node
//    {
//        DelegateKeyObserver data;
//        std::shared_ptr<Node> next;
//    };
//
//    std::shared_ptr<Node> head;
//
//    //--------------------------------------------------------------------------
//
//    void insert(DelegateKey const& key, Observer* ptr)
//    {
//        auto node = std::shared_ptr<Node>(new Node { { key, ptr }, std::atomic_load(&head) }, [](Node* self)
//        {
//            self->data.observer->remove(self->data.delegate);
//            delete self;
//            self = nullptr;
//        });
//
//        //auto node = std::make_shared<Node>();
//        //node->next = std::atomic_load(&head);
//        //node->data.delegate = key;
//        //node->data.observer = ptr;
//
//        while (!std::atomic_compare_exchange_weak(&head, &node->next, node));
//    }
//    void insert(DelegateKey const& key)
//    {
//        this->insert(key, this);
//    }
//
//    //--------------------------------------------------------------------------
//
//    void remove(DelegateKey const& key)
//    {
//        std::shared_ptr<Node> node = std::atomic_load(&head);
//        std::shared_ptr<Node> prev;
//
//        for ( ; node ; prev = node, node = node->next)
//        {
//            if (node->data.delegate == key)
//            {
//                if (prev)
//                {
//                    prev->next = node->next;
//                }
//                else
//                {
//                    while (!std::atomic_compare_exchange_weak(&head, &head, head->next));
//                }
//                node.reset();
//                break;
//            }
//        }
//    }
//
//    //--------------------------------------------------------------------------
//
//    template <typename Delegate, typename... Uref>
//    void onEach(Uref&&... args)
//    {
//        for (auto node = std::atomic_load(&head); node; node = node->next)
//        {
//            Delegate(node->data.delegate)(std::forward<Uref>(args)...);
//        }
//    }
//
//    template <typename Delegate, typename Accumulate, typename... Uref>
//    void onEach_Accumulate(Accumulate&& accumulate, Uref&&... args)
//    {
//        for (auto node = std::atomic_load(&head); node; node = node->next)
//        {
//            accumulate(Delegate(node->data.delegate)(std::forward<Uref>(args)...));
//        }
//    }
//
//};
//
//} // namespace Nano ------------------------------------------------------------

//#include <map>
//
//namespace Nano
//{
//
//class Observer
//{
//    template <typename T> friend class Signal;
//
//    std::map<DelegateKey, Observer*> tracked_connections;
//
//    void insert(DelegateKey const& key, Observer* observer)
//    {
//        tracked_connections.emplace(key, observer);
//    }
//    void insert(DelegateKey const& key)
//    {
//        tracked_connections.emplace(key, this);
//    }
//    void remove(DelegateKey const& key)
//    {
//        tracked_connections.erase(key);
//    }
//
//    template <typename Delegate, typename... Uref>
//    void onEach(Uref&&... args)
//    {
//        for (auto const& slot : tracked_connections)
//        {
//            Delegate(slot.first)(std::forward<Uref>(args)...);
//        }
//    }
//    template <typename Delegate, typename Accumulate, typename... Uref>
//    void onEach_Accumulate(Accumulate&& accumulator, Uref&&... args)
//    {
//        for (auto const& slot : tracked_connections)
//        {
//            accumulator(Delegate(slot.first)(std::forward<Uref>(args)...));
//        }
//    }
//
//protected:
//
//    ~Observer()
//    {
//        auto iter = tracked_connections.cbegin();
//        auto stop = tracked_connections.cend();
//
//        while (iter != stop)
//        {
//            auto const& delegate_key = iter->first;
//            auto const& observer = iter->second;
//
//            std::advance(iter, 1);
//
//            observer->remove(delegate_key);
//        }
//    }
//};
//
//
//} // namespace Nano ------------------------------------------------------------

#endif // NANO_OBSERVER_HPP
