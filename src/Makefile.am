AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

bin_PROGRAMS = cglaunch

cglaunch_SOURCES = \
	cglaunch-config.c \
	cglaunch-cgroup.c \
	cglaunch.c

# options
# Additional library
cglaunch_LDADD = \
	$(top_srcdir)/3rdparty/tomlc99/libtomlc99_3rdparty.a \
	-lrt -lpthread \
	@LIBCGROUP_LIBS@ \
	@LIBCAP_LIBS@

# C compiler options
cglaunch_CFLAGS = \
	-g -Wall -Wno-unused-but-set-variable \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/3rdparty/tomlc99 \
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

cglaunch_CFLAGS   += -D_CRITICAL_ERROR_OUT_STDERROR
cglaunch_CXXFLAGS += -D_CRITICAL_ERROR_OUT_STDERROR

if ENABLE_PRINTFDEBUG
cglaunch_CFLAGS   += -D_PRINTF_DEBUG_
cglaunch_CXXFLAGS += -D_PRINTF_DEBUG_
endif

CLEANFILES = *.gcda *.gcno
