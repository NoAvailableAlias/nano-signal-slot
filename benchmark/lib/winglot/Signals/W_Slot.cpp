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

}