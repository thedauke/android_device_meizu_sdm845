/*
 * Copyright (C) 2020 The MoKee Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

package org.lineageos.device.parts.cspress;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.util.Log;

class ScreenReceiver extends BroadcastReceiver {

    private static final String TAG = "CSPressService";
    private static final boolean DEBUG = false;

    private final CSPressService mService;

    private final IntentFilter mScreenStateFilter = new IntentFilter();

    private boolean mIsListening = false;

    ScreenReceiver(CSPressService service) {
        mService = service;
        mScreenStateFilter.addAction(Intent.ACTION_SCREEN_OFF);
        mScreenStateFilter.addAction(Intent.ACTION_USER_PRESENT);
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        final String action = intent.getAction();
        if (Intent.ACTION_USER_PRESENT.equals(action)) {
            Log.d(TAG, "Device unlocked");
            mService.disableCSPress();
        } else if (Intent.ACTION_SCREEN_OFF.equals(action)) {
            Log.d(TAG, "Device locked");
            mService.onDisplayOn();
        }
    }

    void enable() {
        if (mIsListening) return;
        if (DEBUG) Log.d(TAG, "Enabling");
        mService.registerReceiver(this, mScreenStateFilter);
        mIsListening = true;
    }

    void disable() {
        if (!mIsListening) return;
        if (DEBUG) Log.d(TAG, "Disabling");
        mService.unregisterReceiver(this);
        mIsListening = false;
    }

}
