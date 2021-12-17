#ifndef HL2SDK_H
#define HL2SDK_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#ifndef GNUC
#define GNUC 1
#endif

#ifndef _LINUX
#define _LINUX 1
#endif

#ifndef POSIX
#define POSIX 1
#endif

#ifndef CLIENT_DLL
#define CLIENT_DLL
#endif

#define UNDEF_CALL { throw std::runtime_error("Undefined function called"); }

#include <stdexcept>
#include "public/tier0/platform.h"
#include "public/tier0/basetypes.h"
#include "public/tier1/interface.h"
#include "public/const.h"
#include "public/networkvar.h"
#include "public/icliententity.h"
#include "public/icliententitylist.h"
#include "game/shared/sheetsimulator.h"
#include "game/client/cbase.h"
#include "game/client/c_baseentity.h"
#include "game/client/c_baseplayer.h"

#endif
