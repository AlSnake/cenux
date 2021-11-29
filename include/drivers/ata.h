#ifndef _DRIVERS_ATA_H_
#define _DRIVERS_ATA_H_

#include <stdint.h>
#include <drivers/device.h>

/* DATA PORTS */
#define ATA_PRIMARY_IO 		0x1F0
#define ATA_SECONDARY_IO 	0x170

/* Channels */
#define ATA_PRIMARY_CHANNEL 	0x0
#define ATA_SECONDARY_CHANNEL 	0x1

/* TYPE */
#define ATA_MASTER 		0x0
#define ATA_SLAVE 		0x1

/* Registers */
#define ATA_REG_DATA 		0x0
#define ATA_REG_ERROR		0x1
#define ATA_REG_FEATURES	0x1
#define ATA_REG_SECCOUNT0	0x2
#define ATA_REG_LBALO		0x3
#define ATA_REG_LBAMID		0x4
#define ATA_REG_LBAHI		0x5
#define ATA_REG_HDDEVSEL 	0x6
#define ATA_REG_COMMAND		0x7
#define ATA_REG_STATUS		0x7
#define ATA_REG_ALTSTATUS	0xC

/* Commands */
#define ATA_CMD_IDENTIFY	0xEC
#define ATA_CMD_READ_PIO	0x20
#define ATA_CMD_WRITE_PIO	0x30
#define ATA_CMD_CACHE_FLUSH	0xE7

/* STATUS */
#define ATA_SR_BSY		0x80
#define ATA_SR_DRDY		0x40
#define ATA_SR_DF		0x20
#define ATA_SR_DSC		0x10
#define ATA_SR_DRQ		0x08
#define ATA_SR_CORR		0x04
#define ATA_SR_IDX		0x02
#define ATA_SR_ERR		0x01

/* INFO */
#define ATA_IDENT_DEVICETYPE	0
#define ATA_IDENT_CYLINDERS	2
#define ATA_IDENT_HEADS		6
#define ATA_IDENT_SECTORS	12
#define ATA_IDENT_SERIAL	20
#define ATA_IDENT_MODEL		54
#define ATA_IDENT_CAPABILITIES	98
#define ATA_IDENT_FIELDVALID	106
#define ATA_IDENT_MAX_LBA	120
#define ATA_IDENT_COMMANDSETS	164
#define ATA_IDENT_MAX_LBA_EXT	200

/* EXTRAS */
#define PRIMARY_MASTER_DRIVE 	ATA_PRIMARY_CHANNEL << 1 | ATA_MASTER
#define PRIMARY_SLAVE_DRIVE	ATA_PRIMARY_CHANNEL << 1 | ATA_SLAVE
#define SECONDARY_MASTER_DRIVE	ATA_SECONDARY_CHANNEL << 1 | ATA_MASTER
#define SECONDARY_SLAVE_DRIVE	ATA_SECONDARY_CHANNEL << 1 | ATA_SLAVE

struct ata_device {
	uint8_t drive;
};

int32_t ata_read(struct device* dev, uint32_t lba, void* buf, uint32_t sector);
int32_t ata_write(struct device* dev, uint32_t lba, void* buf, uint32_t sector);
void ata_init();

#endif
