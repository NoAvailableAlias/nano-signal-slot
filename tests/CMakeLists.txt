#XXX find_package(Catch2 REQUIRED)
#FIXME include(Catch) # not found? CK

CPMAddPackage("gh:catchorg/Catch2@2.13.7")

add_library(catch_main OBJECT catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)
target_link_libraries(catch_main PRIVATE project_options)

add_executable(
  Test_Observer_Destruction
  Test_Observer_Destruction.cpp
  Test_ST_Policy_Safe.cpp
  Test_Signal_Disconnect.cpp
  Test_TS_Policy.cpp
  Test_ST_Policy.cpp
  Test_Signal_Connect.cpp
  Test_Signal_Fire.cpp
  Test_TS_Policy_Safe.cpp
)

target_link_libraries(
  Test_Observer_Destruction PRIVATE project_warnings project_options catch_main
)

if(NOT MSVC)
  #NOTE: instead of target_compile_options(Test_Observer_Destruction PRIVATE -Wno-unused-function)
  set_target_properties(Test_Observer_Destruction PROPERTIES UNITY_BUILD ON)
endif()

add_test(NAME Test_Observer_Destruction COMMAND Test_Observer_Destruction)
