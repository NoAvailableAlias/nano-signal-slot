#ifndef E_SLOT_H
#define E_SLOT_H

#include <set>

#include "W_Mutex.h"

namespace W {
    class __SignalBase;

    /**
     * Slot class of the Signal-Slot mechanism
     */
    class Slot : public Mutex {
        public:
            /**
             * Constructs a new Slot class
             */
            Slot();

            /**
             * Construct new Slot class from other Slot class
             * @param  hs Slot object to be copied
             */
            Slot(const Slot& hs);

            /**
             * Destructor releases all not released resources/signals
             */
            virtual ~Slot();

            /**
             * Connect given signal to slot
             * @param sender Signal to be connected
             */
            void signal_connect(__SignalBase* sender);

            /**
             * Disconnect given signal from slot
             * @param sender Signal to be disconneced
             */
            void signal_disconnect(__SignalBase* sender);

            /**
             * Disconnects all signals from this slot
             */
            void disconnect_all();

        private:
            std::set< __SignalBase* > m_senders;
    };
}

/*
 * Implementation
 * Solves circular dependency problems without separate .cpp file
 */
/*
#include "W_Slot.h"
#include "W_SignalBase.h"

namespace W {

    Slot::Slot() {}

    Slot::Slot(const Slot& hs): Mutex(hs) {
        LockBlock lock(this);

        for(auto &sender: hs.m_senders) {
            sender->slot_duplicate(&hs, this);
            m_senders.insert(sender);
        }
    } 

    Slot::~Slot() {
        disconnect_all();
    }

    void Slot::signal_connect(__SignalBase* sender) {
        LockBlock lock(this);
        m_senders.insert(sender);
    }

    void Slot::signal_disconnect(__SignalBase* sender) {
        LockBlock lock(this);
        m_senders.erase(sender);
    }


    void Slot::disconnect_all() {
        LockBlock lock(this);

        for(auto &sender: m_senders) {
            sender->slot_disconnect(this);
        }

        m_senders.erase(m_senders.begin(), m_senders.end());
    }

} *//* W */

#endif /* end of include guard: E_SLOT_H */
