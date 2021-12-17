#include "cssmh.hpp"

void __attribute__((constructor)) cssmh_entry()
{
	CSSMH::Init();
}

void __attribute__((destructor)) cssmh_exit()
{
	CSSMH::Shutdown();
}

