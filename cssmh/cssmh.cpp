#include "cssmh.hpp"
#include <iostream>
#include <fstream>
#include <pthread.h>
#include "utils/memory.hpp"
#include "utils/module.hpp"

ILauncherMgr *CSSMH::Data::LauncherMgr;
IClientEntityList *CSSMH::Data::EntityList;
CBasePlayer *CSSMH::Data::LocalPlayer;
IVEngineClient *CSSMH::Data::EngineClient;
ICvar *CSSMH::Data::EngineCvar;
IBaseClientDLL *CSSMH::Data::BaseClientDLL;
IClientMode *CSSMH::Data::ClientMode;
CInput *CSSMH::Data::Input;
IVModelInfoClient *CSSMH::Data::ModelInfoClient;
IVModelRender *CSSMH::Data::ModelRender;
IMaterialSystem *CSSMH::Data::MaterialSystem;
IVRenderView *CSSMH::Data::RenderView;

VMTMgr *CSSMH::Data::BaseClientDLL_VMT = NULL;
VMTMgr *CSSMH::Data::ModelRender_VMT = NULL;

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
	std::cout << "[*] EntityList: " << (void *)CSSMH::Data::EntityList << std::endl;

	CSSMH::Data::BaseClientDLL = (IBaseClientDLL *)fnCreateInterface(CLIENT_DLL_INTERFACE_VERSION, NULL);
	std::cout << "[*] BaseClientDLL: " << (void *)CSSMH::Data::BaseClientDLL << std::endl;
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
	std::cout << "[*] ClientMode: " << (void *)CSSMH::Data::ClientMode << std::endl;

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

	std::cout << "====================" << std::endl;

	// engine.so
	void *EngineBase = GetModuleBase("bin/engine.so");
	std::cout << "[*] Engine Base: " << EngineBase << std::endl;

	void *EngineHandle = GetModuleHandle(EngineBase);
	std::cout << "[*] Engine Handle: " << EngineHandle << std::endl;

	fnCreateInterface = (CreateInterfaceFn)GetSymbol(EngineHandle, CREATEINTERFACE_PROCNAME);
	std::cout << "[*] Engine CreateInterface: " << (void *)fnCreateInterface << std::endl;

	CSSMH::Data::EngineClient = (IVEngineClient *)fnCreateInterface(VENGINE_CLIENT_INTERFACE_VERSION, NULL);
	std::cout << "[*] Engine Client: " << (void *)CSSMH::Data::EngineClient << std::endl;

	CSSMH::Data::ModelInfoClient = (IVModelInfoClient *)fnCreateInterface(VMODELINFO_CLIENT_INTERFACE_VERSION, NULL);
	std::cout << "[*] ModelInfoClient: " << (void *)CSSMH::Data::ModelInfoClient << std::endl;

	CSSMH::Data::ModelRender = (IVModelRender *)fnCreateInterface(VENGINE_HUDMODEL_INTERFACE_VERSION, NULL);
	std::cout << "[*] ModelRender: " << (void *)CSSMH::Data::ModelRender << std::endl;

	CSSMH::Data::RenderView = (IVRenderView *)fnCreateInterface(VENGINE_RENDERVIEW_INTERFACE_VERSION, NULL);
	std::cout << "[*] RenderView: " << (void *)CSSMH::Data::RenderView << std::endl;

	std::cout << "====================" << std::endl;

	// materialsystem.so
	void *MatSysBase = GetModuleBase("bin/materialsystem.so");
	std::cout << "[*] MatSys Base: " << MatSysBase << std::endl;

	void *MatSysHandle = GetModuleHandle(MatSysBase);
	std::cout << "[*] MatSys Handle: " << MatSysHandle << std::endl;

	fnCreateInterface = (CreateInterfaceFn)GetSymbol(MatSysHandle, CREATEINTERFACE_PROCNAME);
	std::cout << "[*] MatSys CreateInterface: " << (void *)fnCreateInterface << std::endl;

	CSSMH::Data::EngineCvar = (ICvar *)fnCreateInterface(CVAR_INTERFACE_VERSION, NULL);
	std::cout << "[*] EngineCvar: " << (void *)CSSMH::Data::EngineCvar << std::endl;

	CSSMH::Data::MaterialSystem = (IMaterialSystem *)fnCreateInterface(MATERIAL_SYSTEM_INTERFACE_VERSION, NULL);
	std::cout << "[*] MaterialSystem: " << (void *)CSSMH::Data::MaterialSystem << std::endl;

	std::cout << "====================" << std::endl;

	// Dump materials
	MaterialHandle_t hmat;
	std::cout << "[*] Material List Dump" << std::endl;
	hmat = CSSMH::Data::MaterialSystem->FirstMaterial();
	do {
		IMaterial *mat = CSSMH::Data::MaterialSystem->GetMaterial(hmat);
		std::cout << "[*] Material: " << mat->GetName() << std::endl;
	} while ((hmat = CSSMH::Data::MaterialSystem->NextMaterial(hmat)) != CSSMH::Data::MaterialSystem->InvalidMaterial());
	std::cout << "====================" << std::endl;

	// Hooks
	CSSMH::Data::BaseClientDLL_VMT = new VMTMgr(*(void ***)CSSMH::Data::BaseClientDLL);
	std::cout << "[*] BaseClientDLL VMT: " << CSSMH::Data::BaseClientDLL_VMT->Address() << std::endl;
	std::cout << "[*] BaseClientDLL CreateMove: " << CSSMH::Data::BaseClientDLL_VMT->GetFunction(21) << std::endl;
	CSSMH::Data::BaseClientDLL_VMT->Hook(21, (void *)CSSMH::Hooks::CreateMove);

	CSSMH::Data::ModelRender_VMT = new VMTMgr(*(void ***)CSSMH::Data::ModelRender);
	std::cout << "[*] ModelRender VMT: " << CSSMH::Data::ModelRender_VMT->Address() << std::endl;
	std::cout << "[*] ModelRender DrawModelExecute: " << CSSMH::Data::ModelRender_VMT->GetFunction(19) << std::endl;
	CSSMH::Data::ModelRender_VMT->Hook(19, (void *)CSSMH::Hooks::DrawModelExecute);
	std::cout << "====================" << std::endl;

	// ---
	Color col = Color(255, 0, 0, 255);
	CSSMH::Data::EngineCvar->ConsoleColorPrintf(col, "[CSSMH] Loaded\n");
}

void CSSMH::Shutdown()
{
	/*
	// TODO: Fix this crash
	if (CSSMH::Data::BaseClientDLL_VMT)
		delete CSSMH::Data::BaseClientDLL_VMT;
	if (CSSMH::Data::ModelRender_VMT)
		delete CSSMH::Data::ModelRender_VMT;
	*/
	std::system("zenity --info --title=\"[CSSMH]\" --text=\"Ejected\"");
}
