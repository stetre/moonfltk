
# To override the defaults, do (for example):
# $ make V=5.4
# $ make install V=5.4 PREFIX=/home/joe/local

# Lua version:
V=5.3
# Base install directory:
PREFIX = /usr/local
# Directory where to install Lua modules:
L_DIR = $(PREFIX)/share/lua/$(V)
# Directory where to install C modules:
C_DIR = $(PREFIX)/lib/lua/$(V)
# Directory where to install C headers:
H_DIR = $(PREFIX)/include
# Directory where to install C libraries:
S_DIR = $(PREFIX)/lib

ifeq ("$(V)","")
$(error Missing Lua version)
endif

ifeq ("$(PREFIX)","")
$(error Missing base install directory)
endif

# Translates 5.3 into 503:
LUAVER=$(shell lua -e "print((string.gsub("$(V)",'%.','0')))")

# Check the Lua version of the installed lua interpreter:
INSTLUAVER=$(shell lua -e "print(_VERSION:sub(5))")
ifneq ("$(V)","$(INSTLUAVER)")
#$(info $(V))
$(warning Warning: using Lua $(V) but Lua interpreter version is $(INSTLUAVER))
endif

Tgt	:= moonfltk
CSrc := $(wildcard *.c)
CxxSrc := $(wildcard *.cc)
CObjs := $(CSrc:.c=.o)
CxxObjs := $(CxxSrc:.cc=.o)

INCDIR = -I/usr/include
LIBDIR = -L/usr/lib

LIBS := -lfltk

# C/C++ common options
OPT	+= -O2
#OPT	+= -O0 -g
OPT	+= -Wall -Wextra -Wpedantic
OPT += -fpic
OPT += -DLUAVER=$(LUAVER)
ifeq ($(D),1)
OPT	+= -DDEBUG
OPT += -Wfatal-errors
OPT += -Wshadow -Wsign-compare -Wundef -Wwrite-strings
OPT	+= -Wdisabled-optimization
endif

ifeq ($(no-gl),1)
else
OPT += -DUSE_GL=1
LIBS += -lfltk_gl
endif
ifeq ($(no-images),1)
else
OPT += -DUSE_IMAGES=1
LIBS += -lfltk_images
endif


# C only options
COPT = $(OPT)
COPT += -std=gnu99
ifeq ($(D),1)
COPT += -Wdeclaration-after-statement
COPT += -Wmissing-prototypes -Wstrict-prototypes -Wnested-externs
COPT += -Wc++-compat -Wold-style-definition
endif
override CFLAGS = $(COPT) $(INCDIR)

# C++ only options
CXXOPT = $(OPT)
override CXXFLAGS = $(CXXOPT) $(shell fltk-config --cxxflags )

default: build

clean:
	@-rm -f *.so
	@-rm -f $(Tgt).symbols
	@-rm -f *~
	@-rm -f *.log
	@-rm -f *.o *.err *.map *.S

check:
	@-echo $(Tgt)".so will be installed in "$(C_DIR)
	@-echo $(Tgt)"/*.lua will be installed in "$(L_DIR)"/"$(Tgt)
	@-echo $(Tgt)".h will be installed in "$(H_DIR)
	@-echo lib$(Tgt)".so will be installed in "$(S_DIR)
	@-echo

install:
	@-mkdir -pv $(H_DIR)
	@-mkdir -pv $(C_DIR)
	@-mkdir -pv $(S_DIR)
	@-mkdir -pv $(L_DIR)
	@-cp -fpv $(Tgt).h $(H_DIR)
	@-cp -fpv $(Tgt).so $(C_DIR)
	@-ln -fsv $(C_DIR)/$(Tgt).so $(S_DIR)/lib$(Tgt).so
	@-cp -fpvr ../$(Tgt) $(L_DIR)


build:	clean $(Tgt) 

symbols: build
	@objdump -T $(Tgt).so > $(Tgt).symbols

$(Tgt):	$(CxxObjs) $(CObjs)
	@-$(CC) -shared -Wl,-soname,$(Tgt).so,$(LIBDIR) -o $(Tgt).so $(CObjs) $(CxxObjs) $(LIBS)
	@-rm -f $(CObjs)
	@-rm -f *.o
	@echo

