<br />
<div align="center">
  <img src="nuklear.ico" width="64" height="64">
  <h3 align="center">NkArc</h3>
  <img src="https://img.shields.io/github/license/a1ive/NkArc">
  <img src="https://img.shields.io/github/actions/workflow/status/a1ive/NkArc/msbuild.yml">
</div>
<br />

NkArc is a versatile multi filesystem explorer for Windows based on the GRUB2 code.

## Supported Filesystems
(Read-Only, No Encryption)
- **Linux**: Btrfs (zlib, lzo, zstd), EROFS (lz4), ext2/3/4, F2FS, JFS, NILFS2, ReiserFS, XFS
- **Windows**: FAT12/16/32, exFAT, NTFS
- **macOS**: HFS, HFS+
- **UNIX**: MINIX1/2/3, QNX4/6, UFS1/2, ZFS (lz4, lzjb, zle, zlib, zstd)
- **CD/DVD**: ISO9660, UDF
- **Other**: Fbinst1.6/1.7, AFFS, AFS, BFS, LynxFS, romfs, SFS (Amiga Smart FileSystem)
- **Archive**: cpio, SquashFS, tar, WIM (lzx, Xpress), zip
- **Compression**: gz, lzma, lzop, xz, zstd
- **Virtual Disk**: VHD (Fixed/Dynamic), VHDX (Fixed/Dynamic), VDI (Fixed/Dynamic), QCOW1/2/3, DMG (zlib)
## Supported Dynamic Disks
- **Windows**: LDM (Logical Disk Manager)
- **Linux**: LVM (Logical Volume Manager)
- **RAID**: mdraid09, mdraid1x, raid5_rec, raid6_rec, dmraid_nvidia
## Supported Partition Tables
- MBR, GPT, Apple, BSD, ACRON, Amiga, DragonFly, DVH, Plan9, SUN
 
## Planned Features
- [ ] Dokan/WinFsp support
- [ ] APFS
- [ ] ReFS
- [ ] 7z
- [ ] VMDK
- [ ] lz4
- [ ] LUKS encrypted disk support
- [ ] Bitlocker encrypted disk support

## Credits
- [GRUB2](https://www.gnu.org/software/grub/)
- [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear) - GUI
- [Paper Icon](https://github.com/snwh/paper-icon-theme) - Icons
- [VirtualBox](https://www.virtualbox.org) - VHD & VHDX support
- [wimboot](https://github.com/ipxe/wimboot) - WIM support
- [Miniz](https://github.com/richgel999/miniz) - ZIP support
- [VC-LTL5 & YY-Thunks](https://github.com/Chuyu-Team)
