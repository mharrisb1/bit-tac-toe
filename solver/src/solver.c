#include "solver.h"
#include "game.h"

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

/**
 * Bot is maximizing, player is minimizing
 */
int minimax(State *state, int bot_side, int player_side, int is_maximizing) {
  State state_copy = *state;
  if (is_maximizing == 1) {
    int best_score = -2;
    for (enum Square i = 0; i < TOP_RIGHT; i++) {
      if (get_square(&state_copy, i) == 0) {
        Move move = encode_move(bot_side, i);
        int t     = transition(&state_copy, &move);
        int score;
        if (t == bot_side) {
          score = 1;
        } else if (t == player_side) {
          score = -1;
        } else if (t == 1) { // cat's game
          score = 0;
        } else {
          score = minimax(&state_copy, bot_side, player_side, 0);
        }
        best_score = max(score, best_score);
      }
    }
    return best_score;
  } else {
    int best_score = 3;
    for (enum Square i = 0; i < TOP_RIGHT; i++) {
      if (get_square(&state_copy, i) == 0) {
        Move move = encode_move(player_side, i);
        int t     = transition(&state_copy, &move);
        int score;
        if (t == bot_side) {
          score = 1;
        } else if (t == player_side) {
          score = -1;
        } else if (t == 1) { // cat's game
          score = 0;
        } else {
          score = minimax(&state_copy, bot_side, player_side, 1);
        }
        best_score = min(score, best_score);
      }
    }
    return best_score;
  }
}

enum Square best_move(State *state) {
  State state_copy = *state;
  int best_score   = -2;
  int player_side  = get_player_choice(&state_copy);
  int bot_side     = player_side == X ? O : X;
  enum Square square;
  for (enum Square i = 0; i < TOP_RIGHT; i++) {
    if (get_square(&state_copy, i) == 0) {
      int score = minimax(&state_copy, bot_side, player_side, 1);
      if (score > best_score) {
        best_score = score;
        square     = i;
      }
    }
  }
  return square;
}
