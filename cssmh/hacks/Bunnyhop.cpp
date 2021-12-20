#include <cssmh.hpp>

void CSSMH::Hacks::Bunnyhop(CUserCmd *cmd)
{
	if ((cmd->buttons & IN_JUMP) && !(CSSMH::Data::LocalPlayer->GetFlags() & FL_ONGROUND) && CSSMH::Data::LocalPlayer->GetMoveType() != MoveType_t::MOVETYPE_LADDER)
		cmd->buttons &= ~IN_JUMP;
}
