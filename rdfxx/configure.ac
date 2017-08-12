AC_PREREQ(2.61)
AC_INIT(sassy, rdfxx.0.0)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADER([config.h])

AC_CANONICAL_SYSTEM

dnl Initialize automake
AM_INIT_AUTOMAKE

dnl Initialize Libtool
LT_INIT

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CXX

# Checks for packages
PKG_CHECK_MODULES([XML], [libxml-2.0]);
AC_SUBST([XML_CFLAGS])
AC_SUBST([XML_LIBS])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	src/include/Makefile
	src/include/rdfxx/Makefile
	src/librdfxx/Makefile
	src/examples/Makefile
	])

AC_OUTPUT
