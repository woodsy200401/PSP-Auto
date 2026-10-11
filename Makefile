TARGET = pspauto
OBJS = main.o
CFLAGS = -O2 -G0 -Wall -D_PSP_FW_VERSION=600
LIBS = -lpspgum -lpspgu -lm
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = PSP Auto 0.4
PSPSDK ?= $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
