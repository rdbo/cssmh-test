#include <cssmh.hpp>

typedef void (*DrawModelExecuteFn)(IVModelRender *thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);

void CSSMH::Hooks::DrawModelExecute(IVModelRender *thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	static DrawModelExecuteFn fnDrawModelExecute = (DrawModelExecuteFn)CSSMH::Data::ModelRender_VMT->GetOriginal(19);
	static IMaterial *mat_invisible;
	static IMaterial *mat_visible;

	if (pInfo.pModel && CSSMH::Data::LocalPlayer && CSSMH::Data::LocalPlayer->GetTeamNumber() > 1) {
		const char *modelName = CSSMH::Data::ModelInfoClient->GetModelName(pInfo.pModel);
		if (strstr(modelName, "models/player")) {
			CBasePlayer *modelEnt = (CBasePlayer *)CSSMH::Data::EntityList->GetClientEntity(pInfo.entity_index);
			if (modelEnt && !modelEnt->IsDormant() && modelEnt->IsPlayer() && modelEnt->IsAlive() && modelEnt->GetHealth() > 0) {
				// Invisible Mat
				IMaterial *mat = CSSMH::Data::MaterialSystem->FindMaterial("debug/debugambientcube", NULL);
				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
				if (modelEnt->GetTeamNumber() != CSSMH::Data::LocalPlayer->GetTeamNumber()) {
					float col[] = { 0.5f, 0.0f, 1.0f, 1.0f };
					CSSMH::Data::RenderView->SetColorModulation(col);
				} else {
					float col[] = { 0.0f, 0.5f, 1.0f, 1.0f };
					CSSMH::Data::RenderView->SetColorModulation(col);
				}

				CSSMH::Data::ModelRender->ForcedMaterialOverride(mat);
				fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);

				// Visible Mat
				mat = CSSMH::Data::MaterialSystem->FindMaterial("debug/debugambientcube", NULL);
				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				if (modelEnt->GetTeamNumber() != CSSMH::Data::LocalPlayer->GetTeamNumber()) {
					float col[] = { 1.0f, 0.0f, 0.0f, 1.0f };
					CSSMH::Data::RenderView->SetColorModulation(col);
				} else {
					float col[] = { 0.0f, 1.0f, 0.0f, 1.0f };
					CSSMH::Data::RenderView->SetColorModulation(col);
				}

				CSSMH::Data::ModelRender->ForcedMaterialOverride(mat);
				fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);

				CSSMH::Data::ModelRender->ForcedMaterialOverride(nullptr);
				return;
			}
		} else if (strstr(modelName, "models/weapons")) {
			IMaterial *mat = CSSMH::Data::MaterialSystem->FindMaterial("models/gibs/glass/glass", NULL);
			mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
			mat->FindVar("$envmaptint", NULL)->SetVecValue(0.3f, 0.85f, 1.0f);
			CSSMH::Data::ModelRender->ForcedMaterialOverride(mat);
			fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);
			
			CSSMH::Data::ModelRender->ForcedMaterialOverride(nullptr);
			return;
		}
	}

	return fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);
}
