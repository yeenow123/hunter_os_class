#include "PCBHandler.h"

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

void PCBHandler::setCPUTime(PCB *& process, float time) {
	process->total_burst_time += time;
	process->curr_burst_time = time;

}

int PCBHandler::pidcounter = 0;