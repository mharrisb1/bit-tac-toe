#include "game.h"

unsigned int MOVE_COUNT    = 0x3C0000;
unsigned int PLAYER_CHOICE = 0x400000;
unsigned int TURN_TAKER    = 0x800000;

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
  unsigned short expected_turn = (*state & TURN_TAKER) >> 23;
  unsigned short actual_turn   = *move >> 4;

  if (expected_turn != actual_turn) {
    return -1;
  }

  unsigned int shift = 2 * (*move & 0xF);

  if ((*state & (2 << shift)) == 2) {
    return 0;
  }

  unsigned int n = (*state & MOVE_COUNT) >> 18;
  *state |= (n + 1) << 18;

  *state |= 2 << shift;
  *state |= shift == 0 ? actual_turn : actual_turn << (shift - 1);

  return 0;
}
