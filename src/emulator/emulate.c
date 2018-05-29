#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include"emuio.h"
#include"pipeline.h"

int main(int argc, char **argv) {

    /*read*/
    State_t *state = new_state();
    assert(state);
	emuread(argv[1],state);
    
    /*pipeline loop*/
    assert(!state->isTerminated);
    while(!state->isTerminated) {
        pipeline_circle(state);
    }
    
    /*output result*/
    emuwrite(state->storage);
    
    /*free memories*/
	delete_state(state);
    return 0;
}
