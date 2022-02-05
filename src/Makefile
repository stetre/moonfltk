

ifdef MINGW_PREFIX
MINGW=1
else
 OS=$(shell uname)
 ifeq ($(OS),Darwin)
 MACOSX=1
 else
 LINUX=1
 endif
endif

# Lua version
LUAVER?=$(shell lua -e 'print(string.match(_VERSION, "%d+%.%d+") or "5.3")')
ifeq ($(LUAVER),)
  # lua-interpreter not found
  LUAVER=5.3
endif

FLTK_CONFIG = fltk-config

# Base install directory
ifdef MINGW
PREFIX?=$(MINGW_PREFIX)
else # LINUX, MACOSX
PREFIX?=/usr/local
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
INCDIR = -I/usr/include -I/usr/include/lua$(LUAVER)
LIBDIR = -L/usr/lib
LIBS =
endif
ifdef MACOSX
INCDIR = -I/usr/include -I/usr/include/lua$(LUAVER)
LIBDIR = -L/usr/lib
LIBS = -llua
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

ifdef MINGW
FLTK_INC_OPT =
else # LINUX, MACOSX
FLTK_INC_OPT = -DBEGIN_FLTK_INCLUDE='_Pragma("GCC diagnostic push") \
                                     _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")' \
               -DEND_FLTK_INCLUDE='_Pragma("GCC diagnostic pop")'
endif

Tgt	:= moonfltk
CSrc := $(wildcard *.c)
CxxSrc := $(wildcard *.cc)
CObjs := $(CSrc:.c=.o)
CxxObjs := $(CxxSrc:.cc=.o)


# C/C++ common options
OPT	+= -O2
OPT	+= -Wall -Wextra
OPT += -DCOMPAT53_PREFIX=moonfltk_compat_
OPT += -DLUAVER=$(LUAVER)
OPT += -DUSE_GL=1 -DUSE_IMAGES=1
OPT += $(FLTK_INC_OPT)
ifdef LINUX
OPT += -DLINUX
OPT += -fpic
endif
ifdef MACOSX
OPT += -DMACOSX
OPT += -fpic
endif
ifdef MINGW
OPT += -DMINGW
OPT += -Wno-unused-parameter -Wno-long-long
endif
ifdef DEBUG
OPT	+= -DDEBUG
OPT += -Wfatal-errors -Wpedantic
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
override CXXFLAGS = $(CXXOPT) $(shell $(FLTK_CONFIG)  --use-gl --use-images --cxxflags )

override LDFLAGS = $(shell $(FLTK_CONFIG) --use-gl --use-images --ldflags )

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
else # LINUX, MACOSX
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
ifdef MINGW
	@-$(CXX) -shared -o $(Tgt).dll $(CObjs) $(CxxObjs) $(LIBDIR) $(LDFLAGS) $(LIBS)
else # LINUX, MACOSX
	@-$(CXX) -shared -o $(Tgt).so $(CObjs) $(CxxObjs) $(LIBDIR) $(LDFLAGS) $(LIBS)
endif
	@-rm -f *.o
	@echo

