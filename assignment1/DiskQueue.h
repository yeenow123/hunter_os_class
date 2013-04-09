#include "PCBQueue.h"
#include <string>
#ifndef DISKQUEUE_H
#define DISKQUEUE_H
class DiskQueue: public PCBQueue {
	private:
		int cylinders;

	public:
		DiskQueue(string);
	    void setCylinders(int);
};

#endif
