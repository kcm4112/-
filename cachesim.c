#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


void getParameter(int argc, char* argv[]);
void getRequest();

typedef	struct Request {
	char* address;
	char rw;
	char* data;
}Request;

typedef	struct Cache {
	char* tag;
	int** data;
	bool dirty;
	bool valid;
}Cache;

typedef	struct Memory {
	int adress;
	int data;
}Memory;

//구조체 선언
//reqeust
//cache
//memory
//set 구조체???

Memory* memoryList;
Cache* cacheList;
Request* RequestList;
int cache_size = 0;
int set_size = 0;
int block_size = 0;
char* fileName;

int main(int argc, char* argv[]) {

	getParameter(argc, argv);

	FILE* fp = 0;
	
	fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		printf("can't open");
		fflush(stdout);
		exit(-1);
	}
	
	

	getRequest()

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
			char* context = NULL;
			char* ptr = strtok_s(argv[i], "=", &context);      // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
			fileName = strtok_s(NULL, "=", &context);
			printf("%s", fileName);
		}
	}
}

void setRequestList() {


}

void getRequest() {


}
