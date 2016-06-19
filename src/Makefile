
ifdef MINGW_PREFIX
MINGW=1
else
LINUX=1
endif

# Lua version
LUAVER?=5.3

# Base install directory
ifdef LINUX
PREFIX?=/usr/local
endif
ifdef MINGW
PREFIX?=$(MINGW_PREFIX)
endif

# Directory where to install Lua modules
L_DIR=$(PREFIX)/share/lua/$(LUAVER)
# Directory where to install Lua C modules
C_DIR=$(PREFIX)/lib/lua/$(LUAVER)
# Directory where to install C headers
H_DIR=$(PREFIX)/include

ifeq ($(D),1)
DEBUG=1
endif

ifdef LINUX
INCDIR = -I/usr/include
LIBDIR = -L/usr/lib
LIBS =
endif
ifdef MINGW
LIBS = -llua
endif


ifeq ("$(platform)","linux")
LINUX=1
INCDIR = -I/usr/include
LIBDIR = -L/usr/lib
endif
ifeq ("$(platform)","mingw")
MINGW=1
INCDIR = -I/usr/local/include
LIBDIR = -L/usr/bin -L/usr/local/bin -L/usr/local/lib
endif

Tgt	:= moonfltk
CSrc := $(wildcard *.c)
CxxSrc := $(wildcard *.cc)
CObjs := $(CSrc:.c=.o)
CxxObjs := $(CxxSrc:.cc=.o)


# C/C++ common options
OPT	+= -O2
OPT	+= -Wall -Wextra -Wpedantic
OPT += -DLUAVER=$(LUAVER)
OPT += -DUSE_GL=1 -DUSE_IMAGES=1
ifdef LINUX
OPT += -DLINUX
OPT += -fpic
endif
ifdef MINGW
OPT += -DMINGW
OPT += -Wno-unused-parameter -Wno-long-long
endif
ifdef DEBUG
OPT	+= -DDEBUG
OPT += -Wfatal-errors
OPT += -Wshadow -Wsign-compare -Wundef -Wwrite-strings
OPT	+= -Wdisabled-optimization
endif
OPT +=  $(INCDIR)


# C only options
COPT = $(OPT)
COPT += -std=gnu99
ifdef DEBUG
COPT += -Wdeclaration-after-statement
COPT += -Wmissing-prototypes -Wstrict-prototypes -Wnested-externs
COPT += -Wc++-compat -Wold-style-definition
endif
override CFLAGS = $(COPT)

# C++ only options
CXXOPT = $(OPT)
override CXXFLAGS = $(CXXOPT) $(shell fltk-config  --use-gl --use-images --cxxflags )

override LDFLAGS = $(shell fltk-config --use-gl --use-images --ldflags )

default: build

where:
	@echo "PREFIX="$(PREFIX)
	@echo "LUAVER="$(LUAVER)
	@echo $(L_DIR)
	@echo $(C_DIR)
	@echo $(H_DIR)

clean:
	@-rm -f *.so *.dll *.o *.err *.map *.S *~ *.log
	@-rm -f $(Tgt).symbols

install:
	@-mkdir -pv $(H_DIR)
	@-mkdir -pv $(C_DIR)
	@-mkdir -pv $(L_DIR)
	@-cp -fpv $(Tgt).h $(H_DIR)
	@-cp -fpvr ../$(Tgt) $(L_DIR)
ifdef MINGW
	@-cp -fpv $(Tgt).dll $(C_DIR)
endif
ifdef LINUX
	@-cp -fpv $(Tgt).so $(C_DIR)
endif

uninstall:
	@-rm -f $(H_DIR)/$(Tgt).h
	@-rm -f $(C_DIR)/$(Tgt).so
	@-rm -f $(C_DIR)/$(Tgt).dll
	@-rm -fr $(L_DIR)/$(Tgt)

build:	clean $(Tgt) 

symbols: build
	@objdump -T $(Tgt).so > $(Tgt).symbols

$(Tgt):	$(CxxObjs) $(CObjs)
ifdef LINUX
	@-$(CXX) -shared -o $(Tgt).so $(CObjs) $(CxxObjs) $(LIBDIR) $(LDFLAGS) $(LIBS)
endif
ifdef MINGW
	@-$(CXX) -shared -o $(Tgt).dll $(CObjs) $(CxxObjs) $(LIBDIR) $(LDFLAGS) $(LIBS)
endif
	@-rm -f *.o
	@echo

