#ifndef CHRONO_TIMER_HPP
#define CHRONO_TIMER_HPP

#include <chrono>
#include <thread>

class chrono_timer
{
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

    public:

    void reset()
    {
        m_start = std::chrono::high_resolution_clock::now();
    }

    template <typename T>
    static void delay(std::size_t duration)
    {
        std::this_thread::sleep_for(T(duration));
    }
    template <typename T>
    std::size_t count() const
    {
        return std::chrono::duration_cast<T>
          (std::chrono::high_resolution_clock::now() - m_start).count();
    }
};

#endif // CHRONO_TIMER_HPP
