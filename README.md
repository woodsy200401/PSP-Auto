# PSP Auto v0.5.1 — Bridge response fix

This update changes only the PSP networking client. Keep your existing `bridge.py` in Termux.

## Changes
- Sends the HTTP request using its actual length.
- Receives multiple chunks rather than assuming one `recv` returns the complete response.
- Separates HTTP headers from the body and verifies expected fields.
- Displays more specific errors: CONNECT FAILED, EMPTY RESPONSE, HTTP ERROR, UNEXPECTED DATA.

## Build
Replace `main.c`, `font.h`, and `Makefile` in the GitHub repository with the versions here. Run your existing GitHub Actions build and install the resulting EBOOT.PBP in PPSSPP.

## Test
Keep `python ~/bridge.py` running in Termux, check http://127.0.0.1:8765/status in Chrome, then launch PSP Auto v0.5.1 and press TRIANGLE. Open PHONE to see status.

This package has not been compiled or tested on your device. A real PSP-1000 will require a configurable remote bridge IP and compatible legacy Wi-Fi.
