CC := gcc

LD := ld

AS := as

CFLAGS := -ffreestanding -m32 -Wall -Wextra -Iinclude

BUILD := build

ISO_DIR := $(BUILD)/iso

ISO := CookieOS-Flex.iso

make-y += kernel
make-y += arch/x86
make-y += drivers/text

obj-y :=

include $(addsuffix /Makefile,$(make-y))

obj-y := $(addprefix $(BUILD)/,$(obj-y))


all: kImage.bin


kImage.bin: $(obj-y)
	$(LD) -m elf_i386 -T arch/x86/linker.ld -o $@ $^


$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) --32 $< -o $@


iso: kImage.bin
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp kImage.bin $(ISO_DIR)/boot/kImage.bin
	@printf '%s\n' \
		'set timeout=0' \
		'set default=0' \
		'' \
		'menuentry "CookieOS Flex" {' \
		'    multiboot /boot/kImage.bin' \
		'    boot' \
		'}' \
		> $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)


run: iso
	qemu-system-i386 -cdrom $(ISO)


clean:
	rm -rf $(BUILD) kImage.bin $(ISO)


.PHONY: all iso run clean