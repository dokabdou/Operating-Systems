#ifdef CHANGED

#include "pageprovider.h"
#include "system.h"

PageProvider::PageProvider(int numPages) {
	this->numPage = numPage;
	pageMap = new BitMap(numPages);
	lockPageMap = new Lock("lockPageMap");
}

PageProvider::~PageProvider() {
	delete pageMap;
	delete lockPageMap;
}

int PageProvider::GetEmptyPage() {
	lockPageMap->Acquire();
	int numPage = pageMap->Find();
	lockPageMap->Release();
	memset(machine->mainMemory + numPage * machine->currentPageTableSize, 0, machine->currentPageTableSize);
	return numPage;
}

void PageProvider::ReleasePage(int numPage) {
	lockPageMap->Acquire();
	pageMap->Clear(numPage);
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