#include <iostream>
#include "queue.cpp"
#include <vector>
#include <stdlib.h>

using namespace std;

void sys_gen(int &num_p, int &num_d, int &num_c) {
	cout << "Please enter the number of printers in the system: " << endl;
	cin >> num_p;
	cout << "Please enter the number of disks in the system: " << endl;
	cin >> num_d;
	cout << "Please enter the number of CD/RW in the system: " << endl;
	cin >> num_c;
	
}

int main() {
	int num_p, num_d, num_c;
	int i;
	vector<PCBQueue> print_queues;
	vector<PCBQueue> disk_queues;
	vector<PCBQueue> cdrw_queues;
	CPU * cpu = new CPU;
	
	sys_gen(num_p, num_d, num_c);

	// Create all queues and push them to their respective arrays (vectors)
	for (i=0; i<num_p; i++) {
		PCBQueue print_queue("p");
		print_queues.push_back(print_queue);
	}

	for (i=0; i<num_d; i++) {
		PCBQueue disk_queue("d");
		disk_queues.push_back(disk_queue);
	}

	for (i=0; i<num_c; i++) {
		PCBQueue cdrw_queue("c");
		cdrw_queues.push_back(cdrw_queue);
	}

	PCBQueue readyQueue("r");

	cout << "System generation section finished." << endl;

	PCBHandler pcbFactory;

	// Running CPU section of code that responds to user input
	while(1) {

		if (readyQueue.empty()) {
			
		}
		else {

			if (cpu->currPCB == NULL) {
				PCB * readyPCB = readyQueue.pop();
				cpu->currPCB = readyPCB;
				cpu->pcbState = "running";
			}
		}
	
		string input;
		cin >> input;

		if (input == "A") {
			PCB * newPCB = pcbFactory.createPCB();	
			readyQueue.push(newPCB);
			cout << "Created new process with process id: " << newPCB->pid << endl;
		}

		else if (input == "t") {
			cout << "Terminated process id: " << cpu->currPCB->pid << endl;
			pcbFactory.terminatePCB(cpu->currPCB);	
			cpu->currPCB = NULL;
		}
	
		else if (input == "S") {
			string option;
			cin >> option;
			cout << endl;

			if (option == "r") {
				readyQueue.print_pids();
			}

			else if (option == "p") {
				
				PCBQueue::snapshot_headers();

				for (i=0; i<num_p; i++) {
					print_queues[i].snapshot("s");
				}
			}

			else if (option == "d") {
	
				PCBQueue::snapshot_headers();

				for (i=0; i<num_p; i++) {
					disk_queues[i].snapshot("s");
				}
			}
		

			else if (option == "c") {
			
				PCBQueue::snapshot_headers();

				for (i=0; i<num_p; i++) {
					cdrw_queues[i].snapshot("s");
				}
			}
		}

		else  {
			string device = input.substr(0, 1);
			string filename, action;
			int mem_loc, length;

			cout << "Please enter the filename to write to: ";
			cin >> filename;
			cout << "Please enter the starting location in memory: ";
			cin >> mem_loc;
			cout << "Please enter the file length: ";
			cin >> length;

			cpu->currPCB->filename = filename;
			cpu->currPCB->mem_loc = mem_loc;
			cpu->currPCB->length = length;
			cpu->pcbState = "interrupted";

			PCBQueue * currQueue;
			int device_num = atoi(input.substr(1,1).c_str());

			if (device == "p") {
				currQueue = &print_queues[device_num];
				cpu->currPCB->action = "w";								
			}

			else if (device == "d") {
				currQueue = &disk_queues[device_num];
				cout << "Please enter if this is a read or write ('r' or 'w'): ";
				cin >> action;
				cout << endl;
				cpu->currPCB->action = action;
			}

			else if (device == "c") {
				currQueue = &cdrw_queues[device_num];
				cout << "Please enter if this is a read or write ('r' or 'w'): ";
				cin >> action;
				cout << endl;
				cpu->currPCB->action = action;
			}

			currQueue->push(cpu->currPCB);
			cpu->currPCB = NULL;
		}
	
	}	
}

