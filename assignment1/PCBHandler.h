#include "PCBQueue.h"

class PCBHandler {
	private:
		static int pidcounter;
	public:
		PCB * createPCB();
		void setParameters(PCB *, string, int, string, int);
		void terminatePCB(PCB *&);
		void setCPUTime(PCB *&, float);
};