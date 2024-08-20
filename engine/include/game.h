#ifndef GAME_H
#define GAME_H

typedef unsigned int State;
typedef unsigned short Move;

/* Creates an empty state for a new game. Player must choose
 * to either play as `X`(1) or `O`(0)
 */
State new_game(unsigned short player_side);

/* Resets the game state to an empty state. */
void restart_game(State *state);

/* Encodes a move with the specified side and square. */
Move encode_move(unsigned short side, unsigned short square);

/* Transitions the game state with the specified move.
 * Returns 0 if the game can continue, 1 if the move results in a win, 
 * and -1 if the move results in an invalid state. 
 */
int transition(State *state, Move *move);

#endif
