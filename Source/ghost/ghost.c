#include "ghost.h"
#include "music/music.h"
#include "game/game.h"

typedef struct Node {
        int8_t x, y;
				int8_t g, h, f;
} Node;

volatile int8_t ghost_x = GHOST_INITIAL_X;
volatile int8_t ghost_y = GHOST_INITIAL_Y;
volatile bool ghost_is_alive = true; 
volatile GhostState ghost_state = CHASE; 

void draw_ghost(){
	uint8_t dx, dy; 
	for (dy = 0; dy < SQUARE_SIZE; dy++) {
   for (dx = 0; dx < SQUARE_SIZE; dx++) {
		 if(pacman_circle[dy][dx] == 1){
					LCD_SetPoint(SQUARE_SIZE * ghost_x + dx + INITIAL_X, SQUARE_SIZE * ghost_y + dy + INITIAL_Y, ghost_state == FRIGHTENED ? Blue2 : Red);
		 }
   }
  }
}

void clear_ghost(){
	uint8_t dx, dy; 
	for (dy = 0; dy < SQUARE_SIZE; dy++) {
   for (dx = 0; dx < SQUARE_SIZE; dx++) {
		 if(pacman_circle[dy][dx] == 1){
					LCD_SetPoint(SQUARE_SIZE * ghost_x + dx + INITIAL_X, SQUARE_SIZE * ghost_y + dy + INITIAL_Y, Black);
		 }
   }
  }
	if(map[ghost_y][ghost_x] == 2 || map[ghost_y][ghost_x] == 3) {
		draw_in_square(ghost_x, ghost_y, SQUARE_SIZE, map[ghost_y][ghost_x] == 2 ? Magenta : Green, pill_circle);
	}
}

void update_ghost_position(int8_t newX, int8_t newY){
	clear_ghost();
	ghost_x = newX;
	ghost_y = newY; 
	draw_ghost(); 
}

void move_ghost(){
	int8_t startX = ghost_x; 
	int8_t startY = ghost_y; 
	int8_t destX = ghost_state == CHASE ? pac_man_x : GHOST_INITIAL_X; 
	int8_t destY = ghost_state == CHASE ? pac_man_y : GHOST_INITIAL_Y; 
	a_star_search(startX, startY, destX, destY);
}

void check_collision(){
	if (pac_man_x == ghost_x && pac_man_y == ghost_y) {
		if (ghost_state == FRIGHTENED) {
			disable_interrupts();
			score += (uint16_t) GHOST_SCORE;
      ghost_is_alive = false;
			clear_ghost(); 
			changeSong(KILL_GHOST);
			enable_interrupts();
		} else {
			disable_interrupts();
      lives--;
			check_game_status();
			draw_lives();
			enable_interrupts(); 
    }
		 
  }
}

void a_star_search(int8_t startX, int8_t startY, int8_t destX, int8_t destY){

	if(!is_valid(startX, startY) || !is_valid(destX, destY)){
		return;
	}
	
	if(is_destination(startX, startY, destX, destY)){
		return;
	}
	uint16_t yt, xt;
	static Node nodes[ROWS][COLUMNS];
	for(yt=0; yt < ROWS; yt++){
		for(xt= 0; xt < COLUMNS; xt++){
			nodes[yt][xt].g = (int8_t) INT8_MAX;
			nodes[yt][xt].h = (int8_t) INT8_MAX;
			nodes[yt][xt].f = (int8_t) INT8_MAX;
			nodes[yt][xt].x = -1;
			nodes[yt][xt].y = -1;
		}
	}
	static uint8_t closedList[ROWS][COLUMNS];
	//Reset closed list 
  for (yt = 0; yt < ROWS; yt++) {
		for (xt = 0; xt < COLUMNS; xt++) {
			closedList[yt][xt] = 0;  
    }
  }
	
	static int8_t open_list[ROWS * COLUMNS][3]; 
	uint16_t open_list_count = 0;
	
	//Add start node to open list
	nodes[startY][startX].x = startX; 
	nodes[startY][startX].y = startY;
	nodes[startY][startX].g = 0;
  nodes[startY][startX].h = 0;
	nodes[startY][startX].f = 0;
	
	open_list[open_list_count][0] = 0;
	open_list[open_list_count][1] = startY;
	open_list[open_list_count][2] = startX;
	open_list_count++;
	
	
	while(open_list_count > 0){
		//Find node with the lowest F value in the open list 
		uint16_t current_index = 0; 
		uint16_t i;
		for(i = 1; i < open_list_count; i++){
			if(open_list[i][0] < open_list[current_index][0]){
				current_index = i; 
			}
		}
		
		int8_t curr_x = open_list[current_index][2];
		int8_t curr_y = open_list[current_index][1];
		
		//Remove the current node from the open list
		for(i=current_index; i < open_list_count - 1; i++){
			open_list[i][0] = open_list[i+1][0];
			open_list[i][1] = open_list[i+1][1];
			open_list[i][2] = open_list[i+1][2];
		}
		open_list_count--;
		
		//Add current node to closed list
		closedList[curr_y][curr_x] = 1;
		
		int8_t neighbors[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
		//Explore neighbors of current node
		for(i = 0; i < 4; i++){
			int8_t newX = curr_x + neighbors[i][0];
			int8_t newY = curr_y + neighbors[i][1];
			if(is_valid(newX, newY)){
				if(is_destination(newX, newY, destX, destY)){
					nodes[newY][newX].x = curr_x;
					nodes[newY][newX].y = curr_y;
					int8_t path_x = newX;
					int8_t path_y = newY;
					while(nodes[path_y][path_x].x != startX || nodes[path_y][path_x].y != startY){
						int8_t tempX = nodes[path_y][path_x].x;
            int8_t tempY = nodes[path_y][path_x].y;
            path_x = tempX;
            path_y = tempY;
					}
					update_ghost_position(path_x, path_y);
					return; 
				} else if (!closedList[newY][newX]){
					int8_t new_g = nodes[curr_y][curr_x].g + 1;
					int8_t new_h = get_distance(newX, newY, destX, destY);
					int8_t new_f = new_g + new_h;
					
					if(nodes[newY][newX].f == INT8_MAX || nodes[newY][newX].f > new_f){
						open_list[open_list_count][0] = new_f;
						open_list[open_list_count][1] = newY;
						open_list[open_list_count][2] = newX;
						open_list_count++;
						nodes[newY][newX].x = curr_x; 
						nodes[newY][newX].y = curr_y;
						nodes[newY][newX].g = new_g;
						nodes[newY][newX].h = new_h;
						nodes[newY][newX].f = new_f;
					}	
				}
			}
		}
	}
	return;
}

/**
 * Check if the ghost can perform a given move (no teleport allowed)
 */
bool is_valid(int8_t x, int8_t y){
    return (x > 0) && (x < COLUMNS - 1) && (y > 0) && (y < ROWS -1) && (map[y][x] != 1);
}


uint8_t get_distance(int8_t x1, int8_t y1, int8_t x2, int8_t y2){
	//Manhattan's Distance
	return abs(x1 - x2) + abs(y1 - y2);
}

bool is_destination(int8_t x, int8_t y, int8_t destX, int8_t destY){
	return ( x == destX && y == destY); 
}