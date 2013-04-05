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
	else if (queue_type == "d") {
		type = queue_type + i_to_s(d_count); 
		d_count++;
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
	pcbqueue.push_back(block);	
}	

PCB * PCBQueue::pop() {
	if (pcbqueue.empty()) {
		return NULL;
	}
	else {
		PCB * currBlock = pcbqueue.front();
		pcbqueue.pop_front();
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
		for (it = pcbqueue.begin(); it != pcbqueue.end(); ++it) {
			currPCB = *it;
			if (block->burst_estimate < currPCB->burst_estimate) {
				pcbqueue.insert(it, block);
				break;
			}
		}

		// If process has longest burst estimate, push to back of queue (have to account for the last one separately)
		if (it == pcbqueue.end()) {
			pcbqueue.push_back(block);
		}

	}
}

int PCBQueue::size() {
	return pcbqueue.size();
}

bool PCBQueue::empty() {
	if (pcbqueue.empty())
		return true;
	else
		return false;	
}

void PCBQueue::snapshot_headers() {
	cout << setw(5) << "pid" << setw(20) << "filename" << setw(10) << "memstart" << setw(5) << "r/w" << setw(10) << "length" << endl; 
}

void PCBQueue::print_pids() {
	deque<PCB *>::iterator it;
	PCB * currPCB;
	cout << "pid" << endl;
	for (it = pcbqueue.begin(); it != pcbqueue.end(); ++it) {
		currPCB = *it;
		cout << currPCB->pid << " " << currPCB->burst_estimate << endl;
	}
}

void PCBQueue::snapshot() {
	int i;
	PCB * currPCB;
	cout << type << "---" <<  endl;
	for (i = 0; i < pcbqueue.size(); i++) {
		currPCB = pcbqueue.at(i);
		cout << setw(5)  << currPCB->pid; 
		cout << setw(20) << currPCB->filename;
		cout << setw(10) << currPCB->mem_loc;
		cout << setw(5) << currPCB->action;
		cout << setw(10) << currPCB->length;
		cout << endl;	
		 
	}
}

int PCBQueue::p_count = 0;
int PCBQueue::d_count = 0;
int PCBQueue::c_count = 0;


