#ifndef CSSMH_HPP
#define CSSMH_HPP

#include "hl2sdk/hl2sdk.hpp"
#include "utils/vmtmgr.hpp"

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
		extern IVModelInfoClient *ModelInfoClient;
		extern IVModelRender *ModelRender;
		extern IMaterialSystem *MaterialSystem;
		extern IVRenderView *RenderView;

		extern VMTMgr *BaseClientDLL_VMT;
		extern VMTMgr *ModelRender_VMT;
	}

	namespace Hacks {
		void Bunnyhop(CUserCmd *cmd);
		void AutoStrafe(CUserCmd *cmd);
	}

	namespace Hooks {
		void CreateMove(IBaseClientDLL *thisptr, int sequence_number, float input_sample_frametime, bool active);
		void DrawModelExecute(IVModelRender *thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
	}
}

#endif
