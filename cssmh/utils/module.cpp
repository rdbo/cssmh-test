#include "module.hpp"
#include <regex>
#include <dlfcn.h>
#include <link.h>

struct dl_iterate_info_t {
	void *modbase;
	std::regex regex;
};

int DlIterateCallback(dl_phdr_info *info, size_t size, void *data)
{
	dl_iterate_info_t *dl_info = (dl_iterate_info_t *)data;

	if (std::regex_match(info->dlpi_name, dl_info->regex)) {
		dl_info->modbase = (void *)(info->dlpi_addr + info->dlpi_phdr[0].p_vaddr);
		return 1;
	}

	return 0;
}

void *GetModuleBase(const char *modname)
{
	dl_iterate_info_t dl_info = {
		.modbase = NULL,
		.regex = std::regex(std::string(".*") + modname) // check if ends with 'modname'
	};

	dl_iterate_phdr(DlIterateCallback, (void *)&dl_info);

	return dl_info.modbase;
}

void *GetModuleHandle(void *addr)
{
	void *handle = NULL;
	Dl_info info;

	if (!dladdr(addr, &info))
		return handle;
	
	handle = dlopen(info.dli_fname, RTLD_NOLOAD | RTLD_NOW);
	dlclose(handle);
	
	return handle;
}

void *GetSymbol(void *handle, const char *symbol)
{
	return dlsym(handle, symbol);
}
