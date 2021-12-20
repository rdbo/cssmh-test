#ifndef CSSMH_HPP
#define CSSMH_HPP

#include "hl2sdk/hl2sdk.hpp"

typedef void (*CreateMoveFn)(IBaseClientDLL *thisptr, int sequence_number, float input_sample_frametime, bool active);

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
		extern CreateMoveFn fnCreateMove;
	}

	namespace Hacks {
		void Bunnyhop(CUserCmd *cmd);
	}

	namespace Hooks {
		void CreateMove(IBaseClientDLL *thisptr, int sequence_number, float input_sample_frametime, bool active);
	}
}

#endif
