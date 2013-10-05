/* $Id: gtkdatabox_offset_bars.c 4 2008-06-22 09:19:11Z rbock $ */
/* GtkDatabox - An extension to the gtk+ library
 * Copyright (C) 2011 - 2012  Dr. Matt Flax <flatmax@>
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

#include <gtkdatabox_offset_bars.h>

G_DEFINE_TYPE(GtkDataboxOffsetBars, gtk_databox_offset_bars,
	GTK_DATABOX_TYPE_XYYC_GRAPH)

static void gtk_databox_offset_bars_real_draw (GtkDataboxGraph * bars,
					GtkDatabox* box);

/**
 * GtkDataboxOffsetBarsPrivate
 *
 * A private data structure used by the #GtkDataboxOffsetBars. It shields all internal things
 * from developers who are just using the object.
 *
 **/
typedef struct _GtkDataboxOffsetBarsPrivate GtkDataboxOffsetBarsPrivate;

struct _GtkDataboxOffsetBarsPrivate
{
   GdkSegment *data;
};

static void
bars_finalize (GObject * object)
{
   g_free (GTK_DATABOX_OFFSET_BARS_GET_PRIVATE(object)->data);

   /* Chain up to the parent class */
   G_OBJECT_CLASS (gtk_databox_offset_bars_parent_class)->finalize (object);
}

static void
gtk_databox_offset_bars_class_init (GtkDataboxOffsetBarsClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
   GtkDataboxGraphClass *graph_class = GTK_DATABOX_GRAPH_CLASS (klass);

   gobject_class->finalize = bars_finalize;

   graph_class->draw = gtk_databox_offset_bars_real_draw;

   g_type_class_add_private (klass, sizeof (GtkDataboxOffsetBarsPrivate));
}

static void
gtk_databox_offset_bars_complete (GtkDataboxOffsetBars * bars)
{
   GTK_DATABOX_OFFSET_BARS_GET_PRIVATE(bars)->data =
      g_new0 (GdkSegment,
	      gtk_databox_xyyc_graph_get_length
	      (GTK_DATABOX_XYYC_GRAPH (bars)));

}

static void
gtk_databox_offset_bars_init (GtkDataboxOffsetBars *bars)
{
   g_signal_connect (bars, "notify::length",
		     G_CALLBACK (gtk_databox_offset_bars_complete), NULL);
}

/**
 * gtk_databox_offset_bars_new:
 * @len: length of @X, @Y1  and @Y2
 * @X: array of horizontal position values of markers
 * @Y1: array of starting vertical position values of markers
 * @Y2: array of ending vertical position values of markers
 * @color: color of the markers
 * @size: marker size or line width (depending on the @type)
 *
 * Creates a new #GtkDataboxOffsetBars object which can be added to a #GtkDatabox widget
 *
 * Return value: A new #GtkDataboxOffsetBars object
 **/
GtkDataboxGraph *
gtk_databox_offset_bars_new (guint len, gfloat * X, gfloat * Y1, gfloat * Y2,
		      GdkColor * color, guint size)
{
   GtkDataboxOffsetBars *bars;
   g_return_val_if_fail (X, NULL);
   g_return_val_if_fail (Y1, NULL);
   g_return_val_if_fail (Y2, NULL);
   g_return_val_if_fail ((len > 0), NULL);

   bars = g_object_new (GTK_DATABOX_TYPE_OFFSET_BARS,
			"X-Values", X,
			"Y1-Values", Y1,
			"Y2-Values", Y2,
			"length", len, "color", color, "size", size, NULL);

   return GTK_DATABOX_GRAPH (bars);
}

static void
gtk_databox_offset_bars_real_draw (GtkDataboxGraph * graph,
			    GtkDatabox* box)
{
   GtkDataboxOffsetBars *bars = GTK_DATABOX_OFFSET_BARS (graph);
   GtkDataboxOffsetBarsPrivate *priv = GTK_DATABOX_OFFSET_BARS_GET_PRIVATE(graph);
   GdkGC *gc;
   GdkPixmap *pixmap;
   guint i = 0;
   gfloat *X;
   gfloat *Y1;
   gfloat *Y2;
   guint len;
   GdkSegment *data;

   g_return_if_fail (GTK_DATABOX_IS_OFFSET_BARS (bars));
   g_return_if_fail (GTK_IS_DATABOX (box));

   pixmap = gtk_databox_get_backing_pixmap (box);

   if (gtk_databox_get_scale_type_y (box) == GTK_DATABOX_SCALE_LOG)
      g_warning
	 ("gtk_databox_offset_bars do not work well with logarithmic scale in Y axis");

   if (!(gc = gtk_databox_graph_get_gc(graph)))
      gc = gtk_databox_graph_create_gc (graph, box);

   len = gtk_databox_xyyc_graph_get_length (GTK_DATABOX_XYYC_GRAPH (graph));
   X = gtk_databox_xyyc_graph_get_X (GTK_DATABOX_XYYC_GRAPH (graph));
   Y1 = gtk_databox_xyyc_graph_get_Y1 (GTK_DATABOX_XYYC_GRAPH (graph));
   Y2 = gtk_databox_xyyc_graph_get_Y2 (GTK_DATABOX_XYYC_GRAPH (graph));

   data = priv->data;

   for (i = 0; i < len; i++, data++, X++, Y1++, Y2++)
   {
      data->x1 = data->x2 = gtk_databox_value_to_pixel_x (box, *X);
      data->y1 = gtk_databox_value_to_pixel_y (box, *Y1);
      data->y2 = gtk_databox_value_to_pixel_y (box, *Y2);
   }

   /* More than 2^16 bars will cause X IO error on most XServers
      (Hint from Paul Barton-Davis) */
   for (i = 0; i < len; i += 65536)
   {
      gdk_draw_segments (pixmap, gc,
			 priv->data + i, MIN (65536, len - i));
   }

   return;
}
