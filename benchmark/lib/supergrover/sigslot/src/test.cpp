#include <iostream>
using namespace std;

#include "signal.h"

class Emitter
{
    public:
        Signal<int> the_signal;

        void do_emit(int value)
        {
            the_signal.emit(value);
        }
};

class Receiver: public SigSlotBase
{
    public:
        void slot(int v)
        {
            cout << "Yay! " << v << endl;
        }
};

int main(int argc, char* argv[])
{
    // Create both an emitting and receiving instance
    Emitter em;
    Receiver recv;

    // Bind the slot to the signal
    em.the_signal.bind(&Receiver::slot, &recv);

    // Emit
    em.do_emit(42);

    return EXIT_SUCCESS;
}
