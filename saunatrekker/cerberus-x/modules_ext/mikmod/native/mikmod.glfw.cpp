#include <mikmod.h>

#if defined(_WIN32)
#define MikMod_Sleep(ns) Sleep(ns / 1000)
#else
#include <unistd.h>  /* for usleep() */
#define MikMod_Sleep(ns) usleep(ns)
#endif

MODULE *mikModModule = NULL;

void cxMikmod_Init() {
 
	/* initialize MikMod threads */
	MikMod_InitThreads ();

	/* register all the drivers */
	MikMod_RegisterAllDrivers();

	/* register all the module loaders */
	MikMod_RegisterAllLoaders();

	/* init the library */
	/* md_mode |= DMODE_SURROUND | DMODE_INTERP | DMODE_FLOAT | DMODE_STEREO | DMODE_16BITS; */
	md_mode |= DMODE_INTERP;
	md_mode |= md_pansep=0;
	if (MikMod_Init("")) {
		fprintf(stderr, "Could not initialize sound, reason: %s\n",
				MikMod_strerror(MikMod_errno));
	}
	
}

void cxMikmod_Stop() {
	if (mikModModule) {
		Player_Free(mikModModule);
		mikModModule = NULL;
	}
	Player_Stop();
}

void cxMikmod_Play(String path) {
	
		cxMikmod_Stop();
	
		char file[path.Length()];
		std::wcstombs(file, path.Data(), path.Length());
	
		mikModModule = Player_Load( file , 64, 0);

		if (mikModModule) {
			printf("Playing %s (%d chn)\n", mikModModule->songname, (int) mikModModule->numchn);
			Player_Start(mikModModule);

		} else {
			fprintf(stderr, "Could not load module \"%s\", reason: %s\n",
					file, MikMod_strerror(MikMod_errno));
			std::terminate();
		}
		
}


void cxMikmod_Pause() {
	Player_TogglePause();
}
void cxMikmod_Resume() {
	Player_TogglePause();
}

void cxMikmod_Update() {
	MikMod_Update();
}

int cxMikmod_GetRow() {
	return Player_GetRow();
}

int cxMikmod_GetOrder() {
	return Player_GetOrder();
}

void cxMikmod_SetPosition(uint16_t v) {
	Player_SetPosition(v);
}

void cxMikmod_Mute(uint8_t mv) {
	Player_Mute(mv);
}

void cxMikmod_Unmute(uint8_t mv) {
	Player_Unmute(mv);
}

int cxMikmod_SetSpeed(uint8_t speed) {
	Player_SetSpeed(speed);
}

int cxMikmod_StereoSep(uint8_t sep) {
	md_mode |= md_pansep=sep;
}

int cxMikmod_SetTempo(uint8_t tempo) {
	Player_SetTempo(tempo);
}

void cxMikmod_Close() {
	cxMikmod_Stop();
	Player_Stop();
}
