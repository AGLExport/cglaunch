AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

bin_PROGRAMS = \
	test

test_SOURCES =

# options
# Additional library
LDADD =

test_LDADD = \
	${LDADD}

# C compiler options
CFLAGS = \
	-g \
	-fsanitize=address -coverage \
	@GTEST_MAIN_CFLAGS@ \
	@GMOCK_MAIN_CFLAGS@
	-D_GNU_SOURCE

# C++ compiler options
CXXFLAGS = \
	-g \
	-fpermissive \
	-std=c++11 -fno-exceptions \
	-fsanitize=address -coverage \
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
