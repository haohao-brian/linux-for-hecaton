cp ../../kernel_configs/linux-next-default-config .config
make ARCH=arm64 CC=clang -j$(nproc) KCFLAGS="-march=armv8.5-a+pauth+memtag -flegacy-pass-manager -Xclang -load -Xclang $(realpath scripts/pac-mte/libPMCPass.so)"
