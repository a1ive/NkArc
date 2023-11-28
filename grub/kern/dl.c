/*
 *  NkArc
 *  Copyright (C) 2023 A1ive
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <dl.h>
#include <grub/types.h>
#include <grub/procfs.h>
#include <grub/misc.h>

#include "version.h"

static char*
version_get(grub_size_t* sz)
{
	char* buf = grub_xasprintf("%s v%s\n%s", NKGUI_NAME, NKGUI_VERSION_STR, NKGUI_COPYRIGHT);
	if (buf)
		*sz = grub_strlen(buf);
	else
		*sz = 0;
	return buf;
}

struct grub_procfs_entry version_info =
{
  .name = "version",
  .get_contents = version_get
};

void grub_module_init_progress(void);

void grub_module_init_procfs(void);
void grub_module_init_diskfilter(void);
void grub_module_init_windisk(void);
void grub_module_init_ldm(void);
void grub_module_init_lvm(void);
void grub_module_init_dm_nv(void);
void grub_module_init_mdraid09(void);
void grub_module_init_mdraid09_be(void);
void grub_module_init_mdraid1x(void);
void grub_module_init_raid5rec(void);
void grub_module_init_raid6rec(void);
void grub_module_init_loopback(void);

void grub_module_init_part_acorn(void);
void grub_module_init_part_amiga(void);
void grub_module_init_part_apple(void);
void grub_module_init_part_bsd(void);
void grub_module_init_part_dfly(void);
void grub_module_init_part_dvh(void);
void grub_module_init_part_gpt(void);
void grub_module_init_part_msdos(void);
void grub_module_init_part_plan(void);
void grub_module_init_part_sun(void);
void grub_module_init_part_sunpc(void);

void grub_module_init_affs(void);
void grub_module_init_afs(void);
void grub_module_init_bfs(void);
void grub_module_init_btrfs(void);
void grub_module_init_cpio(void);
void grub_module_init_cpio_be(void);
void grub_module_init_erofs(void);
void grub_module_init_exfat(void);
void grub_module_init_ext2(void);
void grub_module_init_f2fs(void);
void grub_module_init_fat(void);
void grub_module_init_fbfs(void);
void grub_module_init_hfs(void);
void grub_module_init_hfsplus(void);
void grub_module_init_hfspluscomp(void);
void grub_module_init_iso9660(void);
void grub_module_init_jfs(void);
void grub_module_init_lynxfs(void);
void grub_module_init_minix(void);
void grub_module_init_minix_be(void);
void grub_module_init_minix2(void);
void grub_module_init_minix2_be(void);
void grub_module_init_minix3(void);
void grub_module_init_minix3_be(void);
void grub_module_init_newc(void);
void grub_module_init_nilfs2(void);
void grub_module_init_ntfs(void);
void grub_module_init_ntfscomp(void);
void grub_module_init_odc(void);
void grub_module_init_qnx4(void);
void grub_module_init_qnx6(void);
void grub_module_init_reiserfs(void);
void grub_module_init_romfs(void);
void grub_module_init_sfs(void);
void grub_module_init_squash4(void);
void grub_module_init_tar(void);
void grub_module_init_udf(void);
void grub_module_init_ufs1(void);
void grub_module_init_ufs1_be(void);
void grub_module_init_ufs2(void);
void grub_module_init_wim(void);
void grub_module_init_xfs(void);
void grub_module_init_zfs(void);
void grub_module_init_zip(void);

void grub_module_init_gzio(void);
void grub_module_init_lz4io(void);
void grub_module_init_lzma(void);
void grub_module_init_lzopio(void);
void grub_module_init_xzio(void);
void grub_module_init_zstd(void);

void grub_module_init_dmg(void);
void grub_module_init_qcow(void);
void grub_module_init_vdi(void);
void grub_module_init_vhd(void);
void grub_module_init_vhdx(void);

void
grub_module_init(void)
{
	grub_module_init_progress();

	grub_module_init_procfs();
	grub_module_init_diskfilter();
	grub_module_init_windisk();
	grub_module_init_ldm();
	grub_module_init_lvm();
	grub_module_init_dm_nv();
	grub_module_init_mdraid09();
	grub_module_init_mdraid09_be();
	grub_module_init_mdraid1x();
	grub_module_init_raid5rec();
	grub_module_init_raid6rec();
	grub_module_init_loopback();

	grub_module_init_part_acorn();
	grub_module_init_part_amiga();
	grub_module_init_part_apple();
	grub_module_init_part_bsd();
	grub_module_init_part_dfly();
	grub_module_init_part_dvh();
	grub_module_init_part_gpt();
	grub_module_init_part_msdos();
	grub_module_init_part_plan();
	grub_module_init_part_sun();
	grub_module_init_part_sunpc();

	grub_module_init_affs();
	grub_module_init_afs();
	grub_module_init_bfs();
	grub_module_init_btrfs();
	grub_module_init_cpio();
	grub_module_init_cpio_be();
	grub_module_init_erofs();
	grub_module_init_exfat();
	grub_module_init_ext2();
	grub_module_init_f2fs();
	grub_module_init_fat();
	grub_module_init_fbfs();
	grub_module_init_hfs();
	grub_module_init_hfsplus();
	grub_module_init_hfspluscomp();
	grub_module_init_iso9660();
	grub_module_init_jfs();
	grub_module_init_lynxfs();
	grub_module_init_minix();
	grub_module_init_minix_be();
	grub_module_init_minix2();
	grub_module_init_minix2_be();
	grub_module_init_minix3();
	grub_module_init_minix3_be();
	grub_module_init_newc();
	grub_module_init_nilfs2();
	grub_module_init_ntfs();
	grub_module_init_ntfscomp();
	grub_module_init_odc();
	grub_module_init_qnx4();
	grub_module_init_qnx6();
	grub_module_init_reiserfs();
	grub_module_init_romfs();
	grub_module_init_sfs();
	grub_module_init_squash4();
	grub_module_init_tar();
	grub_module_init_udf();
	grub_module_init_ufs1();
	grub_module_init_ufs1_be();
	grub_module_init_ufs2();
	grub_module_init_wim();
	grub_module_init_xfs();
	grub_module_init_zfs();
	grub_module_init_zip();

	grub_module_init_gzio();
	//grub_module_init_lz4io();
	grub_module_init_lzma();
	grub_module_init_lzopio();
	grub_module_init_xzio();
	grub_module_init_zstd();

	grub_module_init_dmg();
	grub_module_init_qcow();
	grub_module_init_vdi();
	grub_module_init_vhd();
	grub_module_init_vhdx();

	grub_procfs_register("version", &version_info);
}

void grub_module_fini_progress(void);

void grub_module_fini_procfs(void);
void grub_module_fini_diskfilter(void);
void grub_module_fini_windisk(void);
void grub_module_fini_ldm(void);
void grub_module_fini_lvm(void);
void grub_module_fini_dm_nv(void);
void grub_module_fini_mdraid09(void);
void grub_module_fini_mdraid09_be(void);
void grub_module_fini_mdraid1x(void);
void grub_module_fini_raid5rec(void);
void grub_module_fini_raid6rec(void);
void grub_module_fini_loopback(void);

void grub_module_fini_part_acorn(void);
void grub_module_fini_part_amiga(void);
void grub_module_fini_part_apple(void);
void grub_module_fini_part_bsd(void);
void grub_module_fini_part_dfly(void);
void grub_module_fini_part_dvh(void);
void grub_module_fini_part_gpt(void);
void grub_module_fini_part_msdos(void);
void grub_module_fini_part_plan(void);
void grub_module_fini_part_sun(void);
void grub_module_fini_part_sunpc(void);

void grub_module_fini_affs(void);
void grub_module_fini_afs(void);
void grub_module_fini_bfs(void);
void grub_module_fini_btrfs(void);
void grub_module_fini_cpio(void);
void grub_module_fini_cpio_be(void);
void grub_module_fini_erofs(void);
void grub_module_fini_exfat(void);
void grub_module_fini_ext2(void);
void grub_module_fini_f2fs(void);
void grub_module_fini_fat(void);
void grub_module_fini_fbfs(void);
void grub_module_fini_hfs(void);
void grub_module_fini_hfsplus(void);
void grub_module_fini_hfspluscomp(void);
void grub_module_fini_iso9660(void);
void grub_module_fini_jfs(void);
void grub_module_fini_lynxfs(void);
void grub_module_fini_minix(void);
void grub_module_fini_minix_be(void);
void grub_module_fini_minix2(void);
void grub_module_fini_minix2_be(void);
void grub_module_fini_minix3(void);
void grub_module_fini_minix3_be(void);
void grub_module_fini_newc(void);
void grub_module_fini_nilfs2(void);
void grub_module_fini_ntfs(void);
void grub_module_fini_ntfscomp(void);
void grub_module_fini_odc(void);
void grub_module_fini_qnx4(void);
void grub_module_fini_qnx6(void);
void grub_module_fini_reiserfs(void);
void grub_module_fini_romfs(void);
void grub_module_fini_sfs(void);
void grub_module_fini_squash4(void);
void grub_module_fini_tar(void);
void grub_module_fini_udf(void);
void grub_module_fini_ufs1(void);
void grub_module_fini_ufs1_be(void);
void grub_module_fini_ufs2(void);
void grub_module_fini_wim(void);
void grub_module_fini_xfs(void);
void grub_module_fini_zfs(void);
void grub_module_fini_zip(void);

void grub_module_fini_gzio(void);
void grub_module_fini_lz4io(void);
void grub_module_fini_lzma(void);
void grub_module_fini_lzopio(void);
void grub_module_fini_xzio(void);
void grub_module_fini_zstd(void);

void grub_module_fini_dmg(void);
void grub_module_fini_qcow(void);
void grub_module_fini_vdi(void);
void grub_module_fini_vhd(void);
void grub_module_fini_vhdx(void);

void
grub_module_fini(void)
{
	grub_module_fini_progress();

	grub_module_fini_procfs();
	grub_module_fini_diskfilter();
	grub_module_fini_windisk();
	grub_module_fini_ldm();
	grub_module_fini_lvm();
	grub_module_fini_dm_nv();
	grub_module_fini_mdraid09();
	grub_module_fini_mdraid09_be();
	grub_module_fini_mdraid1x();
	grub_module_fini_raid5rec();
	grub_module_fini_raid6rec();
	grub_module_fini_loopback();

	grub_module_fini_part_acorn();
	grub_module_fini_part_amiga();
	grub_module_fini_part_apple();
	grub_module_fini_part_bsd();
	grub_module_fini_part_dfly();
	grub_module_fini_part_dvh();
	grub_module_fini_part_gpt();
	grub_module_fini_part_msdos();
	grub_module_fini_part_plan();
	grub_module_fini_part_sun();
	grub_module_fini_part_sunpc();

	grub_module_fini_affs();
	grub_module_fini_afs();
	grub_module_fini_bfs();
	grub_module_fini_btrfs();
	grub_module_fini_cpio();
	grub_module_fini_cpio_be();
	grub_module_fini_erofs();
	grub_module_fini_exfat();
	grub_module_fini_ext2();
	grub_module_fini_f2fs();
	grub_module_fini_fat();
	grub_module_fini_fbfs();
	grub_module_fini_hfs();
	grub_module_fini_hfsplus();
	grub_module_fini_hfspluscomp();
	grub_module_fini_iso9660();
	grub_module_fini_jfs();
	grub_module_fini_lynxfs();
	grub_module_fini_minix();
	grub_module_fini_minix_be();
	grub_module_fini_minix2();
	grub_module_fini_minix2_be();
	grub_module_fini_minix3();
	grub_module_fini_minix3_be();
	grub_module_fini_newc();
	grub_module_fini_nilfs2();
	grub_module_fini_ntfs();
	grub_module_fini_ntfscomp();
	grub_module_fini_odc();
	grub_module_fini_qnx4();
	grub_module_fini_qnx6();
	grub_module_fini_reiserfs();
	grub_module_fini_romfs();
	grub_module_fini_sfs();
	grub_module_fini_squash4();
	grub_module_fini_tar();
	grub_module_fini_udf();
	grub_module_fini_ufs1();
	grub_module_fini_ufs1_be();
	grub_module_fini_ufs2();
	grub_module_fini_wim();
	grub_module_fini_xfs();
	grub_module_fini_zfs();
	grub_module_fini_zip();

	grub_module_fini_gzio();
	//grub_module_fini_lz4io();
	grub_module_fini_lzma();
	grub_module_fini_lzopio();
	grub_module_fini_xzio();
	grub_module_fini_zstd();

	grub_module_fini_dmg();
	grub_module_fini_qcow();
	grub_module_fini_vdi();
	grub_module_fini_vhd();
	grub_module_fini_vhdx();

	grub_procfs_unregister(&version_info);
}
