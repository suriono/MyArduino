
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_change_status(lv_event_t * e);
extern void action_go_to_screen2(lv_event_t * e);
extern void action_go_to_mainpage(lv_event_t * e);
extern void action_calibrate_pressure(lv_event_t * e);
extern void action_change_average(lv_event_t * e);


#ifdef __cplusplus
}
#endif

extern int   press_Offset,averageNo;
extern float sumPressA, sumPressB;
extern float TempA , TempB;
extern char  Screen2_Label_Temperature[20];

// ========================= Go To Screen2 =============================

void action_go_to_mainpage(lv_event_t *e) {
  loadScreen(SCREEN_ID_MAIN);
}

void action_go_to_screen2(lv_event_t *e) {
  loadScreen(SCREEN_ID_SCREEN2);
}

// ========================= Calibrate =================================

void action_calibrate_pressure(lv_event_t *e) {
  press_Offset = sumPressB - sumPressA;
}

// ========================= Calibrate =================================

void action_change_average(lv_event_t *e) {
  averageNo = (int)lv_slider_get_value(objects.screen2_slider_change_average);
}
