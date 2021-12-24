/*
 * Copyright (C) 2020 The LineageOS Project
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

#include <compositionengine/FodExtension.h>

#include <fstream>

#define HBM_ENABLE_PATH "/sys/class/meizu/lcm/display/hbm"
#define HBM_ON "1"
#define HBM_OFF "2"

static bool HBMStatus = false;

uint32_t getFodZOrder(uint32_t z, bool touched) {
    if (HBMStatus != touched) {
        std::ofstream HBM(HBM_ENABLE_PATH);
        HBM << (touched ? HBM_ON : HBM_OFF);
        HBMStatus = touched;
    }
    return z;
}

uint64_t getFodUsageBits(uint64_t usageBits, bool touched) {
    (void) touched;
    return usageBits;
}
