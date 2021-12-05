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
	int data;
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
Cache* cacheList;
Request* requestList;
int cache_size = 0;
int set_size = 0;
int block_size = 0;
char* fileName;
int requestListCount = 0;
int main(int argc, char* argv[]) {

	getParameter(argc, argv);
	setRequestList();
	init_cache();
	//for (int i = 0; i < requestListCount; i++) {
		//printf("addr : %lld R/W : %c, Data: %lld\n", requestList[i]->address, requestList[i]->rw, requestList[i]->data);

	//}
	for (int i = 0; i < requestListCount; i++) {
		requestList[i];//hit인지 아닌지

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

void init_cache() {
	



}

bool Hit() {


	//R

	//W

	return false;
}
