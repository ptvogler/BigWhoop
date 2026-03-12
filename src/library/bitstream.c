/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                                                                                                ||
||       /$$$$$$$  /$$                  /$$      /$$ /$$                                          ||
||      | $$__  $$|__/                 | $$  /$ | $$| $$                                          ||
||      | $$  \ $$ /$$  /$$$$$$        | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$       ||
||      | $$$$$$$ | $$ /$$__  $$       | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$      ||
||      | $$__  $$| $$| $$  \ $$       | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$      ||
||      | $$  \ $$| $$| $$  | $$       | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$      ||
||      | $$$$$$$/| $$|  $$$$$$$       | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/      ||
||      |_______/ |__/ \____  $$       |__/     \__/|__/  |__/ \______/  \______/ | $$____/       ||
||                     /$$  \ $$                                                  | $$            ||
||                    |  $$$$$$/                                                  | $$            ||
||                     \______/                                                   |__/            ||
||                                                                                                ||
\*  --------------------------------------------------------------------------------------------  */
/**                                                                                               
 *        @file   bitstream.c
 *
 *        This file describes a set of functions that can be used to create, manipulate
 *        and terminate a bitstream. These functions facilitate the creation or reading
 *        of a compressed bwc codestream and can emit/extract information on a per bit,
 *        symbol (64-bit) or string basis.
 *                                                                                                */
/*  --------------------------------------------------------------------------------------------  *\
||  Copyright (c) 2023, High Performance Computing Center - University of Stuttgart               ||
||                                                                                                ||
||  Redistribution and use in source and binary forms, with or without modification, are          ||
||  permitted provided that the following conditions are met:                                     ||
||                                                                                                ||
||     (1)   Redistributions of source code must retain the above copyright notice, this list of  ||
||           conditions and the following disclaimer.                                             ||
||                                                                                                ||
||     (2)   Redistributions in binary form must reproduce the above copyright notice, this list  ||
||           of conditions and the following disclaimer in the documentation and/or other         ||
||           materials provided with the distribution.                                            ||
||                                                                                                ||
||  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS   ||
||  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF               ||
||  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE    ||
||  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,     ||
||  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF            ||
||  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)        ||
||  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR      ||
||  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  ||
||  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                            ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                                _ _  _ ____ _    _  _ ___  ____                                 ||
||                                | |\ | |    |    |  | |  \ |___                                 ||
||                                | | \| |___ |___ |__| |__/ |___                                 ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libbwc.h"

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||             ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____             ||
||             |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__              ||
||             |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]             ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*================================================================================================*/
/**
 * @details This function is used to evaluate the number of bytes that have already been written 
 *          to the allocated bitstream memory block.
 *
 * @param[in] stream  Helper structure used to assemble/read bitstream.
 *
 * @return  Number of bytes  written to the bitstream.
 */
/*================================================================================================*/
uint64 
bytes_used(bitstream const *const stream)
{
  if(stream->T == 0xFF)
  {
    return stream->L + 1;
  }
  else
  {
    return stream->L;
  }
}

/*================================================================================================*/
/**
 *  @details Initializes a BWC bitstream for encoding or decoding. Stores the provided memory 
 *           handle for stream assembly or reading and initializes the byte buffer counter,
 *           maximum stream size, and size increment according to the supplied flag.
 *
 * @param[in] memory  Memory handle used to store the bitstream.
 * @param[in] size    Initial size of the bitstream.
 * @param[in] instr   Flag indicating encoding ('c') or decoding ('d') mode.
 *
 * @return Memory handle for the initialized BWC stream.
 */
/*================================================================================================*/
bitstream* 
init_bitstream(uchar* memory, uint32 size, char instr)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint64 tmp;

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  bitstream *stream;

  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(memory);
  assert(instr == 'c' || instr == 'd');

  stream = calloc(1, sizeof(bitstream));
  if(!stream)
  {
    // memory allocation error
    fprintf(stderr, MEMERROR);
    return NULL;
  }

  stream->memory    = memory;
  stream->t         = (instr == 'c') ? 8 : 0;
  stream->Lmax      = size;
  tmp               = size / 2;
  stream->size_incr = (uint64)(tmp ? tmp : 1);

  return stream;
}

/*================================================================================================*/
/**
 * @details Writes a chunk of data of size to a bwc bitstream. If the bitstream cannot accomodate 
 *          the additional information an error flag is set, the stream is invalidated (length = 0) 
 *          and the function returns to the caller. In case an error flag is already set the 
 *          function returns without processing.
 *
 * @param[in,out] stream  Memory handle for the BWC bitstream structure.
 * @param[in]     chunck  Memory handle for the data block to write.
 * @param[in]     size    Size of the data block.
 */
/*================================================================================================*/
void
pass_chunk(bitstream *const stream, const uchar* chunck, const uint64 size)
{
  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(stream);
  assert(chunck);

  /*--------------------------------------------------------*\
  ! Check if an error was encountered in a previous writing  !
  ! operation                                                !
  \*--------------------------------------------------------*/
  if(!stream->error)
  {
    /*--------------------------------------------------------*\
    ! Check if the enough memory has been allocated for the    !
    ! stream to store the additional symbol.                   !
    \*--------------------------------------------------------*/
    if((bytes_used(stream) + size) > stream->Lmax)
    {
      // memory allocation error
      stream->error |= 1;
      stream->Lmax   = 0;
      return;
    }
  }
  else
  {
    /*--------------------------------------------------------*\
    ! Exit to function caller if error flag has been set.      !
    \*--------------------------------------------------------*/
    return;
  }

  /*--------------------------------------------------------*\
  ! Copy the additional data to the stream memory block.     !
  \*--------------------------------------------------------*/
  memcpy(stream->memory + stream->L, chunck, size);

  /*--------------------------------------------------------*\
  ! Increment the number of bytes written to the stream with !
  ! the size of the newly added data chunck.                 !
  \*--------------------------------------------------------*/
  stream->L += size;
}

/*================================================================================================*/
/**
 * @details Writes a symbole of a specified size to a bwc bitstream. If the bitstream cannot
 *          accomodate the additional information an error flag is set, the stream is invalidated
 *          (length = 0) and the function returns to the caller. In case an error flag is already
 *          set the function returns without processing.
 *
 * @param[in,out] stream  Memory handle for the BWC bitstream structure.
 * @param[in]     symbol  Integer containing symbol to write.
 * @param[in]     size    Number of significant bytes in the symbol.
 */
/*================================================================================================*/
void
pass_symbol(bitstream *const stream, const uint64 symbol, const uint8 size)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint8              byte;
  int8               i;

  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(stream);
  
  /*--------------------------------------------------------*\
  ! Check if an error was encountered in a previous writing  !
  ! operation                                                !
  \*--------------------------------------------------------*/
  if(!stream->error)
  {
    /*--------------------------------------------------------*\
    ! Check if the enough memory has been allocated for the    !
    ! stream to store the additional symbol.                   !
    \*--------------------------------------------------------*/
    if((bytes_used(stream) + size) > stream->Lmax)
    {
      // memory allocation error
      stream->error |= 1;
      stream->Lmax   = 0;
      return;
    }
  }
  else
  {
    /*--------------------------------------------------------*\
    ! Exit to function caller if error flag has been set.      !
    \*--------------------------------------------------------*/
    return;
  }

  /*--------------------------------------------------------*\
  ! Copy the additional symbol to the stream memory block    !
  ! one byte at a time and Increment the number of bytes     !
  ! written to the stream.                                   !
  \*--------------------------------------------------------*/
  for(i = size; i --> 0;)
  {
    byte = (uint8)((symbol >> (8 * i)) & 0xFF);
    stream->memory[stream->L++] = (uchar)byte;
  }
}

/*================================================================================================*/
/**
 * @details Writes a single bit to the BWC bitstream buffer. If the buffer becomes full, its 
 *          contents are flushed to the stream memory chunk and the buffer is reset. If the 
 *          stream cannot accommodate the data, an error flag is set, the stream is invalidated 
 *          (length = 0), and the function returns. If an error flag is already set, the function 
 *          returns immediately without processing.
 *
 * @param[in,out] stream  Memory handle for the BWC bitstream structure.
 * @param[in]     bit     Integer containing bit to write.
 */
/*================================================================================================*/
void 
emit_bit(bitstream *const stream, const uint64 bit)
{
  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(stream);

  /*--------------------------------------------------------*\
  ! Exit if stream has been flagged with an error.           !
  \*--------------------------------------------------------*/
  if(stream->error)
  {
    return;
  }
  
  /*--------------------------------------------------------*\
  ! Decrement the bit buffer counter.                        !
  \*--------------------------------------------------------*/
  stream->t--;

  /*--------------------------------------------------------*\
  ! If the bit is significant, store the information at the  !
  ! appropriate position in the bit buffer.                  !
  \*--------------------------------------------------------*/
  if(bit)
  {
    stream->T |= (1 << stream->t);
  }

  /*--------------------------------------------------------*\
  ! Check the bit buffer counter to see if the bit buffer is !
  ! full.                                                    !
  \*--------------------------------------------------------*/
  if(!stream->t)
  {
    /*--------------------------------------------------------*\
    ! Check if the enough memory has been allocated for the    !
    ! stream to store the additional byte.                     !
    \*--------------------------------------------------------*/
    if(bytes_used(stream) + 1 > stream->Lmax)
    {
      /*--------------------------------------------------------*\
      ! Increment the stream size to store the additional byte.  !
      \*--------------------------------------------------------*/
      stream->Lmax      += stream->size_incr;
      stream->size_incr  = (uint64)(stream->Lmax / 2);

      /*--------------------------------------------------------*\
      ! Reallocate the stream data block.                        !
      \*--------------------------------------------------------*/
      stream->memory     = realloc(stream->memory, stream->Lmax);
      if(!stream->memory)
      {
        // memory allocation error
        stream->error |= 1;
        stream->Lmax   = 0;
        return;
      }
    }

    /*--------------------------------------------------------*\
    ! Copy the additional byte to the stream memory block and  !
    ! increment the number of bytes written to the stream.     !
    \*--------------------------------------------------------*/
    stream->memory[stream->L++] = (uchar)stream->T;

    /*--------------------------------------------------------*\
    ! Reset the byte buffer counter. If the current buffer has !
    ! a value of 255, limit the counter to 7 bits to ensure    !
    ! that the range FFFF to FF80 is reserved for the code-    !
    ! stream markers.                                          !
    \*--------------------------------------------------------*/
    if(stream->T == 0xFF)
    {
      stream->t = 7;
    }
    else
    {
      stream->t = 8;
    }

    /*--------------------------------------------------------*\
    ! Reset the byte buffer.                                   !
    \*--------------------------------------------------------*/
    stream->T = 0;
  }
}

/*================================================================================================*/
/**
 * @details Reads a data chunk of the specified size from a BWC bitstream. If the bitstream cannot 
 *          accommodate the requested data, an error flag is set. If an error flag is already set,
 *          the function returns immediately without processing. In either error case, a NULL
 *          pointer is returned.
 *
 * @param[in,out] stream  Memory handle for the BWC bitstream structure.
 * @param[in]     size    Size of the data block to read.
 * 
 * @return  Memory handle for the data block, or NULL on error.
 */
/*================================================================================================*/
uchar*
get_chunck(bitstream *const stream, const uint64 size)
{
  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  uchar             *tmp;

  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(stream);

  /*--------------------------------------------------------*\
  ! Check if the number of bytes to be read from the stream  !
  ! does not exceed the number of bytes still present in its !
  ! memory block.                                            !
  \*--------------------------------------------------------*/
  if(bytes_used(stream) + size <= stream->Lmax)
  {
    /*--------------------------------------------------------*\
    ! Allocate a temporary array used to store the bytes that  !
    ! are extracted from the stream.                           !
    \*--------------------------------------------------------*/
    tmp = calloc(size, sizeof(uchar));
    if(!tmp)
    {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return NULL;
    }

    /*--------------------------------------------------------*\
    ! Copy the bytes requested from the function caller from   !
    ! the stream to the temporary data block.                  !
    \*--------------------------------------------------------*/
    memcpy(tmp, stream->memory + stream->L, size);

    /*--------------------------------------------------------*\
    ! Increment the number of bytes read from the bitstream.   !
    \*--------------------------------------------------------*/
    stream->L += size;

    /*--------------------------------------------------------*\
    ! Return the temporary data block to the function caller.  !
    \*--------------------------------------------------------*/
    return tmp;
  }
  else
  {
    /*--------------------------------------------------------*\
    ! If the requested block size exceeds the information left !
    ! in the bitstream, set the bitstream error flag and       !
    ! return a NULL pointer.                                   !
    \*--------------------------------------------------------*/
    stream->error |= 1;
    return NULL;
  }
}

/*================================================================================================*/
/**
 * @details Reads a symbol of the specified size from a BWC bitstream. If the bitstream cannot 
 *          accommodate the requested data, an error flag is set. If an error flag is already set,
 *          the function returns immediately without processing. In either error case, a zero is 
 *          returned.
 *
 * @param[in,out] stream  Memory handle for the BWC bitstream structure.
 * @param[in]     size    Size of the symbol to read.
 * 
 * @return  Symbol, or zero on error.
 */
/*================================================================================================*/
uint64
get_symbol(bitstream *const stream, const uint8 size)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint64             symbol;
  uint8              byte;
  int8               i;

  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(stream);
  
  /*--------------------------------------------------------*\
  ! The return value of type uint64 is limited to 8 bytes.   !
  ! Information is lost if size exceeds that threshold.      !
  \*--------------------------------------------------------*/
  if(size > 8)
  {
    stream->error |= 2;
    return 0;
  }

  /*--------------------------------------------------------*\
  ! Check if the number of bytes to be read from the stream  !
  ! does not exceed the number of bytes still present in its !
  ! memory block.                                            !
  \*--------------------------------------------------------*/
  if(bytes_used(stream) + size <= stream->Lmax)
  {
    /*--------------------------------------------------------*\
    ! Extract the requested information from the bitstream one !
    ! byte at a time. Adjust the number of bytes read from the !
    ! stream accordingly.                                      !
    \*--------------------------------------------------------*/
    for(i = size, symbol = 0; i --> 0;)
    {
      byte    = (uint8)stream->memory[stream->L++];
      symbol |= ((uint64)byte << (i * 8));
    }

    /*--------------------------------------------------------*\
    ! Return the symbol to the function caller.                !
    \*--------------------------------------------------------*/
    return symbol;
  }
  else
  {
    /*--------------------------------------------------------*\
    ! If the requested block size exceeds the information left !
    ! in the bitstream, set the bitstream error flag and       !
    ! return zero.                                             !
    \*--------------------------------------------------------*/
    stream->error |= 1;
    return 0;
  }
}

/*================================================================================================*/
/**
 * @details Reads a single bit from the BWC bitstream. Refills the byte buffer from the stream if
 *          it is empty. If the read index reaches the end of the stream, returns 1. Otherwise,
 *          extracts the bit from the buffer and decrements its counter.
 * 
 * @param[in,out] stream  Memory handle for the BWC bitstream structure.
 * 
 * @return  The extracted bit (0 or 1), or 1 if end of stream is reached.
 */
/*================================================================================================*/
uchar 
get_bit(bitstream *const stream)
{
  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(stream);

  if(!stream->t)
  {
    if(stream->L == stream->Lmax)
    {
      return 1;
    }
    if(stream->T == 0xFF)
    {
      stream->t = 7;
    }
    else
    {
      stream->t = 8;
    }
    stream->T = (uint16)stream->memory[stream->L++];
  }
  stream->t--;
  return ((stream->T & (1 << stream->t)) >> stream->t);
}

/*================================================================================================*/
/**
 * @details Flushes remaining data from the byte buffer to the BWC bitstream. If the bitstream 
 *          cannot accomodate the additional information an error flag is set, the stream is 
 *          invalidated (length = 0) and the function returns to the caller. In case an error 
 *          flag is already set the function returns without processing.
 *
 * @param[in,out] stream  Memory handle for the BWC bitstream structure.
 */
/*================================================================================================*/
void 
flush_stream(bitstream *const stream)
{
  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(stream);

  /*--------------------------------------------------------*\
  ! Exit if stream has been flagged with an error.           !
  \*--------------------------------------------------------*/
  if(stream->error)
  {
    return;
  }

  /*--------------------------------------------------------*\
  ! Check the if the bit buffer contains information.        !
  \*--------------------------------------------------------*/
  if(stream->t != 8)
  {
    /*--------------------------------------------------------*\
    ! Check if the enough memory has been allocated for the    !
    ! stream to store the additional byte.                     !
    \*--------------------------------------------------------*/
    if((bytes_used(stream) + 1) > stream->Lmax)
    {
      /*--------------------------------------------------------*\
      ! Increment the stream size to store the additional byte.  !
      \*--------------------------------------------------------*/
      stream->Lmax      += stream->size_incr;
      stream->size_incr  = (uint64)(stream->Lmax / 2);

      /*--------------------------------------------------------*\
      ! Reallocate the stream data block.                        !
      \*--------------------------------------------------------*/
      stream->memory     = realloc(stream->memory, stream->Lmax);
      if(!stream->memory)
      {
        // memory allocation error
        stream->error |= 1;
        stream->Lmax   = 0;
        return;
      }
    }

    /*--------------------------------------------------------*\
    ! Copy the additional byte to the stream memory block and  !
    ! increment the number of bytes written to the stream.     !
    \*--------------------------------------------------------*/
    stream->memory[stream->L++] = (uchar)stream->T;
  }
}

/*================================================================================================*/
/**
 * @details Shrinks the bitstream memory to the actually filled range.
 *
 * @param[in,out] stream  Memory handle for the BWC bitstream structure.
 *
 * @return  Returns 0 if successfull and 1 if memory could not be resized. 
 */
/*================================================================================================*/
uchar
shrink_to_fit(bitstream *const stream)
{
  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(stream);

  if(stream->error)
  {
    return 1;
  }
  else if(stream->L > stream->Lmax)
  {
    stream->Lmax   = stream->L;
    stream->memory = realloc(stream->memory, stream->Lmax);
    if(!stream->memory)
    {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      stream->Lmax = 0;
      return 1;
    }
  }
  return 0;
}

/*================================================================================================*/
/**
 * @details Swap memory pointer and size to span. Invalidates stream pointers.
 *
 * @param[in,out] stream  Memory handle for the BWC bitstream structure.
 * @param[in,out] span    Memory handle for the BWC span structure.
 *
 * @return  Returns 0 if successfull and 1 if stream had previous memory error.
 */
/*================================================================================================*/
uchar
transfer_to_span(bitstream *const stream, bwc_span *const span)
{
  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(stream);
  assert(span);

  if(stream->error)
  {
    return 1;
  }

  span->memory   = stream->memory;
  span->access   = stream->memory;
  span->size     = stream->L;
  span->position = 0;

  stream->memory = NULL;
  stream->L = 0;

  return 0;
}

/*================================================================================================*/
/**
 * @details Releases all the information stored in a packed bitstream and reset the parameters for 
 *          another (de)compression run.  
 *
 * @param[in,out] stream  Memory handle for the BWC bitstream structure.
 */
/*================================================================================================*/
void
release_packed_stream(bwc_span *stream)
{
  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(stream);

  free(stream->memory);

  stream->access    = NULL;
  stream->position  = 0;
  stream->size      = 0;
}