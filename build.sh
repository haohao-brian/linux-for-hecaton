cp ../../kernel_configs/linux-next-default-config .config
make ARCH=arm64 CC=clang -j$(nproc) 
