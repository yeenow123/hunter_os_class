#include <sstream>
#include <iomanip>
#include <iostream>
#include "PCBQueue.h"

using namespace std;

string i_to_s(int x) {
	stringstream ss;
	string res;
	ss << x;
	res = ss.str();
	return res;	
}

PCBQueue::PCBQueue(string queue_type) {
	if (queue_type == "p") {
		type = queue_type + i_to_s(p_count); 
		p_count++;
	}
	else if (queue_type == "c") {
		type = queue_type + i_to_s(c_count); 
		c_count++;
	
	}
	else if (queue_type == "r") {
		type = queue_type;
	}

}	

void PCBQueue::push(PCB * block) {
	queue.push_back(block);	
}	

PCB * PCBQueue::pop() {
	if (queue.empty()) {
		return NULL;
	}
	else {
		PCB * currBlock = queue.front();
		queue.pop_front();
		return currBlock;
	}
	
}

void PCBQueue::sjf_insert(PCB * block) {
	PCB * currPCB;
	if (empty() == true) {
		push(block);
	}
	else {
		deque<PCB *>::iterator it;
		for (it = queue.begin(); it != queue.end(); ++it) {
			currPCB = *it;
			if (block->burst_estimate <= currPCB->burst_estimate) {
				queue.insert(it, block);
				break;
			}
		}

		// If process has longest burst estimate, push to back of queue (have to account for the last one separately)
		if (it == queue.end()) {
			queue.push_back(block);
		}

	}
}

int PCBQueue::size() {
	return queue.size();
}

bool PCBQueue::empty() {
	if (queue.empty())
		return true;
	else
		return false;	
}

PCB * PCBQueue::peek() {
	return queue.front();
}

void PCBQueue::snapshot_headers() {
	cout << setw(5) << "pid" << setw(20) << "filename" << setw(10) << "memstart" << setw(5) << "r/w" << setw(10) << "length" << setw(10) << "totaltime" << setw(10) << "avgburst" << endl; 
}

void PCBQueue::print_pids() {
	deque<PCB *>::iterator it;
	PCB * currPCB;
	cout << setw(5) << "pid" << setw(10) << "burst_est" << setw(10) << "act_time" << endl;
	for (it = queue.begin(); it != queue.end(); ++it) {
		currPCB = *it;
		cout << setw(5) << currPCB->pid;
		cout << setw(10) << currPCB->burst_estimate;
		cout << setw(10) << currPCB->actual_time << endl;
	}
}

void PCBQueue::snapshot() {
	int i;
	PCB * currPCB;
	cout << type << "---" <<  endl;
	for (i = 0; i < queue.size(); i++) {
		currPCB = queue.at(i);
		cout << setw(5)  << currPCB->pid; 
		cout << setw(20) << currPCB->filename;
		cout << setw(10) << currPCB->mem_loc;
		cout << setw(5) << currPCB->action;
		cout << setw(10) << currPCB->length;
		cout << setw(10) << currPCB->total_burst_time;
		cout << setw(10) << (currPCB->total_burst_time / currPCB->cpu_usage); 
		cout << endl;	
		 
	}
}

int PCBQueue::p_count = 0;
int PCBQueue::c_count = 0;


