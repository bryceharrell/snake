#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static char get_board_at(game_state_t* state, int x, int y);
static void set_board_at(game_state_t* state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t* state, int snum);
static char next_square(game_state_t* state, int snum);
static void update_tail(game_state_t* state, int snum);
static void update_head(game_state_t* state, int snum);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t* state, int x, int y) {
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t* state, int x, int y, char ch) {
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t* create_default_state() {

    struct game_state_t *default_ptr;

    default_ptr = (struct game_state_t*) malloc(sizeof(struct game_state_t));
    
    default_ptr->x_size = 14;
    default_ptr->y_size = 10;
    default_ptr->num_snakes = 1;

    char *rows[10];

    char **gameboard = rows;

    gameboard = (char **) malloc(sizeof(char *) * 10);
    default_ptr->board = gameboard;
    for (int y = 0; y < 10; y++) {
      default_ptr->board[y] = (char *) malloc(sizeof(char) * 15);
      for (int x = 0; x < 14; x++) {
        if (x == 0 || y == 0 || x == 13 || y == 9) {
          set_board_at(default_ptr, x, y, '#');
        }
        else if (x == 9 && y == 2) {
          set_board_at(default_ptr, x, y, '*');
        }
        else if (x == 4 && y == 4) {
          set_board_at(default_ptr, x, y, 'd');
        }
        else if (x == 5 && y == 4) {
          set_board_at(default_ptr, x, y, '>');
        }
        else {
          set_board_at(default_ptr, x, y, ' ');
        }
      }
    }

    struct snake_t default_snakes[0]; 
    struct snake_t *snakes_ptr = default_snakes;
    
    snakes_ptr = (struct snake_t*) malloc(sizeof(struct snake_t));
    default_ptr->snakes = snakes_ptr;


    default_ptr->snakes->tail_x = 4;
    default_ptr->snakes->tail_y = 4;
    default_ptr->snakes->head_x = 5;
    default_ptr->snakes->head_y = 4;
    default_ptr->snakes->live = true;

    

  return default_ptr;
  
}

/* Task 2 */
void free_state(game_state_t* state) {

  for (int y = 0; y < state->y_size; y++) {
    free(state->board[y]);
  }
  free(state->board);
  free(state->snakes);
  free(state);

  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
  //fp = fopen("unit-test-out.snk", "w");
  for (int y = 0; y < state->y_size; y++) {
    fprintf(fp, "%s\n", state->board[y]);
  }
  //fclose(fp);
  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) {
  // TODO: Implement this function.
  if ((c == 'w') || (c == 'a') || (c == 's') || (c == 'd')) {
    return true;
  }
  return false;
}

static bool is_snake(char c) {
  // TODO: Implement this function.
  if ((c == 'w') || (c == 'a') || (c == 's') || (c == 'd') || (c == '^') || (c == '<') || (c == '>') || (c == 'v') || (c == 'x')) {
      return true;
    }
  return false;
}

static char body_to_tail(char c) {
  char b;
  b = '?';
  if (c == '^') {
    b = 'w';
  } else if (c == '<') {
    b = 'a';
  } else if (c == 'v') {
    b = 's';
  } else if (c == '>') {
    b = 'd';
  }
  return b;
}

static int incr_x(char c) {
  if ((c == '>') || (c == 'd')) {
    return 1;
  } else if ((c == '<') || (c == 'a')) {
    return -1;
  }
  return 0;
}

static int incr_y(char c) {
  if ((c == 'v') || (c == 's')) {
    return 1;
  } else if ((c == '^') || (c == 'w')) {
    return -1;
  }
  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t* state, int snum) {
  
snake_t snake = state->snakes[snum];
int x_cord = snake.head_x;
int y_cord = snake.head_y;
char head = get_board_at(state, x_cord, y_cord);
char target = get_board_at(state, x_cord + incr_x(head), y_cord + incr_y(head));  



return target;
}
/* Task 4.3 */
static void update_head(game_state_t* state, int snum) {
  snake_t snake = state->snakes[snum];
  //int x_cord = snake.head_x;
  //int y_cord = snake.head_y;
  
  char head = get_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y);
  int x_inc = incr_x(head);
  int y_inc = incr_y(head);
  
  set_board_at(state, state->snakes[snum].head_x + incr_x(head), state->snakes[snum].head_y + incr_y(head), head);
  state->snakes[snum].head_x = snake.head_x + x_inc;
  state->snakes[snum].head_y = snake.head_y + y_inc;
  
  return;
}

/* Task 4.4 */
static void update_tail(game_state_t* state, int snum) {
  snake_t snake = state->snakes[snum];
  int x_cord = snake.tail_x;
  int y_cord = snake.tail_y;

  char tail = get_board_at(state, x_cord, y_cord);
  int x_inc = incr_x(tail);
  int y_inc = incr_y(tail);

  char future_tail = get_board_at(state, x_cord + x_inc, y_cord + y_inc);
  char new_tail = body_to_tail(future_tail);
  set_board_at(state, x_cord + x_inc, y_cord + y_inc, new_tail);
  set_board_at(state, x_cord, y_cord, ' ');

  state->snakes[snum].tail_x = snake.tail_x + x_inc;
  state->snakes[snum].tail_y = snake.tail_y + y_inc;



  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  
  
    for (int snum = 0; snum < state->num_snakes; snum++) {
      
        if (next_square(state, snum) == ' ') {
          
          update_head(state, snum);
          update_tail(state, snum);
        } else if (next_square(state, snum) == '*') {
          update_head(state, snum);
          add_food(state);
        } else {
          set_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y, 'x');
          state->snakes[snum].live = false;
        }

    }

  return;
}

/* Task 5 */
game_state_t* load_board(char* filename) {
  FILE* f = fopen(filename, "r");
  //go through and determine board size
  //then use rewind to reset stream pointer to beginning
  
  char c = fgetc(f);
  int row_size = 0;
  while (c != '\n') {
    row_size++;
    c = fgetc(f);
  }

  rewind(f);
  int total_chars = 0;
  while(fgetc(f) != EOF) {
    total_chars++;
  }
  rewind(f);

  int col_size = total_chars / (row_size + 1);

  struct game_state_t *state;
  state = (struct game_state_t*) malloc(sizeof(struct game_state_t));

  state->x_size = row_size;
  state->y_size = col_size;

  char *rows[state->y_size];

  char **gameboard = rows;

  gameboard = (char **) malloc(sizeof(char *) * state->y_size);
  state->board = gameboard;

  for (int y = 0; y < state->y_size; y++) {
      state->board[y] = (char *) malloc(sizeof(char) * (state->x_size + 1));
      for (int x = 0; x < state->x_size + 1; x++) {
        if (x < state->x_size) {
        set_board_at(state, x, y, fgetc(f));
        } else {
           fgetc(f);
        }
      }
  }

  fclose(f);

  return state;

  }



/* Task 6.1 */
static void find_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  snake_t snake = state->snakes[snum];
  int tail_x_cord = snake.tail_x;
  int tail_y_cord = snake.tail_y;

  char curr_char = get_board_at(state, tail_x_cord, tail_y_cord);
  char prev_char = ' ';
  int curr_x_cord = tail_x_cord;
  int curr_y_cord = tail_y_cord;
  while (is_snake(curr_char)) {
    curr_x_cord = curr_x_cord + incr_x(curr_char);
    curr_y_cord = curr_y_cord + incr_y(curr_char);
    prev_char = curr_char;
    curr_char = get_board_at(state, curr_x_cord, curr_y_cord);
    
  }
  state->snakes[snum].head_x = curr_x_cord - incr_x(prev_char);
  
  state->snakes[snum].head_y = curr_y_cord - incr_y(prev_char);
  

  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.

  

  int snake_count = 0;
  for (int y = 0; y < state->y_size; y++) {
      for (int x = 0; x < state->x_size + 1; x++) {
        if (is_tail(get_board_at(state, x, y))) {
          snake_count++;
        }
      }
  }
  
  struct snake_t snakes[snake_count - 1]; 
  //snakes = (struct snake_t*) malloc(sizeof(struct snake_t);
  
  struct snake_t *snakes_ptr = snakes;

  
  snakes_ptr = (struct snake_t*) malloc(sizeof(struct snake_t) * snake_count);
  
  

  state->snakes = snakes_ptr;

  int snum = 0;
  for (int y = 0; y < state->y_size; y++) {
      for (int x = 0; x < state->x_size + 1; x++) {
        if (is_tail(get_board_at(state, x, y))) {
          state->snakes[snum].tail_x = x;
          
          state->snakes[snum].tail_y = y;
          
          find_head(state, snum);
          state->snakes[snum].live = true;
          snum++;
        }
      }
  }
 
  state->num_snakes = snum;

  return state;



  
}
