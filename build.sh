cp ../../kernel_configs/linux-next-default-config .config
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- LLVM=1 CC=/usr/bin/clang-14 HOSTCC=/usr/bin/clang-14 olddefconfig
make ARCH=arm64 LLVM=1 LLVM_IAS=1 \
  CROSS_COMPILE=aarch64-linux-gnu- \
  CC=clang-14 HOSTCC=clang-14 LD=ld.lld-14 \
  AR=llvm-ar-14 NM=llvm-nm-14 RANLIB=llvm-ranlib-14 \
  OBJCOPY=llvm-objcopy-14 OBJDUMP=llvm-objdump-14 STRIP=llvm-strip-14 \
  KCFLAGS="-march=armv8.5-a+pauth+memtag -flegacy-pass-manager -Xclang -load -Xclang $(realpath scripts/pac-mte/libPMCPass.so)" \
  -j"$(nproc)"
