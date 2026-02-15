# XRT 2022.1 Ubuntu 24.04 LTS Compatibility Fork

This repository contains a compatibility port of XRT branch `202210.2.13.466` to run on Ubuntu 24.04 (kernel 6.8 series), with focus on enabling VCK5000 systems.

## Why this fork exists

VCK5000 platform files stopped at Vitis/Vivado/XRT version `2022.2`. Newer XRT releases do not run VCK5000 in this environment, so this fork keeps the older XRT line and adapts it to the newer Ubuntu kernel APIs.

## Upstream base

- Upstream project: `https://github.com/Xilinx/XRT`
- Upstream branch/tag base: `202210.2.13.466`
- Goal: preserve behavior of this XRT line while restoring build/install viability on Ubuntu 24.04

## Scope

- Keep XRT release level at `202210.2.13.466`.
- Adapt kernel driver code for newer kernel APIs without changing the packaging flow.
- Preserve the existing build/install loop used during DKMS bring-up.

## Build environment notes

The build was run with:

- `CXXFLAGS='-include cstdint -include cstdint -include array -include cstdint -include array -Wno-error=deprecated-declarations -include cstdint -include array -Wno-error=deprecated-declarations -Wno-error=overloaded-virtual -include cstdint -include array -Wno-error=deprecated-declarations -Wno-error=overloaded-virtual -Wno-error=use-after-free'`

Expected during `./build.sh`: test `13/16 Test #13: [xclbinutil]:ps-kernel` may fail; build is still carried through to package generation.

## Main code adjustments

Key functional fixes include:

- `src/runtime_src/core/pcie/driver/linux/xocl/userpf/xocl_drm.c`
  - Initialize `ddev->driver_features` from `ddev->driver->driver_features` for modern DRM open path behavior.
  - Set `.fop_flags = FOP_UNSIGNED_OFFSET` when available.
- `src/runtime_src/core/pcie/driver/linux/xocl/subdev/feature_rom.c`
  - Update `bin_attribute` callback/signature usage for current kernel type expectations.
- `src/runtime_src/core/pcie/driver/linux/xocl/userpf/xocl_drv.c`
  - Replace deprecated/removed `crc32c_le(...)` usage with `crc32c(...)`.
- `src/runtime_src/core/pcie/driver/linux/xocl/subdev/ulite_stub.c`
  - Add stub and related build wiring to keep build/install flow working on this platform.

In addition to these, multiple driver files under `src/runtime_src/core/pcie/driver/linux/xocl/` were adjusted for API compatibility.

## Rebuild/install loop used

From `build/Debug`:

1. `sudo apt remove xrt -yqq`
2. `make clean`
3. `rm -rf *.deb *.tar.gz`
4. `cd .. && ./build.sh`
5. `cd Debug && make package`
6. `sudo apt install ./xrt_202210.2.13.0_24.04-amd64-xrt.deb`

If DKMS fails, inspect `/var/lib/dkms/xrt/2.13.0/build/make.log`, patch source, and repeat.

## Additional notes

- Detailed porting notes are also in `PORTING_UBUNTU24.md`.
- Original upstream XRT readme remains in `README.rst`.
