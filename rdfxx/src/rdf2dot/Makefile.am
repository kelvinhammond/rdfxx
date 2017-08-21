bin_PROGRAMS=rdf2dot

rdf2dot_SOURCES=rdf2dot.cpp

ACLOCAL_AMFLAGS=-I ../m4
AM_CXXFLAGS = -std=c++11

rdf2dot_LDFLAGS = $(top_builddir)/src/librdfxx/librdfxx.la -lrdf -L$(libdir)/sassy -lcfi  $(XML_LIBS)

rdf2dot_CPPFLAGS = -I. -I$(top_srcdir)/src/include -I$(includedir)/sassy
rdf2dot_CPPFLAGS += -I/usr/include/raptor2 -I/usr/include/rasqal
rdf2dot_CPPFLAGS += $(XML_CFLAGS)
