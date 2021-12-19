#ifndef CSSMH_HPP
#define CSSMH_HPP

#include "hl2sdk/hl2sdk.hpp"

namespace CSSMH {
	void Init();
	void Shutdown();

	namespace Data {
		extern ILauncherMgr *LauncherMgr;
		extern IClientEntityList *EntityList;
		extern CBasePlayer *LocalPlayer;
		extern IVEngineClient *EngineClient;
		extern ICvar *EngineCvar;
		extern IBaseClientDLL *BaseClientDLL;
		extern IClientMode *ClientMode;
		extern CInput *Input;
	}

	namespace Hacks {

	}

	namespace Hooks {

	}
}

#endif
