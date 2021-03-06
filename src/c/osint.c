#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "osint.h"
#include "vecx.h"

#include "AS3.h"

#define EMU_TIMER 30 /* the emulators heart beats at 20 milliseconds */
typedef unsigned char Uint8; 
//static SDL_Surface *screen = NULL;

static long screenx;
static long screeny;
static long scl_factor;
static long offx;
static long offy;

void osint_render(void){
	//SDL_FillRect(screen, NULL, 0);

	int v;
	for(v = 0; v < vector_draw_cnt; v++){
		Uint8 c = vectors_draw[v].color * 256 / VECTREX_COLORS;
		/*aalineRGBA(screen,
				offx + vectors_draw[v].x0 / scl_factor,
				offy + vectors_draw[v].y0 / scl_factor,
				offx + vectors_draw[v].x1 / scl_factor,
				offy + vectors_draw[v].y1 / scl_factor,
				c, c, c, 0xff);*/
	}
	//SDL_Flip(screen);
}

static char *romfilename = "rom.dat";
static char *cartfilename = NULL;

static void init(){
	FILE *f;
	if(!(f = fopen(romfilename, "rb"))){
		perror(romfilename);
		exit(EXIT_FAILURE);
	}
	if(fread(rom, 1, sizeof (rom), f) != sizeof (rom)){
		printf("Invalid rom length\n");
		exit(EXIT_FAILURE);
	}
	fclose(f);

	memset(cart, 0, sizeof (cart));
	if(cartfilename){
		FILE *f;
		if(!(f = fopen(cartfilename, "rb"))){
			perror(cartfilename);
			exit(EXIT_FAILURE);
		}
		fread(cart, 1, sizeof (cart), f);
		fclose(f);
	}
}
/*
void resize(int width, int height){
	long sclx, scly;

	screenx = width;
	screeny = height;
	//screen = SDL_SetVideoMode(screenx, screeny, 0, SDL_SWSURFACE | SDL_RESIZABLE);

//	sclx = ALG_MAX_X / screen->w;
//	scly = ALG_MAX_Y / screen->h;
	sclx = 1;
	scly = 1;
	scl_factor = sclx > scly ? sclx : scly;

	offx = (screenx - ALG_MAX_X / scl_factor) / 2;
	offy = (screeny - ALG_MAX_Y / scl_factor) / 2;
}*/

static void readevents(){
	/*SDL_Event e;
	while(SDL_PollEvent(&e)){
		switch(e.type){
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_VIDEORESIZE:
				resize(e.resize.w, e.resize.h);
				break;
			case SDL_KEYDOWN:
				switch(e.key.keysym.sym){
					case SDLK_ESCAPE:
						exit(0);
					case SDLK_a:
						snd_regs[14] &= ~0x01;
						break;
					case SDLK_s:
						snd_regs[14] &= ~0x02;
						break;
					case SDLK_d:
						snd_regs[14] &= ~0x04;
						break;
					case SDLK_f:
						snd_regs[14] &= ~0x08;
						break;
					case SDLK_LEFT:
						alg_jch0 = 0x00;
						break;
					case SDLK_RIGHT:
						alg_jch0 = 0xff;
						break;
					case SDLK_UP:
						alg_jch1 = 0xff;
						break;
					case SDLK_DOWN:
						alg_jch1 = 0x00;
						break;
					default:
						break;
				}
				break;
			case SDL_KEYUP:
				switch(e.key.keysym.sym){
					case SDLK_a:
						snd_regs[14] |= 0x01;
						break;
					case SDLK_s:
						snd_regs[14] |= 0x02;
						break;
					case SDLK_d:
						snd_regs[14] |= 0x04;
						break;
					case SDLK_f:
						snd_regs[14] |= 0x08;
						break;
					case SDLK_LEFT:
						alg_jch0 = 0x80;
						break;
					case SDLK_RIGHT:
						alg_jch0 = 0x80;
						break;
					case SDLK_UP:
						alg_jch1 = 0x80;
						break;
					case SDLK_DOWN:
						alg_jch1 = 0x80;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}*/
}

void osint_emuloop(){
	//Uint32 next_time = SDL_GetTicks() + EMU_TIMER;
	vecx_reset();
	for(;;){
		vecx_emu((VECTREX_MHZ / 1000) * EMU_TIMER, 0);
		readevents();

		{
			/*Uint32 now = SDL_GetTicks();
			if(now < next_time)
				SDL_Delay(next_time - now);
			else
				next_time = now;
			next_time += EMU_TIMER;*/
		}
	}
}
AS3_Val flash_keydown(void *data, AS3_Val args){
	return AS3_Int(0);
}
AS3_Val flash_tick(void *data, AS3_Val args){
	vecx_emu((VECTREX_MHZ / 1000) * EMU_TIMER, 0);
	return AS3_Int(0);
}

AS3_Val flash_reset(void *data, AS3_Val args){
	vecx_reset();
	return AS3_Int(0);
}

AS3_Val flash_init(void *data, AS3_Val args){
	FILE *f;
	if(!(f = fopen(romfilename, "rb"))){
		perror(romfilename);
		return AS3_Int(1);
	}
	if(fread(rom, 1, sizeof (rom), f) != sizeof (rom)){
		printf("Invalid rom length\n");
		return AS3_Int(2);
	}
	fclose(f);

	memset(cart, 0, sizeof (cart));
	if(cartfilename){
		FILE *f;
		if(!(f = fopen(cartfilename, "rb"))){
			perror(cartfilename);
			return AS3_Int(2);
		}
		fread(cart, 1, sizeof (cart), f);
		fclose(f);
	}
	vecx_reset();
	return AS3_Int(0);
}

int main (int argc, char **argv) {

	AS3_Val initMethod = AS3_Function(NULL, flash_init);
	AS3_Val tickMethod = AS3_Function(NULL, flash_tick);
	AS3_Val resetMethod = AS3_Function(NULL, flash_reset);

	AS3_Val vecxemu = AS3_Object(
		"init:AS3ValType, tick:AS3ValType, reset:AS3ValType", 
		initMethod, tickMethod, resetMethod
	);

	AS3_Release( initMethod );
	AS3_Release( tickMethod );
	AS3_Release( resetMethod );

	AS3_LibInit(vecxemu);

	return 0;
}


/*int main(int argc, char **argv){
	//SDL_Init(SDL_INIT_VIDEO);

	resize(330*3/2, 410*3/2);

	if(argc > 1)
		cartfilename = argv[1];

	init();

	osint_emuloop();

	return 0;
}
*/

