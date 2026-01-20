#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    {
        lv_obj_t *parent_obj = obj;
        {
            // main_status
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.main_status = obj;
            lv_obj_set_pos(obj, -1, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Status ....");
        }
        {
            // Main_Btn_nextpage
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.main_btn_nextpage = obj;
            lv_obj_set_pos(obj, 220, 190);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_go_to_screen2, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfff37821), LV_PART_MAIN | LV_STATE_PRESSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Next Page");
                }
            }
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, -1, 20);
            lv_obj_set_size(obj, 321, 143);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Chart1
                    lv_obj_t *obj = lv_chart_create(parent_obj);
                    objects.chart1 = obj;
                    lv_obj_set_pos(obj, -14, -3);
                    lv_obj_set_size(obj, 233, 131);
                }
            }
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

void create_screen_screen2() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.screen2 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    {
        lv_obj_t *parent_obj = obj;
        {
            // PressureA
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pressure_a = obj;
            lv_obj_set_pos(obj, 88, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Pressure A");
        }
        {
            // Screen2_Btn_mainpage
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.screen2_btn_mainpage = obj;
            lv_obj_set_pos(obj, 0, 190);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_go_to_mainpage, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfff39e21), LV_PART_MAIN | LV_STATE_PRESSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Main Page");
                }
            }
        }
        {
            // PressureB
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pressure_b = obj;
            lv_obj_set_pos(obj, 88, 32);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Pressure B");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Pressure A:");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, -1, 32);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Pressure B:");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 192, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Diff:");
        }
        {
            // Pressure_Diff
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pressure_diff = obj;
            lv_obj_set_pos(obj, 255, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "PressDiff");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, -2, 137);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Temperature: ");
        }
        {
            // Temperature
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.temperature = obj;
            lv_obj_set_pos(obj, 115, 137);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Celcius");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 192, 32);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Cal Diff: ");
        }
        {
            // Pressure_Diff_Cal
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pressure_diff_cal = obj;
            lv_obj_set_pos(obj, 255, 32);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "CalDiff");
        }
        {
            // Screen2_btn_calibrate
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.screen2_btn_calibrate = obj;
            lv_obj_set_pos(obj, 220, 70);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_calibrate_pressure, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfff36f21), LV_PART_MAIN | LV_STATE_PRESSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Calibrate\nPressure");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 196, 191);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Average #");
        }
        {
            // Screen2_slider_change_average
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.screen2_slider_change_average = obj;
            lv_obj_set_pos(obj, 132, 218);
            lv_obj_set_size(obj, 150, 10);
            lv_slider_set_range(obj, 1, 10);
            lv_slider_set_value(obj, 1, LV_ANIM_OFF);
            lv_obj_add_event_cb(obj, action_change_average, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
    }
    
    tick_screen_screen2();
}

void tick_screen_screen2() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_screen2,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
    create_screen_screen2();
}
