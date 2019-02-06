NAME := Lib_sensirion_ess

$(NAME)_SOURCES := sensirion_ess.c ess_device_configs.c

GLOBAL_INCLUDES := .

$(NAME)_COMPONENTS := drivers/sensors/sensirion/SGP30 drivers/sensors/sensirion/SHTC1