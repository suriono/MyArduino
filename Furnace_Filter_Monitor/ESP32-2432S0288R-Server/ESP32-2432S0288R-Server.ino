
/* 
 *  https://www.youtube.com/watch?v=1fXV2woCHqA
 *  
 * This assumes you already have LVGL working with Arduino/ESP32
  - Arduino   
    - C:\Users\kolap\Documents\Arduino
  - EEZ Studio 
    - C:\Users\kolap\eez-projects
    - C:\Users\kolap\Documents\Arduino\EEZ_studio_projects
  - Square Line
    - C:\Users\kolap\Documents\SquareLine\tutorials
    - C:\Users\kolap\Documents\SquareLine\tutorials\Export_UI_files   
    - C:\Users\kolap\Documents\Arduino\SQ_line_CYD
 1. Create UI with Squareline
  - Remember to set both screen width/height and Depth=16 bits as used by ESP32
  - File > Project settings set the following
    - Project Export Root
    - UI Files Export Path (This will contain files you will need to copy over to Arduino project)
      - Remember this location as you will need to copy the contents.
  - Export the UI code
    - Export > Export UI Files
      - This will create files in UI Files Export Path you created above
  - Create a new Arduino project 
  - Open an existing one
  - Copy all UI Files to the Arduino project root
  - In Arduino setup() add:
    - ui_init(); 
    - Run the program
 */
#include <WiFi.h>
#include <lvgl.h>
#include "ui.h"
#include "callbacks.h"

#include <ArduinoJson.h>
#include <string.h>
#include "password.h"

//JSONVar myJSON;
WiFiServer server(8000);
WiFiClient wificlient;

#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
// A library for interfacing with the touch screen
// Can be installed from the library manager (Search for "XPT2046")
//https://github.com/PaulStoffregen/XPT2046_Touchscreen
// ----------------------------
// Touch Screen pins
// ----------------------------
// The CYD touch uses some non default
// SPI pins
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33
SPIClass touchscreenSpi = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
uint16_t touchScreenMinimumX = 200, touchScreenMaximumX = 3700, touchScreenMinimumY = 240, touchScreenMaximumY = 3800;

/*Set to your screen resolution*/
#define TFT_HOR_RES 240
#define TFT_VER_RES 320

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

int   pressA, pressB, press_Offset;
float sumPressA, sumPressB;
int   averageNo=100;
bool  isCalibrated = false;
float TempA , TempB;
char  Main_Label_Status[50], Screen2_Label_PressureA[7], Screen2_Label_PressureB[7];
char  Screen2_Label_PressureDiff[4],  Screen2_Label_PressureDiffCal[4];
char  Screen2_Label_Temperature[20];
int   pressAarr[100], pressBarr[100];        // array to get average pressures
#define CHART_READINGS  30                  // nunber of points in the chart
lv_chart_series_t *chart1_series_max, *chart1_series_min,  *chart2_series_max, *chart2_series_min;

// =================================================================

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf) {
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}
#endif

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  /*Call it to tell LVGL you are ready*/
  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
  if (touchscreen.touched()) {
    TS_Point p = touchscreen.getPoint();
    //Some very basic auto calibration so it doesn't go out of range
    if (p.x < touchScreenMinimumX) touchScreenMinimumX = p.x;
    if (p.x > touchScreenMaximumX) touchScreenMaximumX = p.x;
    if (p.y < touchScreenMinimumY) touchScreenMinimumY = p.y;
    if (p.y > touchScreenMaximumY) touchScreenMaximumY = p.y;
    //Map this to the pixel position
    data->point.x = map(p.x, touchScreenMinimumX, touchScreenMaximumX, 1, TFT_HOR_RES); /* Touchscreen X calibration */
    data->point.y = map(p.y, touchScreenMinimumY, touchScreenMaximumY, 1, TFT_VER_RES); /* Touchscreen Y calibration */
    data->state = LV_INDEV_STATE_PRESSED;
    Serial.print("(X,Y):");Serial.print(data->point.x);Serial.print(",");Serial.println(data->point.y);
    /*
    Serial.print("Touch x ");
    Serial.print(data->point.x);
    Serial.print(" y ");
    Serial.println(data->point.y);
    */
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

lv_indev_t *indev;      //Touchscreen input device
uint8_t *draw_buf;      //draw_buf is allocated on heap otherwise the static area is too big on ESP32 at compile
uint32_t lastTick = 0;  //Used to track the tick timer

// ============================== SET UP ============================================

void setup() {

  //Some basic info on the Serial console
  String LVGL_Arduino = "LVGL demo ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.begin(57600);
  Serial.println(LVGL_Arduino);

  //delay(2000);

  //Initialise the touchscreen
  touchscreenSpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
  touchscreen.begin(touchscreenSpi);                                         /* Touchscreen init */
  lv_init();

  draw_buf = new uint8_t[DRAW_BUF_SIZE];
  lv_display_t *disp;
  disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE);

  //Initialize the XPT2046 input device driver
  indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touchpad_read);

  // For landscape rotate for the UI display and touchscreen, USB plug on the left side
  touchscreen.setRotation(2);
  lv_disp_set_rotation(disp, LV_DISPLAY_ROTATION_270);

  //Done
  Serial.println("LVGL Setup done");

  //Integrate EEZ Studio GUI
  ui_init();

  // Widget set ups ------------- update these -------------------
  lv_label_set_text(objects.main_status, "Starting the WiFi server ...." );
  lv_chart_set_type(objects.chart1, LV_CHART_TYPE_LINE);
  lv_chart_set_point_count(objects.chart1, CHART_READINGS);
  lv_chart_set_axis_range(objects.chart1, LV_CHART_AXIS_PRIMARY_X, 1, 24);
  lv_chart_set_axis_range(objects.chart1, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
  lv_chart_set_update_mode(objects.chart1, LV_CHART_UPDATE_MODE_SHIFT);   // update circular

  // ------ Chart ----------------
  chart1_series_max = lv_chart_add_series(objects.chart1,lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
  chart1_series_min = lv_chart_add_series(objects.chart1,lv_palette_main(LV_PALETTE_YELLOW), LV_CHART_AXIS_PRIMARY_Y);
  chart2_series_max = lv_chart_add_series(objects.chart2,lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
  chart2_series_min = lv_chart_add_series(objects.chart2,lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);
  //lv_chart_set_all_values(objects.chart1, chart1_series_max , 0); lv_chart_set_all_values(objects.chart1, chart1_series_min , 0);

  WiFi.softAP(SSID_NAME,PASS);
  Serial.print("AP IP: "); Serial.println(WiFi.softAPIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
//  snprintf(Label1, sizeof(Label1),"IP: %s", WiFi.softAPIP());
 // lv_label_set_text(objects.label1, Label1 );
  server.begin();  
}

// ========================= Loop ==================================

void loop() {
  static unsigned long last_wifi;
  static unsigned long last_hourly = millis();
  static unsigned long last_daily  = millis();
  static float hourly_max = 0.0;
  static float daily_max  = 0.0;
  static byte aveCount = 0;
  
  lv_tick_inc(millis() - lastTick);  //Update the tick timer. Tick is new for LVGL 9
  lastTick = millis();
  lv_timer_handler();  //Update the UI

  if (incoming_wifi()) {
    Serial.print("   P:");Serial.print(pressA);Serial.print(",");Serial.println(pressB);
    Serial.print("     T:");Serial.print(TempA);Serial.print(",");Serial.println(TempB);

    float diff_cal = sumPressA-sumPressB+press_Offset;
   // itoa(pressA,tmpstr,10);
   // char *label1 = strcat("Pressure: ", tmpstr );
    //Serial.println(label1);

    pressAarr[aveCount] = pressA; pressBarr[aveCount] = pressB;
    sumPressA=0; sumPressB=0;
    for (int nn=0 ; nn<averageNo ; nn++) {
       sumPressA += float(pressAarr[nn]) / float(averageNo);  sumPressB += float(pressBarr[nn]) / float(averageNo);
    }

    // Main screeen -------
    snprintf(Main_Label_Status, sizeof(Main_Label_Status),"Pressure: %.0f, Diff: %.0f, %.0f°F, Refresh:%d", sumPressA,diff_cal,TempA*9/5+32.5,averageNo-aveCount);
    lv_label_set_text(objects.main_status, Main_Label_Status );

    // Screen 2 -------
    snprintf(Screen2_Label_Temperature, sizeof(Screen2_Label_Temperature),"%.0f F (%.0f C)", TempA*9/5+32.5, TempA+0.5);
    snprintf(Screen2_Label_PressureA, sizeof(Screen2_Label_PressureA),"%.1f", sumPressA);
    snprintf(Screen2_Label_PressureB, sizeof(Screen2_Label_PressureB),"%.2f", sumPressB);
    snprintf(Screen2_Label_PressureDiff, sizeof(Screen2_Label_PressureDiff),"%.1f", sumPressA-sumPressB);
    snprintf(Screen2_Label_PressureDiffCal, sizeof(Screen2_Label_PressureDiffCal),"%.1f", diff_cal);
    lv_label_set_text(objects.temperature, Screen2_Label_Temperature);
    lv_label_set_text(objects.pressure_a, Screen2_Label_PressureA);
    lv_label_set_text(objects.pressure_b, Screen2_Label_PressureB);
    lv_label_set_text(objects.pressure_diff, Screen2_Label_PressureDiff);
    lv_label_set_text(objects.pressure_diff_cal, Screen2_Label_PressureDiffCal);

    if (aveCount == averageNo - 1) {   // end of averaging
      unsigned long hourlapsed = millis() - last_hourly;
      if ( hourlapsed > 3600000) {  // one hour has lapsed
        last_hourly = millis();
        lv_chart_set_next_value(objects.chart1, chart1_series_max , hourly_max);
        Serial.print("     Max hourly:"); Serial.println(String(hourly_max));
        hourly_max = 0.0;         // reset the max
      }
      if ((diff_cal > hourly_max) && isCalibrated) {
        hourly_max = diff_cal;
      }     

      unsigned long daylapsed = millis() - last_daily;
      if ( daylapsed > 3600000*24) {  // one day has lapsed
        last_daily = millis();
        lv_chart_set_next_value(objects.chart2, chart2_series_max , daily_max);
        Serial.print("     Max daily:"); Serial.println(String(daily_max));
        daily_max = 0.0;         // reset the max
      }
      if ((diff_cal > daily_max) && isCalibrated) {
        daily_max = diff_cal;
      }     
    }
    
    aveCount = (aveCount + 1) % averageNo;
    last_wifi = millis();
    delay(5);
  } else {  // no incoming data
    if ( (millis()-last_wifi) > 5000) {
        lv_label_set_text(objects.main_status, "No sensor data" );
        last_wifi = millis();  // reset so it does not keep running this
        lv_label_set_text(objects.temperature, Screen2_Label_Temperature);
        Serial.println(Screen2_Label_Temperature);
    }
    delay(5);
  }
 // delay(5);
}
