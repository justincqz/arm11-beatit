#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include"emuio.h"
#include"pipeline.h"

/*TODO: error report*/
int main(int argc, char **argv) {

    /*read*/
    size_t lines;
    uint32_t *instructions = emuread(argv[1], &lines); 
    assert(instructions);
    Storage_t *storage = new_storage();
    State_t *state = new_state(storage,instructions,lines);
    assert(storage);
    assert(state);
    
    /*pipeline loop*/
    assert(!state->isTerminated);
    while(!state->isTerminated) {
        pipeline_circle(state);
    }
    
    /*output result*/
    emuwrite(state->storage);
    
    /*free memories*/
    free(instructions);
    free(storage);
    free(state);
    return 0;
}
