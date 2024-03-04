# Configure the Kernel
make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} defconfig
make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} menuconfig  # (Optional) If you want to customize the configuration
make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} -j$(nproc)
make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} INSTALL_MOD_PATH=${OUTDIR}/rootfs modules_install
make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} INSTALL_PATH=${OUTDIR}/rootfs/boot install
cd ${OUTDIR}
mkdir -p rootfs/{dev,etc,proc,sys,usr/bin,usr/sbin,usr/lib,usr/libexec}
make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} defconfig
make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} menuconfig  # (Optional) Customize busybox configuration
make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} -j$(nproc)
make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} install
ldd ${OUTDIR}/busybox/bin/busybox
cd ${OUTDIR}/rootfs/dev
mknod -m 660 console c 5 1
mknod -m 660 null c 1 3
cd ${FINDER_APP_DIR}
make clean
make CROSS_COMPILE=${CROSS_COMPILE}
cp ${FINDER_APP_DIR}/writer ${OUTDIR}/rootfs/home
cp ${FINDER_APP_DIR}/finder.sh ${OUTDIR}/rootfs/home
# ... (Copy other scripts and executables)
sudo chown -R root:root ${OUTDIR}/rootfs
cd ${OUTDIR}/rootfs
find . | cpio -H newc -o > ${OUTDIR}/initramfs.cpio
gzip -c ${OUTDIR}/initramfs.cpio > ${OUTDIR}/initramfs.cpio.gz
