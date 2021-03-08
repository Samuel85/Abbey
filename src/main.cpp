
#if VITA
#define VITASDK
#endif

#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "system.h"
#include "game.h"

#if VITA
#include <psp2/io/stat.h> 
#endif


#define SAVE_DIR "/usr/local/home/Abbey/"
#define VITA_SAVE_DIR "ux0:data/Abbey"

void checkSaveDirectory()
{
#if RG350		
	DIR* dir = opendir(SAVE_DIR);
	if (dir) {    	
		closedir(dir);
	}
	else if (ENOENT == errno){

		mkdir(SAVE_DIR, 0700);
	}	
#endif	
	
#if VITA
	struct SceIoStat * dirStat = (SceIoStat*)malloc(sizeof(SceIoStat));	
	if(sceIoGetstat(VITA_SAVE_DIR , dirStat) < 0){
		sceIoMkdir(VITA_SAVE_DIR, 0777);
	}
	free(dirStat);
#endif			
}

int main(int argc, char *argv[])
{
#if defined(VITA) || defined(RG350)	
	checkSaveDirectory();
#endif
	
	Game *game = new Game();
	sys->init();
	game->mainLoop();
	sys->quit();
	return 0;
}