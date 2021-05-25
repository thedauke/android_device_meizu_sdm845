/*
 * Copyright (C) 2020 The MoKee Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

package org.lineageos.device.parts.cspress;

import org.lineageos.internal.util.FileUtils;

class Utils {

    static void exitCSPress() {
        FileUtils.writeLine("/sys/cs_press/cs_press/sleep", "1");
    }

}
