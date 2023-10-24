AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

bin_PROGRAMS = \
	memory_injector

memory_injector_SOURCES = \
	memory_injector.c

# options
# Additional library
LDADD =

memory_injector_LDADD = \
	${LDADD}

# C compiler options
CFLAGS = \
	-g \
	@GTEST_MAIN_CFLAGS@ \
	@GMOCK_MAIN_CFLAGS@
	-D_GNU_SOURCE

# C++ compiler options
CXXFLAGS = \
	-g \
	-fpermissive \
	-std=c++11 -fno-exceptions \
	@GTEST_MAIN_CFLAGS@ \
	@GMOCK_MAIN_CFLAGS@
	-D_GNU_SOURCE

# Linker options
LDFLAGS =


# configure option
if ENABLE_ADDRESS_SANITIZER
CFLAGS   += -fsanitize=address
endif

if ENABLE_GCOV
CFLAGS   += -coverage
endif

if ENABLE_PRINTFDEBUG
CFLAGS   += -D_PRINTF_DEBUG_
CXXFLAGS += -D_PRINTF_DEBUG_
endif

CLEANFILES = *.gcda *.gcno
