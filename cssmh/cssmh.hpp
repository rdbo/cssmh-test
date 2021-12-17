#ifndef CSSMH_HPP
#define CSSMH_HPP

#include "hl2sdk/hl2sdk.h"

namespace CSSMH {
	void Init();
	void Shutdown();

	namespace Data {
		extern ILauncherMgr *LauncherMgr;
		extern IClientEntityList *EntityList;
		extern CBasePlayer *LocalPlayer;
		extern IVEngineClient *EngineClient;
		extern ICvar *EngineCvar;
	}

	namespace Hacks {

	}

	namespace Hooks {

	}
}

#endif
