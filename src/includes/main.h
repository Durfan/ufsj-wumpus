#ifndef INCLUDE_MAIN_H
#define INCLUDE_MAIN_H

#define _GNU_SOURCE
#define PROGRAM "wumpus"
#define MAJORV 0
#define MINORV 1

#define CRED  "\x1b[31m"
#define CRSET "\x1b[0m"

#define ROOMS 16 // World Size
#define WCOLS 4
#define WROWS 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>
#include <time.h>
#include <errno.h>

typedef struct room_t {
	bool wumpus;
	bool fedor;
	bool whell;
	bool brisa;
	bool gold;
	bool reluz;
	bool limit;
} Rooms;

#include "util.h"
#include "world.h"
#include "agent.h"
#include "sensor.h"



int g_world[ROOMS][ROOMS];
int g_knowB[ROOMS][ROOMS];
Rooms g_wroom[ROOMS];

//#pragma message __FILE__
#endif // INCLUDE_MAIN_H