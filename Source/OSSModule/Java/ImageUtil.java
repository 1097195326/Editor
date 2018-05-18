package com.oss.dachengshidai;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by YUKUIDONG on 2017/11/30.
 */

public class ImageUtil {

    public static Bitmap getLocalThumbImg(String path) {
        BitmapFactory.Options newOpts = new BitmapFactory.Options();
        newOpts.inJustDecodeBounds = false;
        newOpts.inSampleSize = 1; // 设置缩放比例1表示不缩放
        Bitmap bitmap = BitmapFactory.decodeFile(path, newOpts); // 此时返回bm为空

		//if(!bitmap.isRecycled()){
		// bitmap.recycle(); //回收图片所占的内存
		// System.gc(); //提醒系统及时回收
		//}

		newOpts=null;

        return bitmap;
    }

    /**
     * 根据给定的宽和高进行拉伸
     *
     * @param origin    原图
     * @param newWidth  新图的宽
     * @param newHeight 新图的高
     * @return new Bitmap
     */
    public static  Bitmap scaleBitmap(Bitmap origin, int newWidth, int newHeight) {
        if (origin == null) {
            return null;
        }
        int height = origin.getHeight();
        int width = origin.getWidth();
        float scaleWidth = ((float) newWidth) / width;
        float scaleHeight = ((float) newHeight) / height;
        Matrix matrix = new Matrix();
        matrix.postScale(scaleWidth, scaleHeight);// 使用后乘

		String widthStr = String.valueOf(scaleWidth);
		String heightStr = String.valueOf(scaleHeight);
		System.out.println("!!!scaleWidth:" + widthStr);
		System.out.println("!!!scaleHeight:" + heightStr);

        Bitmap newBM = Bitmap.createBitmap(origin, 0, 0, width, height, matrix, false);
        if (!origin.isRecycled()) {
            origin.recycle();
        }
        return newBM;
    }

    /**
     * 按比例缩放图片
     *
     * @param origin 原图
     * @param ratio  比例
     * @return 新的bitmap
     */
    public static Bitmap scaleBitmap(Bitmap origin, float ratio) {
        if (origin == null) {
            return null;
        }
        int width = origin.getWidth();
        int height = origin.getHeight();
        Matrix matrix = new Matrix();
        matrix.preScale(ratio, ratio);


        Bitmap newBM = Bitmap.createBitmap(origin, 0, 0, width, height, matrix, false);
        if (newBM.equals(origin)) {
            return newBM;
        }
        origin.recycle();
        return newBM;
    }

    /**
     * 裁剪
     *
     * @param bitmap 原图
     * @return 裁剪后的图像
     */
    public static Bitmap cropBitmap(Bitmap bitmap,int cropWidth,int cropHeight) {

        return Bitmap.createBitmap(bitmap, 0, 0, cropWidth, cropHeight, null, false);
    }

    /**
     * 选择变换
     * @param origin 原图
     * @param alpha  旋转角度，可正可负
     * @return 旋转后的图片
     */
    public static Bitmap rotateBitmap(Bitmap origin, float alpha) {
        if (origin == null) {
            return null;
        }
        int width = origin.getWidth();
        int height = origin.getHeight();
        Matrix matrix = new Matrix();
        matrix.setRotate(alpha);
        // 围绕原地进行旋转
        Bitmap newBM = Bitmap.createBitmap(origin, 0, 0, width, height, matrix, false);
        if (newBM.equals(origin)) {
            return newBM;
        }
        origin.recycle();
        return newBM;
    }

    /**
     * 偏移效果
     * @param origin 原图
     * @return 偏移后的bitmap
     */
    public Bitmap skewBitmap(Bitmap origin) {
        if (origin == null) {
            return null;
        }
        int width = origin.getWidth();
        int height = origin.getHeight();
        Matrix matrix = new Matrix();
        matrix.postSkew(-0.6f, -0.3f);
        Bitmap newBM = Bitmap.createBitmap(origin, 0, 0, width, height, matrix, false);
        if (newBM.equals(origin)) {
            return newBM;
        }
        origin.recycle();
        return newBM;
    }
    public static Bitmap buidIcon(String path,int l){
        Bitmap old = getLocalThumbImg(path);
        if(old!=null){
            int w = old.getWidth();
            int h = old.getHeight();
            float ratio = w>h?((114*1.00f)/h):((114*1.00f)/w);
           return cropBitmap(scaleBitmap(old,ratio),l,l);
        }else{
            return old;
        }
    }
    public static  int [] imageInfo(String path){
        Bitmap old = getLocalThumbImg(path);

		System.out.println("zhoujia:!!! imageInfo filePath:"+path);
        int [] result  ={0,0};
        if(old!=null){
            int w = old.getWidth();
            int h = old.getHeight();
            int dpi = old.getDensity();
            result[0] = w;
            result[1] = h;
        }
        return result;
    }
    public static Bitmap buidBig(String path){
        Bitmap old = getLocalThumbImg(path);
        if(old!=null){
            int w = old.getWidth();
            int h = old.getHeight();

			
            int cw =w;
            int ch =h;
	
            boolean wFlag=false;
            boolean hFlag=false;
			double Duishu = Math.log(w) / Math.log(2);
			if(Math.round(Duishu)!=Duishu)
			{
				cw = convert(w);
			}
			else
			{
				wFlag=true;
			}
			Duishu = Math.log(h) / Math.log(2);

			if(Math.round(Duishu)!=Duishu)
			{
				ch = convert(h);
			}else
			{
				hFlag=true;
			}
			if (wFlag && hFlag) 
			{
				return old;
			}
			String cwStr = String.valueOf(cw);
			String chStr = String.valueOf(ch);

			System.out.println("zhoujia:!!!CW:"+cw);
			System.out.println("zhoujia:!!!CH:"+ch);
            return scaleBitmap(old,cw>2048?2048:cw,ch>2048?2048:ch);
        }else{
            return old;
        }
    }
    public static int convert(int i) {
        double a = Math.log(i) / Math.log(2);
        double s =  Math.pow(2,a);
        double e = Math.pow(2,a+1);
        if((e-i)>(i-s)){
            return (int)Math.round(Math.pow(2,Math.round(a)));

        }else{
            return (int )Math.round(Math.pow(2,Math.round(e)));
        }
    }

    /**
     * 保存bitmap到SD卡
     * @param bitName 保存的名字
     * @param mBitmap 图片对像
     * return 生成压缩图片后的图片路径
     */
    public static String saveMyBitmap(String path,String bitName,Bitmap mBitmap,String type) {
        File p= new File(path);
        if(!p.exists()){
            p.mkdirs();
        }
        File f = new File(path+File.separator + bitName + type);
        try {
            if(f.exists()){
                f.delete();
            }
            f.createNewFile();
        } catch (IOException e) {
            System.out.println("在保存图片时出错：" + e.toString());
        }
        FileOutputStream fOut = null;
        try {
            fOut = new FileOutputStream(f);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        try {
			if(type.contains("jpg"))
			{
				mBitmap.compress(Bitmap.CompressFormat.JPEG, 100, fOut);
			}
            else if(type.contains("png"))
			{
				mBitmap.compress(Bitmap.CompressFormat.PNG, 100, fOut);
			}
           
        } catch (Exception e) {
            return "create_bitmap_error";
        }
        try {
            fOut.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            fOut.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return path + bitName + type;
    }
}
