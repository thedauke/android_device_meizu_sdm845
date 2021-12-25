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

#include "PowerFeature.h"

#include <android-base/logging.h>
#include <stdio.h>
#include <stdint.h>

#define GESTURE_CONTROL_PATH "/sys/class/meizu/tp/gesture_control"

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

namespace aidl {
namespace vendor {
namespace aospa {
namespace power {

ndk::ScopedAStatus PowerFeature::setFeature(Feature feature, bool enabled) {
    uint32_t value;
    switch (feature) {
        case Feature::DOUBLE_TAP:
            value = DOUBLE_TAP_ENABLE;
            break;
        case Feature::DRAW_V:
            value = DRAW_V_ENABLE;
            break;
        case Feature::DRAW_O:
            value = DRAW_O_ENABLE;
            break;
        case Feature::DRAW_M:
            value = DRAW_M_ENABLE;
            break;
        case Feature::DRAW_W:
            value = DRAW_W_ENABLE;
            break;
        case Feature::ONE_FINGER_SWIPE_UP:
            value = SLIDE_UP_ENABLE;
            break;
        case Feature::ONE_FINGER_SWIPE_RIGHT:
            value = SLIDE_RIGHT_ENABLE;
            break;
        case Feature::ONE_FINGER_SWIPE_DOWN:
            value = SLIDE_DOWN_ENABLE;
            break;
        case Feature::ONE_FINGER_SWIPE_LEFT:
            value = SLIDE_LEFT_ENABLE;
            break;
        case Feature::DRAW_S:
            value = DRAW_S_ENABLE;
            break;
        default:
            return ndk::ScopedAStatus::fromServiceSpecificError(ENOTSUP);
    }

    LOG(INFO) << "setFeature: Feature " << toString(feature) << " => 0x" << std::hex 
              << value << " (" << std::boolalpha << enabled << ")";

    if (access(GESTURE_CONTROL_PATH, R_OK | W_OK)) {
        return ndk::ScopedAStatus::fromServiceSpecificError(errno);
    }

    FILE *GestureControl = fopen(GESTURE_CONTROL_PATH, "r+b");

    int GestureValue;
    fscanf(GestureControl, "%x", &GestureValue);

    LOG(INFO) << "setFeature: Read GestureValue: 0x" << std::hex << GestureValue;

    GestureValue &= ~ALL_GESTURE_ENABLE;
    if (enabled) {
      GestureValue |= value;
    } else {
      GestureValue &= ~value;
    }
    if (GestureValue) {
        GestureValue |= ALL_GESTURE_ENABLE;
    }

    LOG(INFO) << "setFeature: Will be written GestureValue: 0x" << std::hex << GestureValue;

    uint8_t byteGestureValue[4];
    memcpy(byteGestureValue, &GestureValue, sizeof(GestureValue));

    fwrite(byteGestureValue, sizeof(byteGestureValue), 1, GestureControl);
    fclose(GestureControl);

    return ndk::ScopedAStatus::ok(); 
}

}  // namespace power
}  // namespace aospa
}  // namespace vendor
}  // namespace aidl
