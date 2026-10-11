# PSP Auto v0.4 — GU renderer

This is an **untested source build**, not a precompiled EBOOT.

## Why this version is different
Previous versions drew pixels directly into VRAM. This version uses the PSP Graphics Utility (sceGu) to clear and draw the entire screen, with double buffering. It is intended to fix missing solid rectangles and text in PPSSPP.

## Upload to your existing GitHub repository
Replace root-level `main.c`, `Makefile`, and `font.h` with the files in this ZIP. Keep your existing `.github/workflows/build.yml`. Run the workflow and download the NEW EBOOT.PBP artifact.

## Controls
D-pad: choose tile; X: open; Circle: back; Start: exit.

The navigation and media displays are still demonstrations. No phone bridge exists yet.
