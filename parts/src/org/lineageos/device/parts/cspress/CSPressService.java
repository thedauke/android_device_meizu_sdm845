package org.lineageos.device.parts.cspress;

import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.util.Log;

public class CSPressService extends Service {

    private static final String LOG = "CSPressService";
    private static final boolean DEBUG = false;

    private static final long CSPRESS_DELAY_MS = 2000;

    private ScreenReceiver mScreenReceiver;

    private Handler mHandler = new Handler(Looper.getMainLooper());

    @Override
    public void onCreate() {
        super.onCreate();
        mScreenReceiver = new ScreenReceiver(this);
        mScreenReceiver.enable();
        if (DEBUG) Log.d(LOG, "onCreate");
    }
   
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (DEBUG) Log.d(LOG, "onStartCommand");
        return START_STICKY;
    }
 
    @Override
    public void onDestroy() {
        super.onDestroy();
        mScreenReceiver.disable();
        if (DEBUG) Log.d(LOG, "onDestroy");
    }

    @Override 
    public IBinder onBind(Intent intent) {
        if (DEBUG) Log.d(LOG, "onBind");
        return null;
    }

    void onDisplayOn() {
        mHandler.removeCallbacksAndMessages(null);
    }

    void disableCSPress() {
        Log.d(LOG, "Disabling CS_Press");
        mHandler.postDelayed(() -> {
            Utils.exitCSPress();
            Log.d(LOG, "CS_Press disabled");
        }, CSPRESS_DELAY_MS);
    }

}
