# Makefile configurations
MAKEFLAGS	+=	--no-print-directory

#Binaries
BINFILE		=	birdie26

#General flags
LDFLAGS		+=	-rdynamic -ldarnit_s
CFLAGS		+=	-Wall -std=c99 -g -O0 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=600 -I$(TOPDIR)/include
ASFLAGS		+=

#Extra install targets
INSTARG		=	

#Makefile tools
MKDIR		=	mkdir -p
RMDIR		=	rmdir --ignore-fail-on-non-empty
CP		=	cp

ifeq (TARGET,i586-mingw32msvc-)
LDFLAGS	+=	-lws2_32 -mwindows bin/res.res
BINEXT	=.exe

endif
i586-mingw32msvc-: bin
	@$(WINDRES) platform/windows/res.rc -O coff -o bin/res.res
