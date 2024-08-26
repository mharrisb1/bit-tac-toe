#include <assert.h>
#include <stdio.h>

#include "game.h"

void test_get_move_count() {
  State state;
  for (unsigned int i = 0; i < 9; i++) {
    state = 0x3C0000 & (i << 18);
    assert(get_move_count(&state) == i);
  }
}

void test_get_player_choice() {
  State s1 = new_game(X);
  assert(get_player_choice(&s1) == X);

  State s2 = new_game(O);
  assert(get_player_choice(&s2) == O);
}

void test_get_turn_taker() {
  State state = 0x800000;
  assert(get_turn_taker(&state) == X);

  state = 0x0;
  assert(get_turn_taker(&state) == O);
}

void test_get_square() {
  State state;
  state = 0xB8EC;

  assert(get_square(&state, BOTTOM_LEFT) == EMPTY);
  assert(get_square(&state, BOTTOM_MIDDLE) == X);
  assert(get_square(&state, BOTTOM_RIGHT) == O);
  assert(get_square(&state, MIDDLE_LEFT) == X);
  assert(get_square(&state, MIDDLE_MIDDLE) == EMPTY);
  assert(get_square(&state, MIDDLE_RIGHT) == O);
  assert(get_square(&state, TOP_LEFT) == X);
  assert(get_square(&state, TOP_MIDDLE) == O);
  assert(get_square(&state, TOP_RIGHT) == EMPTY);
}

void test_new_game() {
  State s1 = new_game(X);
  assert(get_move_count(&s1) == 0);
  assert(get_player_choice(&s1) == X);
  assert(get_turn_taker(&s1) == X);

  State s2 = new_game(O);
  assert(get_move_count(&s2) == 0);
  assert(get_player_choice(&s2) == O);
  assert(get_turn_taker(&s2) == X);
}

void test_restart_game() {
  State s1 = new_game(X);
  Move m1  = encode_move(X, TOP_MIDDLE);

  transition(&s1, &m1);
  assert(get_move_count(&s1) == 1);
  assert(get_player_choice(&s1) == X);
  assert(get_turn_taker(&s1) == O);
  assert(get_square(&s1, TOP_MIDDLE) == X);

  restart_game(&s1);
  assert(get_move_count(&s1) == 0);
  assert(get_player_choice(&s1) == X);
  assert(get_turn_taker(&s1) == X);
  assert(get_square(&s1, TOP_MIDDLE) == EMPTY);

  State s2 = new_game(O);
  Move m2  = encode_move(X, TOP_MIDDLE);

  transition(&s2, &m2);
  assert(get_move_count(&s2) == 1);
  assert(get_player_choice(&s2) == O);
  assert(get_turn_taker(&s2) == O);
  assert(get_square(&s2, TOP_MIDDLE) == X);

  restart_game(&s2);
  assert(get_move_count(&s1) == 0);
  assert(get_player_choice(&s2) == O);
  assert(get_turn_taker(&s1) == X);
  assert(get_square(&s2, TOP_MIDDLE) == EMPTY);
}

void test_encode_move() {
  Move m1 = encode_move(O, TOP_MIDDLE);
  assert(m1 == 0x7);

  Move m2 = encode_move(X, TOP_MIDDLE);
  assert(m2 == 0x17);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 9; j++) {
      assert(encode_move(i, j) == i * 16 + j);
    }
  }
}

void test_invalid_transition() {
  State state = new_game(X);
  Move move   = encode_move(O, BOTTOM_LEFT);

  assert(transition(&state, &move) == -1);
}

void test_transition() {
  State state;
  Move move;
  int t;

  state = new_game(X);

  move = encode_move(X, BOTTOM_LEFT);
  t    = transition(&state, &move);

  assert(t == 0);
  assert(get_move_count(&state) == 1);
  assert(get_player_choice(&state) == X);
  assert(get_turn_taker(&state) == O);

  move = encode_move(O, BOTTOM_MIDDLE);
  t    = transition(&state, &move);

  assert(t == 0);
  assert(get_move_count(&state) == 2);
  assert(get_player_choice(&state) == X);
  assert(get_turn_taker(&state) == X);

  move = encode_move(X, BOTTOM_RIGHT);
  t    = transition(&state, &move);

  assert(t == 0);
  assert(get_move_count(&state) == 3);
  assert(get_player_choice(&state) == X);
  assert(get_turn_taker(&state) == O);

  move = encode_move(O, MIDDLE_LEFT);
  t    = transition(&state, &move);

  assert(t == 0);
  assert(get_move_count(&state) == 4);
  assert(get_player_choice(&state) == X);
  assert(get_turn_taker(&state) == X);

  move = encode_move(X, MIDDLE_MIDDLE);
  t    = transition(&state, &move);

  assert(t == 0);
  assert(get_move_count(&state) == 5);
  assert(get_player_choice(&state) == X);
  assert(get_turn_taker(&state) == O);

  assert(state == 0x1B5403BB);

  move = encode_move(O, MIDDLE_RIGHT);
  t    = transition(&state, &move);

  assert(t == 0);
  assert(get_move_count(&state) == 6);
  assert(get_player_choice(&state) == X);
  assert(get_turn_taker(&state) == X);

  move = encode_move(X, TOP_LEFT);
  t    = transition(&state, &move);

  assert(t == X); // X wins
  assert(get_move_count(&state) == 7);
  assert(get_player_choice(&state) == X);
  assert(get_turn_taker(&state) == O);
}

void test_ignore_taken_square() {
  State state;
  Move move;

  state = new_game(X);
  move  = encode_move(X, MIDDLE_RIGHT);

  transition(&state, &move);
  assert(get_move_count(&state) == 1);
  assert(get_player_choice(&state) == X);
  assert(get_turn_taker(&state) == O);

  move = encode_move(O, MIDDLE_RIGHT);

  transition(&state, &move);
  assert(get_move_count(&state) == 1);
  assert(get_player_choice(&state) == X);
  assert(get_turn_taker(&state) == O);
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
