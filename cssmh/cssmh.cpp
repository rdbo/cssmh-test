#include "cssmh.hpp"
#include <iostream>
#include <regex>
#include <fstream>
#include <pthread.h>
#include <dlfcn.h>
#include <link.h>

IClientEntityList *CSSMH::Data::EntityList;
CBasePlayer *CSSMH::Data::LocalPlayer;

static void *GetModuleBase(const char *modname);
static void *GetModuleHandle(void *addr);
static void *GetSymbol(void *handle, const char *symbol);

static void *InitThread(void *arg)
{
	RTLD_LOCAL;
	std::ofstream log_file = std::ofstream("/tmp/cssmh.txt");

	log_file << "[CSSMH]" << std::endl;

	while (true) {
		CBasePlayer *player = (CBasePlayer *)CSSMH::Data::EntityList->GetClientEntity(1);
		log_file << "[*] Player Address: " << (void *)player << std::endl;
		if (player) {
			log_file << "[*] Player Health: " << player->GetHealth() << std::endl;
		}
		sleep(5);
	}

	return arg;
}

void CSSMH::Init()
{
	std::system("zenity --info --title=\"[CSSMH]\" --text=\"Injected\"");
	CreateInterfaceFn fnCreateInterface;

	// client.so
	void *ClientBase = GetModuleBase("/cstrike/bin/client.so");
	void *ClientHandle = GetModuleHandle(ClientBase);
	fnCreateInterface = (CreateInterfaceFn)GetSymbol(ClientHandle, "CreateInterface");
	CSSMH::Data::EntityList = (IClientEntityList *)fnCreateInterface(VCLIENTENTITYLIST_INTERFACE_VERSION, NULL);

	pthread_t thread;
	pthread_create(&thread, NULL, InitThread, NULL);
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
