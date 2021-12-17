#include "cssmh.hpp"
#include <iostream>
#include <regex>
#include <fstream>
#include <pthread.h>
#include <dlfcn.h>
#include <link.h>

IClientEntityList *CSSMH::Data::EntityList;
CBasePlayer *CSSMH::Data::LocalPlayer;
ILauncherMgr *CSSMH::Data::LauncherMgr;
IVEngineClient *CSSMH::Data::EngineClient;
ICvar *CSSMH::Data::EngineCvar;

static void *GetModuleBase(const char *modname);
static void *GetModuleHandle(void *addr);
static void *GetSymbol(void *handle, const char *symbol);

void CSSMH::Init()
{
	std::system("zenity --info --title=\"[CSSMH]\" --text=\"Injected\"");
	std::ofstream log_file = std::ofstream("/tmp/cssmh.txt");
	CreateInterfaceFn fnCreateInterface;

	// client.so
	void *ClientBase = GetModuleBase("cstrike/bin/client.so");
	log_file << "[*] Client Base: " << ClientBase << std::endl;

	void *ClientHandle = GetModuleHandle(ClientBase);
	log_file << "[*] Client Handle: " << ClientHandle << std::endl;

	fnCreateInterface = (CreateInterfaceFn)GetSymbol(ClientHandle, CREATEINTERFACE_PROCNAME);
	log_file << "[*] Client CreateInterface: " << (void *)fnCreateInterface << std::endl;

	CSSMH::Data::EntityList = (IClientEntityList *)fnCreateInterface(VCLIENTENTITYLIST_INTERFACE_VERSION, NULL);
	log_file << "[*] Entity List: " << (void *)CSSMH::Data::EntityList << std::endl;

	// engine.so
	void *EngineBase = GetModuleBase("bin/engine.so");
	log_file << "[*] Engine Base: " << EngineBase << std::endl;

	void *EngineHandle = GetModuleHandle(EngineBase);
	log_file << "[*] Engine Handle: " << EngineHandle << std::endl;

	fnCreateInterface = (CreateInterfaceFn)GetSymbol(EngineHandle, CREATEINTERFACE_PROCNAME);
	log_file << "[*] Engine CreateInterface: " << (void *)fnCreateInterface << std::endl;

	CSSMH::Data::EngineClient = (IVEngineClient *)fnCreateInterface(VENGINE_CLIENT_INTERFACE_VERSION, NULL);
	log_file << "[*] Engine Client: " << (void *)CSSMH::Data::EngineClient << std::endl;

	// materialsystem.so

	void *MatSysBase = GetModuleBase("bin/materialsystem.so");
	log_file << "[*] MatSys Base: " << MatSysBase << std::endl;

	void *MatSysHandle = GetModuleHandle(MatSysBase);
	log_file << "[*] MatSys Handle: " << MatSysHandle << std::endl;

	fnCreateInterface = (CreateInterfaceFn)GetSymbol(MatSysHandle, CREATEINTERFACE_PROCNAME);
	log_file << "[*] MatSys CreateInterface: " << (void *)fnCreateInterface << std::endl;

	CSSMH::Data::EngineCvar = (ICvar *)fnCreateInterface(CVAR_INTERFACE_VERSION, NULL);
	log_file << "[*] Engine Cvar: " << (void *)CSSMH::Data::EngineCvar << std::endl;

	// ---

	Color col = Color(255, 0, 0, 255);
	CSSMH::Data::EngineCvar->ConsoleColorPrintf(col, "[CSSMH] Loaded");

	log_file.close();
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
