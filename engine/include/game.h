#ifndef GAME_H
#define GAME_H

typedef unsigned int State;
typedef unsigned short Move;

unsigned int EMPTY;
unsigned int X;
unsigned int O;
unsigned int MOVE_COUNT;
unsigned int PLAYER_CHOICE;
unsigned int TURN_TAKER;
unsigned int GRAVE_MARKERS;

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
Move encode_move(unsigned short side, unsigned short square);

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
unsigned short get_square(State *state, unsigned short square);

#endif
