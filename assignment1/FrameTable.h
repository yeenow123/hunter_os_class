#include <vector>

#ifndef FRAMETABLE_H
#define FRAMETABLE_H 


class FrameTable {
	public:
		std::vector<int> frame_table;
		FrameTable(int);
		void print_free_frames();
		void print_frame_table();
		void free_frames(int);
		bool isFull(int);
		void allocate_frames(int, int);
};

#endif
