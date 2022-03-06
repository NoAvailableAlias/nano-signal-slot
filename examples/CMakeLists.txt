find_package(Threads REQUIRED)

add_executable(
  doc_view
  doc_view.cpp
)

target_link_libraries(
  doc_view PRIVATE project_warnings project_options Threads::Threads
)

