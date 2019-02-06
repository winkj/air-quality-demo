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
#include "ess_device_configs.h"


#define DELAY_TIME 500


void application_start( )
{
    u16 tvoc_ppb, co2_eq_ppm;
    s32 temperature, humidity;

    wiced_init();

    WPRINT_APP_INFO(("ESS demo\n"));

    while (ess_init(&ESS_DEVICE_CONFIG_DEFAULT) != WICED_SUCCESS) {
        WPRINT_APP_INFO(("ESS probing failed\n"));
        wiced_rtos_delay_milliseconds(DELAY_TIME);
    }

    while (1) {
        wiced_result_t ret = ess_measure_iaq(&tvoc_ppb, &co2_eq_ppm);
        if (ret == WICED_SUCCESS) {
            WPRINT_APP_INFO(("tVOC %4u | CO2eq %4u | ", tvoc_ppb, co2_eq_ppm));
        } else {
            WPRINT_APP_INFO(("error reading IAQ values\n"));
        }

        if (co2_eq_ppm > 3000) {
            ess_set_leds_ryg(1, 0, 0);
        } else if (co2_eq_ppm > 1000) {
            ess_set_leds_ryg(0, 1, 0);
        } else {
            ess_set_leds_ryg(0, 0, 1);
        }

        ret = ess_measure_rht(&temperature, &humidity);
        if (ret == WICED_SUCCESS) {
            WPRINT_APP_INFO(("T   %2.2f | RH   %2.2f\n", temperature / 1000.0f, humidity / 1000.0f));
        } else {
            WPRINT_APP_INFO(("error reading RH/T values\n"));
        }

        wiced_rtos_delay_milliseconds(1000);
    }

}
