/*
 * Copyright (C) 2020 The MoKee Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

package org.lineageos.device.parts.aod;

import android.content.Context;
import android.provider.Settings;

import com.android.internal.R;

import org.lineageos.internal.util.FileUtils;

class Utils {

    static boolean isAODEnabled(Context context) {
        final boolean alwaysOnByDefault = context.getResources()
                .getBoolean(R.bool.config_dozeAlwaysOnEnabled);

        return Settings.Secure.getInt(context.getContentResolver(),
                Settings.Secure.DOZE_ALWAYS_ON,
                alwaysOnByDefault ? 1 : 0) != 0;
    }

    /* static boolean isPulseEnabled(Context context) {
        // https://github.com/crdroidandroid/android_frameworks_base/blob/11.0/packages/SystemUI/src/com/android/systemui/statusbar/phone/NotificationPanelViewController.java#L3027
        return Settings.Secure.getInt(context.getContentResolver(),
            Settings.Secure.PULSE_AMBIENT_LIGHT, 0) != 0;
    } */

    static int getDuration(Context context) {
        // https://github.com/crdroidandroid/android_frameworks_base/blob/11.0/packages/SystemUI/src/com/android/systemui/phone/NotificationLightsView.java#L86
        return Settings.Secure.getInt(context.getContentResolver(),
            Settings.Secure.PULSE_AMBIENT_LIGHT_DURATION, 2) * 1000;
    }

    static void enterAOD() {
        FileUtils.writeLine("/sys/class/meizu/lcm/display/doze_s2", "1");
    }

}
