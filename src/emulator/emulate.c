#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include"emuio.h"
#include"pipeline.h"

int main(int argc, char **argv) {
	
	/* add try catch to avoid errors*/

	int *instructions = malloc(sizeof(int)); /*need to redo*/
  	int lines = emuread(argv[1], instructions);
	Storage_t *storage = malloc(sizeof(Storage_t));
	State_t *state = newState(storage,instructions,lines);
	
	while(1) {
		fetch(state);
		decode(state);
		if(state->isTerminated) {
			emuwrite(state->storage);
			break;
		}
		execute(state);
	}

	assert(state->isTerminated);
	free(instructions);
	free(storage);
	free(state);
	return 0;
}
