#XXX find_package(Catch2 REQUIRED)
#FIXME include(Catch) # not found? CK

CPMAddPackage("gh:catchorg/Catch2@2.13.7")

add_library(catch_main OBJECT catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)
target_link_libraries(catch_main PRIVATE project_options)

add_executable(Test_Observer_Destruction Test_Observer_Destruction.cpp)
target_link_libraries(Test_Observer_Destruction PRIVATE project_warnings project_options catch_main)
add_test(NAME Test_Observer_Destruction COMMAND Test_Observer_Destruction)

