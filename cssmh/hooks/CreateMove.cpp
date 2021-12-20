#include <cssmh.hpp>

typedef void (*CreateMoveFn)(IBaseClientDLL *thisptr, int sequence_number, float input_sample_frametime, bool active);

void CSSMH::Hooks::CreateMove(IBaseClientDLL *thisptr, int sequence_number, float input_sample_frametime, bool active)
{
	static CreateMoveFn fnCreateMove = (CreateMoveFn)CSSMH::Data::BaseClientDLL_VMT->GetOriginal(21);
	CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] CreateMove Start\n");
	fnCreateMove(thisptr, sequence_number, input_sample_frametime, active);

	std::wstring mapname = std::wstring(CSSMH::Data::ClientMode->GetMapName());
	CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] Map Name: %s\n", std::string(mapname.begin(), mapname.end()).c_str());
	CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] Input Sample Frametime: %f\n", input_sample_frametime);
	CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] Sequence Number: %d\n", sequence_number);

	CSSMH::Data::LocalPlayer = (CBasePlayer *)CSSMH::Data::EntityList->GetClientEntity(CSSMH::Data::EngineClient->GetLocalPlayer());
	if (!CSSMH::Data::LocalPlayer)
		return;
	
	CUserCmd *cmd = CSSMH::Data::Input->GetUserCmd(sequence_number);
	if (!cmd)
		return;
	
	// Run hacks
	CSSMH::Hacks::Bunnyhop(cmd);
	
	// Verify cmd
	CInput::CVerifiedUserCmd *verified_cmd = (CInput::CVerifiedUserCmd *)&CSSMH::Data::Input->m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
	verified_cmd->m_cmd = *cmd;
	verified_cmd->m_crc = cmd->GetChecksum();

	CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] CreateMove End\n");
}
