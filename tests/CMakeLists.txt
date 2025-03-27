list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

function(bigwhoop_unit_test TESTCASE)
  add_executable(${TESTCASE} "${TESTCASE}.cpp")
  target_link_libraries(${TESTCASE} PRIVATE Catch2::Catch2WithMain bwclib)
  add_test(NAME ${TESTCASE} COMMAND ${TESTCASE})
endfunction()

bigwhoop_unit_test(test_libbwc)
bigwhoop_unit_test(test_bitstream)
