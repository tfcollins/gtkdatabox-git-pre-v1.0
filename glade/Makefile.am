# $Id: Makefile.am 4 2008-06-22 09:19:11Z rbock $

if BUILD_GLADE

# These are glade-3 interface builder modules, only needed if 
# you are going to design with glade-3

AM_CPPFLAGS = \
	-I$(top_builddir)				\
	-I$(top_srcdir)/gtk				

gladeuimoduledir = @glade_moduledir@
gladeuimodule_LTLIBRARIES = libgladedatabox.la

libgladedatabox_la_CFLAGS = @GLADE3_CFLAGS@

libgladedatabox_la_LIBADD = \
	$(top_builddir)/gtk/libgtkdatabox.la	\
	@GLADE3_LIBS@

libgladedatabox_la_LDFLAGS = \
	-export-dynamic -module -avoid-version 

libgladedatabox_la_SOURCES = \
	gladeui-databox.c

catalogsdir = @glade_catalogdir@
catalogs_DATA = gtkdatabox.xml

# Install icon file
themedir = $(datadir)/icons/hicolor
size = scalable
context = apps
iconsdir = $(themedir)/$(size)/$(context)
icons_DATA = \
	     widget-gladedatabox-gtk_databox.svg \
	     widget-gladedatabox-gtk_databox_ruler.svg

EXTRA_DIST = \
	     $(icons_DATA) \
	     widget-gladedatabox-gtk_databox.fig \
	     widget-gladedatabox-gtk_databox_ruler.fig

endif
