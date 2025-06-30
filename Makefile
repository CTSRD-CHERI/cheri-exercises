# If you would like to choose a different path to the SDK, you can pass it as an
# argument.
ifndef MICROKIT_SDK
	MICROKIT_SDK := ../microkit-sdk-2.0.1-dev
endif

ifndef CHERI_EXERCISE
	CHERI_EXERCISE := buffer-overflow-global
endif

BOARD := qemu_virt_riscv64
MICROKIT_CONFIG := cheri
BUILD_DIR := build

CC := clang
LD := ld.lld
AS := clang
MICROKIT_TOOL ?= $(MICROKIT_SDK)/bin/microkit

PRINTF_OBJS := printf.o util.o
SERIAL_SERVER_OBJS := serial_server.o
CHERI_0_OBJS := $(PRINTF_OBJS) buffer-overflow.o btpalloc.o
OBJS := $(PRINTF_OBJS) $(CHERI_EXERCISE).o

BOARD_DIR := $(MICROKIT_SDK)/board/$(BOARD)/$(MICROKIT_CONFIG)

IMAGES_PART_CHERI_0 := serial_server.elf buffer-overflow-control-flow.elf
IMAGES_CHERI := $(CHERI_EXERCISE).elf
# Note that these warnings being disabled is to avoid compilation errors while in the middle of completing each exercise part

ifeq ($(CHERI),True)
CFLAGS := -G0 -target riscv64-unknown-elf -nostdlib -ffreestanding -g -Wall -Wno-array-bounds -Wno-unused-variable -Wno-unused-function -Werror -I$(BOARD_DIR)/include -Ivmm/src/util -Iinclude -DBOARD_$(BOARD) -march=rv64imafdc_zicsr_zifencei_zcherihybrid -mabi=l64pc128d -cheri-bounds=subobject-safe
LDFLAGS := -L$(BOARD_DIR)/lib -L/home/hesham/cheri/output/sdk/baremetal/baremetal-riscv64-purecap/lib/
LIBS := -lmicrokit_purecap -Tmicrokit.ld
else
CFLAGS := -G0 -target riscv64-unknown-elf -nostdlib -ffreestanding -g -Wall -Wno-array-bounds -Wno-unused-variable -Wno-unused-function -Werror -I$(BOARD_DIR)/include -Ivmm/src/util -Iinclude -DBOARD_$(BOARD) -march=rv64imafdc_zicsr_zifencei -mabi=lp64d
LDFLAGS := -L$(BOARD_DIR)/lib -L/home/hesham/cheri/output/sdk/baremetal/baremetal-riscv64/lib/
LIBS := -lclang_rt.builtins-riscv64 -lmicrokit -Tmicrokit.ld
endif

IMAGE_FILE_CHERI_0 = $(BUILD_DIR)/buffer-overflow-control-flow.img
IMAGE_FILE_CHERI = $(BUILD_DIR)/$(CHERI_EXERCISE).img
IMAGE_FILE = $(BUILD_DIR)/loader.img
REPORT_FILE = $(BUILD_DIR)/report.txt

all: directories $(IMAGE_FILE)

directories:
	$(info $(shell mkdir -p $(BUILD_DIR)))

run: $(IMAGE_FILE)
	qemu-system-riscv64cheri -machine virt \
		-cpu codasip-a730 \
		-serial mon:stdio -nographic -m size=2G \
    -kernel $(IMAGE_FILE)

buffer-overflow-control-flow: directories $(BUILD_DIR)/buffer-overflow-control-flow.elf $(BUILD_DIR)/serial_server.elf $(IMAGE_FILE_CHERI_0)
$(CHERI_EXERCISE): directories $(BUILD_DIR)/$(CHERI_EXERCISE).elf $(IMAGE_FILE_CHERI)

$(BUILD_DIR)/%.o: %.c Makefile
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: src/common/%.c Makefile
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/serial_server.elf: $(addprefix $(BUILD_DIR)/, $(SERIAL_SERVER_OBJS))
	$(info building serial server)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(BUILD_DIR)/%.o: src/missions/buffer-overflow-control-flow/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: src/exercises/$(CHERI_EXERCISE)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/buffer-overflow-control-flow.elf: $(addprefix $(BUILD_DIR)/, $(CHERI_0_OBJS))
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(BUILD_DIR)/$(CHERI_EXERCISE).elf: $(addprefix $(BUILD_DIR)/, $(OBJS))
	$(info Building the ELF)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(IMAGE_FILE_CHERI_0): $(addprefix $(BUILD_DIR)/, $(IMAGES_PART_CHERI_0)) src/missions/buffer-overflow-control-flow/buffer-overflow-control-flow.system
	$(MICROKIT_TOOL) src/missions/buffer-overflow-control-flow/buffer-overflow-control-flow.system --search-path $(BUILD_DIR) --board $(BOARD) --config $(MICROKIT_CONFIG) -o $(IMAGE_FILE) -r $(REPORT_FILE)

$(IMAGE_FILE_CHERI): $(addprefix $(BUILD_DIR)/, $(IMAGES_CHERI)) src/exercises/$(CHERI_EXERCISE)/$(CHERI_EXERCISE).system
	$(info Building $(IMAGE_FILE_CHERI))
	$(MICROKIT_TOOL) src/exercises/$(CHERI_EXERCISE)/$(CHERI_EXERCISE).system --search-path $(BUILD_DIR) --board $(BOARD) --config $(MICROKIT_CONFIG) -o $(IMAGE_FILE) -r $(REPORT_FILE)
