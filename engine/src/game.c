#include "game.h"

unsigned int MOVE_COUNT    = 0x3C0000;
unsigned int PLAYER_CHOICE = 0x400000;
unsigned int TURN_TAKER    = 0x800000;

// Private
void reset_move_count(State *state) { *state &= ~MOVE_COUNT; }

void set_move_count(State *state, unsigned short n) {
  reset_move_count(state);
  *state |= n << 18;
}

void set_square_state(State *state, unsigned short square,
                      unsigned short turn_taker) {
  unsigned int shift = square << 1;
  *state |= 2 << shift;
  *state |= shift == 0 ? turn_taker : turn_taker << (shift - 1);
}

unsigned int is_square_set(State *state, unsigned short square) {
  unsigned int shift = square << 1;
  return *state & 2 << shift >> 1;
}

void toggle_turn_taker(State *state) { *state ^= TURN_TAKER; }

// Public
State new_game(unsigned short player_side) {
  return (2 | (player_side & 0x1)) << 22;
}

void restart_game(State *state) {
  *state = TURN_TAKER | (*state & PLAYER_CHOICE);
}

Move encode_move(unsigned short side, unsigned short square) {
  return ((side & 0x1) << 4) | (square & 0xF);
}

int transition(State *state, Move *move) {
  unsigned short turn_taker = *move >> 4;
  unsigned int square       = *move & 0xF;
  unsigned int move_count   = get_move_count(state);

  if (get_turn_taker(state) != turn_taker) {
    return -1;
  }

  if (is_square_set(state, square) == 1) {
    return 0;
  }

  set_move_count(state, ++move_count);
  set_square_state(state, square, turn_taker);
  toggle_turn_taker(state);

  return 0;
}

unsigned short get_move_count(State *state) {
  return (*state & MOVE_COUNT) >> 18;
}

unsigned short get_player_choice(State *state) {
  return (*state & PLAYER_CHOICE) >> 22;
}

unsigned short get_turn_taker(State *state) {
  return (*state & TURN_TAKER) >> 23;
}
