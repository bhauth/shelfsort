#include <iostream>
#include <time.h>
#include <algorithm>

#define COPY(dest,src,elements) for (int copy_index = elements - 1; copy_index >= 0; copy_index--) { dest[copy_index] = src[copy_index]; }

#define ELEMENT int64_t
#define LESSEQ(a,b) a <= b
#define LESS(a,b) a < b
#define SMALLSORT_SIZE 4

inline void SmallSort(ELEMENT* arr) {
	auto a = arr[0];
	auto b = arr[1];
	auto c = arr[2];
	auto d = arr[3];
	
	bool less1 = LESS(b,a);
	auto a2 = less1 ? b : a;
	auto b2 = less1 ? a : b;
	bool less2 = LESS(d,c);
	auto c2 = less2 ? d : c;
	auto d2 = less2 ? c : d;
	
	if (LESSEQ(b2, c2)) {
		arr[0] = a2;
		arr[1] = b2;
		arr[2] = c2;
		arr[3] = d2;
		return;
		}
	
	bool x = LESSEQ(a2, c2);
	auto b3 = x ? c2 : a2;
	bool y = LESSEQ(b2, d2);
	auto c3 = y ? b2 : d2;
	bool z = LESSEQ(a2, d2);
	arr[0] = x ? a2 : c2;
	arr[1] = z ? b3 : c3;
	arr[2] = z ? c3 : b3;
	arr[3] = y ? d2 : b2;
	}

void MergePair(ELEMENT* p1, ELEMENT* p2, ELEMENT* output, int n) { // n = size-1
	int i_1 = n;
	int i_2 = n;

	for (int i = n * 2 + 1; i_1 >= 0 && i_2 >= 0; i--) {
		auto x = p1[i_1];
		auto y = p2[i_2];
		bool higher = LESS(y, x);
		bool lower = !higher;
		i_1 -= higher;
		i_2 -= lower;
		output[i] = higher ? x : y;
		}

	if (i_1 >= 0) { COPY(output, p1, (i_1 + 1)); }
	else { COPY(output, p2, (i_2 + 1)); }
	}


void BlockMerge(ELEMENT* start_p, ELEMENT* scratch, uint32_t* indices, uint32_t* indices_out, int block_count_1, int block_count_2, int block_size) {
	int index_index_1 = block_count_1 - 1;
	int index_index_2 = block_count_2 - 1;
	int block_id_1 = indices[index_index_1];
	int block_id_2 = indices[block_count_1 + index_index_2];
	ELEMENT* p1 = &start_p[block_id_1 * block_size];
	ELEMENT* p2 = &start_p[(block_count_1 + block_id_2) * block_size];
	
	// merge first blocks into scratch
	ELEMENT* output = scratch;
	int output_block_counter = block_count_1 + block_count_2 - 2;
	int clear_block_id = 0;
	int next_clear_block_id = 0;
	
	int i = block_size * 2 - 1;  // scratch is 2 blocks in size
	int i_1 = block_size - 1;
	int i_2 = i_1;

	// adaptive sort: when in order, just adjust indices
	ELEMENT last_of_first = p1[(indices[0] * block_size) + block_size - 1];
	ELEMENT first_of_last = start_p[(block_count_1 + indices[block_count_1]) * block_size];
	if (LESSEQ(last_of_first, first_of_last)) {
		for (int i = 0; i < block_count_1; i++) {
			indices_out[i] = indices[i];
			}
		for (int i = block_count_1; i < block_count_1 + block_count_2; i++) {
			indices_out[i] = indices[i] + block_count_1;
			}
		return;
		}

block_merging:
		while (i_1 >= 0 && i_2 >= 0 && i >= 0) {
			auto x = p1[i_1];
			auto y = p2[i_2];
			bool higher = LESS(y, x);
			bool lower = !higher;
			output[i] = higher ? x : y;
			i_1 -= higher;
			i_2 -= lower;
			i--;
			}
		
		if (i < 0) {
			output = &start_p[next_clear_block_id * block_size];
			output_block_counter--;
			indices_out[output_block_counter] = next_clear_block_id;
			i = block_size - 1;
			}
		if (i_1 < 0) {
			next_clear_block_id = block_id_1;
			index_index_1--;
			if (index_index_1 < 0) {
				goto finish_left;
				}
			block_id_1 = indices[index_index_1];
			p1 = &start_p[block_id_1 * block_size];
			i_1 = block_size - 1;
			}
		if (i_2 < 0) {
			next_clear_block_id = block_count_1 + block_id_2;
			index_index_2--;
			if (index_index_2 < 0) {
				goto finish_right;
				}
			block_id_2 = indices[block_count_1 + index_index_2];
			p2 = &start_p[(block_count_1 + block_id_2) * block_size];
			i_2 = block_size - 1;
			}
		
		goto block_merging;


finish_left:
		while (i_2 >= 0 && i >= 0) {
			output[i] = p2[i_2];
			i_2--; i--;
			}
		
		if (i < 0) {
			clear_block_id = next_clear_block_id;
			output = &start_p[next_clear_block_id * block_size];
			if (i_2 >= 0) {
				indices_out[output_block_counter] = next_clear_block_id;
				}
			output_block_counter--;
			i = block_size - 1;
			}
		if (i_2 < 0) {
			next_clear_block_id = block_count_1 + block_id_2;
			index_index_2--;
			if (index_index_2 < 0) {
				goto unload_scratch;
				}
			block_id_2 = indices[block_count_1 + index_index_2];
			p2 = &start_p[(block_count_1 + block_id_2) * block_size];
			i_2 = block_size - 1;
			}
		
		goto finish_left;


finish_right:
		while (i_1 >= 0 && i >= 0) {
			output[i] = p1[i_1];
			i_1--; i--;
			}
		
		if (i < 0) {
			clear_block_id = next_clear_block_id;
			output = &start_p[next_clear_block_id * block_size];
			if (i_1 >= 0) {
				indices_out[output_block_counter] = next_clear_block_id;
				}
			output_block_counter--;
			i = block_size - 1;
			}
		if (i_1 < 0) {
			next_clear_block_id = block_id_1;
			index_index_1--;
			if (index_index_1 < 0) {
				goto unload_scratch;
				}
			block_id_1 = indices[index_index_1];
			p1 = &start_p[block_id_1 * block_size];
			i_1 = block_size - 1;
			}
		
		goto finish_right;


unload_scratch: {
		int block_bytes = block_size * sizeof(ELEMENT);
		memcpy(output, scratch, block_bytes);
		memcpy(&start_p[next_clear_block_id * block_size], &scratch[block_size], block_bytes);
		int last = block_count_1 + block_count_2 - 1;
		indices_out[last - 1] = clear_block_id;
		indices_out[last] = next_clear_block_id;
		}
	}


void FinalBlockSorting(ELEMENT* p1, ELEMENT* scratch, uint32_t* indices, int blocks, int block_size) {
	for (int b = 0; b < blocks; b++) {
		int ix = indices[b];
		if (ix != b) {
			int block_bytes = block_size * sizeof(ELEMENT);
			memcpy(scratch, &p1[b * block_size], block_bytes);
			int empty_block = b;
		
			while (ix != b) {
				memcpy(&p1[empty_block * block_size], &p1[ix * block_size], block_bytes);
				indices[empty_block] = empty_block;
				empty_block = ix;
				ix = indices[ix];
				}
			memcpy(&p1[empty_block * block_size], scratch, block_bytes);
			indices[empty_block] = empty_block;
			}
		}
	}


void ShelfSort(ELEMENT* arr, unsigned int size) {
	// determine memory size
	unsigned int v = size;
	unsigned int log_size = 0;
	while (v >>= 1) { log_size++; }
	unsigned int scratch_size = 1 << (2 + (log_size + 1) / 2);

	// allocate memory
	char* allocated_memory = reinterpret_cast<char*> (malloc(scratch_size *
			(sizeof(ELEMENT)
			+ std::max(sizeof(ELEMENT), 2 * sizeof(uint32_t)))));
	ELEMENT* scratch = reinterpret_cast<ELEMENT*> (allocated_memory);
	uint32_t* indices_a = reinterpret_cast<uint32_t*> (&allocated_memory[scratch_size * (sizeof(ELEMENT))]);
	uint32_t* indices_b = &indices_a[scratch_size];

	// test other small sorts?
	for (int i = 0; i < size; i += SMALLSORT_SIZE) {
		SmallSort(&arr[i]);
		}
	unsigned int sorted_zone_size = SMALLSORT_SIZE;
	
	// pingpong quad merge
	while (sorted_zone_size <= (scratch_size / 2)) {
		unsigned int run_len = sorted_zone_size;
		sorted_zone_size *= 2;
		for (int i = 0; i < size; i += sorted_zone_size * 2) {
			ELEMENT* p1 = &arr[i];
			ELEMENT* p2 = &arr[i + run_len];
			bool less1 = LESSEQ(p1[run_len-1], p2[0]);
			if (!less1) {  // skip if already sorted
				MergePair(p1, p2, scratch, run_len-1);
				}
		
			ELEMENT* p3 = &p2[run_len];
			ELEMENT* p4 = &p3[run_len];
			ELEMENT* scratch2 = &scratch[sorted_zone_size];
			bool less2 = LESSEQ(p3[run_len-1], p4[0]);
			if (!less2) {
				MergePair(p3, p4, scratch2, run_len-1);
				}

			if (less1 || less2) {
				if (!less1) {COPY(scratch2, p3, sorted_zone_size);}
				else if (!less2) {COPY(scratch, p1, sorted_zone_size);}
				else {
					bool less3 = LESSEQ(p1[sorted_zone_size-1], p3[0]);
					if (less3) { continue; } else {
						COPY(scratch, p1, sorted_zone_size * 2);
						}
					}
				}
			
			MergePair(scratch, scratch2, p1, sorted_zone_size-1);
			}
		sorted_zone_size *= 2;
		}

	// initialize block indices
	unsigned int block_size = scratch_size / 2;
	int total_blocks = size / block_size;
	int blocks_per_run = sorted_zone_size / block_size;
	for (int i = 0; i < total_blocks; i += blocks_per_run)
		for (int j = 0; j < blocks_per_run; j += 1)
			indices_a[i + j] = j;
	
	// do the block sorting runs
	while (sorted_zone_size <= (size / 2)) {
		unsigned int run_len = sorted_zone_size;
		int blocks1 = (sorted_zone_size / block_size);
		int blocks2 = blocks1;
		sorted_zone_size *= 2;
		
		for (int i = 0; i < total_blocks; i += (blocks1 + blocks2)) {
			BlockMerge(&arr[i * block_size], scratch, &indices_a[i], &indices_b[i], blocks1, blocks2, block_size);
			}

		std::swap(indices_a, indices_b);
		}

	FinalBlockSorting(arr, scratch, indices_a, (sorted_zone_size / block_size), block_size);
	free(allocated_memory);
	}

#define OUT(data) std::cout << data

double TimedTest_Shelfsort(ELEMENT* test_data, int test_size, int cycles) {
	double startTime = (double)clock() / CLOCKS_PER_SEC;
	for (int i=0; i<cycles; i++) {
		ShelfSort(&test_data[i * test_size], test_size);
		}
	double endTime = (double)clock() / CLOCKS_PER_SEC;
	return endTime - startTime;
	}

double TimedTest_stable_sort(ELEMENT* test_data, int test_size, int cycles) {
	double startTime = (double)clock() / CLOCKS_PER_SEC;
	for (int i=0; i<cycles; i++) {
		std::stable_sort(&test_data[i * test_size], &test_data[(i + 1) * test_size]);
		}
	double endTime = (double)clock() / CLOCKS_PER_SEC;
	return endTime - startTime;
	}

void WriteTime(double time, int total_size) {
	double ns = (time * 1000000000) / total_size;
	OUT((int)ns); OUT("."); OUT(((int)(ns * 100)) % 100);
	}

void RunTest(int log_size, int total_size) {
	int test_size = 1 << log_size;
	total_size = std::max(test_size, total_size);
	int cycles = total_size / test_size;
	total_size = cycles * test_size;
	ELEMENT* test_data = reinterpret_cast<ELEMENT*> (malloc(total_size * (sizeof(ELEMENT))));
	ELEMENT* test_data2 = reinterpret_cast<ELEMENT*> (malloc(total_size * (sizeof(ELEMENT))));

    OUT("beginning test: 2^"); OUT(log_size); OUT(" size blocks, ");
	OUT(cycles);  OUT(" cycles\n");

	srand(time(0));
	for (int i = 0; i < test_size; i++) {
		ELEMENT x = rand();
		test_data[i] = x;
		test_data2[i] = x;
		}
	
	double time1 = TimedTest_Shelfsort(test_data, test_size, cycles);
	double time1_sorted = -1;
	time1_sorted = TimedTest_Shelfsort(test_data, test_size, cycles);
	
	double time_std = TimedTest_stable_sort(test_data2, test_size, cycles);
	double time_std_sorted = -1;
	time_std_sorted = TimedTest_stable_sort(test_data2, test_size, cycles);

	bool correct_sort = true;
	for (int i=0; i < total_size; i++) {
		if (test_data[i] != test_data2[i]) {
			correct_sort = false;
			}
		}

	OUT("shelfsort time: "); WriteTime(time1, total_size);
	OUT(" / presorted: "); WriteTime(time1_sorted, total_size); OUT("\n");
	OUT("std::stable_sort time: "); WriteTime(time_std, total_size);
	OUT(" / presorted: "); WriteTime(time_std_sorted, total_size); OUT("\n");
	OUT("matching sort = "); OUT(correct_sort); OUT("\n\n");
	
	free(test_data);
	free(test_data2);
}

int main() {
	OUT("Shelfsort speed test\n");
	OUT("times are in ns/item\n\n");
	int total_size = 1 << 22;
	for (int size = 16; size <= 24; size++)
		{ RunTest(size, total_size); }
}

