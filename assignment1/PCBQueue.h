#include <deque>
#include "PCB.h"

using namespace std;

struct CPU {
	PCB * currPCB;
	string pcbState;
	int num_processes;
	float total_cpu_time;

};

string i_to_s(int);

class PCBQueue {
	private:
		deque<PCB *> pcbqueue;
		string type;
		static int p_count, d_count, c_count;

	public:
		PCBQueue(string);
		void push(PCB *);
		PCB * pop();
		void sjf_insert(PCB *);
		int size();
		bool empty();
		static void snapshot_headers();
		void print_pids();
		void snapshot();

};


