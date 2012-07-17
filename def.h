#ifndef __DEF_H__
#define __DEF_H__

#define DEF_WORLD_X	75
#define DEF_WORLD_Y	75

#define DEF_CAMERA_VIEWPORT_X	75
#define DEF_CAMERA_VIEWPORT_Y	75

#define LIGHT_DEFAULT	10.0f
#define LIGHT_TORCH		5.0f
#define LIGHT_LAMP		10.0f
#define LIGHT_MAX		75.0f

#define ITEM_NOTHING			0
#define ITEM_ORE_STONE			1
#define ITEM_ORE_STONE_WEAK		2
#define ITEM_ORE_STONE_WEAKER	3
#define ITEM_ORE_IRON			4
#define ITEM_ORE_IRON_WEAK		5
#define ITEM_ORE_IRON_WEAKER	6
#define ITEM_ORE_COPPER			7
#define ITEM_ORE_COPPER_WEAK	8
#define ITEM_ORE_COPPER_WEAKER	9
#define ITEM_ORE_SILVER			10
#define ITEM_ORE_SILVER_WEAK	11
#define ITEM_ORE_SILVER_WEAKER	12
#define ITEM_ORE_GOLD			13
#define ITEM_ORE_GOLD_WEAK		14
#define ITEM_ORE_GOLD_WEAKER	15
//
//
#define ITEM_DOOR_CLOSED		18
#define ITEM_DOOR_OPEN			19

#define ACT_WALK_ONTO		0
#define ACT_WALK_INTO		1
#define ACT_OPEN			2
#define ACT_EQUIP			3
#define ACT_DEQUIP			4
#define ACT_MINE			5
#define ACT_SEARCH			6
#define ACT_PLACE_ONTO		100
#define ACT_PLACE_INTO		101
#define ACT_ENGINE_CREATE	500		/* Game engine created object. */
#define ACT_ENGINE_POPULATE	501		/* Populate with item. */
#define ACT_ACTIVATE		1000
#define ACT_NPC_INIT		5000
#define ACT_NPC_THINK		5001
#define ACT_NPC_DEATH		5002
#define ACT_NPC_DELETE		5999


#define MODE_INTRO			0
#define MODE_GAME			1
#define MODE_BLOCK			2
#define MODE_INV			3
#define MODE_OPEN			4
#define MODE_EQUIP			5
#define MODE_DEQUIP			6
#define MODE_LOOK			7
#define MODE_CONSOLE_INPUT	8

#define RACE_NONE			0
#define RACE_HUMAN			1

#define SEX_NONE			0
#define SEX_MALE			1
#define SEX_FEMALE			2
#define SEX_SEXLESS			3
#define SEX_BOTH			4

#define JOB_NONE			0
#define JOB_WARRIOR			1

#endif
