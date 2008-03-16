/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) Superkaramba's Liquid Weather ++ team
 *	for ALL the artwork (icons)
 *        The maintainer (also the main author I believe)
 *        is Matthew <dmbkiwi@yahoo.com>
 *  http://liquidweather.net/icons.html#iconsets
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifndef _weather_home_h
#define _weather_home_h 1
/*******************************************************************************/
#include "weather-common.h"
#include <locale.h>
#include <libintl.h>
/*******************************************************************************/
gboolean change_station_select(GtkWidget *widget, gpointer user_data);
WDB* create_weather_day_button(const char *text, const char *icon,
				const int icon_size, gboolean transparency,
				char font_size, GdkColor *color);
void delete_weather_day_button(gboolean after_all_destroy,WDB **day);
void weather_buttons_fill(gboolean check_error);
void update_weather(void);
void set_font_size(GtkWidget *widget, char font_size);
void set_background_color(GtkWidget *widget, GdkColor *bgc);
void create_panel(GtkWidget* panel, gint layout, gboolean transparency, gchar* st_name, char f_size);
int c2f(int temp);
void weather_window_popup_show(GtkWidget *widget,GdkEvent *event,gpointer user_data);
gboolean download_html(gpointer data);
void update_weather(void);
void free_memory(gboolean flag);
void swap_temperature(int *hi, int *low);
void station_error_window(void);
void menu_init(void);
void set_font_color(GtkWidget *widget, guint16 red, guint16 green, guint16 blue);
void set_widget_background(GtkWidget *widget, guint16 red, guint16 green, guint16 blue);
gboolean switch_timer_handler(gpointer data);
gboolean expose_main_window(GtkWidget *widget, GdkEventExpose *event);
gboolean expose_parent(GtkWidget *widget, GdkEventExpose *event);
gboolean remitted_update(void);
GtkWidget* create_current_weather_simple_widget(GSList *current, char f_size);
GtkListStore* create_user_stations_list(void);
/*******************************************************************************/
extern void timer(int interval);
extern void create_timer_with_interval(guint interval);
extern void weather_initialize_dbus(void);
extern void add_periodic_event(time_t last_update);
extern void remove_daytime_event(void);
extern void clean_download(void);
extern void weather_window_settings(GtkWidget *widget,GdkEvent *event,gpointer user_data);
extern int new_read_config(AppletConfig*);
extern void new_config_save(AppletConfig*);
extern GtkListStore* create_items_list(const char *filename, long start, long end,
					long *items_number);
extern GtkListStore* create_time_update_list(void);
extern void fill_station_from_clock_plugin_data(void);
extern void reinitilize_stations_list2(GSList *stlist);
extern gboolean show_popup_window_handler(GtkWidget *widget, GdkEvent *event,
				    gpointer user_data);
extern int parse_weather_com_xml(void);
extern int new_parse_weather_com_xml(void);
extern time_t last_update_time(GSList *object);
extern float convert_wind_units(int to, float value);
extern void initial_gps_connect(void);
/*******************************************************************************/
extern int	boxs_offset[];
extern gchar	path_large_icon[];
/*******************************************************************************/
typedef struct _HomeAppletHandler HomeAppletHandler;
/*******************************************************************************/
struct _HomeAppletHandler {
    GObject parent;
    gchar *desktoppath;
    gchar *libraryfile;
    GtkEventBox *eventbox;
    gint x;
    gint y;
    gint width;
    gint height;
    gint minwidth;
    gint minheight;
    gint resizable_width;
    gint resizable_height;
};
/*******************************************************************************/
#endif
