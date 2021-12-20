#ifndef MODULE_HPP
#define MODULE_HPP

void *GetModuleBase(const char *modname);
void *GetModuleHandle(void *addr);
void *GetSymbol(void *handle, const char *symbol);

#endif
