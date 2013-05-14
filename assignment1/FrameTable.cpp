#include <iostream>
#include <iomanip>
#include "FrameTable.h"

using namespace std;

FrameTable::FrameTable(int frames) {
	for (int i = 0; i < frames; i++) {
		frame_table.push_back(-1);// -1 denotes that the frame is not allocated		
	}	
}

void FrameTable::free_frames(int pid) {
	int i;
	for (i = 0; i < frame_table.size(); i++) {
		if (frame_table[i] == pid) {
			frame_table[i] = -1;
		}
	}

}

int FrameTable::num_free_frames() {
	int count = 0;

	for (int i = 0; i < frame_table.size(); i++) {
		if (frame_table[i] == -1) {
			count++;
		}
	}
	return count;
}

bool FrameTable::isFull(int num_pages) {
	int count = 0;

	for (int i = 0; i < frame_table.size(); i++) {
		if (frame_table[i] == -1) {
			count++;
		}
	}
	if (num_pages <= count) {
		return false;
	}
	else {
		return true;
	}
}

void FrameTable::allocate_frames(int num_pages, int process) {
	int count = num_pages;
	cout << num_pages << endl;
	if (isFull(num_pages) == false) {
		for (int i = 0; i < frame_table.size(); i++) {
			if (frame_table[i] == -1) {
				frame_table[i] = process;
				count--;
			}
			if (count == 0) {
				break;
			}	
		}			
	}	
}

void FrameTable::print_free_frames() {
	cout << "Free frames are: ";
	for (int i = 0; i < frame_table.size(); i++) {
		if (frame_table[i] == -1) {
			cout << i << ", ";
		}
	}
	cout << endl;
}

void FrameTable::print_frame_table() {
	cout << "Frame table" << endl;	
	for (int i = 0; i < frame_table.size(); i++) {
		if (frame_table[i] == -1) {
			cout << "free, ";
		}
		else {
			cout << frame_table[i] << ", ";
		}
	}
	cout << endl;
}
