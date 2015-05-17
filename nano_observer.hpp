#ifndef NANO_OBSERVER_HPP
#define NANO_OBSERVER_HPP

#include "nano_function.hpp"

#include <forward_list>
#include <cassert>
#include <memory>

namespace Nano
{

class Observer
{
    template <typename T> friend class Signal;

    struct DelegateKeyObserver { DelegateKey delegate; Observer* observer; };
    struct Node { DelegateKeyObserver data; std::shared_ptr<Node> next; };

    std::shared_ptr<Node> head;

    void insert(DelegateKey const& key, Observer* ptr)
    {
        auto node = std::make_shared<Node>();
        node->next = std::atomic_load(&head);

        node->data.delegate = key;
        node->data.observer = ptr;

        while (!std::atomic_compare_exchange_weak(&head, &node->next, node));
    }
    void insert(DelegateKey const& key)
    {
        this->insert(key, this);
    }
    void remove(DelegateKey const& key)
    {
        std::shared_ptr<Node> node = std::atomic_load(&head);
        std::shared_ptr<Node> prev;

        for ( ; node ; prev = node, node = node->next)
        {
            if (node->data.delegate == key)
            {
                if (prev)
                {
                    prev->next = node->next;
                }
                else
                {
                    while (!std::atomic_compare_exchange_weak(&head, &head, head->next));
                }
                node.reset();
                break;
            }
        }
        /*std::shared_ptr<Node> prev = std::atomic_load(&head);
        std::shared_ptr<Node> temp;

        if (prev->data.delegate == key)
        {
            head = head->next;
            prev.reset();
        }
        else
        {
            while (prev->next && prev->next->data.delegate != key)
                prev = prev->next;

            if (prev->next && prev->next->data.delegate == key)
            {
                temp = prev->next;
                prev->next = temp->next;
                prev.reset();
            }
        }*/
    }

    //--------------------------------------------------------------------------

    template <typename Delegate, typename... Uref>
    void onEach(Uref&&... args)
    {
        for (auto c = std::atomic_load(&head); c; c = c->next)
        {
            Delegate(c->data.delegate)(std::forward<Uref>(args)...);
        }
    }
    template <typename Delegate, typename Accumulate, typename... Uref>
    void onEach(Accumulate&& accumulator, Uref&&... args)
    {
        for (auto c = std::atomic_load(&head); c; c = c->next)
        {
            accumulator(Delegate(c->data.delegate)(std::forward<Uref>(args)...));
        }
    }
};

//class Observer
//{
//    template <typename T> friend class Signal;
//
//    struct DelegateKeyObserver
//    {
//        DelegateKeyObserver(DelegateKey k, Observer* o): delegate_key(k), observer(o) {}
//        DelegateKey delegate_key;
//        Observer* observer;
//    };
//
//    std::forward_list<DelegateKeyObserver> connections;
//
//    void insert(DelegateKey const& key, Observer* observer)
//    {
//        connections.emplace_front(key, observer);
//    }
//    void insert(DelegateKey const& key)
//    {
//        connections.emplace_front(key, this);
//    }
//    void remove(DelegateKey const& key)
//    {
//        connections.remove_if([&key](DelegateKeyObserver const& slot) { return key == slot.delegate_key; });
//    }
//    template <typename Delegate, typename... Uref>
//    void onEach(Uref&&... args)
//    {
//        for (auto const& slot : connections)
//        {
//            Delegate(slot.delegate_key)(std::forward<Uref>(args)...);
//        }
//    }
//
//    protected:
//
//    ~Observer()
//    {
//        for (auto const& slot : connections) { slot.observer->remove(slot.delegate_key); }
//    }
//};

} // namespace Nano ------------------------------------------------------------

#endif // NANO_OBSERVER_HPP
