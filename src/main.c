#include <main.h>

GRect r_battery_left;
GRect r_battery_right;
GRect r_battery_top;
GRect r_battery_bottom;
GRect r_time_text;
GRect r_date;
GRect r_time2;
int i_time_text;

unsigned int HexStringToUInt(char const* hexstring)	{
	APP_LOG(APP_LOG_LEVEL_INFO, "HexStringToUInt in");
	unsigned int result = 0;
	char const *c = hexstring;
	unsigned char thisC;

	while( (thisC = *c) != 0 )
	{
		//thisC = toupper(thisC);
		result <<= 4;

		if( isdigit(thisC))
			result += thisC - '0';
		else if(isxdigit(thisC))
			result += thisC - 'A' + 10;
		else
		{
			APP_LOG(APP_LOG_LEVEL_DEBUG, "ERROR: Unrecognised hex character \"%c\"", thisC);
			return 0;
		}
		++c;
	}
	APP_LOG(APP_LOG_LEVEL_INFO, "HexStringToUInt out");
	return result;
}

GColor convert_to_gcolor(char hex_string[8]) {
	APP_LOG(APP_LOG_LEVEL_INFO, "convert_to_gcolor in");
	char str_color[7];
	memcpy(str_color, ++hex_string, 6);
	str_color[6] = '\0';
	int color = HexStringToUInt(str_color);
	APP_LOG(APP_LOG_LEVEL_INFO, "convert_to_gcolor out");
	return GColorFromHEX(color);
}

char *generate_hour_text() {
	APP_LOG(APP_LOG_LEVEL_INFO, "generate_hour_text in");
  	time_t temp = time(NULL);
  	struct tm *tick_time = localtime(&temp);
	
	if (tick_time->tm_min > 37) {
		tick_time->tm_hour += 1;
	}
	switch (tick_time->tm_hour) {
		case 1:
		case 13:
			return "one";
			break;
		case 2:
		case 14:
			return "two";
			break;
		case 3:
		case 15:
			return "three";
			break;
		case 4:
		case 16:
			return "four";
			break;
		case 5:
		case 17:
			return "five";
			break;
		case 6:
		case 18:
			return "six";
			break;
		case 7:
		case 19:
			return "seven";
			break;
		case 8:
		case 20:
			return "eight";
			break;
		case 9:
		case 21:
			return "nine";
			break;
		case 10:
		case 22:
			return "ten";
			break;
		case 11:
		case 23:
			return "eleven";
			break;
		case 12:
		case 0:
			return "twelve";
			break;
	}
	return "";
}

char *get_next_size_font(bool bigger) {
	APP_LOG(APP_LOG_LEVEL_INFO, "get_next_size_font in");
	switch (i_time_text) {
		case 0:
		return (bigger) ? FONT_KEY_GOTHIC_28_BOLD : FONT_KEY_GOTHIC_24_BOLD;
		break;
		case 1:
		return (bigger) ? FONT_KEY_BITHAM_30_BLACK : FONT_KEY_GOTHIC_24_BOLD;
		break;
		case 2:
		return (bigger) ? FONT_KEY_BITHAM_42_BOLD : FONT_KEY_GOTHIC_28_BOLD;
		break;
		case 3:
		return (bigger) ? FONT_KEY_BITHAM_42_BOLD : FONT_KEY_BITHAM_30_BLACK;
		break;
	}
	return "";
}

void update_time_text_font_size() {
	APP_LOG(APP_LOG_LEVEL_INFO, "update_time_text_font_size in");
	APP_LOG(APP_LOG_LEVEL_INFO, "i_time_text: %d", i_time_text);
	GSize max_used_size = text_layer_get_content_size(s_time_layer);
	APP_LOG(APP_LOG_LEVEL_INFO, "max_used_size.h: %d", max_used_size.h);
	APP_LOG(APP_LOG_LEVEL_INFO, "r_time_text.size.h: %d", r_time_text.size.h);
	APP_LOG(APP_LOG_LEVEL_INFO, "max_used_size.w: %d", max_used_size.w);
	APP_LOG(APP_LOG_LEVEL_INFO, "r_time_text.size.w: %d", r_time_text.size.w);
	if (max_used_size.h > r_time_text.size.h) {
		//too big! make it one smaller and finish
		char *s_font = get_next_size_font(false);
		APP_LOG(APP_LOG_LEVEL_INFO, "s_font: %s", s_font);
		text_layer_set_font(s_time_layer, fonts_get_system_font(s_font));
		i_time_text--;
		APP_LOG(APP_LOG_LEVEL_INFO, "update_time_text_font_size out");
		return;
	}
	//maxed out
	if (i_time_text == 3) {
		APP_LOG(APP_LOG_LEVEL_INFO, "update_time_text_font_size out");
		return;
	}
	//make it bigger and recheck
	char *b_font = get_next_size_font(true);
	APP_LOG(APP_LOG_LEVEL_INFO, "b_font: %s", b_font);
	text_layer_set_font(s_time_layer, fonts_get_system_font(b_font));
	i_time_text++;
	APP_LOG(APP_LOG_LEVEL_INFO, "update_time_text_font_size out");
	update_time_text_font_size();
}

void update_time_text_vertical_spacing() {
	APP_LOG(APP_LOG_LEVEL_INFO, "update_time_text_vertical_spacing in");
	GSize max_used_size = text_layer_get_content_size(s_time_layer);
	GRect frame = layer_get_frame(text_layer_get_layer(s_time_layer));
	frame.origin.y = r_time_text.origin.y + (r_time_text.size.h - max_used_size.h) / 2;
	APP_LOG(APP_LOG_LEVEL_INFO, "frame.origin.y : %d", frame.origin.y);
	layer_set_frame(text_layer_get_layer(s_time_layer), frame);
	APP_LOG(APP_LOG_LEVEL_INFO, "update_time_text_vertical_spacing out");
}

void update_time_text() {
	APP_LOG(APP_LOG_LEVEL_INFO, "update_time_text in");
  	time_t temp = time(NULL);
  	struct tm *tick_time = localtime(&temp);
	
	int div = (tick_time->tm_min + 2) / 5;
	char *hour_buffer = generate_hour_text();
	
	// Create a long-lived buffer
  	static char text_buffer[30] = "";
	APP_LOG(APP_LOG_LEVEL_INFO, "hour_buffer: %s", hour_buffer);
	switch (div) {
		case 0:
			strcpy(text_buffer,hour_buffer);
			strcat(text_buffer,"-ish");
			break;
		case 1:
			strcpy(text_buffer,"a little past ");
			strcat(text_buffer,hour_buffer);
			break;
		case 2:
			strcpy(text_buffer,"about ten past ");
			strcat(text_buffer,hour_buffer);
			break;
		case 3:
			strcpy(text_buffer,"quarter past ");
			strcat(text_buffer,hour_buffer);
			break;
		case 4:
			strcpy(text_buffer,"about twenty past ");
			strcat(text_buffer,hour_buffer);
			break;
		case 5:
			strcpy(text_buffer,"almost half past ");
			strcat(text_buffer,hour_buffer);
			break;
		case 6:
			strcpy(text_buffer,"half past ");
			strcat(text_buffer,hour_buffer);
			break;
		case 7:
			strcpy(text_buffer,"just over half past ");
			strcat(text_buffer,hour_buffer);
			break;
		case 8:
			strcpy(text_buffer,"about twenty to ");
			strcat(text_buffer,hour_buffer);
			break;
		case 9:
			strcpy(text_buffer,"quarter to ");
			strcat(text_buffer,hour_buffer);
			break;
		case 10:
			strcpy(text_buffer,"about ten to ");
			strcat(text_buffer,hour_buffer);
			break;
		case 11:
			strcpy(text_buffer,"almost ");
			strcat(text_buffer,hour_buffer);
			break;
		case 12:
			strcpy(text_buffer,hour_buffer);
			strcat(text_buffer,"-ish");
			break;
		default:
			strcpy(text_buffer,"error ");
			strcat(text_buffer,hour_buffer);
			break;
	}
	APP_LOG(APP_LOG_LEVEL_INFO, "text_buffer: %s", text_buffer);
  	text_layer_set_text(s_time_layer, text_buffer);
	
	text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	i_time_text = 0;
	update_time_text_font_size();
	update_time_text_vertical_spacing();
	APP_LOG(APP_LOG_LEVEL_INFO, "update_time_text out");
}

void update_date() {
	APP_LOG(APP_LOG_LEVEL_INFO, "update_date in");
  	// Get a tm structure
  	time_t temp = time(NULL);
  	struct tm *tick_time = localtime(&temp);
	
	static char date_buffer[15] = "";
	strftime(date_buffer, sizeof(date_buffer), "%d %B", tick_time);
  	text_layer_set_text(s_date_layer, date_buffer);
	APP_LOG(APP_LOG_LEVEL_INFO, "update_date out");
}
	
void update_time2() {
	APP_LOG(APP_LOG_LEVEL_INFO, "update_time in");
  	time_t temp = time(NULL);
  	struct tm *tick_time = localtime(&temp);
	
	APP_LOG(APP_LOG_LEVEL_INFO, "hours/minutes: %d/%d", tick_time->tm_hour, tick_time->tm_min);
	int round = settings.round_to;
	APP_LOG(APP_LOG_LEVEL_INFO, "settings.round_to: %d", settings.round_to);
	int min_mod = tick_time->tm_min % round;
	APP_LOG(APP_LOG_LEVEL_INFO, "min_mod: %d", min_mod);
	if (min_mod <= round / 2) {
		tick_time->tm_min -= min_mod;
	} else {
		tick_time->tm_min += (round - min_mod);
	}
	if (tick_time->tm_min == 60) {
		tick_time->tm_min = 0;
		tick_time->tm_hour += 1;
		if (tick_time->tm_hour == 24) {
			tick_time->tm_hour = 0;
		}
	}
	
  	static char time_buffer[5] = "00:00";
	if(clock_is_24h_style()) {
	    // Use 24 hour format
    	strftime(time_buffer, sizeof("00:00"), "%H:%M", tick_time);
	} else {
		// Use 12 hour format
		strftime(time_buffer, sizeof("00:00"), "%I:%M", tick_time);
	}

  	text_layer_set_text(s_time2_layer, time_buffer);
	APP_LOG(APP_LOG_LEVEL_INFO, "update_time out");
}

void update_colors() {
	APP_LOG(APP_LOG_LEVEL_INFO, "update_colors in");
	APP_LOG(APP_LOG_LEVEL_INFO, "settings.back_color: %s", settings.back_color);
	APP_LOG(APP_LOG_LEVEL_INFO, "settings.fore_color: %s", settings.fore_color);
	text_layer_set_background_color(s_background_layer, convert_to_gcolor(settings.back_color));
	text_layer_set_text_color(s_time2_layer, convert_to_gcolor(settings.fore_color));
	text_layer_set_text_color(s_time_layer, convert_to_gcolor(settings.fore_color));
	text_layer_set_text_color(s_date_layer, convert_to_gcolor(settings.fore_color));
	APP_LOG(APP_LOG_LEVEL_INFO, "update_colors out");
}

void minute_handler(struct tm *tick_time, TimeUnits units_changed) {
	APP_LOG(APP_LOG_LEVEL_INFO, "minute_handler in");
  	layer_mark_dirty(s_window_layer);
	APP_LOG(APP_LOG_LEVEL_INFO, "minute_handler out");
}

void battery_handler(BatteryChargeState charge) {
	APP_LOG(APP_LOG_LEVEL_INFO, "battery_handler in");
	layer_mark_dirty(s_battery_layer);
	APP_LOG(APP_LOG_LEVEL_INFO, "battery_handler out");
}

void draw_battery_path(GContext *ctx, Layer *layer, GPath *path, GRect rect) {
	APP_LOG(APP_LOG_LEVEL_INFO, "draw_battery_path in");
	const GPathInfo gpi = {
		.num_points = 4,
		.points = (GPoint []) {
			{rect.origin.x, rect.origin.y},
			{rect.origin.x, rect.origin.y + rect.size.h},
			{rect.origin.x + rect.size.w, rect.origin.y + rect.size.h},
			{rect.origin.x + rect.size.w, rect.origin.y}}
	};
	path = gpath_create(&gpi);
	gpath_draw_filled(ctx, path);
	APP_LOG(APP_LOG_LEVEL_INFO, "draw_battery_path out");
}

void battery_disp_handler(Layer *layer, GContext *ctx) {
	APP_LOG(APP_LOG_LEVEL_INFO, "battery_disp_handler in");
    graphics_context_set_fill_color(ctx, convert_to_gcolor(settings.fore_color));
	
	BatteryChargeState state = battery_state_service_peek();
	APP_LOG(APP_LOG_LEVEL_INFO, "state.charge_percent: %d", state.charge_percent);
	
	int height = state.charge_percent * 168 / 100;
	int width = state.charge_percent * 144 / 100;
	APP_LOG(APP_LOG_LEVEL_INFO, "height: %d", height);
	APP_LOG(APP_LOG_LEVEL_INFO, "width: %d", width);
	
	r_battery_left.origin.y = 168 - height;
	r_battery_left.size.h = height;
	draw_battery_path(ctx, layer, s_battery_left, r_battery_left);
	
	r_battery_right.size.h = height;
	draw_battery_path(ctx, layer, s_battery_right, r_battery_right);
	
	r_battery_top.size.w = width;
	draw_battery_path(ctx, layer, s_battery_top, r_battery_top);
	
	r_battery_bottom.origin.x = 144 - width;
	r_battery_bottom.size.w = width;
	draw_battery_path(ctx, layer, s_battery_bottom, r_battery_bottom);
	APP_LOG(APP_LOG_LEVEL_INFO, "battery_disp_handler out");
}

void in_recv_handler(DictionaryIterator *iterator, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "in_recv_handler in");
	//Get Tuple
  	Tuple *round_to = dict_find(iterator, KEY_ROUND_TO);
	Tuple *back_color = dict_find(iterator, KEY_BACK_COLOR);
	Tuple *fore_color = dict_find(iterator, KEY_FORE_COLOR);
	
	if (round_to) {
		settings.round_to = round_to->value->int16;
	}
	if (back_color) {
		snprintf(settings.back_color, sizeof(settings.back_color), "%s", back_color->value->cstring);
	}
	if (fore_color) {
		snprintf(settings.fore_color, sizeof(settings.fore_color), "%s", fore_color->value->cstring);
	}
	
	persist_write_data(SAVEKEY_SETTINGS, &settings, sizeof(settings));
	layer_mark_dirty(s_window_layer);
	APP_LOG(APP_LOG_LEVEL_INFO, "in_recv_handler out");
}

void display_handler(Layer *me, GContext *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "display_handler in");
	update_time2();
	update_time_text();
	update_date();
	update_colors();
	layer_mark_dirty(s_battery_layer);
	APP_LOG(APP_LOG_LEVEL_INFO, "display_handler out");
}

void set_rects() {
	APP_LOG(APP_LOG_LEVEL_INFO, "set_rects in");
	r_battery_left = GRect(0, 0, s_battery_width, 168);
	r_battery_right = GRect(144 - s_battery_width, 0, s_battery_width, 168);
	r_battery_top = GRect(0, 0, 144, s_battery_width);
	r_battery_bottom = GRect(0, 168 - s_battery_width, 144, s_battery_width);
	
	r_time_text = GRect(s_battery_width + 4, s_battery_width + 28, 144 - (2 * s_battery_width) - 8, 168 - (2 * s_battery_width) - 28 - 28);
	r_date = GRect(s_battery_width, s_battery_width, 144 - (2 * s_battery_width), 28);
	r_time2 = GRect(s_battery_width, 168 - s_battery_width - 28, 144 - (2 * s_battery_width), 28);
	APP_LOG(APP_LOG_LEVEL_INFO, "set_rects out");
}

void init_text_layer(TextLayer *layer, GColor background) {
	APP_LOG(APP_LOG_LEVEL_INFO, "init_text_layer in");
	text_layer_set_background_color(layer, background);
	text_layer_set_text_color(layer, GColorBlack);
	text_layer_set_text_alignment(layer, GTextAlignmentCenter);
	text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	layer_add_child(s_window_layer, text_layer_get_layer(layer));
	APP_LOG(APP_LOG_LEVEL_INFO, "init_text_layer out");
}

void main_window_load(Window *window) {
	APP_LOG(APP_LOG_LEVEL_INFO, "main_window_load in");
	set_rects();
	
	s_window_layer = layer_create(layer_get_frame(window_get_root_layer(s_window)));
	layer_add_child(window_get_root_layer(s_window), s_window_layer);
	layer_set_update_proc(s_window_layer, display_handler);
	
	s_battery_layer = layer_create(layer_get_frame(window_get_root_layer(s_window)));
	layer_add_child(window_get_root_layer(s_window), s_battery_layer);
	layer_set_update_proc(s_battery_layer, battery_disp_handler);
	
	s_background_layer = text_layer_create(layer_get_frame(window_get_root_layer(s_window)));
	init_text_layer(s_background_layer, GColorWhite);	
	
	s_time_layer = text_layer_create(r_time_text);
	init_text_layer(s_time_layer, GColorClear);
	text_layer_set_overflow_mode(s_time_layer, GTextOverflowModeWordWrap);
	i_time_text = 0;
	
	s_date_layer = text_layer_create(r_date);
	init_text_layer(s_date_layer, GColorClear);
	
	s_time2_layer = text_layer_create(r_time2);
	init_text_layer(s_time2_layer, GColorClear);
	
	layer_mark_dirty(s_window_layer);
	APP_LOG(APP_LOG_LEVEL_INFO, "main_window_load out");
}

void main_window_unload(Window *window) {
	APP_LOG(APP_LOG_LEVEL_INFO, "main_window_unload in");
  	text_layer_destroy(s_time_layer);
  	text_layer_destroy(s_time2_layer);
  	text_layer_destroy(s_date_layer);
  	text_layer_destroy(s_background_layer);
	layer_destroy(s_battery_layer);
	layer_destroy(s_window_layer);
	APP_LOG(APP_LOG_LEVEL_INFO, "main_window_unload out");
}

void handle_init() {
	APP_LOG(APP_LOG_LEVEL_INFO, "handle_init in");
  	
	s_window = window_create();
	persist_read_data(SAVEKEY_SETTINGS, &settings, sizeof(settings));
	if (settings.round_to == 0) {
		settings.round_to = 1;
		snprintf(settings.back_color, sizeof(settings.back_color), "%s", "#FFFFFF");
		snprintf(settings.fore_color, sizeof(settings.fore_color), "%s", "#000000");
		persist_write_data(SAVEKEY_SETTINGS, &settings, sizeof(settings));
	}
	  
	window_set_window_handlers(s_window, (WindowHandlers){
		.load = main_window_load,
		.unload = main_window_unload
	});
	
	app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());	
  	tick_timer_service_subscribe(MINUTE_UNIT, minute_handler);
	battery_state_service_subscribe(battery_handler);
	
  	window_stack_push(s_window, true);
	APP_LOG(APP_LOG_LEVEL_INFO, "handle_init out");
}

void handle_deinit() {
	APP_LOG(APP_LOG_LEVEL_INFO, "handle_deinit in");
  	window_destroy(s_window);
	APP_LOG(APP_LOG_LEVEL_INFO, "handle_deinit out");
}

int main() {
  	handle_init();
  	app_event_loop();
  	handle_deinit();
}