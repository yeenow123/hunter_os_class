#include <deque>
#include <string>

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

};

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
		float estimate_burst(float, float);

	public:
		PCBQueue(string);
		void push(PCB *);
		PCB * pop();
		void sjf_schedule();
		int size();
		bool empty();
		static void snapshot_headers();
		void print_pids();
		void snapshot();

};

class PCBHandler {
	private:
		static int pidcounter;
	public:
		PCB * createPCB();
		void setParameters(PCB *, string, int, string, int);
		void terminatePCB(PCB *&);
		void setCPUTime(PCB *&, float);
};
