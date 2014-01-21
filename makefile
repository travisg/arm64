
TARGET := test
BUILDDIR := build-$(TARGET)

# compiler flags, default libs to link against
COMPILEFLAGS := -g -O2 -Iinclude -finline
CFLAGS := -std=c99
CPPFLAGS :=
ASMFLAGS :=
LDFLAGS :=
LDLIBS :=

UNAME := $(shell uname -s)
ARCH := $(shell uname -m)

CC := aarch64-elf-gcc
LD := aarch64-elf-ld
OBJDUMP := aarch64-elf-objdump

# switch any platform specific stuff here
# ifeq ($(findstring CYGWIN,$(UNAME)),CYGWIN)
# ...
# endif
# ifeq ($(UNAME),Darwin)
# ...
# endif

CFLAGS += $(COMPILEFLAGS)
CPPFLAGS += $(COMPILEFLAGS)
ASMFLAGS += $(COMPILEFLAGS)

OBJS := \
	test.o \
	asm.o \
	debug.o \
	exceptions.o \
	exceptions_c.o \
	interrupts.o \
	printf.o \
	strlen.o \
	stdio.o \
	timer.o \

OBJS := $(addprefix $(BUILDDIR)/,$(OBJS))

DEPS := $(OBJS:.o=.d)

LIBGCC := $(shell $(CC) $(CFLAGS) --print-libgcc-file-name)

LINKER_SCRIPT := system-onesegment.ld

$(BUILDDIR)/$(TARGET):  $(OBJS) $(LINKER_SCRIPT)
	$(LD) -T $(LINKER_SCRIPT) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS) $(LIBGCC)
	$(OBJDUMP) -d $@ > $@.lst

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET) $(TARGET).lst

spotless:
	rm -rf build-*

.PHONY: run

run: $(BUILDDIR)/$(TARGET)
	./Foundation_v8 --use-real-time --visualization --no-gicv3 --image $<

runvm: $(BUILDDIR)/$(TARGET)
	scp $(BUILDDIR)/$(TARGET) xubuntu:.
	ssh xubuntu Foundation_v8pkg/models/Linux64_GCC-4.1/Foundation_v8 --visualization --image $(TARGET); \
	ssh xubuntu killall Foundation_v8

# makes sure the target dir exists
MKDIR = if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi

$(BUILDDIR)/%.o: %.c
	@$(MKDIR)
	@echo compiling $<
	@$(CC) $(CFLAGS) -c $< -MD -MT $@ -MF $(@:%o=%d) -o $@

$(BUILDDIR)/%.o: %.cpp
	@$(MKDIR)
	@echo compiling $<
	@$(CC) $(CPPFLAGS) -c $< -MD -MT $@ -MF $(@:%o=%d) -o $@

$(BUILDDIR)/%.o: %.S
	@$(MKDIR)
	@echo compiling $<
	@$(CC) $(ASMFLAGS) -c $< -MD -MT $@ -MF $(@:%o=%d) -o $@

ifeq ($(filter $(MAKECMDGOALS), clean), )
-include $(DEPS)
endif

# vim: set ts=4 sw=4 noexpandtab:
