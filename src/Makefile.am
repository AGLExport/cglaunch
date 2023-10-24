AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

bin_PROGRAMS = cglaunch

cglaunch_SOURCES = \
	cglaunch.c


# options
# Additional library
cglaunch_LDADD = \
	-lrt -lpthread \
	@LIBCGROUP_LIBS@ \
	@LIBCAP_LIBS@

# C compiler options
cglaunch_CFLAGS = \
	-g -Wall -Wno-unused-but-set-variable \
	-I$(top_srcdir)/include \
	@LIBCGROUP_CFLAGS@ \
	@LIBCAP_CFLAGS@ \
	-D_GNU_SOURCE

# C++ compiler options
cglaunch_CXXFLAGS = \
	-g -Wall -Wno-unused-but-set-variable \
	-I$(top_srcdir)/include \
	@LIBCGROUP_CFLAGS@ \
	@LIBCAP_CFLAGS@ \
	-D_GNU_SOURCE

# Linker options
cglaunch_LDFLAGS =


# configure option
if ENABLE_ADDRESS_SANITIZER
CFLAGS   += -fsanitize=address
endif

if ENABLE_GCOV
CFLAGS   += -coverage
endif

cglaunch_CFLAGS   += -DCM_CRITICAL_ERROR_OUT_STDERROR
cglaunch_CXXFLAGS += -DCM_CRITICAL_ERROR_OUT_STDERROR

if ENABLE_PRINTFDEBUG
cglaunch_CFLAGS   += -D_PRINTF_DEBUG_
cglaunch_CXXFLAGS += -D_PRINTF_DEBUG_
endif

CLEANFILES = *.gcda *.gcno
