#ifdef CHANGED
#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H
#include "bitmap.h"
#include "synch.h"
#include "utility.h"

class PageProvider : dontcopythis {
   public:
	PageProvider(int numPages);
	~PageProvider();
	int GetEmptyPage();
	void ReleasePage(int numPage);
	int NumAvailPage();

   private:
	BitMap* pageMap;
	Lock* lockPageMap;
	int numPage;
};

#endif
#endif