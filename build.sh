cp ../../kernel_configs/linux-next-default-config .config
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -j$(nproc)
