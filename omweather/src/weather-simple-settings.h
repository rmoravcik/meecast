/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU weather-config.h General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifndef _weather_simple_settings_h
#define _weather_simple_settings_h 1
/*******************************************************************************/
#include "weather-common.h"
/*******************************************************************************/
#ifdef OS2009
gchar *picker_print_func (HildonTouchSelector *selector, gpointer userdata);
#endif
void widget_styles_save(GtkWidget *window);
void weather_simple_window_settings(gpointer user_data);
GtkWidget* create_and_fill_stations_buttons(GtkWidget *main_table);
GtkTreeIter add_station_to_user_list(gchar *weather_station_name, gchar *weather_station_id,
          gboolean is_gps, gchar *source, gint position);
void highlight_current_item(GtkTreeView *tree_view, GtkListStore *list, gchar *current);
void list_changed(GtkTreeSelection *sel,  gpointer user_data, gchar *name);
GtkWidget* create_station_button(gint station_number, gchar* station_name_s, gchar *station_code_s, gchar *station_source_s,
                      gint country_id, gchar *station_country_s, gint region_id, gchar *station_region_s, gboolean is_gps);
void choose_button_handler(GtkWidget *button, GdkEventButton *event, gpointer user_data);
void save_station(GtkWidget *window);
void widget_style_setup_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data);
GtkWidget* create_button(gchar* name, gchar* value, gchar* button_name, gchar* parameter_name, GtkWidget* widget, GtkListStore *list);
void create_and_fill_units_box(GtkWidget *main_table);
void create_and_fill_update_box(GtkWidget *main_table);
void create_and_fill_widget_style_box(GtkWidget *main_table);
void station_setup_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data);
void update_button_handler(GtkWidget *button, GdkEventButton *event, gpointer user_data);
void update_save(GtkWidget *window);
void units_button_handler(GtkWidget *button, GdkEventButton *event, gpointer user_data);
void units_save(GtkWidget *window);
void manual_button_handler(GtkWidget *window, GdkEventButton *event, gpointer user_data);
void gps_button_handler(GtkWidget *window, GdkEventButton *event, gpointer user_data);
void delete_station_from_user_list(gchar *station_selected, gpointer user_data);
void check_current_connection(void);
void free_list(GSList *list);
void clear_station(GtkWidget *window);
void changing_of_location (GtkWidget *button, gpointer user_data);
void reload_omw_plugin(void);
void true_free(gpointer p);
/*******************************************************************************/
#endif
