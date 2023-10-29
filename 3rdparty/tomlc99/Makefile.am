AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

noinst_LIBRARIES = \
	libtomlc99_3rdparty.a

libtomlc99_3rdparty_a_SOURCES = \
	toml.c

libtomlc99_3rdparty_a_CFLAGS = \
	-g -std=c99 -Wall -Wextra -fpic

# configure option
if ENABLE_ADDRESS_SANITIZER
CFLAGS   += -fsanitize=address
endif

CLEANFILES = *.gcda *.gcno
