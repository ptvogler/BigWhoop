add_executable(public_interface "public_interface.cpp")
target_link_libraries(public_interface PRIVATE Catch2::Catch2WithMain bwclib)
add_test(NAME public_interface COMMAND public_interface)
