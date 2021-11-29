#include <drivers/ata.h>
#include <cpu/ports.h>
#include <kernel/kprintf.h>
#include <kernel/panic.h>
#include <mm/kmem.h>
#include <drivers/device.h>

static void ide_select_drive(uint8_t bus, uint8_t drive)
{
	if (bus == ATA_PRIMARY_CHANNEL)
		if (drive == ATA_MASTER)
			outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xA0);
		else
			outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xB0);
	else if (drive == ATA_MASTER)
		outb(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, 0xA0);
	else
		outb(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, 0xB0);
}

static int8_t ide_identify(uint8_t bus, uint8_t drive, void *buf)
{
	uint16_t port = 0;

	ide_select_drive(bus, drive);

	if (bus == ATA_PRIMARY_CHANNEL)
		port = ATA_PRIMARY_IO;
	else
		port = ATA_SECONDARY_IO;

	outb(port + ATA_REG_SECCOUNT0, 0);
	outb(port + ATA_REG_LBALO, 0);
	outb(port + ATA_REG_LBAMID, 0);
	outb(port + ATA_REG_LBAHI, 0);
	outb(port + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

	uint8_t status = insb(port + ATA_REG_STATUS);
	if (status) {
		while ((insb(port + ATA_REG_STATUS) & ATA_SR_BSY) != 0) {
			if (insb(port + ATA_REG_LBAMID) != 0 ||
			    insb(port + ATA_REG_LBAHI) != 0) // NON ATA DRIVE
				return -1;
		}

	retry:
		status = insb(port + ATA_REG_STATUS);
		if (status & ATA_SR_ERR) {
			kprintf("BUS %d DRIVE %d has error\n", bus, drive);
			return status;
		}

		while (!(status & ATA_SR_DRQ))
			goto retry;

		uint16_t *ptr = buf;
		for (uint32_t i = 0; i < 256; i++) {
			*ptr = insw(port + ATA_REG_DATA);
			ptr++;
		}
	}

	return 0;
}

static void ide_400ns_delay(uint16_t port)
{
	for (uint32_t i = 0; i < 14; i++)
		insb(port + ATA_REG_ALTSTATUS);
}

static void ide_poll(uint16_t port)
{
	ide_400ns_delay(port);

pool:
	uint8_t status = insb(port + ATA_REG_STATUS);
	if (status & ATA_SR_BSY)
		goto pool;

retry:
	status = insb(port + ATA_REG_STATUS);
	if (status & ATA_SR_ERR)
		panic("ATA ERR DEVICE FAILURE\n");

	if (!(status & ATA_SR_DRQ))
		goto retry;
}

/* 28bit LBA MODE */
static int8_t ata_read_sector(uint8_t drive, uint32_t lba, void *buf)
{
	uint16_t port = 0;

	switch (drive) {
	case (ATA_PRIMARY_CHANNEL << 1 | ATA_MASTER):
		port = ATA_PRIMARY_IO;
		drive = ATA_MASTER;
		break;
	case (ATA_PRIMARY_CHANNEL << 1 | ATA_SLAVE):
		port = ATA_PRIMARY_IO;
		drive = ATA_SLAVE;
		break;
	case (ATA_SECONDARY_CHANNEL << 1 | ATA_MASTER):
		port = ATA_SECONDARY_CHANNEL;
		drive = ATA_MASTER;
		break;
	case (ATA_SECONDARY_CHANNEL << 1 | ATA_SLAVE):
		port = ATA_SECONDARY_CHANNEL;
		drive = ATA_SLAVE;
		break;
	default:
		kprintf("UNKNOWN DRIVE: %d\n", drive);
		return -1;
	}

	uint8_t seldri = (drive == ATA_MASTER ? 0xE0 : 0xF0);
	// uint8_t slavebit = (drive == ATA_MASTER ? 0x0 : 0x1);

	outb(port + ATA_REG_HDDEVSEL, (seldri | (uint8_t)(lba >> 24 & 0x0F)));
	outb(port + 1, 0x0);
	outb(port + ATA_REG_SECCOUNT0, 1);
	outb(port + ATA_REG_LBALO, (uint8_t)(lba));
	outb(port + ATA_REG_LBAMID, (uint8_t)(lba >> 8));
	outb(port + ATA_REG_LBAHI, (uint8_t)(lba >> 16));
	outb(port + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

	// READY FOR READING
	ide_poll(port);

	uint16_t *ptr = buf;
	for (uint32_t i = 0; i < 256; i++) {
		*ptr = insw(port + ATA_REG_DATA);
		ptr++;
	}

	ide_400ns_delay(port);

	return 0;
}

/* 28bit LBA MODE */
static int8_t ata_write_sector(uint8_t drive, uint32_t lba, void *buf)
{
	uint16_t port = 0;

	switch (drive) {
	case (ATA_PRIMARY_CHANNEL << 1 | ATA_MASTER):
		port = ATA_PRIMARY_IO;
		drive = ATA_MASTER;
		break;
	case (ATA_PRIMARY_CHANNEL << 1 | ATA_SLAVE):
		port = ATA_PRIMARY_IO;
		drive = ATA_SLAVE;
		break;
	case (ATA_SECONDARY_CHANNEL << 1 | ATA_MASTER):
		port = ATA_SECONDARY_CHANNEL;
		drive = ATA_MASTER;
		break;
	case (ATA_SECONDARY_CHANNEL << 1 | ATA_SLAVE):
		port = ATA_SECONDARY_CHANNEL;
		drive = ATA_SLAVE;
		break;
	default:
		kprintf("UNKNOWN DRIVE: %d\n", drive);
		return -1;
	}

	uint8_t seldri = (drive == ATA_MASTER ? 0xE0 : 0xF0);
	// uint8_t slavebit = (drive == ATA_MASTER ? 0x0 : 0x1);

	outb(port + ATA_REG_HDDEVSEL, (seldri | (uint8_t)(lba >> 24 & 0x0F)));
	outb(port + 1, 0x0);
	outb(port + ATA_REG_SECCOUNT0, 1);
	outb(port + ATA_REG_LBALO, (uint8_t)(lba));
	outb(port + ATA_REG_LBAMID, (uint8_t)(lba >> 8));
	outb(port + ATA_REG_LBAHI, (uint8_t)(lba >> 16));
	outb(port + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

	// READY FOR READING
	ide_poll(port);

	uint16_t *ptr = buf;
	for (uint32_t i = 0; i < 256; i++) {
		outw(port + ATA_REG_DATA, *ptr);
		ptr++;
	}

	outb(port + ATA_REG_COMMAND, ATA_CMD_CACHE_FLUSH);

	ide_400ns_delay(port);

	return 0;
}

static int32_t validate_drive(uint8_t drive)
{
	switch (drive) {
	case (PRIMARY_MASTER_DRIVE):
	case (PRIMARY_SLAVE_DRIVE):
	case (SECONDARY_MASTER_DRIVE):
	case (SECONDARY_SLAVE_DRIVE):
		return 0;
	}

	return -1;
}

int32_t ata_read(struct device *dev, uint32_t lba, void *buf, uint32_t sector)
{
	uint8_t drive = ((struct ata_device *)dev->priv)->drive;

	if (validate_drive(drive) == -1) {
		kprintf("UNKNOWN DRIVE: %d\n", drive);
		return -1;
	}

	uint8_t *ptr = buf;
	for (uint32_t i = 0; i < sector; i++) {
		ata_read_sector(drive, lba, ptr);
		ptr += 512;
	}

	return 0;
}

int32_t ata_write(struct device *dev, uint32_t lba, void *buf, uint32_t sector)
{
	uint8_t drive = ((struct ata_device *)dev->priv)->drive;

	if (validate_drive(drive) == -1) {
		kprintf("UNKNOWN DRIVE: %d\n", drive);
		return -1;
	}

	uint8_t *ptr = buf;
	for (uint32_t i = 0; i < sector; i++) {
		ata_write_sector(drive, lba, ptr);
		ptr += 512;
	}

	return 0;
}

void ata_init()
{
	kprintf("SCANNING ATA DEVICES\n");

	uint8_t *data = kzalloc(512);
	if (!data)
		panic("NOMEM");

	// PRIMARY BUS -> MASTER DRIVE
	if (ide_identify(ATA_PRIMARY_CHANNEL, ATA_MASTER, data) != -1) {
		// Extract Model
		char *model = kzalloc(40);
		if (!model)
			panic("NOMEM");

		for (uint32_t i = 0; i < 40; i += 2) {
			model[i] = data[ATA_IDENT_MODEL + i + 1];
			model[i + 1] = data[ATA_IDENT_MODEL + i];
		}

		struct ata_device *ata_dev = kzalloc(sizeof(struct ata_device));
		if (!ata_dev)
			panic("NOMEM");

		ata_dev->drive = PRIMARY_MASTER_DRIVE;

		struct device *dev = kzalloc(sizeof(struct device));
		if (!dev)
			panic("NOMEM");

		dev->name = model;
		dev->uid = device_generate_uid();
		dev->type = BLOCK_DEVICE;
		dev->priv = ata_dev;
		dev->read = ata_read;
		dev->write = ata_write;

		device_add(dev);
	}
}
