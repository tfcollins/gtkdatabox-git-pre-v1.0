# $Id: Makefile.am 4 2008-06-22 09:19:11Z rbock $

noinst_PROGRAMS         = \
                          basics\
                          basics2\
                          full\
                          logarithmic\
                          rulers\
                          keycontrol\
                          colors\
                          markers\
                          grid\
                          grid_array\
                          signals\
                          lissajous\
                          enable_disable\
                          addremove


basics_SOURCES          = basics.c

basics2_SOURCES         = basics2.c

full_SOURCES         	= full.c

logarithmic_SOURCES     = logarithmic.c

rulers_SOURCES     = rulers.c

keycontrol_SOURCES      = keycontrol.c

colors_SOURCES          = colors.c

markers_SOURCES          = markers.c

grid_SOURCES            = grid.c

grid_array_SOURCES      = grid_array.c

signals_SOURCES         = signals.c

lissajous_SOURCES       = lissajous.c

enable_disable_SOURCES  = enable_disable.c

addremove_SOURCES       = addremove.c

AM_CPPFLAGS		= \
			-I$(top_srcdir)/gtk

AM_CFLAGS			=\
			-O2\
			-W -Wall -pedantic\
			-DGDK_MIN_VERSION_REQUIRED=GDK_VERSION_3_4\
			-DGTK_DISABLE_SINGLE_INCLUDES\
			@GTK_CFLAGS@


LDADD 			= $(top_builddir)/gtk/libgtkdatabox.la\
		 	@GTK_LIBS@ \
			-lm
