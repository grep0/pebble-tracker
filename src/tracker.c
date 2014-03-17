#include <pebble.h>

#define GRECT(x,y,w,h) (GRect){{(x),(y)},{(w),(h)}}
#define LOG(...) app_log(APP_LOG_LEVEL_INFO,__FILE__,__LINE__,__VA_ARGS__)

Window* window;
TextLayer* tl_latitude;
TextLayer* tl_longitude;

char t_latitude[32] = "hello";
char t_longitude[32] = "world";

// latitude and longitude in microdegrees
int32_t latitude_udeg = 37383940;
int32_t longitude_udeg = -122123124;

void format_value(char* t, int32_t value, char plus_c, char minus_c) {
    char c = value>=0 ? plus_c : minus_c;
    if (value<0) value = -value;
    int deg = value / 1000000;
    value = (value - deg*1000000)*60;
    int min = value / 1000000;
    value = (value - min*1000000)*60;
    int sec = value / 1000000;
    int hsec = (value-sec*1000000)/10000;
    snprintf(t, 32, "%d°%02d'%02d\".%02d %c", deg, min, sec, hsec, c);
}

void show_coordinates() {
    format_value(t_latitude, latitude_udeg, 'N', 'S');
    format_value(t_longitude, longitude_udeg, 'E', 'W');
    text_layer_set_text(tl_latitude, t_latitude);
    text_layer_set_text(tl_longitude, t_longitude);
}

void window_load(Window* window) {
    LOG("window_load");
    tl_latitude  = text_layer_create(GRECT(5,5,100,20));
    tl_longitude = text_layer_create(GRECT(5,30,100,20));
    Layer* root = window_get_root_layer(window);
    layer_add_child(root, text_layer_get_layer(tl_latitude));
    layer_add_child(root, text_layer_get_layer(tl_longitude));
}

void window_unload(Window* window) {
    LOG("window_unload");
    text_layer_destroy(tl_latitude);
    text_layer_destroy(tl_longitude);
}

void window_appear(Window* window) {
    LOG("window_appear");
    show_coordinates();
}

void window_disappear(Window* window) {
    LOG("window_disappear");
}

void init() {
    LOG("init");
    window = window_create();
    window_set_window_handlers(window,
        (WindowHandlers){
            .load = window_load,
            .appear = window_appear,
            .disappear = window_disappear,
            .unload = window_unload,
        });
    window_stack_push(window, true);
}

void deinit() {
    LOG("deinit");
    window_stack_pop(true);
    window_destroy(window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
