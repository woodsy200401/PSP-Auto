# PSP Auto 0.1 — PPSSPP build kit

This kit contains the original offline PSP homebrew prototype and a GitHub Actions build workflow.

## Build without installing a compiler

1. Create a GitHub repository and upload `main.c`, `Makefile`, and `.github/workflows/build.yml` in their exact relative paths (hidden `.github` directory must be included).
2. Open repository **Actions** → **Build PSP Auto** → **Run workflow**, or push a commit to main/master to trigger the build.
3. Open the successful run → **Artifacts** → download `PSPAuto-PPSSPP` ZIP. Extract `EBOOT.PBP`.
4. On Android, save `EBOOT.PBP` under `PSP/GAME/PSPAUTO/EBOOT.PBP` inside a storage folder accessible to PPSSPP. Alternatively, use PPSSPP's Browse command to navigate to the containing folder and open `EBOOT.PBP`.
5. Control with PPSSPP's on-screen D-pad, Cross, Circle, Start.

Offline placeholder screens only. The source has NOT been compiled or tested in this environment; GitHub Actions runs the official toolchain and will report compilation errors, if any.

Toolchain: https://github.com/pspdev/pspdev
