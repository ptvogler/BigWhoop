add_executable(bitstream "bitstream.cpp")
target_link_libraries(bitstream PRIVATE Catch2::Catch2WithMain bwclib)
add_test(NAME bitstream COMMAND bitstream)
