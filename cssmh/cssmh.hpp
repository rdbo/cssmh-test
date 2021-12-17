#ifndef CSSMH_HPP
#define CSSMH_HPP

#include "hl2sdk/hl2sdk.h"

namespace CSSMH {
	void Init();
	void Shutdown();

	namespace Data {
		extern IClientEntityList *EntityList;
		extern CBasePlayer *LocalPlayer;
	}

	namespace Hacks {

	}

	namespace Hooks {

	}
}

#endif
