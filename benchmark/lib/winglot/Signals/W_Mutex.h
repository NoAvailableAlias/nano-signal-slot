#ifndef W_MUTEX_H
#define W_MUTEX_H

#include <mutex>
namespace W {
    class Mutex {
        public:
            Mutex() {}
            Mutex(const Mutex&) {}
            virtual ~Mutex() {}

            virtual void lock() {
                m_mutex.lock();
            }

            virtual void unlock() {
                m_mutex.unlock();
            }

        private:
            std::mutex m_mutex;
    };

    class LockBlock {
        public:
            Mutex *m_mutex;

            LockBlock(Mutex *mtx): m_mutex(mtx) {
                m_mutex->lock();
            }

            ~LockBlock() {
                m_mutex->unlock();
            }
    };
}

#endif /* end of include guard: W_MUTEX_H */
