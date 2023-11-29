#ifdef CHANGED
#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H
#include "bitmap.h"
#include "synch.h"
#include "utility.h"

class PageProvider : dontcopythis {
   public:
	PageProvider();
	~PageProvider();
	int GetEmptyPage();
	void ReleasePage(int howManyPage);
	int NumAvailPage();
	bool bookPages(int howManyPage);

   private:
	BitMap* pageMap;
	Lock* lockPageMap;
	int numPage;
	int bookedPage;
};

class NoMoreMemory {};

#endif
#endif