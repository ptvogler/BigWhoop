#include <catch2/catch_test_macros.hpp>

#include <cstdint>

#ifdef __cplusplus
  extern "C" {
#endif
#include "../include/library/private/libbwc.h"
#ifdef __cplusplus
  }
#endif

TEST_CASE( "Stream initialization", "[bwc_init_stream]" ) {
    uint32 size = 4;
    uchar inp_mem[size];
    uchar out_mem[size];
    out_mem[3] = inp_mem[0] = 1;
    out_mem[2] = inp_mem[1] = 2;
    out_mem[1] = inp_mem[2] = 3;
    out_mem[0] = inp_mem[3] = 4;

    bwc_stream *stream = bwc_init_stream(inp_mem, out_mem, comp);

    REQUIRE(((uchar*)stream->inp)[0] == 1);
    REQUIRE(((uchar*)stream->inp)[1] == 2);
    REQUIRE(((uchar*)stream->inp)[2] == 3);
    REQUIRE(((uchar*)stream->inp)[3] == 4);

    REQUIRE(((uchar*)stream->out)[0] == 4);
    REQUIRE(((uchar*)stream->out)[1] == 3);
    REQUIRE(((uchar*)stream->out)[2] == 2);
    REQUIRE(((uchar*)stream->out)[3] == 1);
}
