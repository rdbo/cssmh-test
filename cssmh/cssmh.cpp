#include "cssmh.hpp"
#include <iostream>
#include <fstream>
#include <pthread.h>
#include "utils/memory.hpp"
#include "utils/module.hpp"

#define CREATEMOVE_INDEX 21

IClientEntityList *CSSMH::Data::EntityList;
CBasePlayer *CSSMH::Data::LocalPlayer;
ILauncherMgr *CSSMH::Data::LauncherMgr;
IVEngineClient *CSSMH::Data::EngineClient;
ICvar *CSSMH::Data::EngineCvar;
IBaseClientDLL *CSSMH::Data::BaseClientDLL;
IClientMode *CSSMH::Data::ClientMode;
CInput *CSSMH::Data::Input;
CreateMoveFn CSSMH::Data::fnCreateMove;

void CSSMH::Init()
{
	std::system("zenity --info --title=\"[CSSMH]\" --text=\"Injected\"");
	freopen("/tmp/cssmh.txt", "w", stdout);
	CreateInterfaceFn fnCreateInterface;

	// client.so
	void *ClientBase = GetModuleBase("cstrike/bin/client.so");
	std::cout << "[*] Client Base: " << ClientBase << std::endl;

	void *ClientHandle = GetModuleHandle(ClientBase);
	std::cout << "[*] Client Handle: " << ClientHandle << std::endl;

	fnCreateInterface = (CreateInterfaceFn)GetSymbol(ClientHandle, CREATEINTERFACE_PROCNAME);
	std::cout << "[*] Client CreateInterface: " << (void *)fnCreateInterface << std::endl;

	CSSMH::Data::EntityList = (IClientEntityList *)fnCreateInterface(VCLIENTENTITYLIST_INTERFACE_VERSION, NULL);
	std::cout << "[*] Entity List: " << (void *)CSSMH::Data::EntityList << std::endl;

	CSSMH::Data::BaseClientDLL = (IBaseClientDLL *)fnCreateInterface(CLIENT_DLL_INTERFACE_VERSION, NULL);
	std::cout << "[*] Base Client DLL: " << (void *)CSSMH::Data::BaseClientDLL << std::endl;
	std::cout << "[*] Screen Width: " << CSSMH::Data::BaseClientDLL->GetScreenWidth() << std::endl;
	std::cout << "[*] Screen Height: " << CSSMH::Data::BaseClientDLL->GetScreenHeight() << std::endl;

	/* Get 'g_pClientMode' from CHLClient::HudProcessInput
	 * HudProcessInput:
	 *   mov 0xed94ff04, %eax ; pointer to 'g_pClientMode'
	 *   ...
	 * Offset: 1
	 */

	CSSMH::Data::ClientMode = **(IClientMode ***)(
		&((char *)((*(void ***)CSSMH::Data::BaseClientDLL)[10]))[1]
	);
	std::cout << "[*] Client Mode: " << (void *)CSSMH::Data::ClientMode << std::endl;

	/* Get 'input' from CHLClient::IN_ActivateMouse
	 * IN_ActivateMouse:
	 *   mov 0xed9455fc,%eax ; pointer to 'input'
	 *   ...
	 * Offset: 1
	 */

	CSSMH::Data::Input = **(CInput ***)(
		&((char *)((*(void ***)CSSMH::Data::BaseClientDLL)[14]))[1]
	);
	std::cout << "[*] Input: " << (void *)CSSMH::Data::Input << std::endl;

	// engine.so
	void *EngineBase = GetModuleBase("bin/engine.so");
	std::cout << "[*] Engine Base: " << EngineBase << std::endl;

	void *EngineHandle = GetModuleHandle(EngineBase);
	std::cout << "[*] Engine Handle: " << EngineHandle << std::endl;

	fnCreateInterface = (CreateInterfaceFn)GetSymbol(EngineHandle, CREATEINTERFACE_PROCNAME);
	std::cout << "[*] Engine CreateInterface: " << (void *)fnCreateInterface << std::endl;

	CSSMH::Data::EngineClient = (IVEngineClient *)fnCreateInterface(VENGINE_CLIENT_INTERFACE_VERSION, NULL);
	std::cout << "[*] Engine Client: " << (void *)CSSMH::Data::EngineClient << std::endl;

	// materialsystem.so
	void *MatSysBase = GetModuleBase("bin/materialsystem.so");
	std::cout << "[*] MatSys Base: " << MatSysBase << std::endl;

	void *MatSysHandle = GetModuleHandle(MatSysBase);
	std::cout << "[*] MatSys Handle: " << MatSysHandle << std::endl;

	fnCreateInterface = (CreateInterfaceFn)GetSymbol(MatSysHandle, CREATEINTERFACE_PROCNAME);
	std::cout << "[*] MatSys CreateInterface: " << (void *)fnCreateInterface << std::endl;

	CSSMH::Data::EngineCvar = (ICvar *)fnCreateInterface(CVAR_INTERFACE_VERSION, NULL);
	std::cout << "[*] Engine Cvar: " << (void *)CSSMH::Data::EngineCvar << std::endl;

	// Hooks
	void **BaseClientDLL_VMT = *(void ***)CSSMH::Data::BaseClientDLL;
	std::cout << "[*] BaseClientDLL VMT: " << BaseClientDLL_VMT << std::endl;
	ProtectMemory(&BaseClientDLL_VMT[CREATEMOVE_INDEX], sizeof(BaseClientDLL_VMT[CREATEMOVE_INDEX]), PROT_EXEC | PROT_READ | PROT_WRITE);
	CSSMH::Data::fnCreateMove = (CreateMoveFn)BaseClientDLL_VMT[CREATEMOVE_INDEX];
	std::cout << "[*] BaseClientDLL CreateMove: " << (void *)CSSMH::Data::fnCreateMove << std::endl;
	BaseClientDLL_VMT[CREATEMOVE_INDEX] = (void *)CSSMH::Hooks::CreateMove;
	// TODO: Restore previous protection flags

	// ---
	Color col = Color(255, 0, 0, 255);
	CSSMH::Data::EngineCvar->ConsoleColorPrintf(col, "[CSSMH] Loaded\n");
}

void CSSMH::Shutdown()
{
	// TODO: Restore hooks
	std::system("zenity --info --title=\"[CSSMH]\" --text=\"Ejected\"");
}
