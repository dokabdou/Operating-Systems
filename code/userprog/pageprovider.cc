#ifdef CHANGED

#include "pageprovider.h"
#include "system.h"

PageProvider::PageProvider(int numPhysPages) {
	this->numPage = numPhysPages;
	pageMap = new BitMap(this->numPage);
	lockPageMap = new Lock("lockPageMap");
}

PageProvider::~PageProvider() {
	delete pageMap;
	delete lockPageMap;
}

int PageProvider::GetEmptyPage() {
	lockPageMap->Acquire();
	int pageAvailable = pageMap->Find();
	lockPageMap->Release();
	if (pageAvailable == -1)
		return -1;
	memset(machine->mainMemory + pageAvailable * machine->currentPageTableSize, 0, machine->currentPageTableSize);
	return pageAvailable;
}

void PageProvider::ReleasePage(int pageToRelease) {
	lockPageMap->Acquire();
	pageMap->Clear(pageToRelease);
	lockPageMap->Release();
}

int PageProvider::NumAvailPage() {
	int cpt = 0;
	for (int i = 0; i < this->numPage; i++) {
		if (!this->pageMap->Test(i)) {
			cpt++;
		}
	}
	return cpt;
}

#endif