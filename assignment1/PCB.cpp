#include "PCB.h"

void PCB::estimate_burst(float alpha) {
	float next_burst;
	next_burst = (alpha * burst_estimate) + ((1.0 - alpha) * actual_time);
	burst_estimate = next_burst;
}
