LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE :=  uart_test
LOCAL_SRC_FILES := uart_test.c
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE :=  testuart
LOCAL_SRC_FILES := uart.c
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE :=  libuart_bl
LOCAL_SRC_FILES := libuart_bl.c
include $(BUILD_SHARED_LIBRARY)
