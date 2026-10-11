# PSP Auto v0.2
Graphical homebrew dashboard for PSP-1000 and PPSSPP.

## Update GitHub using Android
In your existing PSP-Auto GitHub repository, replace the old `main.c` and `Makefile` with the versions here, and **add `font.h`**. The existing `.github/workflows/build.yml` can stay unchanged if it already builds v0.1 successfully. Go to Actions > Build PSP Auto > Run workflow, download PSPAuto-PPSSPP artifact, extract EBOOT.PBP and open in PPSSPP.

*Works offline.* Navigation/map and phone/media information are mockups; the Android bridge is not built yet. `X` toggles a display label in Music and Settings but does not control playback or connectivity.

Requirements: PSPDEV toolchain, or GitHub Actions workflow.
