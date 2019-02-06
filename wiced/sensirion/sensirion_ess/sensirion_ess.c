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

#include "wiced.h"
#include "sensirion_ess.h"
#include "sensirion_common_wiced.h"

#include "sgp30.h"
#include "sht.h"

#include "sensirion_i2c.h" /* for init hack */

static ess_device_config_t gDeviceConfig;


/* Debugging */
// #define ESS_DBG_CONT_ON_PROBE_ERROR 1
// #define ESS_DBG_FORCE_INIT_WORKAROUND 1

wiced_result_t ess_init_iaq(wiced_i2c_t port)
{
    wiced_result_t wres;
    uint8_t data[2] = { 0x20, 0x03 }; // init IAQ
    const int SGP30_I2C_ADDR = 0x58;

    wres = sensirion_i2c_write(SGP30_I2C_ADDR, data, 2);
    if (wres != WICED_SUCCESS) {
        WPRINT_APP_INFO(("ess_init_iaq: Write command failed\n"));
        return wres;
    }

    WPRINT_APP_INFO((">> I: Init IAQ success\n"));
    return WICED_SUCCESS;
}

wiced_result_t ess_init(const ess_device_config_t* config)
{
    if (config == NULL) {
        return WICED_BADOPTION;
    }

    gDeviceConfig.i2c_port              = config->i2c_port;
    gDeviceConfig.leds_supported        = config->leds_supported;
    gDeviceConfig.needs_init_workaround = config->needs_init_workaround;
    gDeviceConfig.pin_red               = config->pin_red;
    gDeviceConfig.pin_yellow            = config->pin_yellow;
    gDeviceConfig.pin_green             = config->pin_green;

    sensirion_wiced_set_i2c_port(config->i2c_port);

    if (config->leds_supported) {
        wiced_gpio_init(gDeviceConfig.pin_red,    OUTPUT_PUSH_PULL);
        wiced_gpio_init(gDeviceConfig.pin_yellow, OUTPUT_PUSH_PULL);
        wiced_gpio_init(gDeviceConfig.pin_green,  OUTPUT_PUSH_PULL);
    }


    /*
     * for some reason, some boards return faulty results before running init IAQ.
     *
     * This is under investigation; for the time being, calling init iaq before
     * sgp_probe() fixes this
     */
    int execute_workaround = config->needs_init_workaround;
#ifdef ESS_DBG_FORCE_INIT_WORKAROUND
    execute_workaround = 1;
    WPRINT_APP_INFO(("DBG: init workaround forced\n"));
#endif
    if (execute_workaround) {
        ess_init_iaq(config->i2c_port);
        wiced_rtos_delay_milliseconds(500);
    }

    if (sgp_probe() != STATUS_OK) {
        WPRINT_APP_INFO(("SGP sensor probing failed\n"));
#ifndef ESS_DBG_CONT_ON_PROBE_ERROR
        return WICED_ERROR;
#else
        WPRINT_APP_INFO(("DBG: ignoring SGP probing failure\n"));
#endif
    }
    // TODO: check err
    //u16 err = sgp_iaq_init();
    sgp_iaq_init();


    if (sht_probe() != STATUS_OK) {
        WPRINT_APP_INFO(("SHT sensor probing failed\n"));
        return WICED_ERROR;
    }


    return WICED_SUCCESS;
}

wiced_result_t ess_measure_iaq(u16* tvoc_ppb, u16* co2_eq_ppm)
{
    u16 err = sgp_measure_iaq_blocking_read(tvoc_ppb, co2_eq_ppm);
    if (err == STATUS_OK) {
        return WICED_SUCCESS;
    } else {
        return WICED_ERROR;
    }
}

wiced_result_t ess_measure_rht(s32* temperature, s32* humidity)
{
    s8 ret = sht_measure_blocking_read(temperature, humidity);
    if (ret == STATUS_OK) {
        return WICED_SUCCESS;
    } else {
        return WICED_ERROR;
    }
}

void ess_set_led(wiced_gpio_t pin, int state)
{
    if (state) {
        wiced_gpio_output_high(pin);
    } else {
        wiced_gpio_output_low(pin);
    }
}

void ess_set_leds_ryg(int r, int y, int g)
{
    if (gDeviceConfig.leds_supported) {
        ess_set_led(gDeviceConfig.pin_red, r);
        ess_set_led(gDeviceConfig.pin_yellow, y);
        ess_set_led(gDeviceConfig.pin_green, g);
    }
}
