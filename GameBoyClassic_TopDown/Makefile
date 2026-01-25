ifndef GBDK_HOME
	GBDK_HOME = ../gbdk/
endif

LCC = $(GBDK_HOME)bin/lcc -Wl-m
PNG2ASSETS = $(GBDK_HOME)bin/png2asset 
PNG2ASSETS_FLAGS = -pack_mode gb -map

# GBDK_DEBUG = ON
ifdef GBDK_DEBUG
	LCCFLAGS += -debug -v
endif

# You can set the name of the .gb ROM file here
PROJECTNAME    = Example
RESOURCES = res/

BINS	      = $(PROJECTNAME).gb
CSOURCES   := $(wildcard *.c)
ASMSOURCES := $(wildcard *.s)
RESSOURCES := $(wildcard res/*.c)
DESCRIPTORS := $(wildcard res/Descriptors/*.c)

PNG_FILES := $(wildcard res/*.png)
ASSETS		:= $(PNG_FILES:.png=.c)

# print:
# 	@echo "CSOURCES = $(CSOURCES)"
# 	@echo "RESSOURCES: $(RESSOURCES)"

all: $(BINS)

# compile.bat: Makefile
# 	@echo "REM Automatically generated from Makefile" > compile.bat
# 	@make -sn | sed y/\\//\\\\/ | sed s/mkdir\ \-p/mkdir/ | grep -v make >> compile.bat

# Compile and link all source files in a single call to LCC

$(ASSETS): %.c: %.png
	$(PNG2ASSETS) $< $(PNG2ASSETS_FLAGS)
	@echo $(PNG_FILES)


$(BINS): $(CSOURCES) $(ASMSOURCES) $(ASSETS) $(DESCRIPTORS) $(RESSOURCES)
	$(LCC) $(LCCFLAGS) -o $@ $(CSOURCES) $(RESSOURCES) $(DESCRIPTORS) $(ASMSOURCES)

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm *.noi *.rst

