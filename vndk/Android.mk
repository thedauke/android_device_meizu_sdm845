EXTRA_VNDK_APEX_LIBRARIES := \
    libdexfile_support

define define-vndk-apex-lib
include $$(CLEAR_VARS)
LOCAL_MODULE := $1.$2
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_PREBUILT_MODULE_FILE := $$(call intermediates-dir-for,SHARED_LIBRARIES,$1)/$1.so
LOCAL_STRIP_MODULE := false
LOCAL_MULTILIB := first
LOCAL_MODULE_TAGS := optional
LOCAL_INSTALLED_MODULE_STEM := $1.so
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/apex/com.android.vndk.current.on_vendor/lib64
LOCAL_CHECK_ELF_FILES := false
include $$(BUILD_PREBUILT)

ifneq ($$(TARGET_2ND_ARCH),)
ifneq ($$(TARGET_TRANSLATE_2ND_ARCH),true)
include $$(CLEAR_VARS)
LOCAL_MODULE := $1.$2
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_PREBUILT_MODULE_FILE := $$(call intermediates-dir-for,SHARED_LIBRARIES,$1,,,$(TARGET_2ND_ARCH_VAR_PREFIX))/$1.so
LOCAL_STRIP_MODULE := false
LOCAL_MULTILIB := 32
LOCAL_MODULE_TAGS := optional
LOCAL_INSTALLED_MODULE_STEM := $1.so
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/apex/com.android.vndk.current.on_vendor/lib
LOCAL_CHECK_ELF_FILES := false
include $$(BUILD_PREBUILT)
endif # TARGET_TRANSLATE_2ND_ARCH is not true
endif # TARGET_2ND_ARCH is not empty
endef

$(foreach lib,$(EXTRA_VNDK_APEX_LIBRARIES),\
    $(eval $(call define-vndk-apex-lib,$(lib),vndk-apex-ext-gen)))

include $(CLEAR_VARS)
LOCAL_MODULE := vndk-apex-ext
LOCAL_MODULE_TAGS := optional
LOCAL_REQUIRED_MODULES := \
    $(addsuffix .vndk-apex-ext-gen,$(EXTRA_VNDK_APEX_LIBRARIES))
include $(BUILD_PHONY_PACKAGE)
