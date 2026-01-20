#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *screen2;
    lv_obj_t *main_status;
    lv_obj_t *main_btn_nextpage;
    lv_obj_t *chart1;
    lv_obj_t *pressure_a;
    lv_obj_t *screen2_btn_mainpage;
    lv_obj_t *pressure_b;
    lv_obj_t *pressure_diff;
    lv_obj_t *temperature;
    lv_obj_t *pressure_diff_cal;
    lv_obj_t *screen2_btn_calibrate;
    lv_obj_t *screen2_slider_change_average;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_SCREEN2 = 2,
};

void create_screen_main();
void tick_screen_main();

void create_screen_screen2();
void tick_screen_screen2();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/