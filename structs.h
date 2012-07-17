#ifndef __STRUCTS_H__
#define __STRUCTS_H__

struct sStats {
	char m_cName[128];
	int m_iASCII;

	int m_iRace;
	int m_iSex;
	int m_iJob;

	int m_iLevel;
	int m_iExp;

	/* Hit points */
	int m_iHP;
	int m_iMaxHP;

	/* Skill points */
	int m_iSP;
	int m_iMaxSP;

	/*  Stats */
	int m_iStr;	/* Strength */
	int m_iDex;	/* Dexterity */
	int m_iAgl;	/* Agility */
	int m_iCha;	/* Charisma */
	int m_iWis;	/* Wisdom */
	int m_iInt;	/* Intelligence */
	int m_iAtk;	/* Attack power */
	int m_iDef;	/* Defense power */
	int m_iSpeed; /* Determines Order of Operations */

	int m_iWorldX;
	int m_iWorldY;
	int m_iWorldZ;

	bool m_bWalkable;					/* Can the character walk on this? */
	bool m_bTransparent;				/* Can the character see through this? */
	int m_iFOV;							/* Field of View (if obj distorts visibility) */

	TCOD_color_t m_tcodVisibleColor;	/* Color in FOV */
	TCOD_color_t m_tcodHiddenColor;		/* Color not in FOV */
};

struct sEquip {
	int m_iHead;
	int m_iEar_L;
	int m_iEar_R;
	int m_iFace;
	int m_iNeck;
	int m_iBack;
	int m_iChest;
	int m_iHands;
	int m_iWaist;
	int m_iLegs;
	int m_iFeet;
	int m_iHand_L;
	int m_iHand_R;
};

struct sInventory {
	int m_iItem;
	int m_iCount;
	struct sInventory *next;
};

struct sEgoData {
	sStats m_stats;
	sEquip m_equipment;
	sInventory *m_inventory;
	int m_iVisibility;
	float m_fFOVDistance;
};

struct sItemData {
	int m_iID; // Internal count for customized items.
	sStats m_stats;
	void (*m_vFunc)(int iAction, int iX, int iY, void *vUserData);	// Callback function, if any
};

struct sItemList {
	sItemData m_item;
	struct sItemList *next;
};

struct sCraftingFormula {
	int m_iItem1;		// First item in formula
	int m_iItem1Count;	// Amount of items in forumla
	int m_iItem2;		// Second item in formula
	int m_iItem2Count;	// Amount of items in formula
	int m_iResult;		// Resulting item from crafting
	int m_iResultCount;	// Amount of resulting item to be made
	int m_iSuccessRate;	// Rate of success (percentage)
	void (*m_vFunc)(int iItem1, int iItem1Count, int iItem2, int iItem2Count, int iResult, int iResultCount, void *vUserData); // Just in case something weird should happen.

};

struct sNPCData {
	int m_iID;
	sStats m_stats;
	sEquip m_equipment;
	sInventory *m_inventory;
	int m_iVisibility;
	float m_fFOVDistance;
	void (*m_vFunc)(int iAction, int iX, int iY, void *vUserData); // Callback
	int m_iUID; // Internal unique identifier, for counting purposes.
};

struct sNPCMemory {
	int m_iMemoryGrid[DEF_WORLD_X][DEF_WORLD_Y];
	int m_iPanic;
	int m_iLastEvent;
};

struct sNPCList {
	sNPCData m_npcData;
	struct sNPCList *next;
};

struct sNPCMap {
	int m_iID;
	int m_iUID;
	sNPCData *npc;
};

struct sILightmap {
	int iX;
	int iY;
};

#endif
