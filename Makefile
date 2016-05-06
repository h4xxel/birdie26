first: all

TOPDIR	=	.
include config.mk

# Sub directories to build
SRCDIRS		=	src
#PLUGINSDIR	=	plugins

SUBDIRS		=	$(SRCDIRS) $(PLUGINSDIR)

#LIBS		=	$(addsuffix /out.a,$(SUBDIRS))

.PHONY: all clean release package mrproper
.PHONY: $(SUBDIRS)

all: bin $(SUBDIRS) $(TARGET)
	@echo " [ LD ] bin/$(BINFILE)$(BINEXT)"
	@$(CC) -o bin/$(BINFILE)$(BINEXT) $(CFLAGS) -Wl,--whole-archive $(addsuffix /out.a,$(SRCDIRS)) -Wl,--no-whole-archive $(LDFLAGS)
	@$(CP) -R res bin/
	
	@darnit-tmxconv res_precursor/arne.tmx bin/res/arne.ldmz
	
	@echo "Build complete."
	@echo 

release:
	@+make all
	@$(MKDIR) release/$(BINFILE)
	@$(CP) -R bin/$(BINFILE)$(BINEXT) release/$(BINFILE)/
	@$(CP) -R bin/res release/$(BINFILE)/
	@$(TARGET)strip release/$(BINFILE)/$(BINFILE)$(BINEXT)
	
package: release
	@(cd release; zip -r $(BINFILE).zip $(BINFILE))

clean: $(SUBDIRS)
	@echo " [ RM ] bin/"
	@$(RM) -R bin/
	
	@echo
	@echo "Source tree cleaned."
	@echo

mrproper:
	@+make clean
	@echo " [ RM ] release/"
	@$(RM) -R release/

bin:
	@$(MKDIR) $@

$(SUBDIRS):
	@echo " [ CD ] $(CURRENTPATH)$@/"
	@+make -C "$@" "CURRENTPATH=$(CURRENTPATH)$@/" $(MAKECMDGOALS)
