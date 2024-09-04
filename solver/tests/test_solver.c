#include <assert.h>
#include <stdio.h>

#include "game.h"
#include "solver.h"

int main() {
  State state;
  Move move;

  state = new_game(X);

  move = encode_move(X, BOTTOM_LEFT);
  transition(&state, &move);

  move = encode_move(O, best_move(&state));
  transition(&state, &move);

  move = encode_move(X, BOTTOM_RIGHT);
  transition(&state, &move);

  move = encode_move(O, best_move(&state));
  transition(&state, &move);

  move = encode_move(X, MIDDLE_MIDDLE);
  transition(&state, &move);

  move = encode_move(O, best_move(&state));
  transition(&state, &move);

  printf("%i", state);

  printf("✅ All tests have passed!\n");
  return 0;
}
