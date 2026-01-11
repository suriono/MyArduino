// ========================= Go To Screen2 =============================

static void action_go_to_mainpage(lv_event_t *e) {
  loadScreen(SCREEN_ID_MAIN);
}

static void action_go_to_screen2(lv_event_t *e) {
  loadScreen(SCREEN_ID_SCREEN2);
}

// ========================= Calibrate =================================

static void action_calibrate_pressure(lv_event_t *e) {
  press_Offset = pressB - pressA;
}


// ========================= GUI Actions ===============================

static void action_change_status(lv_event_t *e) {
 // lv_event_code_t code = lv_event_get_code(e);              //Get the event code
 // lv_obj_t *swObj = (lv_obj_t *)lv_event_get_target(e);     //Switch that generated the event
 // lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e);  //Label or other UI elements we want to update (Optional)

  //lv_label_set_text(label, "Press ");
  lv_label_set_text(objects.main_status, "You press the button ...." );
  
  Serial.println("Button pressed");

//  if (code == LV_EVENT_VALUE_CHANGED) {
//    bool isChecked = lv_obj_has_state(swObj, LV_STATE_CHECKED);  //Returns true/false
//    if (isChecked) {
//      digitalWrite(ledPin27, HIGH);
//      lv_label_set_text(label, "ON");
//    } else {
//      digitalWrite(ledPin27, LOW);
//      lv_label_set_text(label, "OFF");
//    }
//  }
}
