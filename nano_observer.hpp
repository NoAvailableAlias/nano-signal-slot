#ifndef NANO_OBSERVER_HPP
#define NANO_OBSERVER_HPP

#include "nano_function.hpp"

#include <atomic>
#include <memory>

namespace Nano
{

class Observer
{
    template <typename T> friend class Signal;

    struct DelegateKeyObserver { DelegateKey delegate; Observer* observer; };
    struct Node { DelegateKeyObserver data; std::shared_ptr<Node> next; };

    std::shared_ptr<Node> head;

    //--------------------------------------------------------------------------

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

    //--------------------------------------------------------------------------

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
    }

    //--------------------------------------------------------------------------

    template <typename Delegate, typename... Uref>
    void onEach(Uref&&... args)
    {
        for (auto node = std::atomic_load(&head); node; node = node->next)
        {
            Delegate(node->data.delegate)(std::forward<Uref>(args)...);
        }
    }

    template <typename Delegate, typename Accumulate, typename... Uref>
    void onEach_Accumulate(Accumulate&& accumulator, Uref&&... args)
    {
        for (auto node = std::atomic_load(&head); node; node = node->next)
        {
            accumulator(Delegate(node->data.delegate)(std::forward<Uref>(args)...));
        }
    }

};

} // namespace Nano ------------------------------------------------------------

#endif // NANO_OBSERVER_HPP
