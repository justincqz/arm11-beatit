#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "arm11io.h"
#include "pipeline.h"

int main(int argc, char **argv) {
  State_t *state = new_state();
  assert(state);
  emu_read(argv[1],state);

  assert(!state->isTerminated);
  while(!state->isTerminated) {
    pipeline_circle(state);
  }

  emu_write(state->storage);

  delete_state(state);
  return 0;
}
