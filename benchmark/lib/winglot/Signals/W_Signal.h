#ifndef W_SIGNAL_H
#define W_SIGNAL_H

#include <list>

#include "W_SignalBase.h"
#include "W_Connection.h"

namespace W {

    /**
     * Class describing a Signal in Signal-Slot mechanism
     */
    template <typename ...T>
    class Signal: public SignalBase<T...> {
        public:
            /**
             * Construct a new Signal
             */
            Signal() {}

            /**
             * Copy constructor
             */
            Signal(const Signal<T...> &s): SignalBase<T...>(s) {}

            /**
             * Desctructor
             */
            virtual ~Signal() {}

            /**
             * Connect a function to be exectued when this signal is emited
             * @param memfun Pointer to function
             */
            void connect(void (*memfun)(T...)) {
                LockBlock lock(this);

                ConnectionFunction<T...>* conn = new ConnectionFunction<T...>(memfun);

                SignalBase<T...>::m_connected_slots.push_back(conn);
            }

            /**
             * Connect a class method to be executed when this signal is emited
             * @param pclass Pointer to class instance
             * @param memfun Pointer to class method
             */
            template <typename DestType>
            void connect(DestType* pclass, void (DestType::*memfun)(T...)) {
                LockBlock lock(this);

                Connection<DestType, T...>* conn = new Connection<DestType, T...>(pclass, memfun);
                SignalBase<T...>::m_connected_slots.push_back(conn);

                pclass->signal_connect(this);
            }

            /**
             * Emit this signal and execute all connected functions
             * @param t Parameters to be passed to the executed functions
             */
            void emit(T ...t) {
                LockBlock lock(this);

                for(auto &connection: SignalBase<T...>::m_connected_slots) {
                    connection->emit(t...);
                }
            }
    };

} /* W */

#endif /* end of include guard: W_SIGNAL_H */
