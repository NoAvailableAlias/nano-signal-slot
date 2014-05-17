#include <string>
#include <stdarg.h>
#include "Signal.h"
#include <stdint.h>
//static std::string string_printf (const char *format, ...) __attribute__ ((__format__ (__printf__, 1, 2)));
static std::string string_printf (const char *format, ...)
{
  std::string result;
  char str[512];
  va_list args;
  va_start (args, format);
  if (vsprintf_s(str, 512, format, args) >= 0)
	printf(str);
    result = str;
  va_end (args);

  return result;
}

class BasicSignalTests {
  static std::string accu;
  struct Foo {
    char
    foo_bool (float f, int i, std::string s)
    {
      accu += string_printf ("Foo: %.2f\n", f + i + s.size());
      return true;
    }
  };
  static char
  float_callback (float f, int, std::string)
  {
    accu += string_printf ("float: %.2f\n", f);
    return 0;
  }
public:
  static void
  run()
  {
    accu = "";
    Signal11::Signal<char (float, int, std::string)> sig1;

	Signal11::ConnectionRef id1 = sig1.connect(float_callback);
    Signal11::ConnectionRef id2 = sig1.connect([] (float, int i, std::string) { accu += string_printf ("int: %d\n", i); return 0; });
    Signal11::ConnectionRef id3 = sig1.connect([] (float, int, const std::string &s) { accu += string_printf ("string: %s\n", s.c_str()); return 0; });
    
	sig1.emit(0.3f, 4, "huhu");
    
	bool success;
	success = id1.disconnect(); assert (success == true);  success = id1.disconnect(); assert (success == false);
    success = id2.disconnect(); assert (success == true);  success = id3.disconnect(); assert (success == true);
    success = id3.disconnect(); assert (success == false); success = id2.disconnect(); assert (success == false);
    
	Foo foo;
	Signal11::ConnectionScope scope;
	scope += sig1.connect(foo, &Foo::foo_bool);
	scope += sig1.connect(&foo, &Foo::foo_bool);
    
	scope.disable();
	sig1.emit (1.5f, 15, "121");
	scope.enable();
	sig1.emit (0.5f, 1, "12");

    Signal11::Signal<void (std::string, int)> sig2;
	Signal11::ScopedConnectionRef scoped1 = sig2.connect([] (std::string msg, int) { accu += string_printf ("msg: %s", msg.c_str()); });
	{
		Signal11::ConnectionRef scoped2 = scope += sig2.connect([] (std::string, int d)   { accu += string_printf (" *%d*\n", d); });
		Signal11::ScopedConnectionRef scoped3 = sig2.connect([] (std::string, int d)   { accu += string_printf (" SHOULDN'T BE HERE", d); });
		scope.removeReleased(scoped2, scoped2);
	}
    sig2.emit ("in sig2", 17);

    accu += "DONE";

    const char *expected =
      "float: 0.30\n"
      "int: 4\n"
      "string: huhu\n"
      "Foo: 3.50\n"
      "Foo: 3.50\n"
      "msg: in sig2 *17*\n"
      "DONE";
    assert (accu == expected);
  }
};
std::string BasicSignalTests::accu;


class TestCollectorVector {
  static int handler1   ()  { return 1; }
  static int handler42  ()  { return 42; }
  static int handler777 ()  { return 777; }
  public:
  static void
  run ()
  {
    Signal11::Signal<int (), Signal11::CollectorVector<int>> sig_vector;
    sig_vector.connect(handler777);
    sig_vector.connect(handler42);
    sig_vector.connect(handler1);
    sig_vector.connect(handler42);
    sig_vector.connect(handler777);
    std::vector<int> results = sig_vector.emit();
    std::vector<int> reference;
	reference.push_back(777); reference.push_back(42); reference.push_back(1); reference.push_back(42); reference.push_back(777);
    assert (results == reference);
  }
};

class TestCollectorUntil0 {
  bool check1, check2;
  TestCollectorUntil0() : check1 (0), check2 (0) {}
  bool handler_true  ()  { check1 = true; return true; }
  bool handler_false ()  { check2 = true; return false; }
  bool handler_abort ()  { abort(); }
  public:
  static void
  run ()
  {
    TestCollectorUntil0 self;
    Signal11::Signal<bool (), Signal11::CollectorUntil0<bool>> sig_until0;
	sig_until0.connect(self, &TestCollectorUntil0::handler_true);
	sig_until0.connect(self, &TestCollectorUntil0::handler_false);
    sig_until0.connect(self, &TestCollectorUntil0::handler_abort);
    assert (!self.check1 && !self.check2);
    const bool result = sig_until0.emit();
    assert (!result && self.check1 && self.check2);
  }
};

class TestCollectorWhile0 {
  bool check1, check2;
  TestCollectorWhile0() : check1 (0), check2 (0) {}
  bool handler_0     ()  { check1 = true; return false; }
  bool handler_1     ()  { check2 = true; return true; }
  bool handler_abort ()  { abort(); }
  public:
  static void
  run ()
  {
    TestCollectorWhile0 self;
    Signal11::Signal<bool (), Signal11::CollectorWhile0<bool>> sig_while0;
	sig_while0.connect(self, &TestCollectorWhile0::handler_0);
	sig_while0.connect(self, &TestCollectorWhile0::handler_1);
	sig_while0.connect(self, &TestCollectorWhile0::handler_abort);
    assert (!self.check1 && !self.check2);
    const bool result = sig_while0.emit();
    assert (result == true && self.check1 && self.check2);
  }
};

int main(int argc, char *argv[])
{
  printf ("Signal/Basic Tests: ");
  BasicSignalTests::run();
  printf ("OK\n");

  printf ("Signal/CollectorVector: ");
  TestCollectorVector::run();
  printf ("OK\n");

  printf ("Signal/CollectorUntil0: ");
  TestCollectorUntil0::run();
  printf ("OK\n");

  printf ("Signal/CollectorWhile0: ");
  TestCollectorWhile0::run();
  printf ("OK\n");

  system("pause");

  return 0;
}