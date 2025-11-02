/*================================================================================================*\
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
||  DESCRIPTION:                                                                                  ||
||  ------------                                                                                  ||
||                                                                                                ||
||        This file describes a set of functions that can be used to create, manipulate           ||
||        and terminate a bitstream. These functions facilitate the creation or reading           ||
||        of a compressed bwc codestream and can emit/extract information on a per bit,           ||
||        symbol (64-bit) or string basis.                                                        ||
||                                                                                                ||
||  --------------------------------------------------------------------------------------------  ||
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
\*================================================================================================*/
/************************************************************************************************************\
||                                      _ _  _ ____ _    _  _ ___  ____                                     ||
||                                      | |\ | |    |    |  | |  \ |___                                     ||
||                                      | | \| |___ |___ |__| |__/ |___                                     ||
||                                                                                                          ||
\************************************************************************************************************/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libbwc.h"

/************************************************************************************************************\
||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uint32 bytes_used(bitstream *const stream)                                                !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to evaluate the number of bytes that have already been                !
!                written to the allocated bitstream memory block.                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                stream                      bitstream*            - Structure that                          !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                unsigned int(32 bit)      - Number of bites that have been written to the                   !
!                                            bitstream.                                                      !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                13.05.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------*\
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to initialize a bwc bitstream. For encoding, a null pointer           !
!                is passed as a memory handle and the function will allocate a memory block with the         !
!                specified stream size. For decoding, a valid memory handle, passed by the function          !
!                caller, will be stored in the bitstream structure. The byte buffer counter t,               !
!                stream size Lmax and size increment are initialized with their appropriate values.          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                size                        unsigned int(32 bit)  - Initial size of the bwc stream.         !
!                                                                                                            !
!                memory                      unsigned char         - Memory handle for the bwc stream memory !
!                                                                    block.                                  !
!                                                                                                            !
!                instr                       char                  - Constant used to instruct the field     !
!                                                                    initialization.                         !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                bitstream*                - Memory handle for the initialized bwc stream.                   !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                19.06.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
bitstream* 
init_bitstream(uchar* memory, uint32 size, char instr)
{
  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  bitstream *stream;

  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(memory);
  assert(instr == 'c' || instr == 'd');

  /*--------------------------------------------------------*\
  ! Allocate the bwc stream structure.                       !
  \*--------------------------------------------------------*/
  stream = calloc(1, sizeof(bitstream));
  if(!stream)
  {
    // memory allocation error
    fprintf(stderr, MEMERROR);
    return NULL;
  }

  /*--------------------------------------------------------*\
  ! Initialize the byte buffer counter, stream size and size !
  ! increment for the current stream.                        !
  \*--------------------------------------------------------*/
  stream->memory    = memory;
  stream->t         = (instr == 'c') ? 8 : 0;
  stream->Lmax      = size;
  stream->size_incr = (uint64)((size / 2) ?: 1);

  /*--------------------------------------------------------*\
  ! Return the stream memory handle.                         !
  \*--------------------------------------------------------*/
  return stream;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void pass_chunk(bitstream *const stream, const uchar* string, const uint64 length)        !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to write an additional chunck of size length to a bwc bitstream.      !
!                                                                                        !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                stream                      bitstream*            - Structure used to assemble a bwc bit-   !
!                                                                    bitstream.                              !
!                                                                                                            !
!                chunck                      unsigned char*        - Memory handle for a data block that is  !
!                                                                    to be written to the bwc bitstream.     !
!                                                                                                            !
!                size                        unsigned int(64 bit)  - Size of the data block.                 !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                22.06.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                stream                      bitstream*            - Structure used to assemble a bwc bit-   !
!                                                                    bitstream.                              !
!                                                                                                            !
!                symbol                      unsigned int(64 bit)  - Symbol that is to be written to the bwc !
!                                                                    bitstream.                              !
!                                                                                                            !
!                size                        unsigned int(8 bit)   - Number of significant bytes in the      !
!                                                                    symbol.                                 !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                22.06.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_emit_bit(bitstream *const stream, const uint64 bit)                              !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                stream                      bitstream*            - Structure used to assemble a bwc bit-   !
!                                                                    bitstream.                              !
!                                                                                                            !
!                bit                         unsigned int(64 bit)  - Bit that is to be written to the bwc    !
!                                                                    bitstream.                              !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                22.06.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                stream                      bitstream*            - Structure used to assemble a bwc bit-   !
!                                                                    bitstream.                              !
!                                                                                                            !
!                size                        unsigned int(64 bit)  - Size of the data block.                 !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                uchar*                    - Data chunck requested by the function caller.                   !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                22.06.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                stream                      bitstream*            - Structure used to assemble a bwc bit-   !
!                                                                    bitstream.                              !
!                                                                                                            !
!                size                        unsigned int(8 bit)   - Number of significant bytes in the      !
!                                                                    symbol.                                 !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                uchar*                    - Symbol requested by the function caller.                        !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------*\
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                Shrinks the bitstream memory to the actually filled range.                                  !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Returns 0 if successfull and 1 if memory could not be resized.                              !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------*\
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                Swap memory pointer and size to span. Invalidates stream pointers.                          !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Returns 0 if successfull and 1 if stream had previous memory error.                         !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void release_packed_stream(bwc_span *stream)                                            !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to release all the information stored in a packed bitstream           !
!                and reset the parameters for another (de)compression run.                                   !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                stream                      bwc_span            - Packed bitstream used to store parts of !
!                                                                    the bwc codestream.                     !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                22.06.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
release_packed_stream(bwc_span *stream)
{
  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(stream);

  /*--------------------------------------------------------*\
  ! Free the data block of the packed bitstream.             !
  \*--------------------------------------------------------*/
  free(stream->memory);

  /*--------------------------------------------------------*\
  ! Reset all the packed stream parameters.                  !
  \*--------------------------------------------------------*/
  stream->access    = NULL;
  stream->position  = 0;
  stream->size      = 0;
}