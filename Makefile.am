## Autotools settings

ACLOCAL_AMFLAGS = -I m4

AM_DISTCHECK_CONFIGURE_FLAGS = \
	--enable-examples \
	--enable-cmocka \
	--disable-leg \
	--disable-valgrind


## File listings

EXTRA_DIST = \
	autogen.sh \
	README.md \
	$(NULL)

CLEANFILES = \
	$(NULL)

noinst_HEADERS = \
	src/output.h \
	src/source-parser.h \
	src/template-parser.h \
	src/utils/utils.h \
	$(NULL)

noinst_LTLIBRARIES = \
	libblogc.la \
	$(NULL)

noinst_PROGRAMS = \
	$(NULL)

bin_PROGRAMS = \
	blogc \
	$(NULL)

check_PROGRAMS = \
	$(NULL)


libblogc_la_SOURCES = \
	src/output.c \
	src/source-parser.c \
	src/template-parser.c \
	src/utils/slist.c \
	src/utils/strings.c \
	src/utils/trie.c \
	$(NULL)

libblogc_la_CFLAGS = \
	$(AM_CFLAGS) \
	-I$(top_srcdir)/src \
	$(NULL)

libblogc_la_LIBADD = \
	$(NULL)


blogc_SOURCES = \
	src/main.c \
	$(NULL)

blogc_CFLAGS = \
	$(AM_CFLAGS) \
	-I$(top_srcdir)/src \
	$(NULL)

blogc_LDADD = \
	libblogc.la \
	$(NULL)


## Build rules: tests

if USE_CMOCKA

check_PROGRAMS += \
	tests/check_source_parser \
	tests/check_template_parser \
	tests/check_utils \
	$(NULL)

tests_check_source_parser_SOURCES = \
	tests/check_source_parser.c \
	$(NULL)

tests_check_source_parser_CFLAGS = \
	$(CMOCKA_CFLAGS) \
	$(NULL)

tests_check_source_parser_LDFLAGS = \
	-no-install \
	$(NULL)

tests_check_source_parser_LDADD = \
	$(CMOCKA_LIBS) \
	libblogc.la \
	$(NULL)

tests_check_template_parser_SOURCES = \
	tests/check_template_parser.c \
	$(NULL)

tests_check_template_parser_CFLAGS = \
	$(CMOCKA_CFLAGS) \
	$(NULL)

tests_check_template_parser_LDFLAGS = \
	-no-install \
	$(NULL)

tests_check_template_parser_LDADD = \
	$(CMOCKA_LIBS) \
	libblogc.la \
	$(NULL)

tests_check_utils_SOURCES = \
	tests/check_utils.c \
	$(NULL)

tests_check_utils_CFLAGS = \
	$(CMOCKA_CFLAGS) \
	$(NULL)

tests_check_utils_LDFLAGS = \
	-no-install \
	$(NULL)

tests_check_utils_LDADD = \
	$(CMOCKA_LIBS) \
	libblogc.la \
	$(NULL)

endif

TESTS = \
	$(check_PROGRAMS)


## Helpers: Valgrind runners

if USE_VALGRIND
valgrind: all
	$(MAKE) check TESTS_ENVIRONMENT=" \
		G_SLICE=always-malloc \
		G_DEBUG=gc-friendly \
		$(LIBTOOL) \
			--mode=execute \
			$(VALGRIND) \
				--tool=memcheck \
				--leak-check=full \
				--leak-resolution=high \
				--num-callers=20 \
				--show-possibly-lost=no"

valgrind-ci: all clean-local
	$(MAKE) check TESTS_ENVIRONMENT=" \
		G_SLICE=always-malloc \
		G_DEBUG=gc-friendly \
		$(LIBTOOL) \
			--mode=execute \
			$(VALGRIND) \
				--tool=memcheck \
				--xml=yes \
				--xml-file=valgrind-%p.xml \
				--leak-check=full \
				--leak-resolution=high \
				--num-callers=20 \
				--show-possibly-lost=no"
endif


# Helpers: Cleanup of helper files

clean-local:
	-rm -rf $(top_builddir)/valgrind-*.xml