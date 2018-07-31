Unit Tests
==========

Currently nano-signal-slot has replaced its previous single test with testing through Visual Studio.

| Test Class | Test Method | Status |
|------------|-------------|-------------|
| Test_Observer_Destruction | | |
| | Test_Stack_Foo_Observer_Destructor | PASS |
| | Test_Stack_Bar_Foo_Observer_Destructor | PASS |
| | Test_Heap_Foo_Observer_Destructor | PASS |
| | Test_Heap_Bar_Foo_Observer_Destructor | PASS |
| Test_Signal_Connect | | |
| | Test_Member_Connect | PASS |
| | Test_Const_Member_Connect | PASS |
| | Test_Overloaded_Member_Connect | PASS |
| | Test_Overloaded_Static_Member_Connect | PASS |
| | Test_Overloaded_Virtual_Member_Connect | PASS |
| | Test_Overloaded_Virtual_Derived_Member_Connect | PASS |
| | Test_Overloaded_Free_Function_Connect | PASS |
| Test_Signal_Disconnect | | |
| | Test_Member_Disconnect | PASS |
| | Test_Const_Member_Disconnect | PASS |
| | Test_Overloaded_Member_Disconnect | PASS |
| | Test_Overloaded_Static_Member_Disconnect | PASS |
| | Test_Overloaded_Virtual_Member_Disconnect | PASS |
| | Test_Overloaded_Virtual_Derived_Member_Disconnect | PASS |
| | Test_Overloaded_Free_Function_Disconnect | PASS |
| Test_Signal_Edge_Cases | | |
| | Test_Fired_Disconnect | PASS |
| | Test_Fire_Disconnects | &mdash; |
| | Test_Fire_Connects | PASS |
| | Test_Fire_Remove_All | PASS |
| | Test_Fire_Recursive_Fire | &mdash; |
| Test_Signal_Fire | | |
| | Test_Member_Fire | PASS |
| | Test_Const_Member_Fire | PASS |
| | Test_Overloaded_Member_Fire | PASS |
| | Test_Overloaded_Static_Member_Fire | PASS |
| | Test_Overloaded_Virtual_Member_Fire | PASS |
| | Test_Overloaded_Virtual_Derived_Member_Fire | PASS |
| | Test_Fire_Accumulate | PASS |

_**Dashes currently denote unsupported use cases.*_