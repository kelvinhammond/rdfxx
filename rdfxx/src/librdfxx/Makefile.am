pkglib_LTLIBRARIES = librdfxx.la

librdfxx_la_SOURCES = model.cpp node.cpp parser.cpp query.cpp
librdfxx_la_SOURCES += query_results.cpp query_string.cpp serializer.cpp statement.cpp
librdfxx_la_SOURCES += storage.cpp stream.cpp uri.cpp world.cpp

AM_CXXFLAGS = -std=c++11 -Wall -Werror

librdfxx_la_CPPFLAGS = -I. -I$(top_srcdir)/src/include -I/usr/include/raptor2 -I/usr/include/rasqal

librdfxx_la_LDFLAGS =
