#include <assert.h>
#include <stdio.h>

#include "game.h"

void test_new_game() {
  assert(new_game(0) == 0x800000);
  assert(new_game(1) == 0xC00000);
}

void test_restart_game() {
  State state = new_game(1);
  state       = 0x440003;
  restart_game(&state);
  assert(state == 0xC00000);

  state = new_game(0);
  state = 0x40002;
  restart_game(&state);
  assert(state == 0x800000);
}

void test_encode_move() {
  Move m1 = encode_move(0, 7);
  assert(m1 == 0x7);

  Move m2 = encode_move(1, 7);
  assert(m2 == 0x17);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 9; j++) {
      assert(encode_move(i, j) == i * 16 + j);
    }
  }
}

void test_invalid_transition() {
  State state = new_game(1);
  Move move   = encode_move(0, 0);

  assert(transition(&state, &move) == -1);
}

void test_transition() {
  State state = new_game(1);
  Move move   = encode_move(1, 0);

  transition(&state, &move);

  printf("%x\n", state);
  assert(state == 0xC40003);
}

void test_ignore_taken_square() {
  State state = new_game(1);
  Move move   = encode_move(1, 0);

  transition(&state, &move);
  assert(state == 0xC40003);

  transition(&state, &move);
  assert(state == 0xC40003);
}

int main() {
  test_new_game();
  test_restart_game();
  test_encode_move();

  test_invalid_transition();
  test_transition();
  test_ignore_taken_square();

  printf("✅ All tests passed!\n");
  return 0;
}
