#include <string>
#ifndef PCB_H
#define PCB_H
using namespace std;

struct PCB {
	int pid;
	string filename;
	int mem_loc;
	int mem_size;
	string action;
	int length;
	int cylinder_loc;
	int cpu_usage;
	float burst_estimate;
	float remaining_time;
	float actual_time;
	float total_burst_time;

	void estimate_burst(float);

};

#endif
