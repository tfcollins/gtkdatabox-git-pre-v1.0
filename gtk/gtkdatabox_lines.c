/* $Id: gtkdatabox_lines.c 4 2008-06-22 09:19:11Z rbock $ */
/* GtkDatabox - An extension to the gtk+ library
 * Copyright (C) 1998 - 2008  Dr. Roland Bock
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <gtkdatabox_lines.h>

G_DEFINE_TYPE(GtkDataboxLines, gtk_databox_lines,
	GTK_DATABOX_TYPE_XYC_GRAPH)

static void gtk_databox_lines_real_draw (GtkDataboxGraph * lines,
					 GtkDatabox* box);

/**
 * GtkDataboxLinesPrivate
 * @see_also: #GtkDatabox, #GtkDataboxGraph, #GtkDataboxPoints, #GtkDataboxBars, #GtkDataboxMarkers
 *
 * A private data structure used by the #GtkDataboxLines. It shields all internal things
 * from developers who are just using the object.
 *
 **/
typedef struct _GtkDataboxLinesPrivate GtkDataboxLinesPrivate;

struct _GtkDataboxLinesPrivate
{
   GdkPoint *data;
};

static void
lines_finalize (GObject * object)
{
   g_free (GTK_DATABOX_LINES_GET_PRIVATE(object)->data);

   /* Chain up to the parent class */
   G_OBJECT_CLASS (gtk_databox_lines_parent_class)->finalize (object);
}

static void
gtk_databox_lines_class_init (GtkDataboxLinesClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
   GtkDataboxGraphClass *graph_class = GTK_DATABOX_GRAPH_CLASS (klass);

   gobject_class->finalize = lines_finalize;

   graph_class->draw = gtk_databox_lines_real_draw;

   g_type_class_add_private (klass, sizeof (GtkDataboxLinesPrivate));
}

static void
gtk_databox_lines_complete (GtkDataboxLines * lines)
{
   GTK_DATABOX_LINES_GET_PRIVATE(lines)->data =
      g_new0 (GdkPoint,
	      gtk_databox_xyc_graph_get_length
	      (GTK_DATABOX_XYC_GRAPH (lines)));

}

static void
gtk_databox_lines_init (GtkDataboxLines *lines)
{
   g_signal_connect (lines, "notify::length",
		     G_CALLBACK (gtk_databox_lines_complete), NULL);
}

/**
 * gtk_databox_lines_new:
 * @len: length of @X and @Y
 * @X: array of horizontal position values of markers
 * @Y: array of vertical position values of markers
 * @color: color of the markers
 * @size: marker size or line width (depending on the @type)
 *
 * Creates a new #GtkDataboxLines object which can be added to a #GtkDatabox widget.
 *
 * Return value: A new #GtkDataboxLines object
 **/
GtkDataboxGraph *
gtk_databox_lines_new (guint len, gfloat * X, gfloat * Y,
		       GdkColor * color, guint size)
{
   GtkDataboxLines *lines;
   g_return_val_if_fail (X, NULL);
   g_return_val_if_fail (Y, NULL);
   g_return_val_if_fail ((len > 0), NULL);

   lines = g_object_new (GTK_DATABOX_TYPE_LINES,
			 "X-Values", X,
			 "Y-Values", Y,
			 "length", len, "color", color, "size", size, NULL);

   return GTK_DATABOX_GRAPH (lines);
}

static void
gtk_databox_lines_real_draw (GtkDataboxGraph * graph,
			     GtkDatabox * box)
{
   GtkDataboxLines *lines = GTK_DATABOX_LINES (graph);
   GdkPoint *data;
   GdkPixmap *pixmap;
   GdkGC *gc;
   guint i = 0;
   gfloat *X;
   gfloat *Y;
   guint len;

   g_return_if_fail (GTK_DATABOX_IS_LINES (lines));
   g_return_if_fail (GTK_IS_DATABOX (box));

   pixmap = gtk_databox_get_backing_pixmap (box);

   if (!(gc = gtk_databox_graph_get_gc(graph)))
      gc = gtk_databox_graph_create_gc (graph, box);

   len = gtk_databox_xyc_graph_get_length (GTK_DATABOX_XYC_GRAPH (graph));
   X = gtk_databox_xyc_graph_get_X (GTK_DATABOX_XYC_GRAPH (graph));
   Y = gtk_databox_xyc_graph_get_Y (GTK_DATABOX_XYC_GRAPH (graph));

   data = GTK_DATABOX_LINES_GET_PRIVATE(graph)->data;

   gtk_databox_values_to_pixels (box, len, X, Y, data);

   /* More than 2^16 lines will cause X IO error on most XServers
      (Hint from Paul Barton-Davis) */
   for (i = 0; i < len; i += 65536)
   {
      gdk_draw_lines (pixmap, gc, data + i,
		      MIN (65536, len - i));
   }

   return;
}
