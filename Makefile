TARGET = powermanga_psp
OBJS = afficheEtoiles.o \
	bonus.o \
	caractere.o \
	chaine.o \
	congratulations.o \
	configfile.o \
	courbe.o \
	displayFonte.o \
	eclairExecution.o \
	ennemis_execution.o \
	etbfonte1.o \
	explosionExecution.o \
	fntscrol.o \
	gardien12.o \
	gardien13.o \
	gardien14.o \
	gardiens.o \
	gfxbase.o \
	grille.o \
	initialiseGameOver.o \
	inits_game.o \
	keymap.o \
	lispreader.o \
	list.o \
	linuxroutines.o \
	main.o \
	menu.o \
	metors.o \
	onde.o \
	options.o \
	playanim.o \
	powermanga.o \
	powmangasub1.o \
	powmangasub2.o \
	score.o \
	sdl_mixer.o \
	sdl_tlk.o \
	special_enemy.o \
	tirsExecutions.o \
	tirsJoueur.o \
	touchesTriches.o \
	vaisseau.o \
	xwindow.o \
	gfxroutines.o

INCDIR = 
#CXX = psp-g++

CC       = psp-gcc
CXX      = psp-g++

CFLAGS = -D_GP2X -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBS=-lSDL_mixer -lSDL_image -lSDL -lpspaudiolib -lpspaudio -lmikmod -logg -lvorbis -lvorbisidec -lpspgum -lpspgu -lpsphprm -lpng -ljpeg -lz -lc -lm -lgcc -lstdc++
LIBDIR =
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = PowerManga

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
