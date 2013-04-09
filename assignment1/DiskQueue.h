#include "PCBQueue.h"
#include <string>
#ifndef DISKQUEUE_H
#define DISKQUEUE_H
class DiskQueue: public PCBQueue {
	private:
		int cylinders;
		int head_pos;
		int direction;

	public:
		DiskQueue(string);
	    void set_cylinders(int);
	    void look_algorithm();
};

#endif
