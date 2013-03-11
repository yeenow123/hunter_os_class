#include <deque>
#include <string>

using namespace std;

struct PCB {
	int pid;
	string filename;
	int mem_loc;
	string action;
	int length;
};

struct CPU {
	PCB * currPCB;
	string pcbState;

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
		int size();
		bool empty();
		static void snapshot_headers();
		void print_pids();
		void snapshot(string);

};

class PCBHandler {
	private:
		static int pidcounter;
	public:
		PCB * createPCB();
		void setParameters(PCB *, string, int, string, int);
		void terminatePCB(PCB *&);
};
