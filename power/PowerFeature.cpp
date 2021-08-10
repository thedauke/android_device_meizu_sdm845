/*
 * Copyright (C) 2021, Paranoid Android
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "vendor.aospa.power-service.meizu_sdm845"
// #define LOG_NDEBUG 0

#include "PowerFeature.h"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <stdio.h>
#include <stdint.h>

using android::base::GetUintProperty;
using android::base::SetProperty;

#define GESTURE_CONTROL_PATH "/sys/class/meizu/tp/gesture_control"
#define GESTURE_CONTROL_PROPERTY "vendor.aospa.gesture_control_value"

#define SLIDE_LEFT_ENABLE   (1 << 0)
#define SLIDE_RIGHT_ENABLE  (1 << 1)
#define SLIDE_UP_ENABLE     (1 << 2)
#define SLIDE_DOWN_ENABLE   (1 << 3)
#define DOUBLE_TAP_ENABLE   (1 << 4)
#define DRAW_W_ENABLE       (1 << 9)
#define DRAW_M_ENABLE       (1 << 10)
#define DRAW_O_ENABLE       (1 << 11)
#define DRAW_S_ENABLE       (1 << 12)
#define DRAW_V_ENABLE       (1 << 13)
#define ALL_GESTURE_ENABLE  (1 << 31)

namespace {
static std::string hex(uint32_t value) {
    char buf[9];
    snprintf(buf, sizeof(buf), "%08x", value);
    return buf;
}

static bool setGesture(uint32_t value, bool enabled) {
    FILE *file = fopen(GESTURE_CONTROL_PATH, "wb");
    if (!file) {
        LOG(ERROR) << "setValue: Failed opening " << GESTURE_CONTROL_PATH;
        return false;
    }

    uint32_t mMainGestureControl = GetUintProperty<uint32_t>(GESTURE_CONTROL_PROPERTY, 0x0);

    mMainGestureControl &= ~ALL_GESTURE_ENABLE;

    if (enabled) {
      mMainGestureControl |= value;
    } else {
      mMainGestureControl &= ~value;
    }

    if (mMainGestureControl != 0) {
        mMainGestureControl |= ALL_GESTURE_ENABLE;
    }

    uint8_t buf[4];
    buf[0] = mMainGestureControl & 0xFF;
    buf[1] = (mMainGestureControl >> 8) & 0xFF;
    buf[2] = (mMainGestureControl >> 16) & 0xFF;
    buf[3] = (mMainGestureControl >> 24) & 0xFF;

    size_t ret = fwrite(buf, sizeof(buf), 1, file);
    fclose(file);

    LOG(INFO) << "setValue: " << hex(mMainGestureControl) << " " << ret;

    SetProperty(GESTURE_CONTROL_PROPERTY, std::to_string(mMainGestureControl));

    return true;
}
}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace aospa {
namespace power {

ndk::ScopedAStatus PowerFeature::setFeature(Feature feature, bool enabled) {
    bool ret;
    switch (feature) {
        case Feature::DOUBLE_TAP:
            ret = setGesture(DOUBLE_TAP_ENABLE, enabled);
            break;
        case Feature::DRAW_V:
            ret = setGesture(DRAW_V_ENABLE, enabled);
            break;
        case Feature::DRAW_O:
            ret = setGesture(DRAW_O_ENABLE, enabled);
            break;
        case Feature::DRAW_M:
            ret = setGesture(DRAW_M_ENABLE, enabled);
            break;
        case Feature::DRAW_W:
            ret = setGesture(DRAW_W_ENABLE, enabled);
            break;
        case Feature::ONE_FINGER_SWIPE_UP:
            ret = setGesture(SLIDE_UP_ENABLE, enabled);
            break;
        case Feature::ONE_FINGER_SWIPE_RIGHT:
            ret = setGesture(SLIDE_RIGHT_ENABLE, enabled);
            break;
        case Feature::ONE_FINGER_SWIPE_DOWN:
            ret = setGesture(SLIDE_DOWN_ENABLE, enabled);
            break;
        case Feature::ONE_FINGER_SWIPE_LEFT:
            ret = setGesture(SLIDE_LEFT_ENABLE, enabled);
            break;
        case Feature::DRAW_S:
            ret = setGesture(DRAW_S_ENABLE, enabled);
            break;
        default:
            return ndk::ScopedAStatus::fromServiceSpecificError(ENOTSUP);
    }

    if (!ret) {
        return ndk::ScopedAStatus::fromServiceSpecificError(EPERM); 
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace power
}  // namespace aospa
}  // namespace vendor
}  // namespace aidl
