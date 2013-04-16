#include "PCBQueue.h"
#include <string>
#ifndef DISKQUEUE_H
#define DISKQUEUE_H
class DiskQueue: public PCBQueue {
	private:
		int head_pos;
		int direction; 
		int switch_flag;
		bool scanning;
		static int d_count;
		deque<PCB *> queue1;
		deque<PCB *> queue2;	
	
	public:
		DiskQueue(string);
	    void set_cylinders(int);
	    void fscan(deque <PCB *>);
		void push(PCB *);
		static void snapshot_headers();
		int size();
		PCB * pop();
		void snapshot();
		int cylinders;
		void switchQueue();
};

#endif
