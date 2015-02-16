#include <iostream>
#include "yassi.h"
using namespace yassi;
using namespace std;

// Declare the signals:
using Event1 = Signal<void()>;
using Event2 = Signal<void(string const &)>;

// Define a class that is able to emit both Event1 and Event2:
class SomeClass: public Emitter<Event1, Event2>
{};

void sayHello()
{
  cout << "Hello";
}

void say(string const &msg)
{
  cout << msg;
}

int main()
{
  SomeClass emitter;
  emitter.connect<Event1>(sayHello);
  emitter.connect<Event2>(say);

  emitter.emit<Event1>();
  emitter.emit<Event2>(" World!\n");
}
