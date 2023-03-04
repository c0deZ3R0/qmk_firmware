// Copyright 2023 Blaine Wordly, aka c0deZ3R0  <blaine_wordly@hotmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "pointing/cirque_trackpad_logic.h"
#include "c0deZ3R0.h"


#include "transactions.h"

#include <quantum.h>
#include "analog.h"

#include "spi_master.h"
#include "i2c_master.h"

#include "pointing_device.h"


#include "drivers/sensors/pmw33xx_common.h"
#include "pointing/pointing_utils.h"

#ifdef CIRQUE_ENABLED
#include "drivers/sensors/cirque_pinnacle.h"
#include "pointing/cirque_trackpad_logic.h"
#endif


#include QMK_KEYBOARD_H

userspace_config_t userspace_config;




#define CONSTRAIN_HID(amt) ((amt) < INT8_MIN ? INT8_MIN : ((amt) > INT8_MAX ? INT8_MAX : (amt)))
#define CONSTRAIN_HID_XY(amt) ((amt) < XY_REPORT_MIN ? XY_REPORT_MIN : ((amt) > XY_REPORT_MAX ? XY_REPORT_MAX : (amt)))

#ifdef CONSOLE_ENABLE
#    include "print.h"
#endif // CONSOLE_ENABLE


bool init_success;

void pointing_device_driver_init(void) 
{
#    ifdef CONSOLE_ENABLE
            if (debug_mouse) dprintf("pointing_device_driver_init .\n");
#    endif
     cirque_trackpad_logic_init();
     if (is_keyboard_left()) {
        
 } else {
       init_success = pmw33xx_init(0);
     //pmw3360_check_signature(0);
 }

}

report_mouse_t pmw3360_get_report(report_mouse_t mouse_report) {
        pmw33xx_report_t report    = pmw33xx_read_burst(0);
    static bool      in_motion = false;

    if (report.motion.b.is_lifted) {
        return mouse_report;
    }

    if (!report.motion.b.is_motion) {
        in_motion = false;
        return mouse_report;
    }

    if (!in_motion) {
        in_motion = true;
        pd_dprintf("PWM3360 (0): starting motion\n");
    }

    mouse_report.x = CONSTRAIN_HID_XY(report.delta_x);
    mouse_report.y = CONSTRAIN_HID_XY(report.delta_y);
    return mouse_report;
}

static inline int8_t pointing_device_movement_clamp(int16_t value) {
    if (value < INT8_MIN) {
        return INT8_MIN;
    } else if (value > INT8_MAX) {
        return INT8_MAX;
    } else {
        return value;
    }
}



report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
if (is_keyboard_left()) {
    report_mouse_t  cirque_pinnacle_report;


    cirque_pinnacle_report = cirque_trackpad_pointing_device_task(mouse_report);

    mouse_report.x = cirque_pinnacle_report.y;
    mouse_report.y = cirque_pinnacle_report.x;
    mouse_report.h = cirque_pinnacle_report.v;
    mouse_report.v = cirque_pinnacle_report.h;

    mouse_report.buttons = cirque_pinnacle_report.buttons;

} else {
    report_mouse_t  cirque_pinnacle_report;
     cirque_pinnacle_report = cirque_trackpad_pointing_device_task(mouse_report);

   //  if(init_success){

     report_mouse_t pmw3360_report =  pmw3360_get_report(mouse_report);

     mouse_report.x = pointing_device_xy_clamp((clamp_range_t) -pmw3360_report.x + cirque_pinnacle_report.y);
     mouse_report.y = pointing_device_xy_clamp((clamp_range_t) pmw3360_report.y + -cirque_pinnacle_report.x);
     mouse_report.h = pointing_device_xy_clamp((clamp_range_t) -pmw3360_report.h + cirque_pinnacle_report.v);
     mouse_report.v = pointing_device_xy_clamp((clamp_range_t) pmw3360_report.v + -cirque_pinnacle_report.h);

     mouse_report.buttons = cirque_pinnacle_report.buttons;
}

    return mouse_report; }

    uint16_t    pointing_device_driver_get_cpi(void) { return cirque_pinnacle_get_scale();}

    void        pointing_device_driver_set_cpi(uint16_t cpi) {
    
    if(is_keyboard_left()){
        cirque_pinnacle_set_scale(cpi);
        } else {
                pmw33xx_set_cpi_all_sensors(cpi);
                cirque_pinnacle_set_scale(cpi);
    }
    }