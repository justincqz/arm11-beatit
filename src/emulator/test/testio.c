#include "../emuio.h"
#define maxn 10000

int main(void) {
	char *fileName = calloc(maxn,sizeof(char));
	scanf("%s", fileName);
	size_t size;

	uint32_t *instructions = emuread(fileName, &size);
	for (int i = 0; i < size; i ++) {
		printf("%u\n", *(instructions + i)); 
	}
}
