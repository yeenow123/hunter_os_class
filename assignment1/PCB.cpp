#include "PCB.h"

void PCB::estimate_burst() {
	float alpha = 0.5;
	float next_burst;
	next_burst = (alpha * burst_estimate) + ((1 - alpha) * curr_burst_time);
	burst_estimate = next_burst;
}