#include "vmtmgr.hpp"
#include "memory.hpp"

VMTMgr::VMTMgr(void **vmt)
{
	this->vmt = vmt;
	this->length = 0;

	do {
		ProtectMemory(this->vmt, (this->length + 1) * sizeof(this->vmt[0]), PROT_EXEC | PROT_READ | PROT_WRITE);
	} while (this->vmt[this->length++]);

	this->orig_vmt.resize(this->length);
	for (size_t i = 0; i < this->length; ++i)
		this->orig_vmt[i] = this->vmt[i];
}

VMTMgr::~VMTMgr()
{
	this->RestoreAll();
	// TODO: Restore VMT original protection
}

void *VMTMgr::Address()
{
	return this->vmt;
}

size_t VMTMgr::Length()
{
	return this->length;
}

void *VMTMgr::GetFunction(size_t index)
{
	return this->vmt[index];
}

void *VMTMgr::GetOriginal(size_t index)
{
	if (index >= this->length)
		return NULL;
	
	return this->orig_vmt[index];
}

void VMTMgr::Hook(size_t index, void *func)
{
	this->vmt[index] = func;
}

void VMTMgr::Restore(size_t index)
{
	this->vmt[index] = this->orig_vmt[index];
}

void VMTMgr::RestoreAll()
{
	for (size_t i = 0; i < this->length; ++i)
		this->Restore(i);
}
