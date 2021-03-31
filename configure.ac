# $Id: configure.ac 4 2008-06-22 09:19:11Z rbock $
#
# This configure.in file has been started by looking at the tutorial of
# Ralph Levien
# at http://www.levien.com/gnome/cvs-lib-howto.html
#
# Additional features where taken from gtk+-1.1.1/configure.in and allied 
# files.
# 
# Thank's to all the gimp-glib-gtk-gnome-etc.-people
#

# Initialize autoconf by any header file of the source
#
m4_define([databox_current],  [1]) # Increment in case of any change in existing API.
m4_define([databox_revision], [0]) # Increment for bug fixes, code cleanup, etc. When 'databox_current' increments, reset 'databox_revision' to 0.
m4_define([databox_age],      [0]) # Increment in case of only new functions being added. If some function were removed or changed, reset 'databox-age' to 0.
m4_define([databox_lt_version],  [databox_current:databox_revision:databox_age])
m4_define([databox_version],  [databox_current.databox_revision.databox_age])
AC_INIT(gtkdatabox, databox_version)

# This configures autoheader to generate a config.h file
#
AM_CONFIG_HEADER(config.h)

AC_SUBST(DATABOX_LT_VERSION, [databox_lt_version])
AC_SUBST(DATABOX_VERSION, [databox_version])

#AM_INIT_AUTOMAKE(gtkdatabox, databox_version)
AM_INIT_AUTOMAKE([tar-ustar])

AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL

AC_PROG_CC_C99
AC_PROG_CPP
AC_PROG_INSTALL

m4_define([gtk_required_version], [3.4.0])
m4_define([pango_required_version], [1.29.0])
m4_define([cairo_required_version], [1.10.0])

PKG_CHECK_MODULES(GTK, [gtk+-3.0 >= [gtk_required_version]])
PKG_CHECK_MODULES(GTK, [gdk-3.0 >= [gtk_required_version]])
PKG_CHECK_MODULES(PANGO, pango >= [pango_required_version])
PKG_CHECK_MODULES(CAIRO, cairo >= [cairo_required_version]) 
AC_SUBST(GTK_REQUIRED, [gtk_required_version])
AC_SUBST(PANGO_REQUIRED, [pango_required_version])
AC_SUBST(CAIRO_REQUIRED, [cairo_required_version])

# Remove some deprecated code for GTK >= 3.18.0
if test x`pkg-config --atleast-version=3.18.0 gtk+-3.0; echo $?` = x0; then
	AC_DEFINE(GTK3_18, 1, Remove deprecated code for GTK version >= 3.18.0)
fi

# ------------------ Glade3 configuration ------------------------

AC_ARG_ENABLE(glade,
	[  --enable-glade	  enable glade-3 support],
	[ac_cv_enable_glade=$enableval], [ac_cv_enable_glade=no])
AC_MSG_CHECKING([whether to build glade-3 support module])
if test x"$ac_cv_enable_glade" = xyes; then
	AC_MSG_RESULT(yes)
	PKG_CHECK_MODULES(GLADE3, gladeui-2.0 >= 3.4.0,
			ac_cv_enable_glade=yes, ac_cv_enable_glade=no)
	if test x"$ac_cv_enable_glade" = xyes; then
		AC_DEFINE(USE_GLADE, 1, Define if you want glade interface builder support)
		glade_moduledir=`pkg-config gladeui-2.0 --variable=moduledir`
		AC_DEFINE_UNQUOTED(GLADE_MODULEDIR, [$glade_moduledir], [Glade-3 module directory])
		glade_catalogdir=`pkg-config gladeui-2.0 --variable=catalogdir`
		AC_DEFINE_UNQUOTED(GLADE_CATALOGDIR, [$glade_catalogdir], [Glade-3 catalog directory])
	else
		AC_MSG_RESULT(not found)
	fi
else
	AC_MSG_RESULT(no)
fi

AC_SUBST(GLADE3_CFLAGS)
AC_SUBST(GLADE3_LIBS)
AC_SUBST(glade_moduledir)
AC_SUBST(glade_catalogdir)

AM_CONDITIONAL(BUILD_GLADE, test x"$ac_cv_enable_glade" = xyes)

# ------------------ End of Glade 3 configuration ------------------------


# Check for gtk-doc
AC_CONFIG_MACRO_DIR(m4)
m4_ifdef([GTK_DOC_CHECK], [
GTK_DOC_CHECK([1.14],[--flavour no-tmpl])
],[
AM_CONDITIONAL([ENABLE_GTK_DOC], false)
])

AC_OUTPUT(Makefile              \
	gtk/Makefile            \
	glade/Makefile		\
	examples/Makefile       \
	doc/Makefile		\
        gtkdatabox.pc           \
        gtkdatabox.spec         \
	)

AC_MSG_RESULT([
	Install glade-3 module:         ${ac_cv_enable_glade}
])

