CC=gcc
LD=ld
AR=ar

CFLAGS=-std=gnu18 -Og -g3 -fPIC -fasynchronous-unwind-tables -pipe -I include/
LDFLAGS=-L.
ARFLAGS=rcs

C_FILES=$(shell find ./src -type f -name "*.c")
OBJS=$(C_FILES:.c=.o)

TEST_FILES=$(wildcard tests/*.c)
TEST_BINS=$(TEST_FILES:.c=.elf)

STATIC=libngi.a
SHARED=libngi.so

PREFIX?=/usr
INSTALL_HEADERS=$(PREFIX)/include/libngi
INSTALL_LIB=$(PREFIX)/lib

# Suppress "Entering directory..."
MAKEFLAGS += --no-print-directory
DOXYFILE=docs/doxyfile
LD_PATH=LD_LIBRARY_PATH="$(pwd);$LD_LIBRARY_PATH"

all: $(STATIC) $(SHARED)

release: CFLAGS=-Wall -std=gnu18 -O2 -DNDEBUG -fsanitize=leak -fsanitize=null \
		-Werror=format-security -Wformat -Werror=implicit-function-declaration \
		-fstack-protector-strong -fasynchronous-unwind-tables -pipe -I include/
release: $(STATIC) $(SHARED)

$(OBJS): $(C_FILES)
	echo "   CC        $*.o"
	$(CC) $(CFLAGS) -c $*.c -o $@

$(STATIC): $(OBJS)
	echo "   AR        $(STATIC)"
	$(AR) $(ARFLAGS) $(STATIC) $(OBJS)

$(SHARED): $(OBJS)
	echo "   LD        $(SHARED)"
	$(LD) -shared $(LDFLAGS) -fPIC -o $(SHARED) $(OBJS)

check: $(STATIC) $(SHARED) $(TEST_BINS)
	cp -f tests/default-test.ngi tests/test.ngi
	for test in $(TEST_BINS); do $(LD_PATH) ./$$test; done
	#for test in $(TEST_BINS); do $(LD_PATH) valgrind ./$$test; done

$(TEST_BINS): CFLAGS=-Wall -O2 -fPIC -I include/ -I tests/ -L. -Wno-unused-function
$(TEST_BINS): $(STATIC) $(SHARED) $(TEST_FILES)
	$(CC) $(CFLAGS) $(TEST_FILES) -o $@ -lngi

docs: clean-docs
	echo "   DOXY        $(DOXYFILE)"
	doxygen $(DOXYFILE)

install: release
	echo "   INSTALL   $(INSTALL_HEADERS)"
	echo "   INSTALL   $(INSTALL_LIB)/$(STATIC)"
	echo "   INSTALL   $(INSTALL_LIB)/$(SHARED)"
	install -d -m755 -p $(INSTALL_HEADERS)
	install -d -m755 -p $(INSTALL_LIB)
	for header in include/libngi/*; do install -m644 -p $$header $(INSTALL_HEADERS)/; done
	install -m755 -p $(STATIC) $(INSTALL_LIB)
	install -m755 -p $(SHARED) $(INSTALL_LIB)

uninstall:
	echo "   RM        $(INSTALL_HEADERS)"
	echo "   RM        $(INSTALL_LIB)/$(STATIC)"
	echo "   RM        $(INSTALL_LIB)/$(SHARED)"
	rm -rf $(INSTALL_HEADERS)
	rm -f $(INSTALL_LIB)/$(STATIC)
	rm -f $(INSTALL_LIB)/$(SHARED)

clean:
	echo "   RM        *.o"
	echo "   CLEAN     tests"
	cp -f tests/default-test.ngi tests/test.ngi
	rm -f $(OBJS) $(TEST_OBJS) $(TEST_BINS)

clean-docs:
	echo "   RM        docs/doxygen/"
	rm -rf docs/doxygen/

mrproper: clean clean-docs
	echo "   RM        $(STATIC)"
	echo "   RM        $(SHARED)"
	rm -f $(STATIC) $(SHARED) .gdb_history

.SILENT:
.PHONY: all check release docs install uninstall clean clean-docs mrproper
