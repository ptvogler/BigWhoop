#include <catch2/catch_test_macros.hpp>

#include <cstdint>

#ifdef __cplusplus
  extern "C" {
#endif
#include "../include/library/private/types.h"
#include "../include/library/private/bitstream.h"
#ifdef __cplusplus
  }
#endif

TEST_CASE( "Bit stream initialization", "[init_bitstream]" ) {
    uint32 size = 4;
    uchar inp_mem[size];
    uchar out_mem[size];
    out_mem[3] = inp_mem[0] = 1;
    out_mem[2] = inp_mem[1] = 2;
    out_mem[1] = inp_mem[2] = 3;
    out_mem[0] = inp_mem[3] = 4;
    char instr = 'c';

    bitstream *stream = init_bitstream(inp_mem, size, instr);

    REQUIRE(((uchar*)stream->memory)[0] == 1);
    REQUIRE(((uchar*)stream->memory)[1] == 2);
    REQUIRE(((uchar*)stream->memory)[2] == 3);
    REQUIRE(((uchar*)stream->memory)[3] == 4);
}
