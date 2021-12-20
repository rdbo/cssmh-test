#include <cssmh.hpp>

void CSSMH::Hacks::AutoStrafe(CUserCmd *cmd)
{
	static ConVar *sidespeed = CSSMH::Data::EngineCvar->FindVar("cl_sidespeed");
	CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] SideSpeed: %f\n", sidespeed->GetFloat());

	if ((cmd->buttons & IN_JUMP) && !(CSSMH::Data::LocalPlayer->GetFlags() & FL_ONGROUND) && CSSMH::Data::LocalPlayer->GetMoveType() != MoveType_t::MOVETYPE_NOCLIP) {
		CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] AutoStrafing\n");
		CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] Mouse Delta X: %f\n", cmd->mousedx);

		if (cmd->mousedx > 0)
			cmd->sidemove = sidespeed->GetFloat();
		else if (cmd->mousedx < 0)
			cmd->sidemove = -sidespeed->GetFloat();
	}
}
