#ifndef W_CONNECTIONBASE_H
#define W_CONNECTIONBASE_H

namespace W {
    class Slot;

    template <typename  ...T>
    class ConnectionBase {
        public:
            virtual ~ConnectionBase() {}
            typedef void (*memberfun)(T ...t);

            virtual Slot* getdest() const { return nullptr; }
            virtual memberfun getmemfun() const { return nullptr; }
            virtual void emit(T ...t) = 0;
            virtual ConnectionBase* clone() = 0;
            virtual ConnectionBase* duplicate(Slot* pnewdest) { (void)pnewdest; return nullptr; }
    };
}

#endif /* end of include guard: W_CONNECTIONBASE_H */
