#include "DiskQueue.h"
#include "PCB.h"
#include <iostream>
#include <iomanip> 

DiskQueue::DiskQueue(string input) : PCBQueue(input) {
	head_pos = 0;
	direction = 0;
	type = "d" + i_to_s(d_count);
	d_count++;
	scanning1 = false;
	scanning2 = false;
	switch_flag = 0;
}

void DiskQueue::set_cylinders(int num) {
	cylinders = num;
}

void DiskQueue::push(PCB * newPCB) {
	if (scanning1 == false)
		queue1.push_back(newPCB);
	else if (scanning2 == false)
		queue2.push_back(newPCB);
}

void DiskQueue::switchQueue() {
	if (queue1.empty() && switch_flag == 0) { 
		switch_flag = 1;
		scanning1 = false;
	}

	else if (queue2.empty() && switch_flag == 1) { 
		switch_flag = 0;
		scanning2 = false;
	}

	if (switch_flag == 0) {
		scanning1 = true;
		fscan(queue1);
		
	}

	else if (switch_flag == 1) {
		scanning2 = true;
		fscan(queue2);
	}	
}

int DiskQueue::size() {
	if (switch_flag == 0)
		return queue1.size();
	else if (switch_flag == 1)
		return queue2.size();
}

PCB * DiskQueue::pop() { 
	PCB * currBlock;
	if (switch_flag == 0) {
		if (queue1.empty())
			currBlock = NULL;
		else {
			currBlock = queue1.front();
			queue1.pop_front();
		}
	}

	else if (switch_flag == 1) {
		if (queue2.empty())
			currBlock = NULL;
		else {
			currBlock = queue2.front();
			queue2.pop_front();
		}
	}
	return currBlock;
}

void DiskQueue::fscan(deque <PCB *> & currQueue) {
	deque<PCB *>::iterator it;
	bool found;
	PCB * currPCB;

	if (currQueue.empty()) {
		return;
	}

	found = false;
	while (!found) {
		if (direction == 0) {
			if (head_pos > cylinders) {
				direction = 1;
				head_pos = cylinders;
			}					

			if (direction == 0) {
				for (it=currQueue.begin(); it != currQueue.end(); ++it) {
					currPCB = *it;	
					if (currPCB->cylinder_loc == head_pos) {
						currQueue.push_front(currPCB);
						cout << "Head position is " << head_pos << endl;
						currQueue.erase(it);	
						found = true;
						break;
					}			
				}

				head_pos++;
			}
		}	

		else if (direction == 1) {
			if (head_pos < 0) {
				direction = 0;
				head_pos = 0;
			}
			
			if (direction == 1) {
				for (it=currQueue.begin(); it != currQueue.end(); ++it) {
					currPCB = *it;	
					if (currPCB->cylinder_loc == head_pos) {
						currQueue.push_front(currPCB);
						currQueue.erase(it);	
						found = true;
						break;
					}			
				}
				head_pos--;
			}
		}
	}	
}

void DiskQueue::snapshot_headers() {
	cout << setw(5) << "pid" << setw(20) << "filename" << setw(10) << "memstart" << setw(5) << "r/w" << setw(10) << "length" << setw(10) << "totaltime" << setw(10) << "avgburst" << setw(10) << "cylinder" << endl; 
}


void DiskQueue::snapshot() {
	int i;
	PCB * currPCB;
	cout << type << "---" <<  endl;
	cout << "Queue1 requests" << endl;
	for (i = 0; i < queue1.size(); i++) {
		currPCB = queue1.at(i);
		cout << setw(5)  << currPCB->pid; 
		cout << setw(20) << currPCB->filename;
		cout << setw(10) << currPCB->mem_loc;
		cout << setw(5) << currPCB->action;
		cout << setw(10) << currPCB->length;
		cout << setw(10) << currPCB->total_burst_time;
		cout << setw(10) << (currPCB->total_burst_time / currPCB->cpu_usage); 
		cout << setw(10) << currPCB->cylinder_loc;
		cout << endl;	
		 
	}

	cout << "Queue2 requests" << endl;
	for (i = 0; i < queue2.size(); i++) {
		currPCB = queue2.at(i);
		cout << setw(5)  << currPCB->pid; 
		cout << setw(20) << currPCB->filename;
		cout << setw(10) << currPCB->mem_loc;
		cout << setw(5) << currPCB->action;
		cout << setw(10) << currPCB->length;
		cout << setw(10) << currPCB->total_burst_time;
		cout << setw(10) << (currPCB->total_burst_time / currPCB->cpu_usage); 
		cout << setw(10) << currPCB->cylinder_loc;
		cout << endl;	
		 
	}
}
int DiskQueue::d_count = 0;
