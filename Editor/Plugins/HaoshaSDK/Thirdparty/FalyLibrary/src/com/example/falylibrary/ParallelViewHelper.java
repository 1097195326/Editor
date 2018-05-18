package com.example.falylibrary;

import com.example.falylibrary.utile.AppUtile;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;


public class ParallelViewHelper implements GyroScopeSensorListener.ISensorListener {

    public static final float TRANSFORM_FACTOR = 0.02f;
    private float mTransformFactor = TRANSFORM_FACTOR;
    private View mParallelView;
    private float mCurrentShiftX;
    private float mCurrentShiftY;
    private GyroScopeSensorListener mSensorListener;
    private ViewGroup.LayoutParams mLayoutParams;
    private int mViewWidth;
    private int mViewHeight;
    private int mShiftDistancePX;

    public ParallelViewHelper(Context context) {
        this(context, AppUtile.dip2px(context,40));
    }

   
    public ParallelViewHelper(Context context,int shiftDistancePX) {
        mShiftDistancePX = shiftDistancePX;
        mSensorListener = new GyroScopeSensorListener(context);
        mSensorListener.setSensorListener(this);
//        mParallelView = targetView;
//        mParallelView.setX(-mShiftDistancePX);
//        mParallelView.setY(-mShiftDistancePX);
//        mLayoutParams = mParallelView.getLayoutParams();
//        mViewWidth = mParallelView.getWidth();
//        mViewHeight = mParallelView.getHeight();
//
//        if (mViewWidth > 0 && mViewHeight > 0) {
//            bindView();
//            return;
//        }
//
//        ViewTreeObserver vto = targetView.getViewTreeObserver();
//        vto.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
//            @Override
//            public void onGlobalLayout() {
//                targetView.getViewTreeObserver().removeGlobalOnLayoutListener(this);
//                mViewWidth = targetView.getWidth();
//                mViewHeight = targetView.getHeight();
//                bindView();
//            }
//        });
    }

    void bindView() {
//        mLayoutParams.width = mViewWidth + mShiftDistancePX * 2;
//        mLayoutParams.height = mViewHeight + mShiftDistancePX * 2;
//        mParallelView.setLayoutParams(mLayoutParams);
    }


  
    public void start() {
        mSensorListener.start();
    }

    
    public void stop() {
        mSensorListener.stop();
    }

   
    public void setTransformFactor(float transformFactor) {
        mTransformFactor = transformFactor;
    }

    @Override
    public void onGyroScopeChange(float horizontalShift, float verticalShift) {
        mCurrentShiftX += mShiftDistancePX * horizontalShift * mTransformFactor;
        mCurrentShiftY += mShiftDistancePX * verticalShift * mTransformFactor;

        if (Math.abs(mCurrentShiftX) > mShiftDistancePX)
            mCurrentShiftX = mCurrentShiftX < 0 ? -mShiftDistancePX : mShiftDistancePX;

        if (Math.abs(mCurrentShiftY) > mShiftDistancePX)
            mCurrentShiftY = mCurrentShiftY < 0 ? -mShiftDistancePX : mShiftDistancePX;
        if (mISensorListener!=null){
            mISensorListener.onGyroScopeChange((int) mCurrentShiftX - mShiftDistancePX,(int) mCurrentShiftY - mShiftDistancePX);
        }

//        //榛樿灏眒argin 璐熺殑杈硅窛灏哄锛屽洜姝� margin鐨勬渶澶у�兼槸 璐熺殑杈硅窛灏哄*2 ~ 0
//        mParallelView.setX((int) mCurrentShiftX - mShiftDistancePX);
//        mParallelView.setY((int) mCurrentShiftY - mShiftDistancePX);
    }
    public ISensorListener mISensorListener;
    public void setSensorListener(ISensorListener sensorListener) {
        this.mISensorListener = sensorListener;
    }
    public interface ISensorListener{
      
        void onGyroScopeChange(int x, int y);
    }
}
