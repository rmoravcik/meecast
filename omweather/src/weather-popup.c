/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006 Vlad Vasiliev
 * Copyright (C) 2006 Pavel Fialko
 * Copyright (C) 2008 Andrew Olmsted
 * 	for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
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
#include "weather-popup.h"
#include "weather-utils.h"
#include "weather-data.h"
#include <sys/time.h>
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
#define MOON_ICONS		"/usr/share/omweather/moon_icons/"
#define COPYRIGHT_ICONS		"/usr/share/omweather/copyright_icons/"
/*******************************************************************************/
/*
#ifdef HILDON
    if(app->timer_for_os2008 != 0)
    	 g_source_remove(app->timer_for_os2008);
#endif

*/
/*******************************************************************************/
GtkWidget* create_sun_time_widget(GSList *day){
    GtkWidget	*main_widget = NULL,
		*main_label;
    gchar	buffer[1024],
	        temp_buffer[1024],
		time_buffer[1024];
    struct tm	time_show = {0};
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!day)
	return NULL;
/* prepare additional time values */

    memset(buffer, 0, sizeof(buffer));
    memset(time_buffer, 0, sizeof(time_buffer));
    memset(temp_buffer, 0, sizeof(temp_buffer));
    
    snprintf(buffer, sizeof(buffer) - 1, "%s", _("Sunrise: "));
    strptime(item_value(day, "day_sunrise"), "%r", &time_show);
    if(strstr(item_value(day, "day_sunrise"), "PM"))
	time_show.tm_hour += 12;
    
    strftime(temp_buffer, sizeof(temp_buffer) - 1,
			"%X", &time_show);
    /* Remove chars of seconds */
    memmove(temp_buffer+4,temp_buffer+7,6);
    
    strcat(buffer, temp_buffer);
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
			" %s", _("Sunset: "));

    memset(time_buffer, 0, sizeof(time_buffer));
    memset(temp_buffer, 0, sizeof(temp_buffer));
    strptime(item_value(day, "day_sunset"), "%r", &time_show);
    if(strstr(item_value(day, "day_sunset"), "PM"))
	time_show.tm_hour += 12;

    strftime(temp_buffer, sizeof(temp_buffer) - 1,
			"%X ", &time_show);
    /* Remove chars of seconds */
    memmove(temp_buffer+4,temp_buffer+7,6);
    
    strcat(buffer, temp_buffer);

    main_label = gtk_label_new(buffer);
    set_font(main_label, NULL, 18);
    main_widget = gtk_hbox_new(FALSE, 10);
    
    /* Packing all to the box */
    gtk_box_pack_start(GTK_BOX(main_widget), main_label, TRUE, TRUE, 0);

    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_moon_phase_widget(GSList *current){
    GtkWidget	*main_widget = NULL,
		*main_label = NULL;
    gchar	buffer[1024],
		icon[50],
		*space_symbol = NULL;
    GdkPixbuf   *icon_buffer;                                                                                                             
    GtkWidget   *icon_image;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!current)
	return NULL;

    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
			"%s",
			(char*)hash_table_find(item_value(current, "moon_phase"), FALSE));
    main_label = gtk_label_new(buffer);
    set_font(main_label, NULL, 14);

    main_widget = gtk_hbox_new(FALSE, 0);
/* Moon icon */
    sprintf(icon, "%s%s.png", MOON_ICONS, item_value(current, "moon_phase"));
    space_symbol = strchr(icon, ' ');
    if(space_symbol)
	*space_symbol = '_';
    icon_buffer = gdk_pixbuf_new_from_file_at_size(icon, GIANT_ICON_SIZE,
						    GIANT_ICON_SIZE, NULL);
    if(icon_buffer){
	icon_image = gtk_image_new_from_pixbuf(icon_buffer);
	g_object_unref(G_OBJECT(icon_buffer));
    }
    else
    	icon_image = NULL;

    if(icon_image)
	gtk_box_pack_start(GTK_BOX(main_widget), icon_image, FALSE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(main_widget), main_label, FALSE, TRUE, 10);
    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_time_updates_widget(GSList *current){
    GtkWidget	*main_widget = NULL,
    		*label_update;
    gchar       buffer[1024],
		full_filename[2048];
    struct stat	statv;
    time_t	tmp_time = 0;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!current)
	return NULL;

    tmp_time = last_update_time(current);

    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer) - 1, "%s", _("Last update at server: "));
    if(tmp_time <= 0)	/* if weather data for station wasn't download */
	strcat(buffer, _("Unknown"));
    else{
	strftime(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
		    "%X %x", localtime(&tmp_time));
	snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
		    " %s", _("station local time"));
    }
    strcat(buffer, "\n");
    
    sprintf(full_filename, "%s/%s.xml", app->config->cache_dir_name,
		app->config->current_station_id);
    if(stat(full_filename, &statv)){
    	sprintf(buffer + strlen(buffer), "%s%s",
		_("Last update from server: "), _("Unknown"));
    }	
    else{ 
	snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
		    "%s", _("Last update from server: "));
    	strftime(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
		"%X %x", localtime(&statv.st_mtime));
	snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
		" %s", _("current location local time"));
    }
    label_update = gtk_label_new(buffer);    
    set_font(label_update, NULL, 10);
    main_widget = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(main_widget), label_update, TRUE, TRUE, 0);

    return main_widget;
}
/*******************************************************************************/
gboolean weather_window_popup(GtkWidget *widget, GdkEvent *event,
                    	    				    gpointer user_data){
    GtkWidget	*window_popup = NULL,
		*notebook = NULL,
		*tab = NULL,
                *hour_tab = NULL,
		*current_tab = NULL,
		*label = NULL,
		*vbox = NULL,
		*buttons_box = NULL,
		*settings_button = NULL,
		*refresh_button = NULL,
		*about_button = NULL,
		*close_button = NULL,
		*label_box = NULL,
		*copyright_box = NULL,
		*no_weather_box = NULL;
    gint	active_tab = 0,
		k = 0,
		i = 0;
    gchar	*day_name = NULL;
    time_t      current_time = 0,
                diff_time,
                current_data_last_update = 0;
    GSList	*tmp = NULL,
		*day = NULL,
                *hour_weather = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* day number */
    active_tab = (gint)user_data;
/* if no one station present in list show settings window */
    if(!app->config->current_station_id){
	weather_window_settings(NULL, NULL, (gpointer)active_tab);
	return FALSE;
    }
/* Main window */
    window_popup = hildon_window_new();

    g_object_set_data(G_OBJECT(window_popup), "active_tab", (gpointer)active_tab);
    gtk_window_fullscreen(GTK_WINDOW(window_popup));
    /* create frame vbox */    
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window_popup), vbox);
/* station name */
    label_box = gtk_event_box_new();
	gtk_container_add(GTK_CONTAINER(label_box),create_window_header(app->config->current_station_name, window_popup));
    gtk_box_pack_start(GTK_BOX(vbox),
			label_box,
			FALSE, TRUE, 0);
	gtk_event_box_set_visible_window(GTK_EVENT_BOX(label_box),FALSE);
/* create tabs widget */
    notebook = gtk_notebook_new();

    gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
/* Current weather */
    current_time = time(NULL); /* get current day */
    /* correct time for current location */
    diff_time = calculate_diff_time(atol(item_value(app->wsd.location, "station_time_zone")));
#ifndef RELEASE
    fprintf(stderr, "\n>>>>>>>Diff time=%li<<<<<<\n", diff_time);
#endif
    current_time += diff_time;

    current_data_last_update = last_update_time(app->wsd.current);
    if(!app->wsd.current_data_is_invalid &&
	    (current_data_last_update >
		( current_time - app->config->data_valid_interval)) &&
	    (current_data_last_update <
		( current_time + app->config->data_valid_interval)))
	current_tab = create_current_tab(app->wsd.current);

    if(current_tab)
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
				current_tab,
				gtk_label_new(_("Now")));
/* if weather is separated than hide one day */
    (app->config->separate) ? (k = 1) : (k = 0);
/* Detailed weather tab */
    if(app->config->show_weather_for_two_hours)
        hour_tab = create_hour_tab();
    if(hour_tab)
        gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
                                    hour_tab,
                                    gtk_label_new(_("Detailed")));
/* Day tabs */
    tmp = app->wsd.days;
    while(tmp && i < app->config->days_to_show){
	day = (GSList*)tmp->data;
	tab = create_day_tab(app->wsd.current, day, &day_name);
	if(tab){
	    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
        				tab,
        				gtk_label_new(day_name));
	}
	day_name && (g_free(day_name), day_name = NULL);
	tmp = g_slist_next(tmp);
	i++;
    }
/* prepare day tabs */
    if(gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 0){
	gtk_box_pack_start(GTK_BOX(vbox), notebook,
			TRUE, TRUE, 0);
        gtk_widget_show(notebook);
    }
/* Bottom buttons box */
    /*buttons_box = gtk_toolbar_new();
    hildon_window_add_toolbar(HILDON_WINDOW(window_popup),GTK_TOOLBAR(buttons_box));*/
    buttons_box = gtk_hbox_new(TRUE,0);
    gtk_widget_set_size_request(buttons_box, -1, 60);
    
    /* Settings button */
    /*settings_button = create_tool_item(BUTTON_ICONS, "settings", 40);*/
    settings_button = create_button_with_image(BUTTON_ICONS, "settings", 40, FALSE, FALSE);
    g_signal_connect(G_OBJECT(settings_button), "button-release-event",
                        G_CALLBACK(settings_button_handler),
			(gpointer)window_popup);
    /* Refresh buton */
    /*refresh_button = create_tool_item(BUTTON_ICONS, "refresh", 40);*/
    refresh_button = create_button_with_image(BUTTON_ICONS, "refresh", 40, FALSE, FALSE);
	g_signal_connect(G_OBJECT(refresh_button), "button-release-event",
                        G_CALLBACK(refresh_button_handler),
			(gpointer)window_popup);
    /* About buton */
    /*about_button = create_tool_item(BUTTON_ICONS, "about", 40);*/
    about_button = create_button_with_image(BUTTON_ICONS, "about", 40, FALSE, FALSE);
    g_signal_connect(G_OBJECT(about_button), "button-release-event",
                        G_CALLBACK(about_button_handler),
			NULL);
    /* Close button */
    /*close_button = create_tool_item(BUTTON_ICONS, "close", 40);*/
    close_button = create_button_with_image(BUTTON_ICONS, "close", 40, FALSE, FALSE);
    g_signal_connect(G_OBJECT(close_button), "button-release-event",
                        G_CALLBACK(popup_close_button_handler),
			(gpointer)window_popup);

/* Pack buttons to the buttons box */
	/*gtk_toolbar_insert(GTK_TOOLBAR(buttons_box), GTK_TOOL_ITEM(settings_button), -1);
	gtk_toolbar_insert(GTK_TOOLBAR(buttons_box), GTK_TOOL_ITEM(refresh_button), -1);
	gtk_toolbar_insert(GTK_TOOLBAR(buttons_box), GTK_TOOL_ITEM(about_button), -1);
	gtk_toolbar_insert(GTK_TOOLBAR(buttons_box), GTK_TOOL_ITEM(close_button), -1);*/
	gtk_box_pack_start(GTK_BOX(buttons_box), settings_button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(buttons_box), refresh_button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(buttons_box), about_button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(buttons_box), close_button, FALSE, FALSE, 0);

    /* check if no data file for this station */
    if(gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 0){
	if(active_tab == -1){
	    hildon_banner_show_information(app->main_window,
					    NULL,
					    _("No weather data for this day."));
	    gtk_widget_destroy(window_popup);
	    return FALSE;
	}
	else{
	    if(app->config->separate){
		if(!current_tab){
		    if(active_tab)
			active_tab--;
		    else{
			hildon_banner_show_information(app->main_window,
							NULL,
							_("No current weather data."));
		        gtk_widget_destroy(window_popup);
			return FALSE;
		    }
		}
	    }
	    else
		if(current_tab && active_tab)
		    active_tab++;
	    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), active_tab);
	}
    }
    else{
	gtk_widget_destroy(notebook);
	no_weather_box = gtk_event_box_new();
	gtk_container_add(GTK_CONTAINER(no_weather_box),label = gtk_label_new(_("No weather data for this station.")));
	gtk_box_pack_start(GTK_BOX(vbox), no_weather_box, TRUE, TRUE, 0);
	gtk_event_box_set_visible_window(GTK_EVENT_BOX(no_weather_box),FALSE);
	set_font(label, NULL, 24);
    }
/* Show copyright widget */
	fprintf(stderr, "\n>>>>>>>>>>>>>>>>Source %d\n", app->config->current_station_source);
	if(app->config->current_station_source < 0)
	    app->config->current_station_source = app->config->weather_source;
	copyright_box = gtk_event_box_new();
	gtk_container_add(GTK_CONTAINER(copyright_box),
			create_copyright_widget(weather_sources[app->config->current_station_source].name, NULL));
    gtk_box_pack_start(GTK_BOX(vbox),
			copyright_box,
			FALSE, TRUE, 0);
	gtk_event_box_set_visible_window(GTK_EVENT_BOX(copyright_box),TRUE);

	gtk_box_pack_start(GTK_BOX(vbox),buttons_box,FALSE,FALSE,0);

    if (app->config->theme_override_in_use)
    {
    	gint i;
    	GdkColor color={0,0.8*65535,0.8*65535,0.8*65535};
    	gtk_rc_parse("/usr/share/omweather/theme/gtk-2.0/gtkrc");
    	gtk_rc_reset_styles(gtk_settings_get_for_screen(gdk_screen_get_default()));
    	if (GTK_IS_NOTEBOOK(notebook))
    	{
			for (i=0;i<gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook));i++)
			{
				g_signal_connect(G_OBJECT(gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook),i)),"expose-event",G_CALLBACK(draw_top_gradient),NULL);
			}
		}
		g_signal_connect(G_OBJECT(no_weather_box),"expose-event",G_CALLBACK(draw_top_gradient),NULL);

		gtk_widget_modify_bg(copyright_box,GTK_STATE_NORMAL,&color);
		g_signal_connect(G_OBJECT(label_box),"expose-event",G_CALLBACK(draw_label_gradient),NULL);
		g_signal_connect(G_OBJECT(buttons_box),"expose-event",G_CALLBACK(draw_bottom_gradient),NULL);
	}
    gtk_widget_show_all(window_popup);
    #ifdef DEBUGFUNCTIONCALL
	END_FUNCTION;
    #endif
    return FALSE;
}
/*******************************************************************************/
/* For debug */
/*
struct timeval tv1,tv2,dtv;

struct timezone tz;

void time_start() { gettimeofday(&tv1, &tz); }

double time_stop()

{  
    gettimeofday(&tv2, &tz);
    dtv.tv_sec= tv2.tv_sec  -  tv1.tv_sec;
    dtv.tv_usec=tv2.tv_usec -  tv1.tv_usec;
    if(dtv.tv_usec<0) { dtv.tv_sec--; dtv.tv_usec+=1000000; }
        return dtv.tv_sec*1000.0+dtv.tv_usec/1000.0;
}
*/
/*******************************************************************************/
void settings_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data){
    gint day_number
	= (gint)g_object_get_data(G_OBJECT(user_data), "active_tab");
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
/* For debug speed of creating setting window */
/* time_start();  */
    gtk_widget_destroy(GTK_WIDGET(user_data));
    weather_window_settings(NULL, NULL, (gpointer)day_number);
/* fprintf(stderr,"Time: %lf msec Pi = %lf\n",time_stop(),weather_window_settings);*/
}
/*******************************************************************************/
void refresh_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    gtk_widget_destroy(GTK_WIDGET(user_data));
    update_weather(TRUE);
}
/*******************************************************************************/
void about_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data){
    create_about_dialog();
}
/*******************************************************************************/
void popup_close_button_handler(GtkWidget *button, GdkEventButton *event,
								gpointer user_data){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    gtk_widget_destroy(GTK_WIDGET(user_data));
}
/*******************************************************************************/
GtkWidget* create_day_tab(GSList *current, GSList *day, gchar **day_name){
    const gint	font_size = 14;
    GtkWidget	*main_widget = NULL,
		*day_night_hbox = NULL,
		*day_vbox = NULL,
		*night_vbox = NULL,
		*title = NULL,
		*day_icon = NULL,
		*night_icon = NULL,
		*day_label = NULL,
		*night_label = NULL,
		*day_label_temperature_hbox = NULL,
		*night_label_temperature_hbox = NULL,
		*day_icon_text_hbox = NULL,
		*night_icon_text_hbox = NULL,
		*day_text_vbox = NULL,
		*night_text_vbox = NULL,
		*day_text = NULL,
		*night_text = NULL;
    gchar	buffer[1024],
		symbol = 'C';
    struct tm	tmp_time_date_struct = {0};
    GdkPixbuf	*icon = NULL;
    gint	hi_temp,
		low_temp,
		day_invalid_count = 0,
		night_invalid_count = 0;
    const gchar	*wind_units_str[] = { "m/s", "km/h", "mi/h" };
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!day)
	return NULL;
    /* prepare temperature */
    if(!strcmp(item_value(day, "day_hi_temperature"), "N/A")){
	hi_temp = INT_MAX;
	day_invalid_count++;
    }
    else
	hi_temp = atoi(item_value(day, "day_hi_temperature"));
    if(!strcmp(item_value(day, "day_low_temperature"), "N/A")){
	low_temp = INT_MAX;
	night_invalid_count++;
    }
    else
	low_temp = atoi(item_value(day, "day_low_temperature"));
    if(app->config->temperature_units == FAHRENHEIT){
	(hi_temp != INT_MAX) && ( hi_temp = c2f(hi_temp) );
	(low_temp != INT_MAX) && ( low_temp = c2f(low_temp) );
	symbol = 'F';
    }
    main_widget = gtk_vbox_new(FALSE, 0);
    /* prepare localized day name */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"%s", item_value(day, "day_name"));
    strptime(buffer, "%a", &tmp_time_date_struct);
    *day_name = g_strdup(buffer);
    /* prepare title */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"%s %s", item_value(day, "day_name"), item_value(day, "day_date"));
    strptime(buffer, "%A %b %d", &tmp_time_date_struct);
    memset(buffer, 0, sizeof(buffer));
    strftime(buffer, sizeof(buffer) - 1, "%A, %d %B", &tmp_time_date_struct);
    title = gtk_label_new(buffer);
    set_font(title, NULL, font_size + 2); 
    /* create 24 hours data widget */
    day_night_hbox = gtk_hbox_new(FALSE, 10);
    /* day data */
    day_vbox = gtk_vbox_new(FALSE, 0);
    /* hbox for day label and temperature */
    day_label_temperature_hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(day_vbox),
			day_label_temperature_hbox,
			TRUE, TRUE, 0);
    /* prepare day label */
    day_label = gtk_label_new(_("Day:"));
    gtk_box_pack_start(GTK_BOX(day_label_temperature_hbox),
			day_label, FALSE, TRUE, 0);
    /* day temperature */
    memset(buffer, 0, sizeof(buffer));
    if(hi_temp == INT_MAX)
	strncat(buffer, (char*)hash_table_find("N/A", FALSE),
			( (strlen((char*)hash_table_find("N/A", FALSE)) > sizeof(buffer)) ?
		        (sizeof(buffer) - 1) :
		        (strlen((char*)hash_table_find("N/A", FALSE))) ) );
    else
	snprintf(buffer, sizeof(hi_temp) + 
			( (strlen(("\302\260%c")) > sizeof(buffer)) ?
		        (sizeof(buffer) - 1) : (strlen("\302\260%c")) ),
			"%d\302\260%c", hi_temp, symbol);
    gtk_box_pack_start(GTK_BOX(day_label_temperature_hbox),
			gtk_label_new(buffer), FALSE, TRUE, 40);
    /* hbox for icon and text */
    day_icon_text_hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(day_vbox),
			day_icon_text_hbox, TRUE, TRUE, 0);
    /* day icon */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s%s.png", path_large_icon, item_value(day, "day_icon"));
    icon = gdk_pixbuf_new_from_file_at_size(buffer,
					    BIG_ICON_SIZE,
					    BIG_ICON_SIZE, NULL);
    day_icon = gtk_image_new_from_pixbuf(icon);
    if(icon)
	g_object_unref(icon);
    gtk_box_pack_start(GTK_BOX(day_icon_text_hbox),
			day_icon, TRUE, TRUE, 5);
/* preapare day text */
    gtk_box_pack_start(GTK_BOX(day_icon_text_hbox),
			day_text_vbox = gtk_vbox_new(FALSE, 0),
			TRUE, TRUE, 0);
    memset(buffer, 0, sizeof(buffer));
    if(!strcmp((char*)item_value(day, "day_title"), "N/A"))
	day_invalid_count++;
    strcat(buffer, (char*)hash_table_find(item_value(day, "day_title"), FALSE));
    strcat(buffer, _("\nHumidity: "));
    if(strcmp(item_value(day, "day_humidity"), "N/A"))
	sprintf(buffer + strlen(buffer), "%s%%\n",
		item_value(day, "day_humidity"));
    else{
	sprintf(buffer + strlen(buffer), "%s\n",
			(char*)hash_table_find("N/A", FALSE));
	day_invalid_count++;
    }
    strcat(buffer, _("Wind: "));
    if(!strcmp((char*)item_value(day, "day_wind_title"), "N/A"))
	day_invalid_count++;
    if(!strcmp((char*)item_value(day, "day_wind_speed"), "N/A"))
	day_invalid_count++;
    sprintf(buffer + strlen(buffer), "%s\n%.2f %s",
	    (char*)hash_table_find(item_value(day, "day_wind_title"), FALSE),
	    convert_wind_units(app->config->wind_units, atof(item_value(day, "day_wind_speed"))),
	    (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
    day_text = gtk_label_new(buffer);
    set_font(day_label, NULL, font_size);
    set_font(day_text, NULL, font_size-1);
    gtk_box_pack_start(GTK_BOX(day_text_vbox),
			day_text, TRUE, TRUE, 0);
    /* night data */
    night_vbox = gtk_vbox_new(FALSE, 0);
    /* hbox for night label and temperature */
    night_label_temperature_hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(night_vbox),
			night_label_temperature_hbox,
			TRUE, TRUE, 0);
    /* prepare night label */
    night_label = gtk_label_new(_("Night:"));
    gtk_box_pack_start(GTK_BOX(night_label_temperature_hbox),
			night_label, FALSE, TRUE, 0);
    /* night temperature */
    memset(buffer, 0, sizeof(buffer));
    if(low_temp == INT_MAX)
	strncat(buffer, (char*)hash_table_find("N/A", FALSE),
			( (strlen((char*)hash_table_find("N/A", FALSE)) > sizeof(buffer)) ?
		        (sizeof(buffer) - 1) :
		        (strlen((char*)hash_table_find("N/A", FALSE))) ) );
    else
	snprintf(buffer, sizeof(low_temp) + 
			( (strlen(("\302\260%c")) > sizeof(buffer)) ?
		        (sizeof(buffer) - 1) : (strlen("\302\260%c")) ),
			"%d\302\260%c", low_temp, symbol);
    gtk_box_pack_start(GTK_BOX(night_label_temperature_hbox),
			gtk_label_new(buffer), FALSE, TRUE, 40);
    /* hbox for icon and text */
    night_icon_text_hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(night_vbox),
			night_icon_text_hbox, TRUE, TRUE, 0);
    /* night icon */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s%s.png", path_large_icon, item_value(day, "night_icon"));
    icon = gdk_pixbuf_new_from_file_at_size(buffer,
					    BIG_ICON_SIZE,
					    BIG_ICON_SIZE, NULL);
    night_icon = gtk_image_new_from_pixbuf(icon);
    if(icon)
	g_object_unref(icon);
    gtk_box_pack_start(GTK_BOX(night_icon_text_hbox),
			night_icon, TRUE, TRUE, 5);
/* preapare night text */
    gtk_box_pack_start(GTK_BOX(night_icon_text_hbox),
			night_text_vbox = gtk_vbox_new(FALSE, 0),
			TRUE, TRUE, 0);
    memset(buffer, 0, sizeof(buffer));
    if(!strcmp((char*)item_value(day, "night_title"), "N/A"))
        night_invalid_count++;
    strcat(buffer, (char*)hash_table_find(item_value(day, "night_title"), FALSE));
    strcat(buffer, _("\nHumidity: "));
    if(strcmp(item_value(day, "night_humidity"), "N/A"))
	sprintf(buffer + strlen(buffer), "%s%%\n",
		item_value(day, "night_humidity"));
    else{
	sprintf(buffer + strlen(buffer), "%s\n",
			(char*)hash_table_find("N/A", FALSE));
	night_invalid_count++;			
    }
    strcat(buffer, _("Wind: "));
    if(!strcmp((char*)item_value(day, "night_wind_title"), "N/A"))
        night_invalid_count++;
    if(!strcmp((char*)item_value(day, "night_wind_speed"), "N/A"))
        night_invalid_count++;
    sprintf(buffer + strlen(buffer), "%s\n%.2f %s",
	    (char*)hash_table_find(item_value(day, "night_wind_title"), FALSE),
	    convert_wind_units(app->config->wind_units, atof(item_value(day, "night_wind_speed"))),
	    (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
    night_text = gtk_label_new(buffer);
    set_font(night_label, NULL, font_size);
    set_font(night_text, NULL, font_size-1);
    gtk_box_pack_start(GTK_BOX(night_text_vbox),
			night_text, TRUE, TRUE, 0);
    /* add day and night items to main widget */
    /* check for N/A fields in day */
    if(day_invalid_count < 5)
	gtk_box_pack_start(GTK_BOX(day_night_hbox),
			    day_vbox,
			    TRUE, FALSE, 5);
    else
	gtk_widget_destroy(day_vbox);
    /* check for N/A fields in night */
    if(night_invalid_count < 5)
	gtk_box_pack_start(GTK_BOX(day_night_hbox),
			    night_vbox,
			    TRUE, FALSE, 5);
    else
	gtk_widget_destroy(night_vbox);
    /* day name with date */
    gtk_box_pack_start(GTK_BOX(main_widget), title, FALSE, FALSE, 5);
    /* day and/or night data */
    gtk_box_pack_start(GTK_BOX(main_widget), day_night_hbox, FALSE, FALSE, 5);
    /* sunrise and sunset time */
    gtk_box_pack_start(GTK_BOX(main_widget),
			create_sun_time_widget(day),
			TRUE, FALSE, 0);
    /* last update time */
    if(current)
	gtk_box_pack_start(GTK_BOX(main_widget),
			    create_time_updates_widget(current),
			    TRUE, FALSE, 5);
    gtk_widget_show_all(main_widget);
    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_current_tab(GSList *current){
    GtkWidget   *main_widget = NULL,
                *icon_text_hbox = NULL,
                *text = NULL,
		*icon_image = NULL;
    gchar       buffer[1024],
                *units;
    const gchar *wind_units_str[] = { "m/s", "km/h", "mi/h" };
    GdkPixbuf   *icon = NULL;
    float       tmp_distance = 0.0f,
		tmp_pressure = 0.0f;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!current)
	return NULL;
    main_widget = gtk_vbox_new(FALSE, 0);
    icon_text_hbox = gtk_hbox_new(FALSE, 0);
/* icon */
    sprintf(buffer,"%s%s.png", path_large_icon, item_value(current, "icon"));
    icon = gdk_pixbuf_new_from_file_at_size(buffer, GIANT_ICON_SIZE,
						    GIANT_ICON_SIZE, NULL);
    icon_image = gtk_image_new_from_pixbuf(icon);
    if(icon)
        g_object_unref(icon);
    gtk_box_pack_start(GTK_BOX(icon_text_hbox), icon_image, TRUE, TRUE, 0);
    /* temperature */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s\n", item_value(current, "title"));
    sprintf(buffer + strlen(buffer), "%s",  _("Temperature: "));
    sprintf(buffer + strlen(buffer), "  %d\302\260",
                ((app->config->temperature_units == CELSIUS) ?
			( atoi(item_value(current, "day_hi_temperature"))) :
			( (int)c2f(atoi(item_value(current, "day_hi_temperature"))))));
    (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C\n")) )
                                                : ( strcat(buffer, _("F\n")) );
    /* feels like */
    sprintf(buffer + strlen(buffer), "%s", _("Feels like:"));
    sprintf(buffer + strlen(buffer), "  %d\302\260",
	    (app->config->temperature_units == CELSIUS) ?
		(atoi(item_value(current, "feel_like"))) :
		((int)c2f(atoi(item_value(current, "feel_like")))));
    (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C\n")) )
                                                : ( strcat(buffer, _("F\n")) );
/* humidity */
    if( strcmp(item_value(current, "humidity"), "N/A") ){
	sprintf(buffer + strlen(buffer), "%s", _("Humidity:"));
        sprintf(buffer + strlen(buffer), "  %d%%\n",
                atoi(item_value(current, "humidity")));
    }
/* visible */
    if( strcmp(item_value(current, "visible"), "N/A") ){
	sprintf(buffer + strlen(buffer), "%s", _("Visible:"));
	if( !strcmp(item_value(current, "visible"), "Unlimited") )
    	    sprintf(buffer + strlen(buffer), "  %s\n",
            	    (char*)hash_table_find("Unlimited", FALSE));
	else{
    	    tmp_distance = atof(item_value(current, "visible"));
            switch(app->config->distance_units){
                default:
                case METERS: units = _("m"); tmp_distance *= 1000.0f; break;
                case KILOMETERS: units = _("km"); tmp_distance *= 1.0f; break;
                case MILES: units = _("mi"); tmp_distance /= 1.609344f; break;
                case SEA_MILES: units = _("mi"); tmp_distance /= 1.852f; break;
            }
            sprintf(buffer + strlen(buffer), "  %.2f %s\n", tmp_distance, units);
        }
    }
/* pressure */
    if( strcmp(item_value(current, "pressure"), "N/A") ){
	sprintf(buffer + strlen(buffer), "%s", _("Pressure:"));

	tmp_pressure = atof(item_value(current, "pressure"));
	switch(app->config->pressure_units){
	    default:
	    case MB: units = _("mb"); break;
	    case INCH: units = _("inHg"); tmp_pressure = mb2inch(tmp_pressure); break;
	}

	sprintf(buffer + strlen(buffer), "  %.2f %s,", tmp_pressure, units);
	sprintf(buffer + strlen(buffer), "  %s\n",
		    item_value(current, "pressure_direction"));
    }
/* wind */
    if( strcmp(item_value(current, "wind_direction"), "N/A") ){
	sprintf(buffer + strlen(buffer), "%s", _("Wind:"));
	sprintf(buffer + strlen(buffer), "  %s\n",
		item_value(current, "wind_direction"));
	if( strcmp(item_value(current, "wind_speed"), "N/A") )
	    sprintf(buffer + strlen(buffer), "%s", _("Speed:"));
	    sprintf(buffer + strlen(buffer), "  %.2f %s\n",
		    convert_wind_units(app->config->wind_units, atof(item_value(current, "wind_speed"))),
		    (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
    }
/* gust */
    if( strcmp(item_value(current, "wind_gust"), "N/A") ){
        sprintf(buffer + strlen(buffer), "%s", _("Gust:"));
	sprintf(buffer + strlen(buffer), "  %.2f %s\n",
		convert_wind_units(app->config->wind_units, atof(item_value(current, "wind_gust"))),
		(char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
    }

    text = gtk_label_new(buffer);
    set_font(text, NULL, 14);
    gtk_box_pack_start(GTK_BOX(icon_text_hbox), text, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(icon_text_hbox), create_moon_phase_widget(current),
			TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(main_widget), icon_text_hbox, TRUE, TRUE, 0);
    /* last update time */
    gtk_box_pack_start(GTK_BOX(main_widget),
			    create_time_updates_widget(current),
			    TRUE, FALSE, 5);
    gtk_widget_show_all(main_widget);
    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_hour_tab(){
   GtkWidget   *main_widget = NULL,
               *window = NULL,
               *window_tmp = NULL, 
               *icon_window = NULL,
               *icon_text_hbox = NULL,
               *text = NULL,
               *icon_image = NULL;
   GSList      *hour_weather = NULL,
               *tmp = NULL; 
   gchar       buffer[1024],
               *units;
   const gchar *wind_units_str[] = {"m/s", "km/h", "mi/h" };
   GdkPixbuf   *icon = NULL;
   float       tmp_distance = 0.0f,
               tmp_pressure = 0.0f;
   int         i = 0,
               j = 0,  
               period = 6;
   #ifdef DEBUGFUNCTIONCALL 
        START_FUNCTION;
   #endif
   if(!app->wsd.hours_weather)
      return NULL;

   main_widget = gtk_vbox_new(FALSE, 0);
   window_tmp = gtk_hbox_new(FALSE, 0);
   tmp = app->wsd.hours_weather;
   while(tmp){
        hour_weather = (GSList*)tmp->data; 
        window = gtk_vbox_new(FALSE, 0);
        icon_text_hbox = gtk_hbox_new(FALSE, 0);
   /* icon */
        sprintf(buffer,"%s%s.png", path_large_icon, item_value(hour_weather, "hour_icon"));
        icon = gdk_pixbuf_new_from_file_at_size(buffer, SMALL_ICON_SIZE,
                                                   SMALL_ICON_SIZE, NULL);
        icon_image = gtk_image_new_from_pixbuf(icon);
        if(icon)
            g_object_unref(icon);
        gtk_box_pack_start(GTK_BOX(icon_text_hbox), icon_image, TRUE, FALSE, 0);
        memset(buffer, 0, sizeof(buffer));
   /* hour */
        sprintf(buffer + strlen(buffer), "%s", _("Time: "));
        sprintf(buffer + strlen(buffer), "  %s:00\n",
                                item_value(hour_weather, "hours"));
        sprintf(buffer + strlen(buffer), "%s\n",
                                item_value(hour_weather, "hour_title"));
   /* temperature */
        sprintf(buffer + strlen(buffer), "%s",  _("Temperature: "));
        sprintf(buffer + strlen(buffer), "  %d\302\260",
                   ((app->config->temperature_units == CELSIUS) ?
                   ( atoi(item_value(hour_weather, "hour_temperature"))) :
                   ( (int)c2f(atoi(item_value(hour_weather, "hour_temperature"))))));
        (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C\n")))
                                  : ( strcat(buffer, _("F\n")));
   /* feels like */
        sprintf(buffer + strlen(buffer), "%s", _("Feels like:"));
        sprintf(buffer + strlen(buffer), "  %d\302\260",
                  (app->config->temperature_units == CELSIUS) ?
                  (atoi(item_value(hour_weather, "hour_feels_like"))) :
                  ((int)c2f(atoi(item_value(hour_weather, "hour_feels_like")))));
        (app->config->temperature_units == CELSIUS) ? ( strcat(buffer, _("C\n")))                                               : ( strcat(buffer, _("F\n")));
   /* humidity */
        sprintf(buffer + strlen(buffer), "%s", _("Humidity:"));
        if( strcmp(item_value(hour_weather, "hour_humidity"), "N/A") ){
            sprintf(buffer + strlen(buffer), "  %d%%\n",
                           atoi(item_value(hour_weather, "hour_humidity")));
        }
        else{
            sprintf(buffer + strlen(buffer), "%s\n",
                            (char*)hash_table_find("N/A", FALSE));

        }
   /* wind */
        if( strcmp(item_value(hour_weather, "hour_wind_direction"), "N/A") ){
            sprintf(buffer + strlen(buffer), "%s", _("Wind:"));
            sprintf(buffer + strlen(buffer), "  %s\n",
                           item_value(hour_weather, "hour_wind_direction"));
            if( strcmp(item_value(hour_weather, "hour_wind_speed"), "N/A") )
                sprintf(buffer + strlen(buffer), "%s", _("Speed:"));
            sprintf(buffer + strlen(buffer), "  %.2f %s\n",
                convert_wind_units(app->config->wind_units, atof(item_value(hour_weather, "hour_wind_speed"))),
                (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));    
        }
   /* gust */
        if( strcmp(item_value(hour_weather, "hour_wind_gust"), "N/A") ){
            sprintf(buffer + strlen(buffer), "%s", _("Gust:"));
            sprintf(buffer + strlen(buffer), "  %.2f %s\n",
                 convert_wind_units(app->config->wind_units, atof(item_value(hour_weather, "hour_wind_gust"))),
                 (char*)hash_table_find((gpointer)wind_units_str[app->config->wind_units], FALSE));
        }
        text = gtk_label_new(buffer);
        set_font(text, NULL, 14);
        gtk_box_pack_start(GTK_BOX(window), icon_text_hbox, TRUE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(window), text, TRUE, FALSE, 0); 
        gtk_box_pack_start(GTK_BOX(window_tmp), window, TRUE, FALSE, 0);
        for (i=1;i<period;i++){
            tmp = g_slist_next(tmp);
            if (!tmp)
                break;
        }
   }

   gtk_box_pack_start(GTK_BOX(main_widget), window_tmp, TRUE, TRUE, 0);
   /* last update time */
   if(hour_weather)
       gtk_box_pack_start(GTK_BOX(main_widget),
                             create_time_updates_widget(hour_weather),
                             TRUE, FALSE, 5);
   gtk_widget_show_all(main_widget);
   return main_widget;
}
/***************************************************************************
****/
GtkWidget* create_copyright_widget(const gchar *text, const gchar *image){
    GtkWidget	*main_widget = NULL,
		*hbox = NULL,
		*label= NULL,
		*icon = NULL;
    GdkPixbuf   *icon_buffer = NULL;
    gchar	image_buffer[512],
		text_buffer[512];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    main_widget = gtk_vbox_new(FALSE, 0);
    hbox = gtk_hbox_new(FALSE, 0);
/* text */
    text_buffer[0] = 0;
    snprintf(text_buffer, sizeof(text_buffer) - 1, "%s%s",
		_("Weather data provided by: "), text);
    gtk_box_pack_start(GTK_BOX(hbox),
			label = gtk_label_new(text_buffer),
			FALSE, FALSE, 10);
    set_font(label, NULL, 12);
/* icon */
    if(image){
	sprintf(image_buffer, "%s%s.png", COPYRIGHT_ICONS, image);
	icon_buffer = gdk_pixbuf_new_from_file_at_size(image_buffer,
							MEDIUM_ICON_SIZE,
							MEDIUM_ICON_SIZE,
							NULL);
	if(icon_buffer){
	    icon = gtk_image_new_from_pixbuf(icon_buffer);
	    g_object_unref(G_OBJECT(icon_buffer));
	}
	else
    	    icon = NULL;
	if(icon)
	    gtk_box_pack_start(GTK_BOX(hbox), icon, FALSE, FALSE, 0);
    }
    gtk_box_pack_start(GTK_BOX(main_widget), hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(main_widget), gtk_hseparator_new(), FALSE, TRUE, 5);

    return main_widget;
}
/*******************************************************************************/
GtkWidget* create_window_header(const gchar *station_name, GtkWidget *popup_window){
    GtkWidget	*main_widget = NULL,
    	*vbox = NULL,
		*label = NULL,
		*previos_button = NULL,
		*next_button = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    main_widget = gtk_hbox_new(FALSE, 0);
    vbox = gtk_vbox_new(FALSE,20);
/* station name */
    label = gtk_label_new(station_name);
    set_font(label, NULL, 28);
/* previos button */
    previos_button = create_button_with_image(NULL, "qgn_list_hw_button_left",
                    				26, FALSE, FALSE);
/* next button */
    next_button = create_button_with_image(NULL, "qgn_list_hw_button_right",
                    				26, FALSE, FALSE);
    if(previos_button){
	gtk_box_pack_start(GTK_BOX(main_widget), previos_button, FALSE, FALSE, 10);
	g_signal_connect(G_OBJECT(previos_button), "button_press_event",
                	G_CALLBACK(change_station_prev), popup_window);
    }
    if(label)
	gtk_box_pack_start(GTK_BOX(main_widget), label, TRUE, TRUE, 0);
    if(next_button){
	gtk_box_pack_start(GTK_BOX(main_widget), next_button, FALSE, FALSE, 10);
	g_signal_connect(G_OBJECT(next_button), "button_press_event",
                	G_CALLBACK(change_station_next), popup_window);
    }
	gtk_box_pack_start(GTK_BOX(vbox),main_widget,FALSE,FALSE,20);
    return vbox;
}
/*******************************************************************************/
