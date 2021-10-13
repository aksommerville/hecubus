# Compile-time config for linux-glx

CC+=-include src/linux-glx/arduino_glue.h
LDPOST:=-lm -lGL -lGLX -lX11 -lpulse -lpulse-simple -lpthread
