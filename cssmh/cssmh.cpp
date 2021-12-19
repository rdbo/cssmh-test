#include "cssmh.hpp"
#include <iostream>
#include <regex>
#include <fstream>
#include <pthread.h>
#include <dlfcn.h>
#include <link.h>
#include <sys/mman.h>

IClientEntityList *CSSMH::Data::EntityList;
CBasePlayer *CSSMH::Data::LocalPlayer;
ILauncherMgr *CSSMH::Data::LauncherMgr;
IVEngineClient *CSSMH::Data::EngineClient;
ICvar *CSSMH::Data::EngineCvar;
IBaseClientDLL *CSSMH::Data::BaseClientDLL;
IClientMode *CSSMH::Data::ClientMode;
CInput *CSSMH::Data::Input;

static void *GetModuleBase(const char *modname);
static void *GetModuleHandle(void *addr);
static void *GetSymbol(void *handle, const char *symbol);

typedef void (*CreateMoveFn)(IBaseClientDLL *thisptr, int sequence_number, float input_sample_frametime, bool active);

static CreateMoveFn oCreateMove;

void hkCreateMove(IBaseClientDLL *thisptr, int sequence_number, float input_sample_frametime, bool active)
{
	CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] CreateMove Start\n");
	oCreateMove(thisptr, sequence_number, input_sample_frametime, active);

	std::wstring mapname = std::wstring(CSSMH::Data::ClientMode->GetMapName());
	CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] Map Name: %s\n", std::string(mapname.begin(), mapname.end()).c_str());
	CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] Input Sample Frametime: %f\n", input_sample_frametime);
	CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] Sequence Number: %d\n", sequence_number);

	CBasePlayer *LocalPlayer = (CBasePlayer *)CSSMH::Data::EntityList->GetClientEntity(CSSMH::Data::EngineClient->GetLocalPlayer());
	if (!LocalPlayer)
		return;
	
	CUserCmd *cmd = CSSMH::Data::Input->GetUserCmd(sequence_number);
	if (!cmd)
		return;
	
	if ((cmd->buttons & IN_JUMP) && !(LocalPlayer->GetFlags() & FL_ONGROUND))
		cmd->buttons &= ~IN_JUMP;
	
	// Verify usercmd
	CInput::CVerifiedUserCmd *verified_cmd = (CInput::CVerifiedUserCmd *)&CSSMH::Data::Input->m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
	verified_cmd->m_cmd = *cmd;
	verified_cmd->m_crc = cmd->GetChecksum();

	CSSMH::Data::EngineCvar->ConsolePrintf("[CSSMH] CreateMove End\n");
	return;
}

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

	// ---

	Color col = Color(255, 0, 0, 255);
	CSSMH::Data::EngineCvar->ConsoleColorPrintf(col, "[CSSMH] Loaded\n");

	// CreateMove test hook
	void **BaseClientDLL_VMT = *(void ***)CSSMH::Data::BaseClientDLL;
	std::cout << "[*] BaseClientDLL VMT: " << BaseClientDLL_VMT << std::endl;
	mprotect((void *)((uintptr_t)BaseClientDLL_VMT & -sysconf(_SC_PAGE_SIZE)), (size_t)sysconf(_SC_PAGE_SIZE), PROT_EXEC | PROT_READ | PROT_WRITE); // bad but works
	oCreateMove = (CreateMoveFn)BaseClientDLL_VMT[21];
	std::cout << "[*] BaseClientDLL CreateMove: " << (void *)oCreateMove << std::endl;
	BaseClientDLL_VMT[21] = (void *)hkCreateMove;
}

void CSSMH::Shutdown()
{
	std::system("zenity --info --title=\"[CSSMH]\" --text=\"Ejected\"");
}

struct dl_iterate_info_t {
	void *modbase;
	std::regex regex;
};

static int DlIterateCallback(dl_phdr_info *info, size_t size, void *data)
{
	dl_iterate_info_t *dl_info = (dl_iterate_info_t *)data;

	if (std::regex_match(info->dlpi_name, dl_info->regex)) {
		dl_info->modbase = (void *)(info->dlpi_addr + info->dlpi_phdr[0].p_vaddr);
		return 1;
	}

	return 0;
}

static void *GetModuleBase(const char *modname)
{
	dl_iterate_info_t dl_info = {
		.modbase = NULL,
		.regex = std::regex(std::string(".*") + modname) // check if ends with 'modname'
	};

	dl_iterate_phdr(DlIterateCallback, (void *)&dl_info);

	return dl_info.modbase;
}

static void *GetModuleHandle(void *addr)
{
	void *handle = NULL;
	Dl_info info;

	if (!dladdr(addr, &info))
		return handle;
	
	handle = dlopen(info.dli_fname, RTLD_NOLOAD | RTLD_NOW);
	dlclose(handle);
	
	return handle;
}

static void *GetSymbol(void *handle, const char *symbol)
{
	return dlsym(handle, symbol);
}
