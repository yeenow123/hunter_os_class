#include <ctype.h>
#include <cmath>
#include <iostream>
#include "PCBHandler.h"
#include "DiskQueue.h"
#include "FrameTable.h"
#include <vector>
#include <stdlib.h>
#include <limits>

using namespace std;


float validate_float() {
	string input;
	float output;
	cin >> input;

	while (input.find_first_not_of("1234567890.-") != string::npos) {
		cout << "Invalid input.  Please try again: " << endl;
		cin >> input;
	}	
	output = atof(input.c_str());
	return output;
}

bool isPowerOfTwo(int x) {
	return (x != 0) && ((x & (x - 1)) == 0);
}

void validate_hex(int &input) {
	cout << "Please enter the starting location in memory as a hexadecimal integer: " << endl;
	cin >> hex >> input;
}

void sys_gen(int &num_p, int &num_d, int &num_c, float &burst_estimate, int &total_mem, int &max_process_mem, int &page_size) {

	cout << "Please enter the number of printers in the system: ";
	cin >> num_p;
	cout << "Please enter the number of disks in the system: ";
	cin >> num_d;
	cout << "Please enter the number of CD/RW in the system: ";
	cin >> num_c;
	
	cout << "Please enter the initial burst estimate of all processes (float): " << endl;
	burst_estimate = validate_float();


	cout << "Please enter the max process size: " << endl;
	cin >> max_process_mem;

	while (page_size <= 0 || total_mem % page_size != 0 || total_mem <= 0) {
			cin.clear();	
			page_size = 0;	
			while (isPowerOfTwo(page_size) != true) { 
				cout << "Please enter the page size: " << endl;
				cin >> page_size;
			}
			cout << "Please enter the total size of memory: " << endl;
			cin >> total_mem;
	}
}

void validate_integer(string cout_text, int &input) {
	while (cout << cout_text && !(cin >> input)) {
		cout << "Invalid input. Please try again." << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

PCB * create_process(PCBQueue &readyQueue, PCBHandler pcbFactory) {
	PCB * newPCB = pcbFactory.createPCB();	
	
	newPCB->actual_time = 0;
	newPCB->total_burst_time = 0;
	newPCB->cpu_usage = 0;
	readyQueue.sjf_insert(newPCB);

	cout << "Created new process with process id: " << newPCB->pid << endl;

	return newPCB;
}

void terminate_process(CPU *&cpu, PCBHandler pcbFactory, int &terminated_pid) {
	if (cpu->currPCB == NULL) {
		cout << "No processes running in the CPU." << endl;
	}
	else {
		PCB * terminated = NULL;
		terminated = cpu->currPCB;
		cout << "Terminated process id: " << terminated->pid << endl;
		cout << "Process had a total CPU time of " << terminated->total_burst_time << endl;
		
		terminated_pid = terminated->pid;
		cpu->total_cpu_time += terminated->total_burst_time;
		cpu->num_processes++;
		cpu->currPCB = NULL;
		pcbFactory.terminatePCB(terminated);
	}
}


int main() {	
	int num_p, num_d, num_c;
	int i;
	float alpha = -1;
	float burst_estimate;
	int total_mem, max_process_mem, page_size = -1;
	vector<PCBQueue> print_queues;
	vector<DiskQueue> disk_queues;
	vector<PCBQueue> cdrw_queues;
	
	CPU * cpu = new CPU;
	cpu->total_cpu_time = 0;
	cpu->currPCB = NULL;
	
	sys_gen(num_p, num_d, num_c, burst_estimate, total_mem, max_process_mem, page_size);

	// Create all queues and push them to their respective arrays (vectors)
	for (i=0; i<num_p; i++) {
		PCBQueue print_queue("p");
		print_queues.push_back(print_queue);
	}

	for (i=0; i<num_d; i++) {
		DiskQueue disk_queue("d");
		disk_queues.push_back(disk_queue);
	}

	int num_cylinders;

	for (i=0; i<disk_queues.size(); i++) {
		validate_integer("Enter the number of cylinders for disk " + i_to_s(i) + ": ", num_cylinders);
		disk_queues[i].set_cylinders(num_cylinders);		 
	}

	for (i=0; i<num_c; i++) {
		PCBQueue cdrw_queue("c");
		cdrw_queues.push_back(cdrw_queue);
	}

	PCBQueue readyQueue("r");
	PCBQueue job_pool("j");

	while (alpha > 1.0 || alpha < 0.0) {
		cout << "Please enter the value for alpha, a number between 0.0 and 1.0" << endl;
		alpha = validate_float();
	}

	FrameTable frame_table(total_mem / page_size);

	cout << "System generation section finished." << endl;

	PCBHandler pcbFactory;
	string input;

	// Running CPU section of code that responds to user input
	bool sys_call = true;
	while(1) {
		// Always have a process in CPU if the ready queue isn't empty
		if (!readyQueue.empty()) {
			if (cpu->currPCB == NULL) {
				PCB * readyPCB = readyQueue.pop();
				cpu->currPCB = readyPCB;
			}
			else if (cpu->currPCB->burst_estimate > readyQueue.peek()->remaining_time && sys_call == true) {
				float usage;
				cout << "Another process with a shorter burst estimate has pre-empted the current process." << endl;
				cout << "Please enter the amount of time the process has run: " << endl;
				usage = validate_float();
				cpu->currPCB->actual_time += usage;
				cpu->currPCB->remaining_time -= usage;

				if (cpu->currPCB->remaining_time > readyQueue.peek()->remaining_time && sys_call == true) {
					PCB * readyPCB = readyQueue.pop();
					readyQueue.sjf_insert(cpu->currPCB);
					cpu->currPCB = readyPCB;			
				}	
				else {
				
				}
			}	
		}
		
		if (job_pool.check_fit(frame_table.num_free_frames()) == true && job_pool.empty() == false) {
			PCB * pooledPCB;
			cout << "Check fit" << endl;
			pooledPCB = job_pool.largest_fit(frame_table.num_free_frames());
			pooledPCB->setup_page_table(pooledPCB->mem_size, page_size);								
			frame_table.allocate_frames(pooledPCB->pages, pooledPCB->pid);

			int m, n;
			for (m = 0; m < frame_table.frame_table.size(); m++) {
				if (frame_table.frame_table[m] == pooledPCB->pid) {
					for (n = 0; n < pooledPCB->pages; n++) {
						if (pooledPCB->page_table[n] == -1) {
							pooledPCB->page_table[n] = m;
							break;	
						}	
					}
				}
			}	
		}			
		cin >> input;

		// Create new process
		if (input == "A") {
			int proc_mem;
			validate_integer("Please enter the process memory size: ", proc_mem);
			if (proc_mem > total_mem) {
				cout << "Process is larger than the total system memory." << endl;
			}

			else if (proc_mem > max_process_mem) {
				cout << "Process is larger than the max process size." << endl;
			}

			else {
				int num_pages;
				PCB * newPCB = create_process(readyQueue, pcbFactory);
				newPCB->remaining_time = burst_estimate;
				newPCB->burst_estimate = burst_estimate;
				newPCB->actual_time = 0;
				newPCB->mem_size = proc_mem;
				if (frame_table.isFull(newPCB->pages)) {
					job_pool.push(newPCB);
				}

				else {
						newPCB->setup_page_table(proc_mem, page_size);								
						frame_table.allocate_frames(newPCB->pages, newPCB->pid);

						int m, n;
						for (m = 0; m < frame_table.frame_table.size(); m++) {
							if (frame_table.frame_table[m] == newPCB->pid) {
									for (n = 0; n < newPCB->pages; n++) {
										if (newPCB->page_table[n] == -1) {
											newPCB->page_table[n] = m;
											break;	
										}	
									}
							}
						}	
				}
				sys_call = true;
			}
		}

		// Terminate the currently running process
		else if (input == "t") {
			int terminated_pid;
			terminate_process(cpu, pcbFactory, terminated_pid);

			frame_table.free_frames(terminated_pid);
			sys_call = true;
		}
	
		// Output a snapshot of current processes
		else if (input == "S") {
			string option;
			cin >> option;
			cout << endl;

			if (option == "r") {
				readyQueue.print_pids();
				cout << "System's average time per process is: " << floor((cpu->total_cpu_time / cpu->num_processes) * 100 + 0.5) / 100 << endl;
			}

			else if (option == "p") {
				
				PCBQueue::snapshot_headers();

				for (i=0; i<num_p; i++) {
					print_queues[i].snapshot();
				}
			}

			else if (option == "d") {
	
				DiskQueue::snapshot_headers();

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

			else if (option == "m") {
				frame_table.print_frame_table();
				frame_table.print_free_frames();
			}
			sys_call = false;
		}

		else  {
			
			string device = input.substr(0, 1);
			int device_num = atoi(input.substr(1,1).c_str());
				
			PCBQueue * currQueue = NULL;
			DiskQueue * currDiskQueue = NULL;

			// Process is requesting I/O
			if (islower(device[0]) && (device == "p" || device == "d" || device == "c")) {
				string filename, action;
				int mem_loc, length;
				int cylinder_loc = -1;

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
					validate_integer("Please enter the file length: ", length);
					
				}

				else if (device == "d") {
					if (device_num >= disk_queues.size()) {
						cout << "Requested device does not exist." << endl;
						continue;
					}

					currDiskQueue = &disk_queues[device_num];
					
					while (!(action == "r" || action == "w")) {
						cout << "Please enter if this is a read or write ('r' or 'w'): "; 
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cin >> action;
					}

					while (cylinder_loc > currDiskQueue->cylinders || cylinder_loc < 0) {
						validate_integer("Please enter the cylinder to access: ", cylinder_loc);
					}
					cpu->currPCB->action = action;

					if (action == "w") {
						// Verify length is a number
						validate_integer("Please enter the file length: ", length);
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
						validate_integer("Please enter the file length: ", length);
					}

					else {
						length = 0;
					}
				}

				cout << "Please enter the filename to read from or write to: ";
				cin >> filename;

				// Verify memory location is a number
				
				validate_hex(mem_loc);

				// Ask how much time the process has run
				float process_time;
				cout << "Please enter the amount of time the current process has run (in milliseconds): " << endl;
				process_time = validate_float();

				cpu->currPCB->filename = filename;
				cpu->currPCB->mem_loc = mem_loc;
				cpu->currPCB->length = length;
				cpu->currPCB->actual_time += process_time;
				cpu->currPCB->estimate_burst(alpha);
				cpu->currPCB->total_burst_time += cpu->currPCB->actual_time;
				cpu->currPCB->cpu_usage++;
				cpu->currPCB->cylinder_loc = cylinder_loc;
				cpu->currPCB->actual_time = 0;
				cpu->currPCB->remaining_time = cpu->currPCB->burst_estimate;	

				if (currQueue == NULL) {
					currDiskQueue->push(cpu->currPCB);
				}
				else {
					currQueue->push(cpu->currPCB);
				}
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
					disk_queues[device_num].switchQueue();
					currDiskQueue = &disk_queues[device_num];
				}

				else if (device == "C") {
					if (device_num >= cdrw_queues.size()) {
						cout << "Requested device does not exist." << endl;
						continue;
					}
					currQueue = &cdrw_queues[device_num];
				}
				
				PCB * readyPCB = NULL;
				if (currQueue == NULL) {
					readyPCB = currDiskQueue->pop();
				}
				else {
					readyPCB = currQueue->pop();
				}
				if (readyPCB == NULL) {
					cout << "There are no processes in the device's queue." << endl;
				}

				else {
					readyQueue.sjf_insert(readyPCB);
					cout << "Process with id " << readyPCB->pid << " has moved to the ready queue." << endl;
				}
			}

			else if (device == "K") {
				int kill_pid = device_num;		
				PCB * killPCB = NULL;
				// Search for the process
			
				if (cpu->currPCB->pid == kill_pid) {
					killPCB = cpu->currPCB;
					terminate_process(cpu, pcbFactory, kill_pid);
				}		
				else {
					int e;
					if (killPCB == NULL) {
						killPCB = readyQueue.getPCB(kill_pid);

					}	
					if (killPCB == NULL) {
						for (e = 0; e < print_queues.size(); e++) {
							if (killPCB == NULL) {
								killPCB = print_queues[e].getPCB(kill_pid);	
								
							}
							else {
								break;
							}
						}
					}
					
					if (killPCB == NULL) {
						
						for (e = 0; e < cdrw_queues.size(); e++) {
							if (killPCB == NULL) {
								killPCB = cdrw_queues[e].getPCB(kill_pid);	
							}
							else
								break;
						}
					}	

					if (killPCB == NULL) {
						
						for (e = 0; e < disk_queues.size(); e++) {
							if (killPCB == NULL) {
								killPCB = disk_queues[e].getPCB(kill_pid);	
							}
							else
								break;
						}
					}
					if (killPCB == NULL) {
						cout << "Process not found." << endl;
					}
					else {
						
						PCB * terminated = NULL;
						terminated = killPCB;
						cout << "Terminated process id: " << terminated->pid << endl;
						cout << "Process had a total CPU time of " << terminated->total_burst_time << endl;
						
						//pcbFactory.terminatePCB(terminated);
						frame_table.free_frames(terminated->pid);
						cout << "Finished free frames" << endl;
						sys_call = true;
					}
				}
			}

			else {
				cout << "That is not a command." << endl;
			}
			sys_call = true;
		}
	
	}	
}

