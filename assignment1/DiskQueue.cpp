#include "DiskQueue.h"
#include "PCB.h"

DiskQueue::DiskQueue(string input) : PCBQueue(input) {
	head_pos = 0;
	direction = 0;
}

void DiskQueue::set_cylinders(int num) {
	cylinders = num;
}

void DiskQueue::look_algorithm() {
	int i;
	int curr_head = head_pos;
	int seek_time;
	PCB * currPCB;

	deque<PCB *>::iterator it;
	for (it = queue.begin(); it != queue.end(); ++it) {
		currPCB = *it;
		deque<PCB *>::iterator jt;
		// Going towards cylinder 0
		if (direction == 0) {
			if (head_pos > currPCB->mem_loc) {
				for (jt = queue.end(); jt != queue.begin(); --jt) {
					
				}
			}
			
			
		}

		else if (direction == 1) {
			if (head_pos < currPCB->mem_loc) {

			}
		}
		

	}
}

void DiskQueue::move_out() {
}

void DiskQueue::move_in() {

}