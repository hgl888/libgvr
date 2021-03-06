/* Copyright 2016 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.mj.vr;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Vibrator;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import com.google.vr.ndk.base.AndroidCompat;
import com.google.vr.ndk.base.GvrLayout;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/** A Gvr API sample application. */
public class TreasureActivityJNI extends Activity {
  private final String TAG = "TreasureActivityJNI";
  private GvrLayout gvrLayout;
  private long nativeTreasureHuntRenderer;
  private GLSurfaceView surfaceView;
  public static int cout = 0;

  // This is done on the GL thread because refreshViewerProfile isn't thread-safe.
  private final Runnable refreshViewerProfileRunnable =
      new Runnable() {
        @Override
        public void run() {
          gvrLayout.getGvrApi().refreshViewerProfile();
        }
      };

  //todo hgl
//  static {
//    System.loadLibrary("gvr");
//    System.loadLibrary("gvr_audio");
//  }

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);

    // Ensure fullscreen immersion.
    setImmersiveSticky();
    getWindow()
        .getDecorView()
        .setOnSystemUiVisibilityChangeListener(
            new View.OnSystemUiVisibilityChangeListener() {
              @Override
              public void onSystemUiVisibilityChange(int visibility) {
                if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
                  setImmersiveSticky();
                }
              }
            });

    // Initialize GvrLayout and the native renderer.
    gvrLayout = new GvrLayout(this);
    nativeTreasureHuntRenderer = nativeCreateRenderer(getClass().getClassLoader(), this.getApplicationContext(), gvrLayout.getGvrApi().getNativeGvrContext());

    // Add the GLSurfaceView to the GvrLayout.
    surfaceView = new GLSurfaceView(this);
    surfaceView.setEGLContextClientVersion(2);
    surfaceView.setEGLConfigChooser(8, 8, 8, 0, 0, 0);
    surfaceView.setPreserveEGLContextOnPause(true);
    surfaceView.setRenderer(
        new GLSurfaceView.Renderer() {
          @Override
          public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            nativeInitializeGl(nativeTreasureHuntRenderer);
          }

          @Override
          public void onSurfaceChanged(GL10 gl, int width, int height) {}

          @Override
          public void onDrawFrame(GL10 gl) {
            nativeDrawFrame(nativeTreasureHuntRenderer);
          }
        });
    surfaceView.setOnTouchListener(
        new View.OnTouchListener() {
          @Override
          public boolean onTouch(View v, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
              // Give user feedback and signal a trigger event.
              ((Vibrator) getSystemService(Context.VIBRATOR_SERVICE)).vibrate(50);
              nativeOnTriggerEvent(nativeTreasureHuntRenderer);
              return true;
            }
            return false;
          }
        });
    gvrLayout.setPresentationView(surfaceView);

    // Add the GvrLayout to the View hierarchy.
    setContentView(gvrLayout);

    // Enable scan line racing.
    if (gvrLayout.setAsyncReprojectionEnabled(true)) {
      // Scanline racing decouples the app framerate from the display framerate,
      // allowing immersive interaction even at the throttled clockrates set by
      // sustained performance mode.
      AndroidCompat.setSustainedPerformanceMode(this, true);
    }

    // Enable VR Mode.
    AndroidCompat.setVrModeEnabled(this, true);

    // Prevent screen from dimming/locking.
    getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
  }

  @Override
  protected void onPause() {
    super.onPause();
    nativeOnPause(nativeTreasureHuntRenderer);
    gvrLayout.onPause();
    surfaceView.onPause();
  }

  @Override
  protected void onResume() {
    super.onResume();
    nativeOnResume(nativeTreasureHuntRenderer);
    gvrLayout.onResume();
    surfaceView.onResume();
    surfaceView.queueEvent(refreshViewerProfileRunnable);
//    nativeTestGvrCreate(this.getApplicationContext(), getClassLoader());
  }

  @Override
  protected void onDestroy() {
    super.onDestroy();
    // Destruction order is important; shutting down the GvrLayout will detach
    // the GLSurfaceView and stop the GL thread, allowing safe shutdown of
    // native resources from the UI thread.
    gvrLayout.shutdown();
    nativeDestroyRenderer(nativeTreasureHuntRenderer);
  }

  @Override
  public void onWindowFocusChanged(boolean hasFocus) {
    super.onWindowFocusChanged(hasFocus);
    if (hasFocus) {
      setImmersiveSticky();
    }
  }

  @Override
  public boolean dispatchKeyEvent(KeyEvent event) {
    // Avoid accidental volume key presses while the phone is in the VR headset.
    if (event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_UP || event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_DOWN) {
      return true;
    }
    return super.dispatchKeyEvent(event);
  }

  @Override
  public boolean dispatchTouchEvent(MotionEvent ev) {
    int action = ev.getAction();

    switch (action) {
      case MotionEvent.ACTION_DOWN:
        Log.d(TAG, "dispatchTouchEvent action:ACTION_DOWN");
        break;
      case MotionEvent.ACTION_MOVE:
        Log.d(TAG, "dispatchTouchEvent action:ACTION_MOVE");
        break;
      case MotionEvent.ACTION_UP:
        Log.d(TAG, "dispatchTouchEvent action:ACTION_UP");
        break;
      case MotionEvent.ACTION_CANCEL:
        Log.d(TAG, "dispatchTouchEvent action:ACTION_CANCEL");
        break;
    }

    return super.dispatchTouchEvent(ev);
  }

//  @Override
  public boolean onInterceptTouchEvent(MotionEvent ev) {
    int action = ev.getAction();

    switch (action) {
      case MotionEvent.ACTION_DOWN:
        Log.d(TAG, "onInterceptTouchEvent action:ACTION_DOWN");
        break;
      case MotionEvent.ACTION_MOVE:
        Log.d(TAG, "onInterceptTouchEvent action:ACTION_MOVE");
        break;
      case MotionEvent.ACTION_UP:
        Log.d(TAG, "onInterceptTouchEvent action:ACTION_UP");
        break;
      case MotionEvent.ACTION_CANCEL:
        Log.d(TAG, "onInterceptTouchEvent action:ACTION_CANCEL");
        break;
    }
    return false;
  }

  @Override
  public boolean onTouchEvent(MotionEvent ev) {
    int action = ev.getAction();

    switch (action) {
      case MotionEvent.ACTION_DOWN:
        Log.d(TAG, "---onTouchEvent action:ACTION_DOWN");
        break;
      case MotionEvent.ACTION_MOVE:
        Log.d(TAG, "---onTouchEvent action:ACTION_MOVE");
        break;
      case MotionEvent.ACTION_UP:
        Log.d(TAG, "---onTouchEvent action:ACTION_UP");
        break;
      case MotionEvent.ACTION_CANCEL:
        Log.d(TAG, "---onTouchEvent action:ACTION_CANCEL");
        break;
    }

    return true;
  }

  private void setImmersiveSticky() {
    getWindow()
        .getDecorView()
        .setSystemUiVisibility(
            View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
  }

  private native long nativeTestGvrCreate(Context ctx, ClassLoader classLoader);

  private native long nativeCreateRenderer(ClassLoader appClassLoader, Context context, long nativeGvrContext);

  private native void nativeDestroyRenderer(long nativeTreasureHuntRenderer);

  private native void nativeInitializeGl(long nativeTreasureHuntRenderer);

  private native long nativeDrawFrame(long nativeTreasureHuntRenderer);

  private native void nativeOnTriggerEvent(long nativeTreasureHuntRenderer);

  private native void nativeOnPause(long nativeTreasureHuntRenderer);

  private native void nativeOnResume(long nativeTreasureHuntRenderer);

}
