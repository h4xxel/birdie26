# Makefile configurations
MAKEFLAGS	+=	--no-print-directory

#Binaries
BINFILE		=	gronsak

#General flags
LDFLAGS		+= -ldarnit_s -lm -lmodplug -lbz2
CFLAGS		+=	-Wall -std=c99 -g -O0 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=600 -I$(TOPDIR)/include
ASFLAGS		+=

#Extra install targets
INSTARG		=	

#Makefile tools
MKDIR		=	mkdir -p
RMDIR		=	rmdir --ignore-fail-on-non-empty
CP		=	cp

ifeq ($(TARGET),i586-mingw32msvc-)
LDFLAGS	+=	-static -lws2_32 -lopengl32 -lwinmm -lgdi32 -lshlwapi -lstdc++ -mwindows bin/res.res
BINEXT	=.exe
endif

i586-mingw32msvc-: bin
	@$(TARGET)windres platform/windows/res.rc -O coff -o bin/res.res

ifeq ($(TARGET),)
LDFLAGS	+= -lSDL -lasound -lGL
endif
