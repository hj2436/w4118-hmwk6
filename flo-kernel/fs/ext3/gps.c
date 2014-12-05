/*
 * flo-kernel/fs/ext3/gps.c
 * Backend ext3 gps-locations-related operations
 *
 * Copyright (C) 2014 V. Atlidakis, G. Koloventzos, A. Papancea
 *
 * COMS W4118 Fall 2014, Columbia University
 */
#include "ext3.h"
#include <linux/gps.h>

/*
 * ext3_set_gps_location:
 *
 * @inode:
 *
 * NOTE: Caller must hold the inode lock but does it?
 */
int ext3_set_gps_location(struct inode *inode)
{
	struct gps_location local;
	struct ext3_inode_info *ei;

	get_location(&local);

	ei = EXT3_I(inode);
	memcpy(&ei->i_latitude, &local.latitude, sizeof(unsigned long long));
	memcpy(&ei->i_longitude, &local.longitude, sizeof(unsigned long long));
	memcpy(&ei->i_accuracy, &local.accuracy, sizeof(unsigned int));
	printk(KERN_ERR "ext3_set_gps_location: I am trying...\n");

	return 0;
}

/*
 * ext3_get_gps_location:
 *
 * @inode:
 * @locagtion:
 *
 *NOTE: The caller must not hold i_lock and it isn't.
 *      This function is passed into get_gps_location
 *      inode operation callback which is wrapped with
 *      vfs_get_gps_location and exposed to the rest of
 *      the kernel.
 */
int ext3_get_gps_location(struct inode *inode, struct gps_location *location)
{
	struct gps_location local;
	struct ext3_inode_info *ei;

	spin_lock(&inode->i_lock);
	ei = EXT3_I(inode);
	memcpy(&local.latitude, &ei->i_latitude, sizeof(unsigned long long));
	memcpy(&local.longitude, &ei->i_longitude, sizeof(unsigned long long));
	memcpy(&local.accuracy, &ei->i_accuracy, sizeof(unsigned int));
	spin_unlock(&inode->i_lock);

	memcpy(location, &local, sizeof(local));

	return 0;
}
