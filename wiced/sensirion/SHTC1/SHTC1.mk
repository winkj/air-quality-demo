NAME := Lib_SHTC1

$(NAME)_SOURCES := sht_common.c shtc1.c git_version.c 

GLOBAL_INCLUDES := .

$(NAME)_COMPONENTS := drivers/sensors/sensirion/sensirion_common
