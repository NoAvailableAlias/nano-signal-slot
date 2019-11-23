Unit Tests
==========

Currently nano-signal-slot now uses Visual Studio for all tests.

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
| | Test_Fire_Disconnects | PASS |
| | Test_Fire_Connects | PASS |
| | Test_Fire_Disconnect_All | PASS |
| | Test_Fire_Recursive_Fire | &mdash; |
| | Test_Signal_Copy | &mdash; |
| Test_Signal_Fire | | |
| | Test_Member_Fire | PASS |
| | Test_Const_Member_Fire | PASS |
| | Test_Overloaded_Member_Fire | PASS |
| | Test_Overloaded_Static_Member_Fire | PASS |
| | Test_Overloaded_Virtual_Member_Fire | PASS |
| | Test_Overloaded_Virtual_Derived_Member_Fire | PASS |
| | Test_Fire_Accumulate | PASS |
| Test_ST_Policy | | |
| | Test_Global_Signal | PASS |
| Test_ST_Policy_Safe | | |
| | Test_Global_Signal | PASS |
| Test_TS_Policy | | |
| | Test_Shared_Signal | PASS |
| Test_TS_Policy_Safe | | |
| | Test_Shared_Signal | PASS |

_**Dashes currently denote unsupported use cases.*_