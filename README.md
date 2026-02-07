# firmwareApp

Firmware application and bootloader for an STM32L0x3 target.

## Layout
- `app/`: main firmware application
- `bootloader/`: bootloader
- `shared/`: shared code used by both
- `libopencm3/`: external dependency (excluded from this repo)

## Prerequisites
- ARM GCC toolchain (`arm-none-eabi-gcc` and related tools)
- `libopencm3` checked out at `./libopencm3` (not included in this repo)

## Build
```sh
make -C app
make -C bootloader
```

## Notes
- Build outputs are ignored by git via `.gitignore`.
- If you want `libopencm3` tracked, consider adding it as a submodule.
