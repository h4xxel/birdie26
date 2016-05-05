# Makefile configurations
MAKEFLAGS	+=	--no-print-directory

#Binaries
BINFILE		=	birdie26

#General flags
LDFLAGS		+=	-rdynamic -pthread -lfcgi -ldl
CFLAGS		+=	-Wall -std=c99 -g -O0 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=600 -I$(TOPDIR)/include -pthread
ASFLAGS		+=

#Extra install targets
INSTARG		=	

#Makefile tools
MKDIR		=	mkdir -p
RMDIR		=	rmdir --ignore-fail-on-non-empty
CP		=	cp

ifeq (TARGET,windows)
LDFLAGS	+=	-lws2_32 -mwindows
BINEXT	=.exe
endif
