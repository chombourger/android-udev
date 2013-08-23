LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

S:=dist/src/libudev

LOCAL_SRC_FILES:= \
 $(S)/libudev-device-private.c \
 $(S)/libudev-device.c \
 $(S)/libudev-enumerate.c \
 $(S)/libudev-hwdb.c \
 $(S)/libudev-list.c \
 $(S)/libudev-monitor.c \
 $(S)/libudev-queue-private.c \
 $(S)/libudev-queue.c \
 $(S)/libudev-util.c \
 $(S)/libudev.c \
 $(S)/../shared/conf-files.c \
 $(S)/../shared/dev-setup.c \
 $(S)/../shared/env-util.c \
 $(S)/../shared/fileio.c \
 $(S)/../shared/hashmap.c \
 $(S)/../shared/exit-status.c \
 $(S)/../shared/label.c \
 $(S)/../shared/mkdir.c \
 $(S)/../shared/path-util.c \
 $(S)/../shared/set.c \
 $(S)/../shared/strbuf.c \
 $(S)/../shared/strv.c \
 $(S)/../shared/strxcpyx.c \
 $(S)/../shared/time-util.c \
 $(S)/../shared/utf8.c \
 $(S)/../shared/util.c \
 android/glob.c \
 android/log.c \
 android/missing.c


LOCAL_C_INCLUDES += \
 external/udev/android \
 external/udev/dist/src/shared \
 external/udev/dist/src/systemd \

LOCAL_CFLAGS += -Wno-missing-field-initializers
LOCAL_CFLAGS += -std=gnu99
LOCAL_CFLAGS += -include external/udev/android/udev.h

LOCAL_SHARED_LIBRARIES += libcutils

LOCAL_LDLIBS := -llog

LOCAL_MODULE:= libudev
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE:= false

include $(BUILD_SHARED_LIBRARY)

###

include $(CLEAR_VARS)

S:=dist/src/udev

LOCAL_SRC_FILES:= \
 $(S)/udev-ctrl.c \
 $(S)/udev-builtin.c \
 $(S)/udev-builtin-btrfs.c \
 $(S)/udev-builtin-hwdb.c \
 $(S)/udev-builtin-input_id.c \
 $(S)/udev-builtin-keyboard.c \
 $(S)/udev-builtin-net_id.c \
 $(S)/udev-builtin-path_id.c \
 $(S)/udev-builtin-usb_id.c \
 $(S)/udev-event.c \
 $(S)/udev-node.c \
 $(S)/udev-rules.c \
 $(S)/udev-watch.c \
 $(S)/udevadm.c \
 $(S)/udevadm-control.c \
 $(S)/udevadm-hwdb.c \
 $(S)/udevadm-info.c \
 $(S)/udevadm-monitor.c \
 $(S)/udevadm-settle.c \
 $(S)/udevadm-test.c \
 $(S)/udevadm-test-builtin.c \
 $(S)/udevadm-trigger.c \
 android/qsortr.c

LOCAL_C_INCLUDES += \
 external/udev/android \
 external/udev/dist/src/shared \
 external/udev/dist/src/systemd \
 external/udev/dist/src/libudev \

LOCAL_CFLAGS += -Wno-missing-field-initializers
LOCAL_CFLAGS += -std=gnu99
LOCAL_CFLAGS += -include external/udev/android/udev.h
LOCAL_CFLAGS += -DVERSION=\"$(shell head -n 1 $(LOCAL_PATH)/VERSION)\"

LOCAL_SHARED_LIBRARIES := libudev

LOCAL_MODULE := udevadm
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)

###

include $(CLEAR_VARS)

S:=dist/src/udev

LOCAL_C_INCLUDES += \
 external/udev/android \
 external/udev/dist/src/shared \
 external/udev/dist/src/systemd \
 external/udev/dist/src/libudev \

LOCAL_SRC_FILES:= \
 $(S)/udevd.c \
 $(S)/udev-builtin.c \
 $(S)/udev-builtin-btrfs.c \
 $(S)/udev-builtin-firmware.c \
 $(S)/udev-builtin-hwdb.c \
 $(S)/udev-builtin-input_id.c \
 $(S)/udev-builtin-keyboard.c \
 $(S)/udev-builtin-net_id.c \
 $(S)/udev-builtin-path_id.c \
 $(S)/udev-builtin-usb_id.c \
 $(S)/udev-ctrl.c \
 $(S)/udev-event.c \
 $(S)/udev-node.c \
 $(S)/udev-rules.c \
 $(S)/udev-watch.c

LOCAL_CFLAGS += -Wno-missing-field-initializers
LOCAL_CFLAGS += -std=gnu99
LOCAL_CFLAGS += -include external/udev/android/udev.h
LOCAL_CFLAGS += -DVERSION=\"$(shell head -n 1 $(LOCAL_PATH)/VERSION)\"

LOCAL_SHARED_LIBRARIES := libudev

LOCAL_MODULE := udevd
LOCAL_MODULE_TAGS := optional

$(LOCAL_PATH)/$(S)/keyboard-keys.txt: $(LOCAL_PATH)/Android.mk
	$(TARGET_CC) -E $(TARGET_C_INCLUDES:%=-I%) -dM -include linux/input.h - < /dev/null | awk '/^#define[ \t]+KEY_[^ ]+[ \t]+[0-9]/ { if ($$2 != "KEY_MAX") { print $$2 } }' | sed 's/^KEY_COFFEE$$/KEY_SCREENLOCK/' > $@

$(LOCAL_PATH)/$(S)/keyboard-keys-from-name.gperf: $(LOCAL_PATH)/$(S)/keyboard-keys.txt $(LOCAL_PATH)/Android.mk
	awk 'BEGIN{ print "struct key { const char* name; unsigned short id; };"; print "%null-strings"; print "%%";} { print tolower(substr($$1 ,5)) ", " $$1 }' < $< > $@

$(LOCAL_PATH)/$(S)/keyboard-keys-from-name.h: $(LOCAL_PATH)/$(S)/keyboard-keys-from-name.gperf $(LOCAL_PATH)/Android.mk
	gperf -L ANSI-C -t -N keyboard_lookup_key -H hash_key_name -p -C < $< > $@

$(LOCAL_PATH)/$(S)/keyboard-keys-to-name.h: $(LOCAL_PATH)/$(S)/keyboard-keys.txt $(LOCAL_PATH)/Android.mk
	awk 'BEGIN{ print "const char* const key_names[KEY_CNT] = { "} { print "[" $$1 "] = \"" $$1 "\"," } END{print "};"}' < $< > $@

$(LOCAL_PATH)/$(S)/udev-builtin-keyboard.c: \
 $(LOCAL_PATH)/$(S)/keyboard-keys-from-name.h \
 $(LOCAL_PATH)/$(S)/keyboard-keys-to-name.h

include $(BUILD_EXECUTABLE)

