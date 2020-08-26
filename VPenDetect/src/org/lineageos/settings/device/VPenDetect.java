package org.lineageos.settings.device;

import android.app.Service;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.IBinder;

import android.os.UEventObserver;
import android.util.Log;

import android.os.Build;
import android.content.Context;
import android.os.Vibrator;
import android.os.VibrationEffect;
import android.preference.PreferenceManager;

public class VPenDetect extends Service {
    private Context mContext;
    private static final String KEY_VPEN_ENABLE = "vpen_enable";
    private static final String KEY_VPEN_MODE = "vpen_mode";
    private boolean mVPenEnabled = true;
    private int mVPenMode;

    private static final String LOG_TAG = "V-PEN";
    private static final String PEN_CONNECT = "ON";
    private static final String PEN_DISCONNECT = "OFF";
    private final Object mLock = new Object();

    private static final int LETTER_MODE = 0;
    private static final int DRAW_MODE = 1;

    static native int setTouchMode(int state);

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        mContext = this;

        mUEventObserver.startObserving("DEVPATH=/devices/virtual/switch/touch_pen_detection");

        Log.i(LOG_TAG, "V-Pen Monitor service started");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        Log.i(LOG_TAG, "V-Pen Monitor service stopped");
    }

    private final UEventObserver mUEventObserver = new UEventObserver() {
        @Override
        public void onUEvent(UEvent event) {
            synchronized (mLock) {
                String switchState = event.get("SWITCH_STATE");

                SharedPreferences sharedPrefs = PreferenceManager.
                        getDefaultSharedPreferences(mContext);

                mVPenEnabled = sharedPrefs.getBoolean(
                        KEY_VPEN_ENABLE, true);

                mVPenMode = Integer.parseInt(sharedPrefs.getString(KEY_VPEN_MODE,
                        Integer.toString(LETTER_MODE)));

                if (PEN_CONNECT.equals(switchState)) {
                    Log.d(LOG_TAG, "CONNECTED");
                    setTouchMode(0x0);
                } else if (PEN_DISCONNECT.equals(switchState)) {
                    Log.d(LOG_TAG, "DISCONNECTED");
                    if (mVPenEnabled) {
                        if (mVPenMode == LETTER_MODE) setTouchMode(0x2);
                        else if (mVPenMode == DRAW_MODE) setTouchMode(0x3);
                    }
                }

                if (mVPenEnabled) {
                    Vibrator v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
                    assert v != null;
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                        v.vibrate(VibrationEffect.createOneShot(80,
                                VibrationEffect.DEFAULT_AMPLITUDE));
                    } else {
                        v.vibrate(80);
                    }
                }
            }
        }
    };

    static {
        System.loadLibrary("tm_switch");
    }
}
