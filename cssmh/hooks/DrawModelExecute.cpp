#include <cssmh.hpp>

typedef void (*DrawModelExecuteFn)(IVModelRender *thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);

void CSSMH::Hooks::DrawModelExecute(IVModelRender *thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	static DrawModelExecuteFn fnDrawModelExecute = (DrawModelExecuteFn)CSSMH::Data::ModelRender_VMT->GetOriginal(19);

	/*
	// Dump materials
	static bool init = false;
	static MaterialHandle_t hmat;
	if (!init) {
		hmat = CSSMH::Data::MaterialSystem->FirstMaterial();
		do {
			IMaterial *mat = CSSMH::Data::MaterialSystem->GetMaterial(hmat);
			std::cout << "[*] Material: " << mat->GetName() << std::endl;
		} while ((hmat = CSSMH::Data::MaterialSystem->NextMaterial(hmat)) != INVALID_MATERIAL_HANDLE);

		init = true;
	}
	*/

	if (pInfo.pModel && CSSMH::Data::LocalPlayer && CSSMH::Data::LocalPlayer->GetTeamNumber() > 1) {
		const char *modelName = CSSMH::Data::ModelInfoClient->GetModelName(pInfo.pModel);
		if (strstr(modelName, "models/player")) {
			CBasePlayer *modelEnt = (CBasePlayer *)CSSMH::Data::EntityList->GetClientEntity(pInfo.entity_index);
			if (modelEnt && !modelEnt->IsDormant() && modelEnt->IsPlayer() && modelEnt->IsAlive() && modelEnt->GetHealth() > 0) {
				IMaterial *mat = CSSMH::Data::MaterialSystem->FindMaterial("models/gibs/metalgibs/metal_gibs", NULL);
				
				// Invisible Mat
				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
				if (modelEnt->GetTeamNumber() != CSSMH::Data::LocalPlayer->GetTeamNumber()) {
					mat->ColorModulate(1.0f, 0.0f, 0.0f);
					mat->AlphaModulate(1.0f);
					modelEnt->m_clrRender.SetR(255);
					modelEnt->m_clrRender.SetG(0);
					modelEnt->m_clrRender.SetB(0);
					modelEnt->m_clrRender.SetA(255);
				} else {
					mat->ColorModulate(0.0f, 1.0f, 0.0f);
					mat->AlphaModulate(1.0f);
					modelEnt->m_clrRender.SetR(0);
					modelEnt->m_clrRender.SetG(255);
					modelEnt->m_clrRender.SetB(0);
					modelEnt->m_clrRender.SetA(255);
				}

				CSSMH::Data::ModelRender->ForcedMaterialOverride(mat);
				fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);

				// Visible Mat
				mat = CSSMH::Data::MaterialSystem->FindMaterial("debug/debugambientcube", NULL);
				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				if (modelEnt->GetTeamNumber() != CSSMH::Data::LocalPlayer->GetTeamNumber()) {
					mat->ColorModulate(1.0f, 0.0f, 0.0f);
					mat->AlphaModulate(1.0f);
				} else {
					mat->ColorModulate(0.0f, 1.0f, 0.0f);
					mat->AlphaModulate(1.0f);
				}

				CSSMH::Data::ModelRender->ForcedMaterialOverride(mat);
				fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);

				CSSMH::Data::ModelRender->ForcedMaterialOverride(nullptr);
				return;
			}
		}

		else if (strstr(modelName, "models/weapons")) {
			IMaterial *mat = CSSMH::Data::MaterialSystem->FindMaterial("models/gibs/glass/glass", NULL);
			
			mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
			//mat->SetMaterialVarFlag(MaterialVarFlags_t::MATERIAL_VAR_WIREFRAME, true);
			mat->ColorModulate(1.0f, 0.0f, 0.0f);
			mat->AlphaModulate(1.0f);
			CSSMH::Data::ModelRender->ForcedMaterialOverride(mat);
			fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);
			CSSMH::Data::ModelRender->ForcedMaterialOverride(nullptr);
			return;
		}
	}

	return fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);
}
