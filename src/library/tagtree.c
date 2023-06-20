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
||      FILE NAME:   tagtree.c                                                                                                      ||
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
||      - kill_tagtree                                                                                                              ||
||      - reset_tagtree                                                                                                             ||
||      - tagtree_get_value                                                                                                         ||
||      - tagtree_set_value                                                                                                         ||
||      - initialize_tagtree                                                                                                        ||
||      - encode_tagtree                                                                                                            ||
||      - decode_tagtree                                                                                                            ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            18.05.2018  Patrick Vogler     B87D120     V 0.1.0     source file created                            ||
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
#include <stdio.h>
#include <stdlib.h>

#include "bitstream.h"
#include "macros.h"
#include "types.h"
#include "tagtree.h"

/************************************************************************************************************\
||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void kill_tagtree(bwc_tagtree* tagtree)                                                   !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function deallocates a tagtree instance used to encode codeblock contributions to a    !
!                specific quality layer as well as the number of magnitude bit planes used to represent the  !
!                samples of a specific codeblock.                                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                tagtree                     bwc_tagtree*          - Structure defining a tagtree instance.  !
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
!                14.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void 
kill_tagtree(bwc_tagtree* tagtree)
{
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(tagtree);

   free(tagtree->nodes);
   free(tagtree);
   return;
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
!                14.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void 
reset_tagtree(bwc_tagtree* const tagtree)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64 i = 0;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(tagtree);

   do
   {
      tagtree->nodes[i].value = 0xFFFF;
      tagtree->nodes[i].threshold = 0;
   }while(tagtree->nodes[i++].parent);
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
!                14.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uint16 
tagtree_get_value(const bwc_tagtree* const tagtree, const uint64 leaf_index)
{
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(tagtree);
   assert(leaf_index < tagtree->leavesX * tagtree->leavesY * tagtree->leavesZ * tagtree->leavesTS);
   assert(tagtree->nodes[leaf_index].value != 0xFFFF);

   return tagtree->nodes[leaf_index].value;
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
!                14.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void 
tagtree_set_value(bwc_tagtree* const tagtree, const uint64 leaf_index, const uint16 value)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_tagtree_node  *node;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(tagtree);
   assert(leaf_index < tagtree->leavesX * tagtree->leavesY * tagtree->leavesZ * tagtree->leavesTS);
   assert(tagtree->nodes[leaf_index].value == 0xFFFF);
   assert(value < 0xFFFF);

   node = &tagtree->nodes[leaf_index];

   do
   {
      node->value = value;
      node = node->parent;
   }while(node && (node->value > value));
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
!                14.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
bwc_tagtree* 
initialize_tagtree(const uint64 leafsX, const uint64 leafsY, const uint64 leafsZ, const uint64 leafsTS)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64 levels, number_of_nodes;
   uint32 nlX[32] = {0}, nlY[32] = {0}, nlZ[32] = {0}, nlTS[32] = {0};
   uint32 i, j, k, l, n;
   uint16 a,b,c,d;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_tagtree_node  *node, *parent_x, *parent_y, *parent_z, *parent_ts;
   bwc_tagtree       *tagtree;

   for(nlX[0] = leafsX, nlY[0]  = leafsY, 
       nlZ[0] = leafsZ, nlTS[0] = leafsTS,
       levels = 0, number_of_nodes = 1; 
       nlX[levels] * nlY[levels] * 
       nlZ[levels] * nlTS[levels] > 1; 
       ++levels)
   {
      number_of_nodes += nlX[levels] * nlY[levels] * nlZ[levels] * nlTS[levels];
      nlX[levels + 1]   = (nlX[levels]  + 1) >> 1;
      nlY[levels + 1]   = (nlY[levels]  + 1) >> 1;
      nlZ[levels + 1]   = (nlZ[levels]  + 1) >> 1;
      nlTS[levels + 1]  = (nlTS[levels] + 1) >> 1;
   }

   tagtree = calloc(1, sizeof(bwc_tagtree));
   if (!tagtree) 
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 0;
   }

   tagtree->nodes = calloc(number_of_nodes, sizeof(bwc_tagtree_node));
   if (!tagtree->nodes) 
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(tagtree);
      return 0;
   }

   node = tagtree->nodes;

   for(i = 0; i < number_of_nodes; ++i)
   {
      node->index = i;
      ++node;
   }

   tagtree->leavesX  = leafsX;
   tagtree->leavesY  = leafsY;
   tagtree->leavesZ  = leafsZ;
   tagtree->leavesTS = leafsTS;

   parent_ts = tagtree->nodes; 
   node = tagtree->nodes;  
   for(n = 0; n < levels; ++n)
   {
      parent_ts += (uint32)(nlX[n] * nlY[n] * nlZ[n] * nlTS[n]);
      for(l = 0, d = 0; l < nlTS[n]; ++l, ++d)
      {
         if(d >> 1)
         {
            parent_ts += nlX[n+1] * nlY[n+1] * nlZ[n+1];
            d = 0;
         }
         for(k = 0, c = 0, parent_z = parent_ts; k < nlZ[n]; ++k, ++c)
         {
            if(c >> 1)
            {
               parent_z += nlX[n+1] * nlY[n+1];
               c = 0;
            }
            for(j = 0, b = 0, parent_y = parent_z; j < nlY[n]; ++j, ++b)
            {
               if(b >> 1)
               {
                  parent_y += nlY[n + 1];
                  b = 0;
               }
               for(i = 0, a = 0, parent_x  = parent_y; i < nlX[n]; ++i, ++a)
               {
                  if(a >> 1)
                  {
                     parent_x++;
                     a = 0;
                  }
                  node->parent = parent_x;
                  node++;
               }
            }
         }
      }
   }
   reset_tagtree(tagtree);
   return tagtree;
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
!                14.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void 
encode_tagtree(bwc_tagtree *const tagtree, bwc_stream *const stream, const uint32 threshold, const uint32 leaf_index, const uchar estimate)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint32 threshold_tmp, threshold_min;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_tagtree_node   *node;
   bwc_tagtree_node  **branch_ptr, *branch[32];

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(tagtree);
   assert(stream);
   assert(leaf_index < (tagtree->leavesX * tagtree->leavesY * tagtree->leavesZ * tagtree->leavesTS));

   node = &tagtree->nodes[leaf_index];
   branch_ptr = branch;
   
   while(node->parent)
   {
      *branch_ptr++ = node;
      node = node->parent;
   }

   for(threshold_min = 0; ; node = *--branch_ptr)
   {
      threshold_tmp = node->threshold;

      if(threshold_tmp < threshold_min)
      {
         threshold_tmp = threshold_min;
      }
      while((node->value >= threshold_tmp) && (threshold_tmp < threshold)) //this conditional jump needs to be checked
      {
         threshold_tmp++;
         if(node->value >= threshold_tmp)
         {
            bwc_emit_bit(stream, 0);
         }
         else
         {
            bwc_emit_bit(stream, 1);
         }
      }

      threshold_min = MIN(node->value, threshold_tmp);

      if(!estimate)
      {
         node->threshold = threshold_tmp;
      }

      if(node == &tagtree->nodes[leaf_index])
      {
         break;
      }
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
!                14.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar 
decode_tagtree(bwc_tagtree *const tagtree, bwc_stream *const stream, const uint32 threshold, const uint32 leaf_index)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint32 threshold_min;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_tagtree_node   *node;
   bwc_tagtree_node  **branch_ptr, *branch[32];

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(tagtree);
   assert(stream);
   assert(leaf_index < (tagtree->leavesX * tagtree->leavesY * tagtree->leavesZ * tagtree->leavesTS));

   node = &tagtree->nodes[leaf_index];
   branch_ptr = branch;
   
   while(node)
   {
      if(node->value == 0xFFFF)
      {
         node->value = 0;
      }

      *branch_ptr++ = node;
      node = node->parent;
   }

   for(*branch_ptr--, node = *branch_ptr--, threshold_min = 0; ; node = *branch_ptr--)
   {
      if(node->threshold < threshold_min)
      {
         node->value     = threshold_min;
         node->threshold = threshold_min;
      }
      while((node->value == node->threshold) && (node->threshold < threshold))
      {
         node->threshold++;
         if(!bwc_get_bit(stream))
         {
            node->value++;
         }
      }

      threshold_min = MIN(node->value, node->threshold);

      if(node == &tagtree->nodes[leaf_index])
      {
         break;
      }
   }
   return (node->value < threshold) ? 0 : 1;
}