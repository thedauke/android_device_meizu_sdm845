#
# Copyright (C) 2020 The MoKee Open Source Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/product_launched_with_o_mr1.mk)

# Inherit from m1882 device
$(call inherit-product, device/meizu/sdm845/m1882/m1882.mk)

# Inherit some common aosp stuff.
$(call inherit-product, vendor/aosp/config/common_full_phone.mk)

# Credits to XiNGRZ

PRODUCT_NAME := aosp_m1882
PRODUCT_BRAND := Meizu
PRODUCT_DEVICE := m1882
PRODUCT_MANUFACTURER := Meizu
PRODUCT_MODEL := 16th

PRODUCT_GMS_CLIENTID_BASE := android-meizu

PRODUCT_BUILD_PROP_OVERRIDES += \
    TARGET_DEVICE="16th" \
    PRODUCT_NAME="meizu_16th_CN" \
    PRIVATE_BUILD_DESC="meizu_16th_CN-user 10 QKQ1.191222.002 1594833800 release-keys"

BUILD_FINGERPRINT := meizu/qssi/qssi:10/QKQ1.191222.002/1594833800:user/release-keys

# Bootanimation Resolution
TARGET_BOOT_ANIMATION_RES := 1080

#GAPPS
TARGET_GAPPS_ARCH := arm64

# Exclude AudioFX
TARGET_EXCLUDES_AUDIOFX := true

# Pixel customization
TARGET_FACE_UNLOCK_SUPPORTED := true
TARGET_SUPPORTS_GOOGLE_RECORDER := true
TARGET_INCLUDE_STOCK_ARCORE := true
TARGET_INCLUDE_LIVE_WALLPAPERS := true


