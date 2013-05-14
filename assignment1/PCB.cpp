#include "PCB.h"
#include <iostream>

void PCB::estimate_burst(float alpha) {
	float next_burst;
	next_burst = (alpha * burst_estimate) + ((1.0 - alpha) * actual_time);
	burst_estimate = next_burst;

}

int PCB::roundUp(int numToRound, int multiple) 
{ 
 if(multiple == 0) 
 { 
  return numToRound; 
 } 

 int remainder = numToRound % multiple;
 if (remainder == 0)
  return numToRound;
 return numToRound + multiple - remainder;
} 

void PCB::setup_page_table(int size, int page_size) {
	int i, rounded;
	rounded = roundUp(size, page_size);
	pages = rounded / page_size;
	if (pages == 0)
		pages = 1;

	page_table = new int[pages];		
	for (i=0; i < pages; i++) {
		page_table[i] = -1;	
	}	
}

void PCB::print_page_table() {
	for (int i=0; i<pages; i++) {
		cout << page_table[i] << ", ";
	}
	cout << endl;
}

