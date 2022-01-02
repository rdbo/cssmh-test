#include <cssmh.hpp>

void CSSMH::Hacks::AutoStrafe(CUserCmd *cmd)
{
	static ConVar *sidespeed = CSSMH::Data::EngineCvar->FindVar("cl_sidespeed");
	static ConVar *forwardspeed = CSSMH::Data::EngineCvar->FindVar("cl_forwardspeed");
	CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] SideSpeed: %f\n", sidespeed->GetFloat());

	auto vel = CSSMH::Data::LocalPlayer->GetLocalVelocity();

	if ((cmd->buttons & IN_JUMP) && !(CSSMH::Data::LocalPlayer->GetFlags() & FL_ONGROUND) && CSSMH::Data::LocalPlayer->GetMoveType() != MoveType_t::MOVETYPE_NOCLIP) {
		CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] AutoStrafing\n");
		CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] Mouse Delta X: %f\n", cmd->mousedx);

		if (cmd->mousedx > 0 || ((cmd->buttons & IN_RIGHT) && !(cmd->buttons & IN_LEFT))) {
			cmd->sidemove = sidespeed->GetFloat();
		} else if (cmd->mousedx < 0 || ((cmd->buttons & IN_LEFT) && !(cmd->buttons & IN_RIGHT))) {
			cmd->sidemove = -sidespeed->GetFloat();
		} else if (vel.Length() <= 50.0f) {
			cmd->forwardmove = forwardspeed->GetFloat();
		} else {
			const float val = 30.0f;
			float yaw_offset = val * fabsf(val / vel.Length());
			if (cmd->command_number % 2 == 0) {
				cmd->viewangles.y += yaw_offset;
				cmd->sidemove = sidespeed->GetFloat();
			} else {
				cmd->viewangles.y -= yaw_offset;
				cmd->sidemove = -sidespeed->GetFloat();
			}
		}

		if (cmd->buttons & IN_ATTACK)
			CSSMH::Data::EngineClient->GetViewAngles(cmd->viewangles);
	}
}
