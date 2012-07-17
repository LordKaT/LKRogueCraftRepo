#include <libtcod/libtcod.h>

#include "def.h"
#include "structs.h"

extern sEgoData g_ego;

extern int g_iWorld[DEF_WORLD_X][DEF_WORLD_Y];						/* Walls/Doors */
extern bool g_bVisible[DEF_WORLD_X][DEF_WORLD_Y];					/* Visibility flags */
extern sInventory *g_inventoryWorldList[DEF_WORLD_X][DEF_WORLD_Y];	/* Item list for each square */
extern sILightmap g_iWorldLightmap[DEF_WORLD_X][DEF_WORLD_Y];		/* Lightmap for lighting effects */
extern sNPCData *g_npcMap[DEF_WORLD_X][DEF_WORLD_Y];				/* NPC map for collisions and such */

extern TCOD_map_t g_tcodMap;
extern TCOD_map_t g_tcodLightmap;

extern sItemData g_itemData[];
extern sItemList *g_itemDynamic;

extern sInventory *g_itemLook;

extern sNPCData g_npcData[];
extern sNPCList *g_npcList;

extern sCraftingFormula g_CraftingFormula[];
extern int g_iItemTracker;
extern int g_iGameMode;
extern char g_cConsole[32][672];
extern char g_cConsoleInput[32];
extern int g_iConsoleInputCount;

void globalsInit();

char g_cRaceNames[][32];
char g_cRaceNamesShort[][4];
char g_cSexNames[][32];
char g_cSexNameShort[][4];
char g_cJobNames[][32];
char g_cJobNamesShort[][32];
