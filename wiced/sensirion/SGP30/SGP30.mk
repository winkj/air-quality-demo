NAME := Lib_SGP30

$(NAME)_SOURCES := sgp30_featureset.c sgp30.c 

GLOBAL_INCLUDES := .

$(NAME)_COMPONENTS := drivers/sensors/sensirion/sensirion_common