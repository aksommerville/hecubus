# Compile-time config for linux-glx

RPIINC:=-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
RPILIB:=-L/opt/vc/lib -lbcm_host 

CC+=-include src/linux-glx/arduino_glue.h $(RPIINC)
LDPOST:=-lm -lasound -lpthread $(RPILIB)
