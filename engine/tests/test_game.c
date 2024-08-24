#include <assert.h>
#include <stdio.h>

#include "game.h"

void test_get_move_count() {
  State state;
  for (unsigned int i = 0; i < 9; i++) {
    state = MOVE_COUNT & (i << 18);
    assert(get_move_count(&state) == i);
  }
}

void test_get_player_choice() {
  State s1 = new_game(1);
  assert(get_player_choice(&s1) == 1);

  State s2 = new_game(0);
  assert(get_player_choice(&s2) == 0);
}

void test_get_turn_taker() {
  State state = 0x800000;
  assert(get_turn_taker(&state) == 1);

  state = 0x0;
  assert(get_turn_taker(&state) == 0);
}

void test_get_square() {
  State state;
  state = 0xB8EC;

  assert(get_square(&state, 0) == EMPTY);
  assert(get_square(&state, 1) == X);
  assert(get_square(&state, 2) == O);
  assert(get_square(&state, 3) == X);
  assert(get_square(&state, 4) == EMPTY);
  assert(get_square(&state, 5) == O);
  assert(get_square(&state, 6) == X);
  assert(get_square(&state, 7) == O);
}

void test_new_game() {
  State s1 = new_game(1);
  assert(get_move_count(&s1) == 0);
  assert(get_player_choice(&s1) == 1);
  assert(get_turn_taker(&s1) == 1);

  State s2 = new_game(0);
  assert(get_move_count(&s2) == 0);
  assert(get_player_choice(&s2) == 0);
  assert(get_turn_taker(&s2) == 1);
}

void test_restart_game() {
  State s1 = new_game(1);
  Move m1  = encode_move(1, 7);

  transition(&s1, &m1);
  assert(get_move_count(&s1) == 1);
  assert(get_player_choice(&s1) == 1);
  assert(get_turn_taker(&s1) == 0);

  restart_game(&s1);
  assert(get_move_count(&s1) == 0);
  assert(get_player_choice(&s1) == 1);
  assert(get_turn_taker(&s1) == 1);

  State s2 = new_game(0);
  Move m2  = encode_move(1, 7);

  transition(&s2, &m2);
  assert(get_move_count(&s2) == 1);
  assert(get_player_choice(&s2) == 0);
  assert(get_turn_taker(&s2) == 0);

  restart_game(&s2);
  assert(get_move_count(&s1) == 0);
  assert(get_player_choice(&s2) == 0);
  assert(get_turn_taker(&s1) == 1);
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
  State state;
  Move move;
  int t;

  state = new_game(1);

  move = encode_move(1, 0);
  t    = transition(&state, &move);

  assert(t == 0);
  assert(get_move_count(&state) == 1);
  assert(get_player_choice(&state) == 1);
  assert(get_turn_taker(&state) == 0);

  move = encode_move(0, 1);
  t    = transition(&state, &move);

  assert(t == 0);
  assert(get_move_count(&state) == 2);
  assert(get_player_choice(&state) == 1);
  assert(get_turn_taker(&state) == 1);

  move = encode_move(1, 2);
  t    = transition(&state, &move);

  assert(t == 0);
  assert(get_move_count(&state) == 3);
  assert(get_player_choice(&state) == 1);
  assert(get_turn_taker(&state) == 0);
}

void test_ignore_taken_square() {
  State state;
  Move move;

  state = new_game(1);
  move  = encode_move(1, 5);

  transition(&state, &move);
  assert(get_move_count(&state) == 1);
  assert(get_player_choice(&state) == 1);
  assert(get_turn_taker(&state) == 0);

  move = encode_move(0, 5);

  transition(&state, &move);
  assert(get_move_count(&state) == 1);
  assert(get_player_choice(&state) == 1);
  assert(get_turn_taker(&state) == 0);
}

int main() {
  test_get_move_count();
  test_get_player_choice();
  test_get_turn_taker();
  test_get_square();

  test_new_game();
  test_restart_game();
  test_encode_move();

  test_invalid_transition();
  test_transition();
  test_ignore_taken_square();

  printf("✅ All tests passed!\n");
  return 0;
}
