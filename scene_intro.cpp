#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include <SDL/SDL.h>

#include <libtcod/libtcod.h>

#include "def.h"
#include "globals.h"
#include "structs.h"
#include "bsp.h"
#include "map.h"
#include "items.h"
#include "player.h"
#include "inventory.h"
#include "console.h"

int g_iSceneIntroNum = 0;
int g_iSceneIntroInput = 0;

void scene_intro() {
	switch (g_iSceneIntroNum) {
		case 0:
			TCOD_console_print_left(NULL, 0, 0, TCOD_BKGND_NONE, "What is your name? %s", g_ego.m_stats.m_cName);
			break;
		case 1:
			TCOD_console_print_left(NULL, 0, 0, TCOD_BKGND_NONE, "%s is this correct? (y/n)", g_ego.m_stats.m_cName);
			break;
		case 2:
			TCOD_console_print_left(NULL, 0, 0, TCOD_BKGND_NONE, "Please choose a race: ");
			TCOD_console_print_left(NULL, 0, 2, TCOD_BKGND_NONE, "a) Human");
			break;
		case 3:
			TCOD_console_print_left(NULL, 0, 0, TCOD_BKGND_NONE, "Please choose a sex: ");
			TCOD_console_print_left(NULL, 0, 2, TCOD_BKGND_NONE, "a) Male");
			TCOD_console_print_left(NULL, 0, 3, TCOD_BKGND_NONE, "b) Female");
			TCOD_console_print_left(NULL, 0, 4, TCOD_BKGND_NONE, "c) Neither");
			TCOD_console_print_left(NULL, 0, 5, TCOD_BKGND_NONE, "d) Both");
			break;
		case 4:
			TCOD_console_print_left(NULL, 0, 0, TCOD_BKGND_NONE, "Please choose a job: ");
			TCOD_console_print_left(NULL, 0, 2, TCOD_BKGND_NONE, "a) Warrior");
			break;
		case 5:
			TCOD_console_print_left(NULL, 0, 0, TCOD_BKGND_NONE, "%s the %s %s %s", g_ego.m_stats.m_cName, g_cSexNames[g_ego.m_stats.m_iSex], g_cRaceNames[g_ego.m_stats.m_iRace], g_cJobNames[g_ego.m_stats.m_iJob]);
			TCOD_console_print_left(NULL, 0, 2, TCOD_BKGND_NONE, "\"Breaking the Eternal Stalemate\"");
			TCOD_console_print_left(NULL, 0, 4, TCOD_BKGND_NONE, "The world has been the battlegrounds between good and evil for an eternity.");
			TCOD_console_print_left(NULL, 0, 5, TCOD_BKGND_NONE, "The armies of Darkness and Light wage a never-ending war just outside of the forest,");
			TCOD_console_print_left(NULL, 0, 6, TCOD_BKGND_NONE, "   which encompasses The City, a safe haven for the remaining civilized peoples.");
			TCOD_console_print_left(NULL, 0, 8, TCOD_BKGND_NONE, "The priests of your world have come to a decision - The Eternal Stalemate must be broken.");
			TCOD_console_print_left(NULL, 0, 9, TCOD_BKGND_NONE, "   ... that's where you come in.");
			switch (g_ego.m_stats.m_iJob) {
				case JOB_WARRIOR:
					TCOD_console_print_left(NULL, 0, 11, TCOD_BKGND_NONE, "A strong Warrior, able to physically endure and dish out punishment stands before the priests.");
					TCOD_console_print_left(NULL, 0, 13, TCOD_BKGND_NONE, "\"With your strength, %s, you should be able to tip the balance of this war in God's favor.\"", g_ego.m_stats.m_cName);
					TCOD_console_print_left(NULL, 0, 14, TCOD_BKGND_NONE, "\"Go forth, conquer the armies of hell, slay Satan, and end this Eternal Stalemate!\"");
					break;
			}
			TCOD_console_print_left(NULL, 0, 16, TCOD_BKGND_NONE, "As he speaks the other priests combine their efforts and cast a spell.");
			TCOD_console_print_left(NULL, 0, 17, TCOD_BKGND_NONE, "A red portal shimmers in front of you. The caverns at the entrance of hell await your arrival.");
			TCOD_console_print_left(NULL, 0, 20, TCOD_BKGND_NONE, "You nod to the priests, and step through the portal.");
			TCOD_console_print_left(NULL, 0, 22, TCOD_BKGND_NONE, "Press ENTER to continue.");
			break;
		default:
			break;
	}
	return; 
}

void scene_intro_input(TCOD_key_t tKey, char cCharCode) {
	switch (g_iSceneIntroNum) {
		case 0: /* Input name */
			if (tKey.vk == TCODK_BACKSPACE) {
				if (g_iSceneIntroInput > 0) {
					g_iSceneIntroInput--;
					g_ego.m_stats.m_cName[g_iSceneIntroInput] = '\0';
				}
			}
			else if (tKey.vk == TCODK_ENTER)
				g_iSceneIntroNum = 1;
			else {
				if (g_iSceneIntroInput < 32 && cCharCode >= 32 && cCharCode <= 126) {
					g_ego.m_stats.m_cName[g_iSceneIntroInput] = (tKey.shift == false) ? cCharCode : cCharCode - 32;
					g_iSceneIntroInput++;
				}
			}
			break;
		case 1: /* Confirm name */
			if (cCharCode == 'y') /* Yes, I typed the name correctly. */
				g_iSceneIntroNum = 2;
			if (cCharCode == 'n') { /* Am a dumb. */
				memset(&g_ego.m_stats.m_cName, 0, sizeof(g_ego.m_stats.m_cName));
				g_iSceneIntroInput = 0;
				g_iSceneIntroNum = 0;
			}
			break;
		case 2: /* Race */
			switch (cCharCode) {
				case 'a':	/* Human */
					g_ego.m_stats.m_iRace = RACE_HUMAN;
					g_iSceneIntroNum = 3;
					break;
				default:
					break;
			}
			break;
		case 3: /* Sex */
			switch (cCharCode) {
				case 'a': /* Male */
					g_ego.m_stats.m_iSex = SEX_MALE;
					g_iSceneIntroNum = 4;
					break;
				case 'b': /* Female */
					g_ego.m_stats.m_iSex = SEX_FEMALE;
					g_iSceneIntroNum = 4;
					break;
				case 'c': /* Neither */
					g_ego.m_stats.m_iSex = SEX_SEXLESS;
					g_iSceneIntroNum = 4;
					break;
				case 'd': /* Both */
					g_ego.m_stats.m_iSex = SEX_BOTH;
					g_iSceneIntroNum = 4;
					break;
				default:
					break;
			}
			break;
		case 4: /* Job */
			switch (cCharCode) {
				case 'a': /* Warrior */
					g_ego.m_stats.m_iJob = JOB_WARRIOR;
					g_ego.m_stats.m_iLevel = 1;
					g_ego.m_stats.m_iHP = 30;
					g_ego.m_stats.m_iMaxHP = 30;
					g_ego.m_stats.m_iSP = 5;
					g_ego.m_stats.m_iMaxSP = 5;
					g_ego.m_stats.m_iStr = 10;
					g_ego.m_stats.m_iDex = 5;
					g_ego.m_stats.m_iAgl = 3;
					g_ego.m_stats.m_iCha = 3;
					g_ego.m_stats.m_iWis = 3;
					g_ego.m_stats.m_iInt = 3;
					g_ego.m_stats.m_iAtk = 5;
					g_ego.m_stats.m_iDef = 5;
					g_iSceneIntroNum = 5;
					break;
				default:
					break;
			}
			break;
		case 5: /* Gogogo */
			if (tKey.vk == TCODK_ENTER || tKey.vk == TCODK_KPENTER) {
				g_iGameMode = MODE_GAME;
				TCOD_console_clear(NULL);
				consoleAdd("The portal closes.");
			}
			break;
		default:
			break;
	}
	return;
}
