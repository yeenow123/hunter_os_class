#include <sstream>
#include <deque>
#include <iomanip>
#include <string>
using namespace std;

string i_to_s(int x) {
	stringstream ss;
	string res;
	ss << x;
	res = ss.str();
	return res;	
}

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

class PCBQueue {
	deque<PCB *> pcbqueue; 
	string type;
	static int p_count, d_count, c_count;

	public:
		
		PCBQueue(string queue_type) {
			if (queue_type == "p") {
				type = queue_type + i_to_s(p_count); 
				p_count++;
			}
			else if (queue_type == "d") {
				type = queue_type + i_to_s(d_count); 
				d_count++;
			}
			else if (queue_type == "c") {
				type = queue_type + i_to_s(c_count); 
				c_count++;
			
			}
			else if (queue_type == "r") {
				type = queue_type;
			}

		}		

		void push(PCB * block) {
			pcbqueue.push_back(block);	
		}	

		PCB * pop() {
			PCB * currBlock = pcbqueue.front();
			pcbqueue.pop_front();
			return currBlock;
		}

		int size() {
			return pcbqueue.size();
		}

		bool empty() {
			if (pcbqueue.empty())
				return true;
			else
				return false;	
		}

		static void snapshot_headers() {
			cout << setw(5) << "pid" << setw(20) << "filename" << setw(10) << "memstart" << setw(5) << "r/w" << setw(10) 			<< "length" << endl; 
		}

		void print_pids() {
			int i;
			cout << "pid" << endl;
			for (i=0; i<pcbqueue.size(); i++) {
				cout << pcbqueue.at(i)->pid << endl;
			}
		}
		void snapshot(string input) {
			int i;
			PCB * currPCB;
			cout << type << "---" <<  endl;
			for (i = 0; i < pcbqueue.size(); i++) {
				currPCB = pcbqueue.at(i);
				cout << setw(5)  << currPCB->pid; 
				cout << setw(20) << currPCB->filename;
				cout << setw(10) << currPCB->mem_loc;
				cout << setw(5) << currPCB->action;
				cout << setw(10) << currPCB->length;
				cout << endl;	
				 
			}
		}
};

int PCBQueue::p_count = 0;
int PCBQueue::d_count = 0;
int PCBQueue::c_count = 0;

class PCBHandler {
	int pidcounter = 0;

	public:
		PCB * createPCB() {
			PCB * newProcess = new PCB;
			newProcess->pid = pidcounter;
			pidcounter++;
			return newProcess;	
		}

		void setParameters(PCB * currPCB, string filename, int mem_loc, string action, int length) {
			currPCB->filename = filename;
			currPCB->mem_loc = mem_loc;
			currPCB->action = action;
			currPCB->length = length;
		}


		void terminatePCB(PCB * process) {
			delete process;
		}
};

