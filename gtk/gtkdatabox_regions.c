/* $Id: gtkdatabox_regions.c 4 2008-06-22 09:19:11Z rbock $ */
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <gtkdatabox_regions.h>

static void gtk_databox_regions_real_draw (GtkDataboxGraph * regions,
					GtkDatabox* box);

struct _GtkDataboxRegionsPrivate
{
   gint16 *xpixels;
   gint16 *y1pixels;
   gint16 *y2pixels;
   guint pixelsalloc;
};

static gpointer parent_class = NULL;

static void
regions_finalize (GObject * object)
{
   GtkDataboxRegions *regions = GTK_DATABOX_REGIONS (object);

   g_free (regions->priv->xpixels);
   g_free (regions->priv->y1pixels);
   g_free (regions->priv->y2pixels);
   g_free (regions->priv);

   /* Chain up to the parent class */
   G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gtk_databox_regions_class_init (gpointer g_class /*, gpointer g_class_data */ )
{
   GObjectClass *gobject_class = G_OBJECT_CLASS (g_class);
   GtkDataboxGraphClass *graph_class = GTK_DATABOX_GRAPH_CLASS (g_class);
   GtkDataboxRegionsClass *klass = GTK_DATABOX_REGIONS_CLASS (g_class);

   parent_class = g_type_class_peek_parent (klass);

   gobject_class->finalize = regions_finalize;

   graph_class->draw = gtk_databox_regions_real_draw;
}

static void
gtk_databox_regions_instance_init (GTypeInstance * instance)
{
   GtkDataboxRegions *regions = GTK_DATABOX_REGIONS (instance);

   regions->priv = g_new0 (GtkDataboxRegionsPrivate, 1);
   regions->priv->xpixels = NULL;
   regions->priv->y1pixels = NULL;
   regions->priv->y2pixels = NULL;
   regions->priv->pixelsalloc = 0;
}

GType
gtk_databox_regions_get_type (void)
{
   static GType type = 0;

   if (type == 0)
   {
      static const GTypeInfo info = {
	 sizeof (GtkDataboxRegionsClass),
	 NULL,			/* base_init */
	 NULL,			/* base_finalize */
	 (GClassInitFunc) gtk_databox_regions_class_init,	/* class_init */
	 NULL,			/* class_finalize */
	 NULL,			/* class_data */
	 sizeof (GtkDataboxRegions),	/* instance_size */
	 0,			/* n_preallocs */
	 (GInstanceInitFunc) gtk_databox_regions_instance_init,	/* instance_init */
	 NULL,			/* value_table */
      };
      type = g_type_register_static (GTK_DATABOX_TYPE_XYYC_GRAPH,
				     "GtkDataboxRegions", &info, (GTypeFlags)0);
   }

   return type;
}

/**
 * gtk_databox_regions_new:
 * @len: length of @X, @Y1  and @Y2
 * @X: array of ordinates
 * @Y1: array of co-ordinates
 * @Y2: array of co-ordinates
 * @color: color of the markers
 *
 * Creates a new #GtkDataboxRegions object which can be added to a #GtkDatabox widget
 *
 * Return value: A new #GtkDataboxRegions object
 **/
GtkDataboxGraph *
gtk_databox_regions_new (guint len, gfloat * X, gfloat * Y1, gfloat * Y2, GdkColor * color)
{
   GtkDataboxRegions *regions;
   g_return_val_if_fail (X, NULL);
   g_return_val_if_fail (Y1, NULL);
   g_return_val_if_fail (Y2, NULL);
   g_return_val_if_fail ((len > 0), NULL);

   regions = g_object_new (GTK_DATABOX_TYPE_REGIONS,
			"X-Values", X,
			"Y1-Values", Y1,
			"Y2-Values", Y2,
			"xstart", 0,
			"y1start", 0,
			"y2start", 0,
			"xstride", 1,
			"y1stride", 1,
			"y2stride", 1,
			"xtype", G_TYPE_FLOAT,
			"ytype", G_TYPE_FLOAT,
			"length", len,
			"maxlen", len,
			"color", color,NULL);

   return GTK_DATABOX_GRAPH (regions);
}

/**
 * gtk_databox_regions_new_full:
 * @maxlen: maximum length of @X and @Y
 * @len: actual number of @X and @Y values to plot
 * @X: array of ordinates
 * @Y1: array of co-ordinates
 * @Y2: array of co-ordinates
 * @xstart: the first element in the X array to plot (usually 0)
 * @y1start: the first element in the Y1 array to plot (usually 0)
 * @y2start: the first element in the Y2 array to plot (usually 0)
 * @xstride: successive elements in the X array are separated by this much (1 if array, ncols if matrix)
 * @y1stride: successive elements in the Y1 array are separated by this much (1 if array, ncols if matrix)
 * @y2stride: successive elements in the Y2 array are separated by this much (1 if array, ncols if matrix)
 * @xtype: the GType of the X array elements.  G_TYPE_FLOAT, G_TYPE_DOUBLE, etc.
 * @ytype: the GType of the Y1/Y2 array elements.  G_TYPE_FLOAT, G_TYPE_DOUBLE, etc.
 * @color: color of the markers
 *
 * Creates a new #GtkDataboxRegions object which can be added to a #GtkDatabox widget
 *
 * Return value: A new #GtkDataboxRegions object
 **/
GtkDataboxGraph *
gtk_databox_regions_new_full (guint maxlen, guint len,
			void * X, guint xstart, guint xstride, GType xtype,
			void * Y1, guint y1start, guint y1stride,
			void * Y2, guint y2start, guint y2stride, GType ytype,
		    GdkColor * color)
{
   GtkDataboxRegions *regions;
   g_return_val_if_fail (X, NULL);
   g_return_val_if_fail (Y1, NULL);
   g_return_val_if_fail (Y2, NULL);
   g_return_val_if_fail ((len > 0), NULL);

   regions = g_object_new (GTK_DATABOX_TYPE_REGIONS,
			"X-Values", X,
			"Y1-Values", Y1,
			"Y2-Values", Y2,
			"xstart", xstart,
			"y1start", y1start,
			"y2start", y2start,
			"xstride", xstride,
			"y1stride", y1stride,
			"y2stride", y2stride,
			"xtype", xtype,
			"ytype", ytype,
			"length", len,
			"maxlen", maxlen,
			"color", color,NULL);

   return GTK_DATABOX_GRAPH (regions);
}

static void
gtk_databox_regions_real_draw (GtkDataboxGraph * graph,
			    GtkDatabox* box)
{
   GtkDataboxRegions *regions = GTK_DATABOX_REGIONS (graph);
   GdkPoint data1, data2, data3, data4;
   guint i = 0;
   void *X;
   void *Y1;
   void *Y2;
   guint len, maxlen;
   cairo_t *cr;
   gint16 *xpixels, *y1pixels, *y2pixels;
   guint xstart, xstride, y1start, y1stride, y2start, y2stride;
   GType xtype, ytype;

   g_return_if_fail (GTK_DATABOX_IS_REGIONS (regions));
   g_return_if_fail (GTK_IS_DATABOX (box));

   if (gtk_databox_get_scale_type_y (box) == GTK_DATABOX_SCALE_LOG)
      g_warning
	 ("gtk_databox_regions do not work well with logarithmic scale in Y axis");

   len = gtk_databox_xyyc_graph_get_length (GTK_DATABOX_XYYC_GRAPH (graph));
   maxlen = gtk_databox_xyyc_graph_get_maxlen (GTK_DATABOX_XYYC_GRAPH (graph));

   if (regions->priv->pixelsalloc < len)
   {
   	regions->priv->pixelsalloc = len;
	regions->priv->xpixels = (gint16 *)g_realloc(regions->priv->xpixels, len * sizeof(gint16));
	regions->priv->y1pixels = (gint16 *)g_realloc(regions->priv->y1pixels, len * sizeof(gint16));
	regions->priv->y2pixels = (gint16 *)g_realloc(regions->priv->y2pixels, len * sizeof(gint16));
   }

   xpixels = regions->priv->xpixels;
   y1pixels = regions->priv->y1pixels;
   y2pixels = regions->priv->y2pixels;

   X = gtk_databox_xyyc_graph_get_X (GTK_DATABOX_XYYC_GRAPH (graph));
   xstart = gtk_databox_xyyc_graph_get_xstart (GTK_DATABOX_XYYC_GRAPH (graph));
   xstride = gtk_databox_xyyc_graph_get_xstride (GTK_DATABOX_XYYC_GRAPH (graph));
   xtype = gtk_databox_xyyc_graph_get_xtype (GTK_DATABOX_XYYC_GRAPH (graph));
   gtk_databox_values_to_xpixels(box, xpixels, X, xtype, maxlen, xstart, xstride, len);

   ytype = gtk_databox_xyyc_graph_get_ytype (GTK_DATABOX_XYYC_GRAPH (graph));
   Y1 = gtk_databox_xyyc_graph_get_Y1 (GTK_DATABOX_XYYC_GRAPH (graph));
   y1start = gtk_databox_xyyc_graph_get_y1start (GTK_DATABOX_XYYC_GRAPH (graph));
   y1stride = gtk_databox_xyyc_graph_get_y1stride (GTK_DATABOX_XYYC_GRAPH (graph));
   gtk_databox_values_to_ypixels(box, y1pixels, Y1, ytype, maxlen, y1start, y1stride, len);

   Y2 = gtk_databox_xyyc_graph_get_Y2 (GTK_DATABOX_XYYC_GRAPH (graph));
   y2start = gtk_databox_xyyc_graph_get_y2start (GTK_DATABOX_XYYC_GRAPH (graph));
   y2stride = gtk_databox_xyyc_graph_get_y2stride (GTK_DATABOX_XYYC_GRAPH (graph));
   gtk_databox_values_to_ypixels(box, y2pixels, Y2, ytype, maxlen, y2start, y2stride, len);

   cr = gtk_databox_graph_create_gc (graph, box);

   data3.x = *xpixels;
   data3.y = *y2pixels;
   data4.x = *xpixels;
   data4.y = *y1pixels;
   xpixels++; y1pixels++; y2pixels++;
   for (i = 0; i < len-1; i++, xpixels++, y1pixels++, y2pixels++)
   {
      data1.x = data3.x; /* 4 points in the polygon */
      data1.y = data3.y;
      data2.x = data4.x;
      data2.y = data4.y;
      data3.x = *xpixels;
      data3.y = *y2pixels;
      data4.x = *xpixels;
      data4.y = *y1pixels;
      cairo_move_to(cr, data2.x, data2.y);
      cairo_line_to(cr, data1.x, data1.y);
      cairo_line_to(cr, data3.x, data3.y);
      cairo_line_to(cr, data4.x, data4.y);
      cairo_close_path  (cr);
      cairo_fill(cr);
   }
   cairo_destroy(cr);

   return;
}

