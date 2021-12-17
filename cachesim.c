#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>  

typedef	struct Request {
	long long address;
	char rw;
	long long data;
}RequestStruct;
typedef RequestStruct* Request;

typedef struct Block {
	long long tag;
	long long* dataList;
	bool dirty;
	bool valid;
	int rank;
}BlockStruct;
typedef BlockStruct* Block;

typedef struct Set {
	int rank_num;
	Block* block_pp;
}SetStruct;
typedef SetStruct* Set;

typedef	struct Cache {
	Set* set_pp;
}CacheStruct;
typedef CacheStruct* Cache;

typedef	struct Memory {
	int adress;
	int* data;
	struct Memory* next;
}MemoryStruct;
typedef MemoryStruct* Memory;

void getParameter(int argc, char* argv[]);
void setRequestList();
void init();
bool isHit(Request request);
bool isWrite(char c);
void findBlock();
void dataToCache(Request request);
void printCache();
void printAnalytics();
Memory findMemory(long long addr);
void makeMemory(long long addr);
int makeLog2(int t);
Memory* memoryList;
Memory memoryHead = NULL;

Cache cache;
Request* requestList;
int cache_size = 0;
int set_size = 0;
int block_size = 0;
char fileName[100] = { '\0' };
int requestListCount = 0;
int byte_offset = 0;
int block_offset = 0;
int tag;
int idx;
int index_count;
double hit_count = 0;
double miss_count = 0;
int total_dirty = 0;
int dirty_count = 0;
int print_count = 0;
int main(int argc, char* argv[]) {

	getParameter(argc, argv);
	setRequestList();
	init();
	for (int i = 0; i < requestListCount; i++) {
		if (isHit(requestList[i])) {
			Block b = cache->set_pp[idx]->block_pp[block_offset];
			if (isWrite(requestList[i]->rw)) {
				b->rank = cache->set_pp[idx]->rank_num;
				cache->set_pp[idx]->rank_num++;
				b->dataList[byte_offset >> 2] = requestList[i]->data;
				b->dirty = 1;
				//우선순위
			}
			else {
				b->rank = cache->set_pp[idx]->rank_num;
				cache->set_pp[idx]->rank_num++;
			}
		}
		else {

			//memory to cache
			findBlock();
			dataToCache(requestList[i]);

			Block b = cache->set_pp[idx]->block_pp[block_offset];
			if (isWrite(requestList[i]->rw)) {
				b->rank = cache->set_pp[idx]->rank_num;
				b->dataList[byte_offset >> 2] = requestList[i]->data;
				b->dirty = 1;
				b->tag = tag;
				b->valid = 1;
			}
			else {
				b->dirty = 0;
			}
			cache->set_pp[idx]->rank_num++;
		}
		//insert to cache

	}
	printCache();
	printAnalytics();
	return 0;
}

void getParameter(int argc, char* argv[]) {
	for (int i = 1; i < argc; i++) {
		//"-s"일 때 cache size 가져오는 부분
		if (strncmp("-s", argv[i], 2) == 0) {
			for (int j = 0; j < strlen(argv[i]); j++) {
				if (argv[i][j] == 61) {
					for (int k = j + 1; k < strlen(argv[i]); k++) {
						cache_size = cache_size * 10 + (int)(argv[i][k] - 48);
					}
				}
			}
		}

		//"-a"일 때 set size 가져오는 부분
		if (strncmp("-a", argv[i], 2) == 0) {
			for (int j = 0; j < strlen(argv[i]); j++) {
				if (argv[i][j] == 61) {
					for (int k = j + 1; k < strlen(argv[i]); k++) {
						set_size = set_size * 10 + (int)(argv[i][k] - 48);
					}
				}
			}
		}
		//"-b"일 때 block size 가져오는 부분
		if (strncmp("-b", argv[i], 2) == 0) {
			for (int j = 0; j < strlen(argv[i]); j++) {
				if (argv[i][j] == 61) {
					for (int k = j + 1; k < strlen(argv[i]); k++) {
						block_size = block_size * 10 + (int)(argv[i][k] - 48);
					}
				}
			}
		}
		if (strncmp("-f", argv[i], 2) == 0) {
			for (int j = 0; j < strlen(argv[i]); j++) {
				if (argv[i][j] == 61) {
					for (int k = j + 1; k < strlen(argv[i]); k++) {
						fileName[k - j - 1] = argv[i][k];
					}
				}
			}
			fileName[strlen(argv[i]) - 1] = '\0';
		}
	}
}

void setRequestList() {
	FILE* trace = 0;
	char temp[101];
	trace = fopen(fileName, "r");
	if (trace == NULL)
	{
		printf("can't open");
		fflush(stdout);
		exit(-1);
	}
	while (fgets(temp, 101, trace)) {
		if (strcmp(temp, "\n") == 0) {
			break;
		}

		requestListCount++;
	}
	fclose(trace);

	requestList = malloc(sizeof(Request) * requestListCount);

	int top = -1;
	trace = fopen(fileName, "r");
	if (trace == NULL)
	{
		printf("can't open");
		fflush(stdout);
		exit(-1);
	}
	for (int i = 0; i < requestListCount; i++) {
		fgets(temp, 101, trace);
		if (strcmp(temp, "\n") == 0) {
			break;
		}

		if (temp[strlen(temp) - 1] == '\n') {
			temp[strlen(temp) - 1] = '\0';
		}

		char* ptr = strtok(temp, " ");
		requestList[++top] = malloc(sizeof(RequestStruct));
		requestList[top]->address = strtoll(ptr, NULL, 16);
		ptr = strtok(NULL, " ");
		requestList[top]->rw = ptr[0];
		if (ptr[0] == 'W') {
			ptr = strtok(NULL, " ");
			requestList[top]->data = atoi(ptr);
		}
	}
	fclose(trace);
}

void init() {
	cache = malloc(sizeof(CacheStruct));
	index_count = cache_size / block_size / set_size;
	cache->set_pp = malloc(sizeof(Set) * index_count);
	for (int i = 0; i < index_count; i++) {
		cache->set_pp[i] = malloc(sizeof(SetStruct));
		cache->set_pp[i]->rank_num = 1;
		cache->set_pp[i]->block_pp = malloc(sizeof(Block) * set_size);
		for (int j = 0; j < set_size; j++) {
			cache->set_pp[i]->block_pp[j] = malloc(sizeof(BlockStruct));
			cache->set_pp[i]->block_pp[j]->dataList = malloc(sizeof(int) * block_size / 4);
			for (int k = 0; k < block_size / 4; k++) {
				cache->set_pp[i]->block_pp[j]->dataList[k] = 0;
			}
			cache->set_pp[i]->block_pp[j]->dirty = 0;
			cache->set_pp[i]->block_pp[j]->rank = INT_MAX;
			cache->set_pp[i]->block_pp[j]->tag = 0;
			cache->set_pp[i]->block_pp[j]->valid = 0;
		}
	}

}

bool isHit(Request request) {
	int offset_size = makeLog2(block_size);

	byte_offset = request->address & (block_size - 1);
	idx = (request->address >> offset_size) & (index_count - 1);

	tag = request->address - idx - byte_offset;

	for (int i = 0; i < set_size; i++) {
		Block b = cache->set_pp[idx]->block_pp[i];
		if (b->valid == 1 && b->tag == tag) {
			hit_count += 1;
			block_offset = i;
			return true;
		}
	}
	miss_count += 1;
	return false;
}


bool isWrite(char c) {
	if (c == 'R') return false;
	else return true;
}

void findBlock() {
	Set s = cache->set_pp[idx];
	int min = INT_MAX;
	int min_index = -1;
	//빈칸 찾기
	for (int i = 0; i < set_size; i++) {
		Block b = s->block_pp[i];
		if (b->valid == 0) {
			block_offset = i;
			return;
		}
		if (min > b->rank) {
			min = b->rank;
			min_index = i;
		}
	}
	block_offset = min_index;
}

void dataToCache(Request request) {
	Block b = cache->set_pp[idx]->block_pp[block_offset];
	int rank_num = cache->set_pp[idx]->rank_num;
	int offset_size = makeLog2(block_size);
	int cache_address = (b->tag) + (idx << offset_size);
	Memory mem = NULL;
	if (b->dirty == 1) {
		mem = findMemory(cache_address);
		//메모리 인덱스 찾기
		if (mem == NULL) {
			makeMemory(cache_address);
			for (int i = 0; i < block_size / 4; i++) {
				memoryHead->data[i] = b->dataList[i];
			}
		}
		else {
			for (int i = 0; i < block_size / 4; i++) {
				mem->data[i] = b->dataList[i];
			}
		}
		total_dirty += 1;
	}

	//read 일때 memory-> cache
	long long standardAddr = request->address & (-1 - (block_size - 1));
	mem = findMemory(standardAddr);
	if (mem == NULL) {
		makeMemory(standardAddr);
		for (int i = 0; i < block_size / 4; i++) {
			b->dataList[i] = memoryHead->data[i];
		}
	}
	else {
		for (int i = 0; i < block_size / 4; i++) {
			b->dataList[i] = mem->data[i];
		}
	}
	b->tag = tag;
	b->valid = 1;
	b->rank = rank_num;
}

void makeMemory(long long addr) {
	Memory temp = (Memory)malloc(sizeof(MemoryStruct));
	temp->adress = addr;
	temp->data = malloc(sizeof(int) * (block_size) / 4);
	if (temp == NULL) {
		printf("메모리 생성오류\n");
		return;
	}
	for (int j = 0; j < block_size / 4; j++) {
		temp->data[j] = 0;
	}
	temp->next = memoryHead;
	memoryHead = temp;
}

Memory findMemory(long long addr) {
	Memory temp = memoryHead;

	while (temp != NULL) {
		if (temp->adress == addr) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}


void printCache() {
	for (int i = 0; i < index_count; i++) {

		printf("%d: ", i);
		for (int j = 0; j < set_size; j++) {
			Block b = cache->set_pp[i]->block_pp[j];
			if (j != 0) {
				printf("   ");
			}

			for (int k = 0; k < block_size / 4; k++) {
				printf("%08llX ", cache->set_pp[i]->block_pp[j]->dataList[k]);
			}
			int v;
			int d;
			if (b->valid == true) v = 1;
			else v = 0;
			if (b->dirty == true) {
				dirty_count += 1;
				d = 1;
			}
			else d = 0;
			printf("v:%d d:%d", v, d);
			printf("\n");
		}
	}
	printf("\n");
}
void printAnalytics() {
	printf("total number of hits: %.f\n", hit_count);
	printf("total number of misses: %.f\n", miss_count);
	printf("miss rate: %.1f%%\n", miss_count / (hit_count + miss_count) * 100);
	printf("total number of dirty blocks: %d\n", dirty_count);
	printf("average memory access cycle: %.1f", (hit_count + miss_count * 201+ total_dirty * 200) / (hit_count + miss_count));
}
int makeLog2(int t) {
	int count = 0;
	while (t > 1) {
		t = t >> 1;
		count += 1;
	}
	return count;
}