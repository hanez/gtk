/* GDK - The GIMP Drawing Kit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/.
 */

/*
 * Private uninstalled header defining things local to the Wayland backend
 */

#ifndef __GDK_PRIVATE_WAYLAND_H__
#define __GDK_PRIVATE_WAYLAND_H__

#include "config.h"

#include <gdk/gdkcursor.h>
#include <gdk/wayland/gdkwayland.h>
#include <gdk/wayland/gdkdisplay-wayland.h>

#include <xkbcommon/xkbcommon.h>

#include "gdkinternals.h"
#include "wayland/gtk-primary-selection-client-protocol.h"

#define WL_SURFACE_HAS_BUFFER_SCALE 3
#define WL_POINTER_HAS_FRAME 5

#define GDK_SURFACE_IS_WAYLAND(win)    (GDK_IS_SURFACE_IMPL_WAYLAND (((GdkSurface *)win)->impl))

GdkKeymap *_gdk_wayland_keymap_new (GdkDisplay *display);
void       _gdk_wayland_keymap_update_from_fd (GdkKeymap *keymap,
                                               uint32_t   format,
                                               uint32_t   fd,
                                               uint32_t   size);
struct xkb_state *_gdk_wayland_keymap_get_xkb_state (GdkKeymap *keymap);
struct xkb_keymap *_gdk_wayland_keymap_get_xkb_keymap (GdkKeymap *keymap);
gboolean           _gdk_wayland_keymap_key_is_modifier (GdkKeymap *keymap,
                                                        guint      keycode);

void       _gdk_wayland_display_init_cursors (GdkWaylandDisplay *display);
void       _gdk_wayland_display_finalize_cursors (GdkWaylandDisplay *display);

struct wl_cursor_theme * _gdk_wayland_display_get_scaled_cursor_theme (GdkWaylandDisplay *display_wayland,
                                                                       guint              scale);

void       _gdk_wayland_display_get_default_cursor_size (GdkDisplay *display,
                                                         guint       *width,
                                                         guint       *height);
void       _gdk_wayland_display_get_maximal_cursor_size (GdkDisplay *display,
                                                         guint       *width,
                                                         guint       *height);
gboolean   _gdk_wayland_display_supports_cursor_alpha (GdkDisplay *display);
gboolean   _gdk_wayland_display_supports_cursor_color (GdkDisplay *display);

void       gdk_wayland_display_system_bell (GdkDisplay *display,
                                            GdkSurface  *surface);

struct wl_buffer *_gdk_wayland_cursor_get_buffer (GdkWaylandDisplay *display,
                                                  GdkCursor         *cursor,
                                                  guint              desired_scale,
                                                  guint              image_index,
                                                  int               *hotspot_x,
                                                  int               *hotspot_y,
                                                  int               *w,
                                                  int               *h,
                                                  int               *scale);
guint      _gdk_wayland_cursor_get_next_image_index (GdkWaylandDisplay *display,
                                                     GdkCursor         *cursor,
                                                     guint              scale,
                                                     guint              current_image_index,
                                                     guint             *next_image_delay);

void       gdk_wayland_surface_sync (GdkSurface *surface);
void       gdk_wayland_surface_request_frame (GdkSurface *surface);
void            gdk_wayland_surface_attach_image           (GdkSurface           *surface,
                                                            cairo_surface_t      *cairo_surface,
                                                            const cairo_region_t *damage);

void            _gdk_wayland_surface_register_dnd          (GdkSurface *surface);
GdkDragContext *_gdk_wayland_surface_drag_begin            (GdkSurface *surface,
                                                            GdkDevice *device,
                                                            GdkContentProvider *content,
                                                            GdkDragAction actions,
                                                            gint       dx,
                                                            gint       dy);
void            _gdk_wayland_surface_offset_next_wl_buffer (GdkSurface *surface,
                                                            int        x,
                                                            int        y);
GdkDragContext * _gdk_wayland_drop_context_new (GdkDevice             *device,
                                                GdkContentFormats     *formats,
                                                struct wl_data_offer  *offer);
void _gdk_wayland_drag_context_set_source_surface (GdkDragContext *context,
                                                  GdkSurface      *surface);
void _gdk_wayland_drag_context_set_dest_surface (GdkDragContext *context,
                                                 GdkSurface      *dest_surface,
                                                 uint32_t        serial);
void _gdk_wayland_drag_context_emit_event (GdkDragContext *context,
                                           GdkEventType    type,
                                           guint32         time_);
void _gdk_wayland_drag_context_set_coords (GdkDragContext *context,
                                           gdouble         x,
                                           gdouble         y);

void gdk_wayland_drag_context_set_action (GdkDragContext *context,
                                          GdkDragAction   action);

GdkDragContext * gdk_wayland_drag_context_lookup_by_data_source   (struct wl_data_source *source);
GdkDragContext * gdk_wayland_drag_context_lookup_by_source_surface (GdkSurface *surface);
struct wl_data_source * gdk_wayland_drag_context_get_data_source  (GdkDragContext *context);

void _gdk_wayland_display_create_surface_impl (GdkDisplay    *display,
                                              GdkSurface     *surface,
                                              GdkSurface     *real_parent,
                                              GdkEventMask   event_mask,
                                              GdkSurfaceAttr *attributes);

gint        _gdk_wayland_display_text_property_to_utf8_list (GdkDisplay    *display,
                                                             GdkAtom        encoding,
                                                             gint           format,
                                                             const guchar  *text,
                                                             gint           length,
                                                             gchar       ***list);
gchar *     _gdk_wayland_display_utf8_to_string_target (GdkDisplay  *display,
                                                        const gchar *str);

void        _gdk_wayland_display_create_seat    (GdkWaylandDisplay *display,
                                                 guint32                  id,
                                                 struct wl_seat          *seat);
void        _gdk_wayland_display_remove_seat    (GdkWaylandDisplay       *display,
                                                 guint32                  id);

GdkKeymap *_gdk_wayland_device_get_keymap (GdkDevice *device);
uint32_t _gdk_wayland_device_get_implicit_grab_serial(GdkWaylandDevice *device,
                                                      const GdkEvent   *event);
uint32_t _gdk_wayland_seat_get_last_implicit_grab_serial (GdkSeat           *seat,
                                                          GdkEventSequence **seqence);
struct wl_data_device * gdk_wayland_device_get_data_device (GdkDevice *gdk_device);
void gdk_wayland_device_set_selection (GdkDevice             *gdk_device,
                                       struct wl_data_source *source);

GdkDragContext * gdk_wayland_device_get_drop_context (GdkDevice *gdk_device);

void gdk_wayland_device_unset_touch_grab (GdkDevice        *device,
                                          GdkEventSequence *sequence);

void     _gdk_wayland_display_deliver_event (GdkDisplay *display, GdkEvent *event);
GSource *_gdk_wayland_display_event_source_new (GdkDisplay *display);
void     _gdk_wayland_display_queue_events (GdkDisplay *display);

GdkAppLaunchContext *_gdk_wayland_display_get_app_launch_context (GdkDisplay *display);

GdkDisplay *_gdk_wayland_display_open (const gchar *display_name);

GList *gdk_wayland_display_get_toplevel_surfaces (GdkDisplay *display);

int gdk_wayland_display_get_output_refresh_rate (GdkWaylandDisplay *display_wayland,
                                                 struct wl_output  *output);
guint32 gdk_wayland_display_get_output_scale (GdkWaylandDisplay *display_wayland,
                                              struct wl_output  *output);
struct wl_output *gdk_wayland_display_get_wl_output (GdkDisplay *display,
                                                     int         monitor_num);

void _gdk_wayland_surface_set_grab_seat (GdkSurface      *surface,
                                        GdkSeat        *seat);

guint32 _gdk_wayland_display_get_serial (GdkWaylandDisplay *display_wayland);
void _gdk_wayland_display_update_serial (GdkWaylandDisplay *display_wayland,
                                         guint32            serial);

cairo_surface_t * _gdk_wayland_display_create_shm_surface (GdkWaylandDisplay *display,
                                                           int                width,
                                                           int                height,
                                                           guint              scale);
struct wl_buffer *_gdk_wayland_shm_surface_get_wl_buffer (cairo_surface_t *surface);
gboolean _gdk_wayland_is_shm_surface (cairo_surface_t *surface);

GdkWaylandSelection * gdk_wayland_display_get_selection (GdkDisplay *display);
GdkWaylandSelection * gdk_wayland_selection_new (void);
void gdk_wayland_selection_free (GdkWaylandSelection *selection);


struct wl_data_source * gdk_wayland_selection_get_data_source (GdkSurface *owner);
void gdk_wayland_selection_unset_data_source (GdkDisplay *display);

EGLSurface gdk_wayland_surface_get_egl_surface (GdkSurface *surface,
                                               EGLConfig config);
EGLSurface gdk_wayland_surface_get_dummy_egl_surface (GdkSurface *surface,
                                                     EGLConfig config);

struct gtk_surface1 * gdk_wayland_surface_get_gtk_surface (GdkSurface *surface);

void gdk_wayland_seat_set_global_cursor (GdkSeat   *seat,
                                         GdkCursor *cursor);

struct wl_output *gdk_wayland_surface_get_wl_output (GdkSurface *surface);

void gdk_wayland_surface_inhibit_shortcuts (GdkSurface *surface,
                                           GdkSeat   *gdk_seat);
void gdk_wayland_surface_restore_shortcuts (GdkSurface *surface,
                                           GdkSeat   *gdk_seat);

void gdk_wayland_surface_update_scale (GdkSurface *surface);


#endif /* __GDK_PRIVATE_WAYLAND_H__ */
