SRC += c0deZ3R0.c
SRC += definitions/keycodes.c
SRC += keyrecords/process_records.c
SRC += keyrecords/achordion.c
SRC += keyrecords/combos.c
SRC += keyrecords/persistent_layers.c
SRC += keyrecords/taphold.c
SRC += keyrecords/os_toggle.c
#SRC += keyrecords/caps_word.c
SRC += keyrecords/select_word.c
SRC += keyrecords/casemodes.c
#SRC += keyrecords/tap_dance.c


SRC += pointing/pointing.c
SRC += callbacks.c




ifeq ($(strip $(POINTING_DEVICE_ENABLE)), yes)
 SRC += pointing/pointing_utils.c
 SRC += $(QUANTUM_DIR)/pointing_device/pointing_device_gestures.c
endif


ifdef CIRQUE_ENABLED
     ifeq ($(strip $(CIRQUE_ENABLED)), yes)
          SRC += pointing/cirque_trackpad_logic.c
		  SRC += drivers/sensors/cirque_pinnacle.c
		  SRC += drivers/sensors/cirque_pinnacle_i2c.c
		  SRC += drivers/sensors/cirque_pinnacle_gestures.c

		  OPT_DEFS += -DCIRQUE_ENABLED
       endif
endif

ifdef PMW3360_ENABLE
     ifeq ($(strip $(PMW3360_ENABLE)), yes)
	      SRC += drivers/sensors/pmw3360.c
		  SRC += drivers/sensors/pmw33xx_common.c
          #SRC += pointing/trackball_logic.c
		  OPT_DEFS += -DPMW3360_ENABLE
       endif
endif

CUSTOM_SPLIT_TRANSPORT_SYNC ?= yes
ifeq ($(strip $(CUSTOM_SPLIT_TRANSPORT_SYNC)), yes)
    ifeq ($(strip $(SPLIT_KEYBOARD)), yes)
        QUANTUM_LIB_SRC += split/transport_sync.c
        OPT_DEFS += -DCUSTOM_SPLIT_TRANSPORT_SYNC
    endif

endif

CAPS_WORD_ENABLE ?= no
ifeq ($(strip $(CAPS_WORD_ENABLE)), yes)
    SRC += keyrecords/caps_word.c
    OPT_DEFS += -DCAPS_WORD_ENABLE
endif


OPT_DEFS += -DSTM32_I2C -DHAL_USE_I2C=TRUE  -DSTM32_SPI -DHAL_USE_SPI=TRUE
QUANTUM_LIB_SRC += spi_master.c i2c_master.c



