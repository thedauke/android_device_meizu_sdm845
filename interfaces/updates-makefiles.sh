#!/bin/bash

source $ANDROID_BUILD_TOP/system/tools/hidl/update-makefiles-helper.sh

do_makefiles_update \
  "vendor.synaptics:device/meizu/sdm845/interfaces"
  "vendor.goodix:device/meizu/sdm845/interfaces"
