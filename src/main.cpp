#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "system.h"
#include "game.h"

#define SAVE_DIR "/usr/local/home/Abbey/"

void checkSaveDirectory()
{
	DIR* dir = opendir(SAVE_DIR);
	if (dir) 
	{    	
    	closedir(dir);
	}
	else if (ENOENT == errno)
	{
		mkdir(SAVE_DIR, 0700);
	}
}

int main(int argc, char *argv[])
{
	#if RG350
	checkSaveDirectory();
	#endif
	
	Game *game = new Game();
	sys->init();
	game->mainLoop();
	sys->quit();
	return 0;
}