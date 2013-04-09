#include <string>
#ifndef PCB_H
#define PCB_H
using namespace std;

struct PCB {
	int pid;
	string filename;
	int mem_loc;
	string action;
	int length;
	int cpu_usage;
	float burst_estimate;
	float curr_burst_time;
	float total_burst_time;

	void estimate_burst();

};

#endif
