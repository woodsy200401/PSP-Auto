# PSP Auto v0.5 experimental phone bridge

This is a **test**, not a guaranteed working PPSSPP connection. It has not been compiled or run on your device.

## On Android
1. In Termux: `pkg install python termux-api`
2. Install the separate Termux:API Android app (matching your Termux installation source) and grant battery access if prompted.
3. Put `bridge.py` in Termux's home folder, run `python bridge.py`.
4. In your phone browser open `http://127.0.0.1:8765/status` and check that it shows battery and time.

## On GitHub
Replace the repository root `main.c`, `Makefile`, `font.h` with this package's files, run existing Actions build, and download the newest EBOOT.PBP.

## In PPSSPP
Launch v0.5 and press TRIANGLE to test localhost TCP connectivity. Open PHONE to read the status. If it says LOCALHOST UNREACHABLE or INET NOT SUPPORTED, this is a PPSSPP networking limitation or incompatibility, not necessarily your setup. No need to repeatedly rebuild.

## Real PSP compatibility
The PSP-1000 will not be able to use Android's `127.0.0.1`: that refers to the PSP itself. A real PSP build will need a configurable companion IP, PSP Wi-Fi infrastructure connection, and compatible isolated legacy wireless networking.

The track and artist are *test placeholders*, not real Spotify metadata. This experiment tests a data path only.
