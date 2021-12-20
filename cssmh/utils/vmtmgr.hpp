#ifndef VMTMGR_HPP
#define VMTMGR_HPP

#include <iostream>
#include <vector>

class VMTMgr {
private:
	void **vmt;
	std::vector<void *> orig_vmt;
	size_t length;
public:
	VMTMgr(void **vmt);
	~VMTMgr();
	void *Address();
	size_t Length();
	void *GetFunction(size_t index);
	void *GetOriginal(size_t index);
	void Hook(size_t index, void *func);
	void Restore(size_t index);
	void RestoreAll();
};

#endif
