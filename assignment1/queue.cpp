#include <sstream>
#include <iomanip>
#include <iostream>
#include "queue.h"

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
	int i;
	cout << "pid" << endl;
	for (i=0; i<pcbqueue.size(); i++) {
		cout << pcbqueue.at(i)->pid << endl;
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


PCB * PCBHandler::createPCB() {
	PCB * newProcess = new PCB;
	newProcess->pid = pidcounter;
	pidcounter++;
	return newProcess;	
}

void PCBHandler::setParameters(PCB * currPCB, string filename, int mem_loc, string action, int length) {
	currPCB->filename = filename;
	currPCB->mem_loc = mem_loc;
	currPCB->action = action;
	currPCB->length = length;
}


void PCBHandler::terminatePCB(PCB *& process) {
	delete process;
	process = NULL;
}

int PCBHandler::pidcounter = 0;

