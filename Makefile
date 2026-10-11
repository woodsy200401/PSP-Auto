TARGET = pspauto
OBJS = main.o
CFLAGS = -O2 -G0 -Wall
LIBS = -lpspdebug -lpspdisplay -lpspge -lpspctrl
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = PSP Auto 0.1
PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
