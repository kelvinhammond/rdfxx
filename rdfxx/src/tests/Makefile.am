
check_PROGRAMS=rdftest


TESTS_ENVIRONMENT = SASSY_CONF=$(abs_top_builddir)/sassy-check.xml
TESTS = rdftest

#######################################
# Build information for each executable. The variable name is derived
# by use the name of the executable with each non alpha-numeric character is
# replaced by '_'. So a.out becomes a_out and the appropriate suffex added.
# '_SOURCES' for example.

# Sources for the unit tests
rdftest_SOURCES = rdftest.cpp rdftest.h

AM_CXXFLAGS = -std=c++11
RDF_CFLAGS = -I$(top_srcdir)/src/include -I$(includedir)/sassy
RDF_LIBS = $(top_builddir)/src/librdfxx/librdfxx.la -L$(libdir)/sassy -lcdi -lcfi -lrdf

# Linker options for unit tests
rdftest_LDFLAGS = $(RDF_LIBS) $(XML_LIBS)

# Compiler options for unit tests
rdftest_CPPFLAGS = $(RDF_CFLAGS) $(XML_CFLAGS)
