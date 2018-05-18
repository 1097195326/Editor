package com.example.falylibrary;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class GyroScopeSensorListener implements SensorEventListener {
    public Context context;
    private SensorManager sensorManager;
    private Sensor sensor;
    
    private ISensorListener sensorListener;

    public GyroScopeSensorListener(Context context) {
        this.context = context;
        sensorManager = (SensorManager) context
                .getSystemService(Context.SENSOR_SERVICE);

        if (sensorManager == null)
            return;

        sensor = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);

    }

    public void setSensorListener(ISensorListener sensorListener) {
        this.sensorListener = sensorListener;
    }

    public void start() {
        if (sensor == null)
            return;

        sensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_GAME);
    }

    public void stop() {
        if (sensor == null)
            return;

        sensorManager.unregisterListener(this);
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        float x = event.values[SensorManager.DATA_X];
        float y = event.values[SensorManager.DATA_Y];

      
        if (sensorListener != null)
            sensorListener.onGyroScopeChange(y, x);
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

   
    public interface ISensorListener {
      
        void onGyroScopeChange(float horizontalShift, float verticalShift);
    }
}
