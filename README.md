# PSP Auto v0.3 framebuffer fix

This replaces the v0.2 source and addresses the likely reason why lines appeared but filled rectangles and text did not: writes to the cached VRAM alias were not consistently visible to the display. The renderer now writes to PSP's uncached VRAM alias (0x44000000) while the display uses 0x04000000. It also corrects the callback start signature.

## Updating GitHub from a phone

Keep your existing .github/workflows/build.yml unchanged. Replace the repository root `main.c` and `Makefile` with these files. Keep `font.h` as-is (or replace it with this identical version). Run the GitHub Actions workflow and download the resulting EBOOT.PBP.

This is an untested candidate fix, not a verified executable. If the dashboard still shows only lines, share the new screenshot.
