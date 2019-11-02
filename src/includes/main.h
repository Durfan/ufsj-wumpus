#ifndef INCLUDE_MAIN_H
#define INCLUDE_MAIN_H

#define _GNU_SOURCE
#define PROGRAM "wumpus"
#define MAJORV 0
#define MINORV 1

#define QUAD 16 // World Size
#define WROW 4
#define WCOL 4
#define START 0

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <errno.h>
#include <raylib.h>

#include "linkedlst.h"
#include "structs.h"
#include "sensor.h"
#include "util.h"
#include "world.h"
#include "agent.h"

//#pragma message __FILE__
#endif // INCLUDE_MAIN_H