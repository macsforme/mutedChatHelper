lib_LTLIBRARIES = mutedChatHelper.la

mutedChatHelper_la_SOURCES = mutedChatHelper.cpp
mutedChatHelper_la_CPPFLAGS= -I$(top_srcdir)/include -I$(top_srcdir)/plugins/plugin_utils
mutedChatHelper_la_LDFLAGS = -module -avoid-version -shared
mutedChatHelper_la_LIBADD = $(top_builddir)/plugins/plugin_utils/libplugin_utils.la

AM_CPPFLAGS = $(CONF_CPPFLAGS)
AM_CFLAGS = $(CONF_CFLAGS)
AM_CXXFLAGS = $(CONF_CXXFLAGS)

EXTRA_DIST = \
	README.mutedChatHelper.txt \
	mutedChatHelper.sln \
	mutedChatHelper.vcxproj	\
	mutedChatHelper.vcxproj.filters

MAINTAINERCLEANFILES =	\
	Makefile.in
