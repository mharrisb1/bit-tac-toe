#include "game.h"

static const unsigned int WIN_COMBOS_COUNT  = 8;
enum Square WIN_COMBOS[WIN_COMBOS_COUNT][3] = {
    {BOTTOM_LEFT, BOTTOM_MIDDLE, BOTTOM_RIGHT},
    {MIDDLE_LEFT, MIDDLE_MIDDLE, MIDDLE_RIGHT},
    {TOP_LEFT, TOP_MIDDLE, TOP_RIGHT},
    {BOTTOM_LEFT, MIDDLE_LEFT, TOP_LEFT},
    {BOTTOM_MIDDLE, MIDDLE_MIDDLE, TOP_MIDDLE},
    {BOTTOM_RIGHT, MIDDLE_RIGHT, TOP_RIGHT},
    {BOTTOM_LEFT, MIDDLE_MIDDLE, TOP_RIGHT},
    {TOP_LEFT, MIDDLE_MIDDLE, BOTTOM_RIGHT},
};

#define MOVE_COUNT_MASK    0x3C0000
#define PLAYER_CHOICE_MASK 0x400000
#define TURN_TAKER_MASK    0x800000
#define GRAVE_MARKERS_MASK 0xFF000000

#define WIN_O_MASK     0x2A
#define WIN_X_MASK     0x3F
#define CATS_GAME_MASK 0x1

void reset_move_count(State *state) { *state &= ~MOVE_COUNT_MASK; }

void set_move_count(State *state, unsigned short n) {
  reset_move_count(state);
  *state |= n << 18 & MOVE_COUNT_MASK;
}

void set_square_state(State *state, enum Square square,
                      unsigned short turn_taker) {
  unsigned int shift = square << 1;
  *state |= 2 << shift;
  *state |= shift == 0 ? turn_taker : turn_taker << shift;
}

unsigned short is_square_set(State *state, unsigned short square) {
  return get_square(state, square) >> 1 & 0x1;
}

void toggle_turn_taker(State *state) { *state ^= TURN_TAKER_MASK; }

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
  return (buf ^ WIN_O_MASK) == 0 || (buf ^ WIN_X_MASK) == 0;
}

unsigned short is_viable(State *state, unsigned short a, unsigned short b,
                         unsigned short c) {
  unsigned short buf = 0;
  packbuf(state, &buf, a, b, c);
  unsigned short hw = bitcount(buf);
  return hw < 2 || hw % 2 == 0;
}

unsigned short is_grave_marked(State *state, unsigned short ix) {
  return ((*state & GRAVE_MARKERS_MASK) >> 24) & (1 << ix);
}

void mark_grave(State *state, unsigned short ix) { *state |= 1 << (24 + ix); }

unsigned short is_cats_game(State *state) {
  return (*state & GRAVE_MARKERS_MASK) == GRAVE_MARKERS_MASK;
}

State new_game(unsigned short player_side) {
  return (2 | (player_side & 0x1)) << 22;
}

void restart_game(State *state) {
  *state = TURN_TAKER_MASK | (*state & PLAYER_CHOICE_MASK);
}

Move encode_move(unsigned short side, enum Square square) {
  return ((side & 0x1) << 4) | (square & 0xF);
}

int transition(State *state, Move *move) {
  unsigned short turn_taker = (*move >> 4) | O;
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
    for (unsigned int i = 0; i < WIN_COMBOS_COUNT; i++) {
      enum Square *combo = WIN_COMBOS[i];
      if (is_grave_marked(state, i)) {
        continue;
      }
      if (is_win(state, combo[0], combo[1], combo[2])) {
        return turn_taker;
      }
      if (!is_viable(state, combo[0], combo[1], combo[2])) {
        mark_grave(state, i);
      }
      if (is_cats_game(state)) {
        return CATS_GAME_MASK;
      }
    }
  }

  return 0;
}

unsigned short get_move_count(State *state) {
  return (*state & MOVE_COUNT_MASK) >> 18;
}

unsigned short get_player_choice(State *state) {
  return ((*state & PLAYER_CHOICE_MASK) >> 22) | O;
}

unsigned short get_turn_taker(State *state) {
  return ((*state & TURN_TAKER_MASK) >> 23) | O;
}

unsigned short get_square(State *state, enum Square square) {
  unsigned int shift = square << 1;
  return (*state >> shift) & X;
}
