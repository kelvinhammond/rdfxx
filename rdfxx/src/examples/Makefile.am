bin_PROGRAMS=foaf projection

foaf_SOURCES=foaf.cpp
projection_SOURCES=projection.cpp

ACLOCAL_AMFLAGS=-I ../m4
AM_CXXFLAGS = -std=c++11

foaf_LDFLAGS = $(top_builddir)/src/librdfxx/librdfxx.la -lrdf
projection_LDFLAGS = $(top_builddir)/src/librdfxx/librdfxx.la -lrdf

foaf_CPPFLAGS = -I. -I$(top_srcdir)/src/include -I/usr/include/raptor2 -I/usr/include/rasqal
projection_CPPFLAGS = -I. -I$(top_srcdir)/src/include -I/usr/include/raptor2 -I/usr/include/rasqal
