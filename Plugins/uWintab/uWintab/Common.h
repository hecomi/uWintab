#pragma once

#include "wintab.h"
#define PACKETDATA (PK_X | PK_Y | PK_TIME | PK_BUTTONS | PK_NORMAL_PRESSURE | PK_ORIENTATION | PK_CURSOR)
#define PACKETEXPKEYS PKEXT_ABSOLUTE
#define PACKETMODE PK_BUTTONS
#include <Wintab/pktdef.h>
#include <Wintab/msgpack.h>

constexpr const char *kLibraryName = "uWintab";