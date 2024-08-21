#include "game.h"

unsigned int COMBO_COUNT        = 8;
unsigned short WIN_COMBOS[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
    {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {6, 4, 2},
};

unsigned int MOVE_COUNT    = 0x3C0000;
unsigned int PLAYER_CHOICE = 0x400000;
unsigned int TURN_TAKER    = 0x800000;
unsigned int GRAVE_MARKERS = 0xFF000000;

unsigned short WIN_O = 0x2A;
unsigned short WIN_X = 0x3F;

unsigned short CATS_GAME = 0x3;

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

unsigned short is_square_set(State *state, unsigned short square) {
  return get_square(state, square) >> 1 & 0x1;
}

void toggle_turn_taker(State *state) { *state ^= TURN_TAKER; }

/* Kernighan's Hamming weight */
unsigned short bitcount(unsigned short n) {
  int count = 0;
  while (n) {
    n &= (n - 1);
    count++;
  }
  return count;
}

void packbuf(State *state, unsigned short *buf, unsigned short a,
             unsigned short b, unsigned short c) {
  *buf |= get_square(state, a);
  *buf |= get_square(state, b) << 2;
  *buf |= get_square(state, c) << 4;
}

unsigned short is_win(State *state, unsigned short a, unsigned short b,
                      unsigned short c) {
  unsigned short buf = 0;
  packbuf(state, &buf, a, b, c);
  return (buf ^ WIN_O) == 0 || (buf ^ WIN_X) == 0;
}

unsigned short is_viable(State *state, unsigned short a, unsigned short b,
                         unsigned short c) {
  unsigned short buf = 0;
  packbuf(state, &buf, a, b, c);
  unsigned short hw = bitcount(buf);
  return hw < 2 || hw % 2 == 0;
}

unsigned short is_grave_marked(State *state, unsigned short ix) {
  return ((*state & GRAVE_MARKERS) >> 24) & (1 << ix);
}

void mark_grave(State *state, unsigned short ix) { *state |= 1 << (24 + ix); }

unsigned short is_cats_game(State *state) {
  return (*state & GRAVE_MARKERS) == GRAVE_MARKERS;
}

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

  if (move_count > 4) {
    for (unsigned int i = 0; i < COMBO_COUNT; i++) {
      unsigned short *combo = WIN_COMBOS[i];
      if (is_grave_marked(state, i)) {
        continue;
      }
      if (is_win(state, combo[0], combo[1], combo[3])) {
        return turn_taker + 1;
      }
      if (!is_viable(state, combo[0], combo[1], combo[3])) {
        mark_grave(state, i);
      }
      if (is_cats_game(state)) {
        return CATS_GAME;
      }
    }
  }

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

unsigned short get_square(State *state, unsigned short square) {
  unsigned int shift = square << 1;
  return *state >> shift;
}
