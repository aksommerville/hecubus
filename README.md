# Hecubus

The dungeon master's helper, for TinyArcade. [https://tinycircuits.com/].
(in case you weren't a child of the 90's, "Hecubus" is a reference to this Kids in the Hall Skit [https://www.youtube.com/watch?v=rADdKqPNdaM]).

## Features

- Dice roller
- Count-down timer
- Stopwatch
- Stats record

## TODO

-[ ] Sound for countdown.
-[ ] Run countdown while other pages focussed.

## Requirements

- Arduino IDE 1.8.16.
- TinyArcade hardware.
- Optional: Linux, GLX.

Builds from a Makefile that calls out to Arduino IDE.
It's configured for my installation, expect to have to change a few constants in the Makefile.

If you set `NATIVE_PLATFORM:=linux-glx` in the Makefile, it also builds a version you can run on the PC.

- `make` to build everything.
- `make launch` to build for TinyArcade and launch via USB.
- - Beware, this wipes your default program. You'll have to rebuild the arcade menu later.
- `make run` to build the native binary and launch it.

"Native" is not an emulator! It's entirely possible a thing could run Native but fail on the hardware.

## License

Public Domain. Use at your own risk. NO WARRANTY.
