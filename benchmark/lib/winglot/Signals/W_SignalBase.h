#ifndef W_SIGNALBASE_H
#define W_SIGNALBASE_H

#include <list>

#include "W_ConnectionBase.h"
#include "W_Mutex.h"
#include "W_Slot.h"

namespace W {

    class __SignalBase: public Mutex {
        public:
            virtual ~__SignalBase() {}
            virtual void slot_disconnect(Slot *pslot) = 0;
            virtual void slot_duplicate(const Slot *oldtarget, Slot *newtarget) = 0;
    };

    template <typename ...T>
    class SignalBase: public __SignalBase {
        protected:
            std::list< ConnectionBase<T...>* > m_connected_slots;

        public:
            SignalBase() {}

            SignalBase(const SignalBase<T...> &s) {
                LockBlock lock(this);

                for(auto &obj: s.m_connected_slots) {
                    obj->getdest()->signal_connect(this);
                    m_connected_slots.push_back(obj->clone());
                }

            }

            virtual ~SignalBase() {
                disconnect_all();
            }

            void disconnect_all() {
                LockBlock lock(this);

                for(auto &obj: m_connected_slots) {
                    if(obj->getdest() != nullptr) {
                        obj->getdest()->signal_disconnect(this);
                    }
                    delete obj;
                }

                m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
            }

            void disconnect(typename ConnectionBase<T...>::memberfun memfun) {
                LockBlock lock(this);

                auto it = m_connected_slots.begin();
                auto itEnd = m_connected_slots.end();

                while(it != itEnd && (*it)->getmemfun() != memfun) {
                    it++;
                }

                if(it != itEnd) {
                    delete *it;
                    m_connected_slots.erase(it);
                }
            }

            void disconnect(Slot *pclass) {
                LockBlock lock(this);

                auto it = m_connected_slots.begin();
                auto itEnd = m_connected_slots.end();

                while(it != itEnd && (*it)->getdest() != pclass) {
                    it++;
                }

                if(it != itEnd) {
                    delete *it;
                    m_connected_slots.erase(it);
                    pclass->signal_disconnect(this);
                }
            }

            void slot_disconnect(Slot *pslot) {
                LockBlock lock(this);

                auto it = m_connected_slots.begin();
                auto itEnd = m_connected_slots.end();
                auto itNext = it;

                while(it != itEnd) {
                    auto obj = *it;
                    itNext++;

                    if(obj->getdest() == pslot) {
                        m_connected_slots.erase(it);
                        delete obj;
                    }

                    it = itNext;
                }
            }

            void slot_duplicate(const Slot* oldtarget, Slot* newtarget) {
                LockBlock lock(this);

                auto it = m_connected_slots.begin();
                auto itEnd = m_connected_slots.end();
                
                while(it != itEnd) {
                    auto obj = *it;

                    if(obj->getdest() == oldtarget) {
                        m_connected_slots.push_back(obj->duplicate(newtarget));
                    }

                    it++;
                }
            }
    };

} /* W */

#endif /* end of include guard: W_SIGNALBASE_H */
