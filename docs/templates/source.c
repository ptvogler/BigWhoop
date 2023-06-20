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
||      FILE NAME:   source.c                                                                                                       ||
||                                                                                                                                  ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||      DESCRIPTION NEEDED.                                                                                                         ||
||                                                                                                                                  ||
||      FILE REFERENCES:                                                                                                            ||
||      ----------------                                                                                                            ||
||                                                                                                                                  ||
||                         Name              I/O             Description                                                            ||
||                         ----              ---             -----------                                                            ||
||                         none               -                   -                                                                 ||
||                                                                                                                                  ||
||                                                                                                                                  ||
||      PRIVATE FUNCTIONS:                                                                                                          ||
||      ------------------                                                                                                          ||
||                                                                                                                                  ||
||      PUBLIC FUNCTIONS:                                                                                                           ||
||      -----------------                                                                                                           ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            -           Patrick Vogler     B87D120     V -         source file created                            ||
||                            -           Patrick Vogler     B880CA2     V -         source file patched                            ||
||                            -           Patrick Vogler     B87E7E4     V -         source file updated                            ||
||                            -           Patrick Vogler     B87F684     V -         source file new version                        ||
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
/************************************************************************************************************\
||            ____ _  _ ___ ____ ____ _  _ ____ _       _  _ ____ ____ _ ____ ___  _    ____ ____           ||
||            |___  \/   |  |___ |__/ |\ | |__| |       |  | |__| |__/ | |__| |__] |    |___ [__            ||
||            |___ _/\_  |  |___ |  \ | \| |  | |___     \/  |  | |  \ | |  | |__] |___ |___ ___]           ||
||                                                                                                          ||
\************************************************************************************************************/
/************************************************************************************************************\
||           ____ _  _ ___ ____ ____ _  _ ____ _       ____ ____ _  _ ____ ___ ____ _  _ ___ ____           ||
||           |___  \/   |  |___ |__/ |\ | |__| |       |    |  | |\ | [__   |  |__| |\ |  |  [__            ||
||           |___ _/\_  |  |___ |  \ | \| |  | |___    |___ |__| | \| ___]  |  |  | | \|  |  ___]           ||
||                                                                                                          ||
\************************************************************************************************************/

/************************************************************************************************************\
||                ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                ||
||                |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__                 ||
||                |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]                ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *template(void)                                                                      !
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
!                -           Patrick Vogler     B87D120     V -         function created                     !
!                -           Patrick Vogler     B880CA2     V -         function patched                     !
!                -           Patrick Vogler     B87E7E4     V -         function updated                     !
!                -           Patrick Vogler     B87F684     V -         function new version                 !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/

/************************************************************************************************************\
||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
||                                                                                                          ||
\************************************************************************************************************/

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
!                -           Patrick Vogler     B87D120     V -         function created                     !
!                -           Patrick Vogler     B880CA2     V -         function patched                     !
!                -           Patrick Vogler     B87E7E4     V -         function updated                     !
!                -           Patrick Vogler     B87F684     V -         function new version                 !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
/*-----------------------*\
! DEFINE INT VARIABLES:   !
\*-----------------------*/
/*-----------------------*\
! DEFINE FLOAT VARIABLES: !
\*-----------------------*/
/*-----------------------*\
! DEFINE CHAR VARIABLES:  !
\*-----------------------*/
/*-----------------------*\
!  DEFINE STRUCTS:        !
\*-----------------------*/ 
/*--------------------------------------------------------*\
! COMMENTCOMMENTCOMMENTCOMMENTCOMMENTCOMMENTCOMMENTCOMMENT !
\*--------------------------------------------------------*/