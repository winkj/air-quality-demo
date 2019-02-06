/*
 *  Copyright (c) 2017-2018, Sensirion AG
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither the name of Sensirion AG nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SENSIRION_ESS_H
#define _SENSIRION_ESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sensirion_common.h"

/**
 * @file
 *
 * @brief Wiced interface for Sensirion Environmental Sensor Shield
 *
 * This module provides convenient functions to use the ESS on
 * Wiced Wifi based platforms
 */

typedef struct {
    wiced_i2c_t i2c_port;
    uint8_t needs_init_workaround;
    uint8_t leds_supported;
    wiced_gpio_t pin_red;
    wiced_gpio_t pin_green;
    wiced_gpio_t pin_yellow;
} ess_device_config_t;


/**
 * initialize the ESS board on a specific I2C port
 * @param port the I2C port the ESS is connected to
 * @return WICED_SUCCESS on success, WICED_ERROR otherwise
 */
wiced_result_t ess_init(const ess_device_config_t* config);

/**
 * trigger a measurement of Indoor Air Quality (IAQ), and
 * store values in @a tvoc_ppb and @a co2_eq_ppm4
 * Note: the first 15-20s, the ESS is in an initialization
 * mode, and will return default values (0ppb/400ppm)
 *
 * @param (output) tvoc_ppb Total VOC level in parts per billion
 * @param (output) co2_eq_ppm CO2 equivalent in parts per million
 * @return WICED_SUCCESS on success, WICED_ERROR otherwise
 */
wiced_result_t ess_measure_iaq(u16* tvoc_ppb, u16* co2_eq_ppm);

/**
 * trigger a measurement of humidity and temperature, and
 * store values in @a temperature and @a humidity
 *
 * @param (output) temperature temperature value in degree celcius
 * @param (output) humidity humidity value in percent (0..100%)
 */
wiced_result_t ess_measure_rht(s32* temperature, s32* humidity);

/**
 * control the red, yellow and green LED on the ESS
 * @param r pass 1 to turn red    LED on, 0 to turn it off
 * @param y pass 1 to turn yellow LED on, 0 to turn it off
 * @param g pass 1 to turn green  LED on, 0 to turn it off
 */
void ess_set_leds_ryg(int r, int y, int g);

#ifdef __cplusplus
}
#endif

#endif /* _SENSIRION_ESS_H */
