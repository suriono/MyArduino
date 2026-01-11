#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_change_status(lv_event_t * e);
extern void action_go_to_screen2(lv_event_t * e);
extern void action_go_to_mainpage(lv_event_t * e);
extern void action_calibrate_pressure(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/