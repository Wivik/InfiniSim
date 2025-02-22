#include "displayapp/LittleVgl.h"
#include "displayapp/lv_pinetime_theme.h"

//#include <FreeRTOS.h>
//#include <task.h>
////#include <projdefs.h>
#include "drivers/Cst816s.h"
#include "drivers/St7789.h"

using namespace Pinetime::Components;

//lv_style_t* LabelBigStyle = nullptr;
//
//static void disp_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p) {
//  auto* lvgl = static_cast<LittleVgl*>(disp_drv->user_data);
//  lvgl->FlushDisplay(area, color_p);
//}
//
bool touchpad_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data) {
  auto* lvgl = static_cast<LittleVgl*>(indev_drv->user_data);
  return lvgl->GetTouchPadInfo(data);
}

LittleVgl::LittleVgl(Pinetime::Drivers::St7789& lcd, Pinetime::Drivers::Cst816S& touchPanel)
  : lcd {lcd}, touchPanel {touchPanel}, previousClick {0, 0} {

}

void LittleVgl::Init() {
//  lv_init();
//  InitDisplay();
//  InitTheme();
  InitTouchpad();
}

//void LittleVgl::InitDisplay() {
//  lv_disp_draw_buf_init(&disp_buf_2, buf2_1, buf2_2, LV_HOR_RES_MAX * 4); /*Initialize the display buffer*/
//  lv_disp_drv_init(&disp_drv);                                       /*Basic initialization*/
//
//  /*Set up the functions to access to your display*/
//
//  /*Set the resolution of the display*/
//  disp_drv.hor_res = 240;
//  disp_drv.ver_res = 240;
//
//  /*Used to copy the buffer's content to the display*/
//  disp_drv.flush_cb = disp_flush;
//  /*Set a display buffer*/
//  disp_drv.draw_buf = &disp_buf_2;
//  disp_drv.user_data = this;
//
//  /*Finally register the driver*/
//  lv_disp_drv_register(&disp_drv);
//}

void LittleVgl::InitTouchpad() {
  lv_indev_drv_t indev_drv;

  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpad_read;
  indev_drv.user_data = this;
  lv_indev_drv_register(&indev_drv);
}

void LittleVgl::SetFullRefresh(FullRefreshDirections direction) {
  if (scrollDirection == FullRefreshDirections::None) {
    scrollDirection = direction;
    if (scrollDirection == FullRefreshDirections::Down) {
      lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_NONE);
    } else if (scrollDirection == FullRefreshDirections::Right) {
      lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_NONE);
    } else if (scrollDirection == FullRefreshDirections::Left) {
      lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_NONE);
    } else if (scrollDirection == FullRefreshDirections::RightAnim) {
      lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_NONE);
    } else if (scrollDirection == FullRefreshDirections::LeftAnim) {
      lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_NONE);
    }
  }
}
//
//
//void LittleVgl::DisplayDownScroll(){
//  // We are controlling the drawing process, disable lvgl timers
//  lv_timer_enable(false);
//  
//  // For each segment, draw the full width, 4 lines at a time starting from the bottom
//  // TODO: Should probably calculate this from the size of the draw buffer
//  int16_t height = 4;
//  int16_t width = 240;
//  int16_t y2 = 240;
//  int16_t y1 = 240 - height;
//  
//  lv_area_t area;
//  area.x1 = 0;
//  area.x2 = width;
//  
//  // Start from the bottom and create a 4 line high box
//  for (y1 = 240 - height; y1 >= 0; y1 -= height) {
//    y2 = y1 + height - 1;
//    
//    // If the box has reached the end of the visible line on the lcd controller...
//    if (y2 == visibleNbLines - 1) {
//      // move past the non visible lines
//      writeOffset += (totalNbLines - visibleNbLines);
//      // and wrap around to the start of address space
//      writeOffset %= totalNbLines;
//    }
//    // Set new box
//    area.y1 = y1;
//    area.y2 = y2;
//    
//    // Scroll as we draw
//    uint16_t toScroll = height;
//    if (scrollOffset >= toScroll)
//      scrollOffset -= toScroll;
//    else { // now we need to wrap the scroll address
//      toScroll -= scrollOffset;
//      scrollOffset = totalNbLines - toScroll;
//    }
//    lcd.VerticalScrollStartAddress(scrollOffset);
//  
//    lv_disp_t* disp = lv_disp_get_default();
//    // Clear invalid area list / tells lvgl that nothing on the screen needs to be updated
//    _lv_inv_area(disp, nullptr);
//    // invalidate only the segment we want to update in this portion of the animation
//    _lv_inv_area(disp, &area);
//    // cancel any current flushes in the display driver
//    // Since we've stopped timers, it will be waiting forever if there is currently a flush
//    lv_disp_flush_ready(disp->driver);
//    lv_refr_now(disp);
//  }
//  // Done! clear flags and enable timers
//  scrollDirection = FullRefreshDirections::None;
//  animating = false;
//  lv_timer_enable(true);
//}
//
//void LittleVgl::DisplayHorizAnim() {
//  lv_timer_enable(false);
//  
//  int16_t height, width, x1, x2;
//  lv_area_t area;
//  
//  height = 240;
//  width = 4;
//  int16_t (*NextStep)(int16_t, int16_t){};
//  bool (*CheckEnd)(int16_t){};
//  
//  area.y1=0;
//  area.y2=height;
//
//  if (scrollDirection == FullRefreshDirections::RightAnim) {
//    x1 = 0;
//
//    CheckEnd = [](int16_t x) -> bool {
//      return (x < LV_HOR_RES_MAX);
//    };
//    NextStep = [](int16_t x, int16_t width) -> int16_t {
//      auto newx = x + width * 2;
//      if (newx < 240) {return newx;};
//      return (newx < 240 + width) ? (newx - 240 + width) : newx;
//    };
//
//  } else if (scrollDirection == FullRefreshDirections::LeftAnim) {
//    x1 = 240 - width;
//
//    CheckEnd = [](int16_t x) -> bool {
//      return (x >= 0);
//    };
//    NextStep = [](int16_t x, int16_t width) -> int16_t {
//      auto newx = x - width * 2;
//      if (newx >= 0) {return newx;}
//      return (newx >= 0 - width) ? (newx + 240 - width) : newx;
//    };
//
//  } else {
//    // Not set for a horizontal animation!
//    lv_timer_enable(true);
//    return;
//  }
//
//  for (; CheckEnd(x1); x1 = NextStep(x1, width)) {
//      x2 = x1 + width-1;
//
//      if (area.y2 == visibleNbLines - 1) {
//        writeOffset += (totalNbLines - visibleNbLines);
//        writeOffset %= totalNbLines;
//      }
//      area.x1 = x1;
//      area.x2 = x2;
//  
//      lv_disp_t* disp = lv_disp_get_default();
//      _lv_inv_area(disp, nullptr);
//      _lv_inv_area(disp, &area);
//      lv_disp_flush_ready(disp->driver);
//      lv_refr_now(disp);
//    }
//  scrollDirection = FullRefreshDirections::None;
//  animating = false;
//  lv_timer_enable(true);
//}
//
//void LittleVgl::FlushDisplayManually() {
//  switch(scrollDirection){
//    case FullRefreshDirections::Down:
//      DisplayDownScroll();
//      break;
//    case FullRefreshDirections::RightAnim:
//    case FullRefreshDirections::LeftAnim:
//      DisplayHorizAnim();
//      break;
//    default:
//      break;
//  }
//}
//
void LittleVgl::FlushDisplay(const lv_area_t* area, lv_color_t* color_p) {
//  uint16_t y1, y2, width, height = 0;
//
//  ulTaskNotifyTake(pdTRUE, 200);
//  // NOtification is still needed (even if there is a mutex on SPI) because of the DataCommand pin
//  // which cannot be set/clear during a transfert.
//  
//  if (!animating && (scrollDirection == FullRefreshDirections::Down ||
//                     scrollDirection == FullRefreshDirections::RightAnim ||
//                     scrollDirection == FullRefreshDirections::LeftAnim)){
//    animating = true;
//    FlushDisplayManually();
//    return;
//  }
//
//  if ((scrollDirection == FullRefreshDirections::Up) && (area->y1 == 0)) {
//    writeOffset = (writeOffset + visibleNbLines) % totalNbLines;
//  }
//
//  y1 = (area->y1 + writeOffset) % totalNbLines;
//  y2 = (area->y2 + writeOffset) % totalNbLines;
//
//  width = (area->x2 - area->x1) + 1;
//  height = (area->y2 - area->y1) + 1;
//
//  if (scrollDirection == FullRefreshDirections::Up) {
//
//    if (area->y1 > 0) {
//      if (area->y2 == visibleNbLines - 1) {
//        scrollOffset += (height * 2);
//        scrollDirection = FullRefreshDirections::None;
////        lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_NONE);
//      } else {
//        scrollOffset += height;
//      }
//      scrollOffset = scrollOffset % totalNbLines;
//      lcd.VerticalScrollStartAddress(scrollOffset);
//    }
//  } else if (scrollDirection == FullRefreshDirections::Left or scrollDirection == FullRefreshDirections::LeftAnim) {
//    if (area->x2 == visibleNbLines - 1) {
//      scrollDirection = FullRefreshDirections::None;
////      lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_NONE);
//    }
//  } else if (scrollDirection == FullRefreshDirections::Right or scrollDirection == FullRefreshDirections::RightAnim) {
//    if (area->x1 == 0) {
//      scrollDirection = FullRefreshDirections::None;
////      lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_NONE);
//    }
//  }
//
//  if (y2 < y1) {
//    height = totalNbLines - y1;
//
//    if (height > 0) {
//      lcd.DrawBuffer(area->x1, y1, width, height, reinterpret_cast<const uint8_t*>(color_p), width * height * 2);
//      ulTaskNotifyTake(pdTRUE, 100);
//    }
//
//    uint16_t pixOffset = width * height;
//    height = y2 + 1;
//    lcd.DrawBuffer(area->x1, 0, width, height, reinterpret_cast<const uint8_t*>(color_p + pixOffset), width * height * 2);
//
//  } else {
//    lcd.DrawBuffer(area->x1, y1, width, height, reinterpret_cast<const uint8_t*>(color_p), width * height * 2);
//  }
//
//  // IMPORTANT!!!
//  // Inform the graphics library that you are ready with the flushing
//  lv_disp_flush_ready(&disp_drv);

  lv_disp_t *disp = lv_disp_get_default();
  lv_disp_drv_t *disp_drv = &disp->driver;
  lv_area_t area_trimmed = *area;
  if (area->x1 < 0)
    area_trimmed.x1 = 0;
  if (area->x2 >= LV_HOR_RES)
    area_trimmed.x2 = LV_HOR_RES-1;
  if (area->y1 < 0)
    area_trimmed.y1 = 0;
  if (area->y2 >= LV_VER_RES)
    area_trimmed.y2 = LV_VER_RES-1;
  // tell flush_cb this is the last thing to flush to get the monitor refreshed
  lv_disp_get_buf(disp)->flushing_last = true;
  disp_drv->flush_cb(disp_drv, &area_trimmed, color_p);
}

void LittleVgl::SetNewTouchPoint(uint16_t x, uint16_t y, bool contact) {
  tap_x = x;
  tap_y = y;
  tapped = contact;
}

bool LittleVgl::GetTouchPadInfo(lv_indev_data_t* ptr) {
  ptr->point.x = tap_x;
  ptr->point.y = tap_y;
  if (tapped) {
    ptr->state = LV_INDEV_STATE_PR;
  } else {
    ptr->state = LV_INDEV_STATE_REL;
  }
  return false;
}

//void LittleVgl::InitTheme() {
//  if (!lv_pinetime_theme_is_inited()) {
//    lv_theme_t* th = lv_pinetime_theme_init(lv_disp_get_default(), lv_color_white(), lv_color_hex(0xC0C0C0), &jetbrains_mono_bold_20);
//    lv_disp_set_theme(lv_disp_get_default(), th);
//  }
//}
