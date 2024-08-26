#ifndef GAME_H
#define GAME_H

#define EMPTY 0x0
#define X     0x3
#define O     0x2

typedef unsigned int State;
typedef unsigned short Move;

enum Square {
  BOTTOM_LEFT   = 0,
  BOTTOM_MIDDLE = 1,
  BOTTOM_RIGHT  = 2,
  MIDDLE_LEFT   = 3,
  MIDDLE_MIDDLE = 4,
  MIDDLE_RIGHT  = 5,
  TOP_LEFT      = 6,
  TOP_MIDDLE    = 7,
  TOP_RIGHT     = 8
};

/**
 * Creates an empty state for a new game.
 * Player must choose to either play as `X`(1) or `O`(0).
 */
State new_game(unsigned short player_side);

/**
 * Resets the game state to an empty state.
 */
void restart_game(State *state);

/**
 * Encodes a move with the specified side and square.
 */
Move encode_move(unsigned short side, enum Square square);

/**
 * Updates the game state based on the provided move.
 *
 * Returns:
 * - 0 if the game can continue,
 * - 1 if the move results in a win for `O`,
 * - 2 if the move results in a win for `X`,
 * - 3 if the move results in a cat's game (draw),
 * - -1 if the move results in an invalid state.
 */
int transition(State *state, Move *move);

/**
 * Returns the current game move count.
 */
unsigned short get_move_count(State *state);

/**
 * Returns the player's choice of side.
 */
unsigned short get_player_choice(State *state);

/**
 * Returns which side is currently the turn taker.
 */
unsigned short get_turn_taker(State *state);

/**
 * Returns the value of the given square
 */
unsigned short get_square(State *state, enum Square square);

#endif
