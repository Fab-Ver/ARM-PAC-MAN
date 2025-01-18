#include "music.h"

#include "../timer/timer.h"

// Melody for Pill Eaten
NOTE pill_melody[] = {
    {c4, time_semicroma}, 
    {e4, time_semicroma}, 
    {g4, time_semicroma}
};

// Melody for Power Pill Eaten
NOTE power_pill_melody[] = {
    {g3, time_semicroma}, 
    {c4, time_semicroma}, 
    {e4, time_croma}, 
    {c4, time_semicroma}
};

// Melody for New Life
NOTE new_life_melody[] = {
    {c4, time_croma}, 
    {e4, time_croma}, 
    {g4, time_croma}, 
    {c5, time_semiminima}
};

// Melody for Killing the Ghost
NOTE kill_ghost_melody[] = {
    {g4, time_semicroma}, 
    {f4, time_semicroma}, 
    {e4, time_croma}, 
    {c4, time_semicroma}
};

typedef struct song_type{
	int current_note;
	int length;
	NOTE *melody;
} song_type;

song_type songs[4];

volatile BOOL song_ended = TRUE; 
volatile Song current_song;
volatile int current_note; 

void init_music(){
	songs[PILL].length = (sizeof(pill_melody) / sizeof(pill_melody[0]));
	songs[PILL].current_note = 0;
	songs[PILL].melody = pill_melody;
	
	songs[POWER].length = (sizeof(power_pill_melody) / sizeof(power_pill_melody[0]));
	songs[POWER].current_note = 0;
	songs[POWER].melody = power_pill_melody;
	
	songs[LIFE].length = (sizeof(new_life_melody) / sizeof(new_life_melody[0]));
	songs[LIFE].current_note = 0;
	songs[LIFE].melody = new_life_melody;
	
	songs[KILL_GHOST].length = (sizeof(kill_ghost_melody) / sizeof(kill_ghost_melody[0]));
	songs[KILL_GHOST].current_note = 0;
	songs[KILL_GHOST].melody = kill_ghost_melody;
}

void playNote(NOTE note)
{
	if(note.freq != pause)
	{
		reset_timer(1);
		init_timer(1, note.freq);
		enable_timer(1);
	}
	reset_timer(3);
	init_timer(3, note.duration);
	enable_timer(3);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM1->TCR != 0) || (LPC_TIM3->TCR != 0));
}

void playSong(){
	if(!song_ended){
		playNote(songs[current_song].melody[songs[current_song].current_note++]);
		if(songs[current_song].current_note >= songs[current_song].length){
			song_ended = TRUE;
		}
	}
}
void changeSong(Song song_id){
		song_ended = FALSE;
		current_song = song_id;
		songs[song_id].current_note = 0;
}
