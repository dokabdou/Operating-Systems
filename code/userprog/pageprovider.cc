#ifdef CHANGED

#include "pageprovider.h"
#include "machine.h"
#include "system.h"

PageProvider::PageProvider() {
	this->numPage = NumPhysPages;
	this->bookedPage = 0;
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
	bookedPage--;
	lockPageMap->Release();
	if (pageAvailable == -1)
		return -1;  // shouldn't happen because every process has to book pages before acquiring them
	pageAvailable = (numPage - 1) - pageAvailable;
	memset(machine->mainMemory + pageAvailable * machine->currentPageTableSize, 0, machine->currentPageTableSize);
	return pageAvailable;
}

bool PageProvider::bookPages(int howManyPages) {
	lockPageMap->Acquire();
	if (this->NumAvailPage() >= howManyPages + bookedPage) {
		bookedPage += howManyPages;
		lockPageMap->Release();
		return true;
	}
	lockPageMap->Release();
	return false;
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