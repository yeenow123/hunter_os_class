#include "DiskQueue.h"
#include "PCB.h"

DiskQueue::DiskQueue(string input) : PCBQueue(input) {
}

void DiskQueue::setCylinders(int num) {
	cylinders = num;
}
