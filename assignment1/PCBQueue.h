#include <deque>
#include "PCB.h"

#ifndef PCBQUEUE_H
#define PCBQUEUE_H

using namespace std;

struct CPU {
	PCB * currPCB;
	int num_processes;
	float total_cpu_time;

};

string i_to_s(int);

class PCBQueue {
	protected:
		deque<PCB *> queue;
		string type;
		static int p_count, c_count;

	public:
		PCBQueue(string);
		void push(PCB *);
		PCB * peek();
		PCB * pop();
		void sjf_insert(PCB *);
		int size();
		bool empty();
		static void snapshot_headers();
		void print_pids();
		void snapshot();
		PCB * largest_fit(int);
		bool check_fit(int);
		PCB * getPCB(int);
};

#endif
