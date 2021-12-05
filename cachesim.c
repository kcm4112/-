#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
void getParameter(int argc, char* argv[]);
void setRequestList();
void init_cache();


typedef	struct Request {
	long long address;
	char rw;
	long long data;
}RequestStruct;
typedef RequestStruct* Request;

typedef struct Block {
	int tag;
	long long data;
	bool dirty;
	bool valid;
	int rank;
}BlockStruct;
typedef BlockStruct* Block;

typedef struct Set {
	Block* block_pp;
}SetStruct;
typedef SetStruct* Set;

typedef	struct Cache {
	Set* set_pp;
}CacheStruct;
typedef CacheStruct* Cache;

typedef	struct Memory {
	int adress;
	int data;
}MemoryStruct;
typedef MemoryStruct* Memory;
Memory* memoryList;
Cache cache;
Request* requestList;
int cache_size = 0;
int set_size = 0;
int block_size = 0;
char* fileName;
int requestListCount = 0;
int main(int argc, char* argv[]) {

	getParameter(argc, argv);
	setRequestList();
	init();
	for (int i = 0; i < requestListCount; i++) {
		 //hit 아니다
		

		//insert to cache

	}
	


	//init request -> mm memory 동적할당

	//


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
			printf("cache size는 %d B입니다.\n", cache_size);
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
			printf("set size는 %d 입니다.\n", set_size);
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
			printf("block size는 %d B입니다.\n", block_size);
		}
		if (strncmp("-f", argv[i], 2) == 0) {
			int t_length = strlen(argv[i]);
			char* ptr = strtok(argv[i], "=");      // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
			fileName = strtok(NULL, "=");
			printf("%s", fileName);
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

	while (!feof(trace)) {
		fgets(temp, 101, trace);
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
	while (!feof(trace)) {
		fgets(temp, 101, trace);
		if (strcmp(temp, "\n") == 0) {
			break;
		}
		temp[strlen(temp) - 1] = '\0';
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
	int total_set = cache_size / block_size / set_size;
	cache->set_pp = malloc(sizeof(Set)*total_set);
	for (int i = 0; i < total_set; i++) {
		cache->set_pp[i] = malloc(sizeof(SetStruct));
		cache->set_pp[i]->block_pp = malloc(sizeof(Block)*set_size);
		for (int j = 0; j < set_size; j++) {
			cache->set_pp[i]->block_pp[j] = malloc(sizeof(BlockStruct));
			cache->set_pp[i]->block_pp[j]->data = 0;
			cache->set_pp[i]->block_pp[j]->dirty = 0;
			cache->set_pp[i]->block_pp[j]->rank = 0;
			cache->set_pp[i]->block_pp[j]->tag = 0;
			cache->set_pp[i]->block_pp[j]->valid = 0;
		}
	}

	memoryList = malloc(sizeof(Memory) * 1000);
	for (int i = 0; i < 1000; i++) {
		memoryList[i] = malloc(sizeof(MemoryStruct));
		memoryList[i]->adress = 0;
		memoryList[i]->data = 0;
	}

}

bool Hit() {


	//R

	//W

	return false;
}
#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
void getParameter(int argc, char* argv[]);
void setRequestList();
void init_cache();


typedef	struct Request {
	long long address;
	char rw;
	long long data;
}RequestStruct;
typedef RequestStruct* Request;

typedef struct Block {
	int tag;
	long long data;
	bool dirty;
	bool valid;
	int rank;
}BlockStruct;
typedef BlockStruct* Block;

typedef struct Set {
	Block* block_pp;
}SetStruct;
typedef SetStruct* Set;

typedef	struct Cache {
	Set* set_pp;
}CacheStruct;
typedef CacheStruct* Cache;

typedef	struct Memory {
	int adress;
	int data;
}MemoryStruct;
typedef MemoryStruct* Memory;
Memory* memoryList;
Cache cache;
Request* requestList;
int cache_size = 0;
int set_size = 0;
int block_size = 0;
char* fileName;
int requestListCount = 0;
int main(int argc, char* argv[]) {

	getParameter(argc, argv);
	setRequestList();
	init();
	for (int i = 0; i < requestListCount; i++) {
		 //hit 아니다
		

		//insert to cache

	}
	


	//init request -> mm memory 동적할당

	//


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
			printf("cache size는 %d B입니다.\n", cache_size);
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
			printf("set size는 %d 입니다.\n", set_size);
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
			printf("block size는 %d B입니다.\n", block_size);
		}
		if (strncmp("-f", argv[i], 2) == 0) {
			int t_length = strlen(argv[i]);
			char* ptr = strtok(argv[i], "=");      // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
			fileName = strtok(NULL, "=");
			printf("%s", fileName);
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

	while (!feof(trace)) {
		fgets(temp, 101, trace);
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
	while (!feof(trace)) {
		fgets(temp, 101, trace);
		if (strcmp(temp, "\n") == 0) {
			break;
		}
		temp[strlen(temp) - 1] = '\0';
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
	int total_set = cache_size / block_size / set_size;
	cache->set_pp = malloc(sizeof(Set)*total_set);
	for (int i = 0; i < total_set; i++) {
		cache->set_pp[i] = malloc(sizeof(SetStruct));
		cache->set_pp[i]->block_pp = malloc(sizeof(Block)*set_size);
		for (int j = 0; j < set_size; j++) {
			cache->set_pp[i]->block_pp[j] = malloc(sizeof(BlockStruct));
			cache->set_pp[i]->block_pp[j]->data = 0;
			cache->set_pp[i]->block_pp[j]->dirty = 0;
			cache->set_pp[i]->block_pp[j]->rank = 0;
			cache->set_pp[i]->block_pp[j]->tag = 0;
			cache->set_pp[i]->block_pp[j]->valid = 0;
		}
	}

	memoryList = malloc(sizeof(Memory) * 1000);
	for (int i = 0; i < 1000; i++) {
		memoryList[i] = malloc(sizeof(MemoryStruct));
		memoryList[i]->adress = 0;
		memoryList[i]->data = 0;
	}

}

bool Hit() {


	//R

	//W

	return false;
}
