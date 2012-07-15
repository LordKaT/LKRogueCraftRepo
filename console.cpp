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

void consoleInputAdd(char cChar) {
	if (g_iConsoleInputCount < 32) {
		g_cConsoleInput[g_iConsoleInputCount] = cChar;
		g_iConsoleInputCount++;
	}
	return;
}

void consoleInputDel() {
	if (g_iConsoleInputCount > 0) {
		g_cConsoleInput[g_iConsoleInputCount] = '\0';
		g_iConsoleInputCount--;
	}
	return;
}

void consoleInputClear() {
	memset(&g_cConsoleInput, 0, sizeof(g_cConsoleInput));
	g_iConsoleInputCount = 0;
	for (int x = 76; x < 99; x++)
		TCOD_console_put_char(NULL, x, 73, ' ', TCOD_BKGND_NONE);
	return;
}

void consoleInputDraw() {
	for (int x = 76; x < 99; x++)
		TCOD_console_put_char(NULL, x, 73, ' ', TCOD_BKGND_NONE);

	TCOD_console_print_left_rect(NULL, 76, 73, 23, 1, TCOD_BKGND_NONE, g_cConsoleInput);
	return;
}

void consoleAdd(char *cText, ...) {
	char cBuffer[1024];
	for (int i = 30; i >= 0; i--)
		if (g_cConsole[i] != NULL)
			strcpy(g_cConsole[i+1], g_cConsole[i]);

	va_list vArgs;
	va_start(vArgs, cText);
	vsprintf(cBuffer, cText, vArgs);
	va_end(vArgs);

	strcpy(g_cConsole[0], cBuffer);

	return;
}

void consoleDraw() {
	TCOD_console_put_char(NULL, 75, 44, 204, TCOD_BKGND_NONE);
	for (int x = 76; x < 99; x++)
		TCOD_console_put_char(NULL, x, 44, 205, TCOD_BKGND_NONE);
	TCOD_console_put_char(NULL, 99, 44, 185, TCOD_BKGND_NONE);
	TCOD_console_print_left(NULL, 78, 44, TCOD_BKGND_NONE, "Console");

	for (int x = 76; x < 99; x++)
		for (int y = 45; y < 73; y++)
			TCOD_console_put_char(NULL, x, y, ' ', TCOD_BKGND_NONE);

	int iCount = 0;
	int iHeight = 0;

	for (int i = 0; i < 32; i++) {
		if (g_cConsole[i] != NULL) {
			iCount += iHeight = TCOD_console_height_left_rect(NULL, 76, 45, 23, 29, g_cConsole[i]);
			if (iCount < 29)
				TCOD_console_print_left_rect(NULL, 76, 73-iCount, 23, iHeight, TCOD_BKGND_NONE, g_cConsole[i]);
		}
	}
	return;
}
