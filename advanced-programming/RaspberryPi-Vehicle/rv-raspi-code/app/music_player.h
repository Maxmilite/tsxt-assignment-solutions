#ifndef __MUSIC_PLAYER_H
#define __MUSIC_PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

const char music_list[][BUFFER_SIZE] = {
	"i_got_smoke",
	"ni_kan_dao_de_wo",
	"jie_jiu_shi_nv_wang"
};
const char jukebox_list[][BUFFER_SIZE] = {

};

char buf[BUFFER_SIZE];

void play_music(int x) {
	memset(buf, 0, sizeof(buf));
	system("killall -9 madplay");
	if (x < 0) return;
	sprintf(buf, "madplay /home/maxmilite/app/music/%s.mp3 -r &", music_list[x]);
	system(buf);
}

void play_jukebox(int x) {
	memset(buf, 0, sizeof(buf));
	system("killall -9 madplay");
	if (x < 0) return;
	sprintf(buf, "madplay /home/maxmilite/app/music/%s.mp3 -r &", jukebox_list[x]);
	system(buf);
}

#endif