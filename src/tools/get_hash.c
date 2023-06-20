/*==================================================================================================================================*\
||                                                                                                                                  ||
||                         /$$$$$$$  /$$                 /$$      /$$ /$$                                                           ||
||                        | $$__  $$|__/                | $$  /$ | $$| $$                                                           ||
||                        | $$  \ $$ /$$  /$$$$$$       | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$                        ||
||                        | $$$$$$$ | $$ /$$__  $$      | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$                       ||
||                        | $$__  $$| $$| $$  \ $$      | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$                       ||
||                        | $$  \ $$| $$| $$  | $$      | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$                       ||
||                        | $$$$$$$/| $$|  $$$$$$$      | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/                       ||
||                        |_______/ |__/ \____  $$      |__/     \__/|__/  |__/ \______/  \______/ | $$____/                        ||
||                                       /$$  \ $$                                                 | $$                             ||
||                                      |  $$$$$$/                                                 | $$                             ||
||                                       \______/                                                  |__/                             ||
||                                                                                                                                  ||
||      FILE NAME:   get_hash.c                                                                                                     ||
||                                                                                                                                  ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||                         This is a simple command line tool that converts the command-line arguments to a uniquely identifiable   ||
||                         hash.                                                                                                    ||
||                                                                                                                                  ||
||                                                                                                                                  ||
||      PUBLIC FUNCTIONS:                                                                                                           ||
||      -----------------                                                                                                           ||
||      - main                                                                                                                      ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            02.05.2019  Patrick Vogler     B87D120     V 0.1.0     module created                                 ||
||                                                                                                                                  ||
||       --------------------------------------------------------------------------------------------------------------------       ||
||                                                                                                                                  ||
||       Copyright (c) 2023, High Performance Computing Center - University of Stuttgart                                            ||
||                                                                                                                                  ||
||       Redistribution and use in source and binary forms, with or without modification, are permitted provided that the           ||
||       following conditions are met:                                                                                              ||
||                                                                                                                                  ||
||          (1)   Redistributions of source code must retain the above copyright notice, this list of conditions and                ||
||                the following disclaimer.                                                                                         ||
||                                                                                                                                  ||
||          (2)   Redistributions in binary form must reproduce the above copyright notice, this list of conditions                 ||
||                and the following disclaimer in the documentation and/or other materials provided with the                        ||
||                distribution.                                                                                                     ||
||                                                                                                                                  ||
||       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,         ||
||       INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          ||
||       DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,          ||
||       SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR            ||
||       SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,          ||
||       WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE           ||
||       USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                   ||
||                                                                                                                                  ||
\*==================================================================================================================================*/

/************************************************************************************************************\
||                                      _ _  _ ____ _    _  _ ___  ____                                     ||
||                                      | |\ | |    |    |  | |  \ |___                                     ||
||                                      | | \| |___ |___ |__| |__/ |___                                     ||
||                                                                                                          ||
\************************************************************************************************************/
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: int main(int argc, char* argv[])                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                                                                                                            !
!                This function uses a variant of the DJB hash function to turn the command-line argument     !
!                strings and converts them to a uniquely identifiable hash. The hashes are written to the    !
!                command-line in a hexadecimal format.                                                       !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                argc                        int                   - Number of strings pointed to by argv.   !
!                                                                                                            !
!                argv                        char**                - Array of arguments.                     !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                int                       - Return value signaling a normal or abnormal process exit.       !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                02.05.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64_t hash;
   uint8_t  c, i;

   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   char*    str;

   /*--------------------------------------------------------*\
   ! Loop through all additional command-line arguments.      !
   \*--------------------------------------------------------*/
   for(i = 1, str = argv[1]; i < argc; ++i, str = argv[i])
   {
      /*--------------------------------------------------------*\
      ! Initialize the hash with a magic number.                 !
      \*--------------------------------------------------------*/
      hash = 0x1505;

      /*--------------------------------------------------------*\
      ! Walk through all the characters in the string.           !
      \*--------------------------------------------------------*/
      while ((c = *str++))
      {
         /*--------------------------------------------------------*\
         ! Convert the current characters to uppercase.             !
         \*--------------------------------------------------------*/
         if((c >= 97) && (c <= 122))
         {
            c = c - 32;
         }

         /*--------------------------------------------------------*\
         ! Multiply the hash with 33 and add the current character  !
         ! to the hash.                                             !
         \*--------------------------------------------------------*/      
         hash = (hash * 33) ^ c;
      }

      /*--------------------------------------------------------*\
      ! Write the hash to the command-line.                      !
      \*--------------------------------------------------------*/
      printf("%#020lX \n", hash);
   }
   return 0;
}