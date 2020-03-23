TARGET = simple3D

# TODO: find out how to set icon for boot-menu (PSP_EBOOT_ICON ??)

MODULES = core/callback core/control core/graphics core/maths util main

SRC_DIR = $(addprefix src/,$(MODULES))
INCDIR = $(SRC_DIR)

BUILD_PRX=1

SRCS = $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.c))
OBJS = $(shell find . -name "*.c" | sed 's/\.c/\.o/g')

CFLAGS = -g -gstabs -G0
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =

# be careful with ordering
LIBS = -lpspgum -lpspgu -lpspvalloc -lpng -lm -lz

#LIBS = -lpng -lpspgum_vfpu -lpspvfpu -lpspgu -lpspaudiolib -lpspaudio -lpsprtc -lm -lpspvalloc

#DEPS = $(OBJS:.o=.d)

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = simple3D

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

#-include $(DEPS)

.PHONY: debug

debug: ;$(info $$MODUES is [${MODULES}])
	$(info $$SRC_DIR is [${SRC_DIR}])
	$(info $$OBJS is [${OBJS}])
	$(info $$INCDIR is [${INCDIR}])
