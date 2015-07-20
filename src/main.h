#include <pebble.h>
#include <ctype.h>
	
#define SAVEKEY_SETTINGS 1

enum {
	KEY_ROUND_TO = 0,
	KEY_BACK_COLOR = 1,
	KEY_FORE_COLOR = 2
};

Window *s_window;
Layer *s_window_layer;
TextLayer *s_background_layer;
TextLayer *s_time_layer;
TextLayer *s_time2_layer;
TextLayer *s_date_layer;
GPath *s_battery_left;
GPath *s_battery_right;
GPath *s_battery_top;
GPath *s_battery_bottom;
Layer *s_battery_layer;

int s_battery_width = 5;
extern GRect r_battery_left;
extern GRect r_battery_right;
extern GRect r_battery_top;
extern GRect r_battery_bottom;
extern GRect r_time_text;
extern GRect r_date;
extern GRect r_time2;
extern int i_time_text;


struct {
	int round_to;
	char back_color[8];
	char fore_color[8];
} settings;

unsigned int HexStringToUInt(char const* hexstring);
GColor convert_to_gcolor(char hex_string[8]);
char *generate_hour_text();
char *get_next_size_font(bool bigger);
void update_time_text_font_size();
void update_time_text_vertical_spacing();
void update_time_text();
void update_date();
void update_time2();
void update_colors();
void minute_handler(struct tm *tick_time, TimeUnits units_changed);
void battery_handler(BatteryChargeState charge);
void draw_battery_path(GContext *ctx, Layer *layer, GPath *path, GRect rect);
void battery_disp_handler(Layer *layer, GContext *ctx);
void in_recv_handler(DictionaryIterator *iterator, void *context);
void display_handler(Layer *me, GContext *context);
void set_rects();
void init_text_layer(TextLayer *layer, GColor background);
void main_window_load(Window *window);
void main_window_unload(Window *window);
void handle_init();
void handle_deinit();
