#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>

#include <libtcod/libtcod.h>

#include "def.h"
#include "globals.h"
#include "structs.h"
#include "items.h"
#include "itemfunc.h"
#include "inventory.h"
#include "console.h"

void npcPopulate(int iNPCID, int iAmount, int iPercentage, int iIterations);
void npcExecute(sNPCList **npcList);
int npcListCheck(sNPCList **npcList, int iNPCID);
void npcListAdd(sNPCList **npcList, sNPCData npcData);
void npcListDel(sNPCList **npcList, int iUID);
void npcListList(sNPCList **inventory);
