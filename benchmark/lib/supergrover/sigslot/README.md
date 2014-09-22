sigslot
=======

C++11 signal/slot implementation

## Requirements ##
* c++11

## How to use ##
For a code sample, see test.cpp

```
class Emitter
{
    public:
        Signal<int> the_signal;

        void do_emit(int value)
        {
            the_signal.emit(value);
        }
};
```

The only thing required for a class to emit signals, is to declare a field of the Signal<> type. The template arguments will be the parameter types of the slots to be bound to the signal.

```
class Receiver: public Object
{
    public:
        void slot(int v)
        {
            cout << "Yay! " << v << endl;
        }
};
```

The only thing required for a class to be able to bind to signals, is for it to derive from Object. After that, every method of the object can be bound to signals.

```
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
```

Here we instantiate both classes, bind the signal and emit it.


## Donations ##

Like it?

If you find this useful, please consider donating to BTC: 1Pck5gui4iXhTQUiUc3qFD7uPbj1cn5ZSB

