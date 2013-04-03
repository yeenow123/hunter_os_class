#include <ctype.h>
#include <iostream>
#include "queue.h"
#include <vector>
#include <stdlib.h>
#include <limits>

using namespace std;

void sys_gen(int &num_p, int &num_d, int &num_c) {
	cout << "Please enter the number of printers in the system: ";
	cin >> num_p;
	cout << "Please enter the number of disks in the system: ";
	cin >> num_d;
	cout << "Please enter the number of CD/RW in the system: ";
	cin >> num_c;
	
}

int main() {
	int num_p, num_d, num_c;
	int i;
	vector<PCBQueue> print_queues;
	vector<PCBQueue> disk_queues;
	vector<PCBQueue> cdrw_queues;
	CPU * cpu = new CPU;
	cpu->currPCB = NULL;
	
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
	string input;

	// Running CPU section of code that responds to user input
	while(1) {

		// Always have a process in CPU if the ready queue isn't empty
		if (!readyQueue.empty()) {
			if (cpu->currPCB == NULL) {
				PCB * readyPCB = readyQueue.pop();
				cpu->currPCB = readyPCB;
				cpu->pcbState = "running";
			}
		}
			
		cin >> input;

		// Create new process
		if (input == "A") {
			string burst_estimate;
			
			cout << "Please enter the initial burst estimate of the new process in milliseconds: " << endl;
			cin >> burst_estimate;

			while (burst_estimate.find_first_not_of("1234567890.-") != string::npos) {
				cout << "Invalid input.  Please try again: " << endl;
				cin >> burst_estimate;
			}		

			PCB * newPCB = pcbFactory.createPCB();	
			
			newPCB->burst_estimate = atof(burst_estimate.c_str());
			newPCB->curr_burst_time = 0;
			newPCB->total_burst_time = 0;
			readyQueue.push(newPCB);

			cout << "Created new process with process id: " << newPCB->pid << endl;
			cout << newPCB->burst_estimate;

		}

		// Terminate the currently running process
		else if (input == "t") {
			if (cpu->currPCB == NULL) {
				cout << "No processes running in the CPU." << endl;
			}
			else {
				PCB * terminated = NULL;
				terminated = cpu->currPCB;
				cout << "Terminated process id: " << terminated->pid << endl;
				cout << "Process had a total CPU time of " << terminated->total_burst_time << endl;

				cpu->total_cpu_time += terminated->total_burst_time;
				cpu->num_processes++;
				cpu->currPCB = NULL;
				pcbFactory.terminatePCB(terminated);
			}
		}
	
		// Output a snapshot of current processes
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
					print_queues[i].snapshot();
				}
			}

			else if (option == "d") {
	
				PCBQueue::snapshot_headers();

				for (i=0; i<num_d; i++) {
					disk_queues[i].snapshot();
				}
			}
		

			else if (option == "c") {
			
				PCBQueue::snapshot_headers();

				for (i=0; i<num_c; i++) {
					cdrw_queues[i].snapshot();
				}
			}
		}

		else  {
			
			string device = input.substr(0, 1);
			int device_num = atoi(input.substr(1,1).c_str());
				
			PCBQueue * currQueue;

			// Process is requesting I/O
			if (islower(device[0]) && (device == "p" || device == "d" || device == "c")) {
				string filename, action;
				int mem_loc, length;

				if (cpu->currPCB == NULL) {
					cout << "CPU is currently idle with no processes ready." << endl;
				}

				else if (device == "p") {
					if (device_num >= print_queues.size()) {
						cout << "Requested device does not exist." << endl;
						continue;
					}

					currQueue = &print_queues[device_num];
					cpu->currPCB->action = "w";

					// Verify length is a number
					while (cout << "Please enter the file length: " && !(cin >> length)) {
						cout << "Invalid input. Please try again." << endl;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
					}
					
				}

				else if (device == "d") {
					if (device_num >= disk_queues.size()) {
						cout << "Requested device does not exist." << endl;
						continue;
					}

					currQueue = &disk_queues[device_num];
					
					while (!(action == "r" || action == "w")) {
						cout << "Please enter if this is a read or write ('r' or 'w'): "; 
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cin >> action;
					}

					cpu->currPCB->action = action;

					if (action == "w") {
						// Verify length is a number
						while (cout << "Please enter the file length: " && !(cin >> length)) {
							cout << "Invalid input. Please try again." << endl;
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
						}
					}
					else {
						length = 0;
					}
				}

				else if (device == "c") {
					if (device_num >= cdrw_queues.size()) {
						cout << "Requested device does not exist." << endl;
						continue;
					}

					currQueue = &cdrw_queues[device_num];
					
					while (!(action == "r" || action == "w")) {
						cout << "Please enter if this is a read or write ('r' or 'w'): "; 
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cin >> action;
					}

					cpu->currPCB->action = action;

					if (action == "w") {

						// Verify length is a number
						while (cout << "Please enter the file length: " && !(cin >> length)) {
							cout << "Invalid input. Please try again." << endl;
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
						}
					}

					else {
						length = 0;
					}
				}

				cout << "Please enter the filename to read from or write to: ";
				cin >> filename;

				// Verify memory location is a number
				while (cout << "Please enter the starting location in memory (integer): " && !(cin >> mem_loc)) {
						cout << "Invalid input. Please try again." << endl;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}

				cpu->currPCB->filename = filename;
				cpu->currPCB->mem_loc = mem_loc;
				cpu->currPCB->length = length;
				cpu->pcbState = "interrupted";

				currQueue->push(cpu->currPCB);
				cpu->currPCB = NULL;
			}
			
			// Signal completion of I/O request
			else if (isupper(device[0]) && (device == "P" || device == "D" || device == "C")) {

				if (device  == "P") {
					if (device_num >= print_queues.size()) {
						cout << "Requested device does not exist." << endl;
						continue;
					}
					currQueue = &print_queues[device_num];
				}

				else if (device == "D") {
					if (device_num >= disk_queues.size()) {
						cout << "Requested device does not exist." << endl;
						continue;
					}
					currQueue = &disk_queues[device_num];
				}

				else if (device == "C") {
					if (device_num >= cdrw_queues.size()) {
						cout << "Requested device does not exist." << endl;
						continue;
					}
					currQueue = &cdrw_queues[device_num];
				}
				
				PCB * readyPCB = NULL;
				readyPCB = currQueue->pop();

				if (readyPCB == NULL) {
					cout << "There are no processes in the device's queue." << endl;
				}

				else {
					readyQueue.push(readyPCB);
					cout << "Process with id " << readyPCB->pid << " has moved to the ready queue." << endl;
				}
			}

			else {
				cout << "That is not a command." << endl;
			}
		}
	
	}	
}

