package com.example.falylibrary.utile;

import android.annotation.TargetApi;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Bitmap.Config;
import android.os.Build;
import android.os.Environment;
import android.util.Base64;

import java.io.BufferedOutputStream;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URLDecoder;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

/**
 * Created by min on 2017/12/6.
 */

public class AppUtile {
	public static String photoPath="";

	public static String spiltURL(String string) {
		String str = string.substring(7, string.length() - 1);
		try {
			str = URLDecoder.decode(str, "UTF-8");
		} catch (Exception e) {
		}
		return str;
	}

	/**
	 * 判断微信是否 安装
	 * @param context
	 * @return
	 */
	public static boolean isWeixinAvilible(Context context) {
		final PackageManager packageManager = context.getPackageManager();
		List<PackageInfo> pinfo = packageManager.getInstalledPackages(0);
		if (pinfo != null) {
			for (int i = 0; i < pinfo.size(); i++) {
				String pn = pinfo.get(i).packageName;
				if (pn.equals("com.tencent.mm")) {
					return true;
				}
			}
		}
		return false;
	}

	public static int dip2px(Context context, float dpValue) {
		final float scale = context.getResources().getDisplayMetrics().density;
		return (int) (dpValue * scale + 0.5f);
	}

	public static int px2dip(Context context, float pxValue) {
		final float scale = context.getResources().getDisplayMetrics().density;
		return (int) (pxValue / scale + 0.5f);
	}

	public static Bitmap getImage(Context context, String strUrl) {
		InputStream is = null;
		try {
			is = context.getAssets().open(strUrl);
			Bitmap bitmap = BitmapFactory.decodeStream(is);
			return bitmap;
		} catch (IOException e) {
			e.printStackTrace();
		}
		return null;
	}

	public static String bitmapTo(Bitmap bm) {

		ByteArrayOutputStream baos = new ByteArrayOutputStream();

		// 1.5M的压缩后在100Kb以内，测试得值,压缩后的大小=94486字节,压缩后的大小=74473字节
		// 这里的JPEG 如果换成PNG，那么压缩的就有600kB这样
		bm.compress(Bitmap.CompressFormat.JPEG, 100, baos);
		byte[] b = baos.toByteArray();
		return Base64.encodeToString(b, Base64.DEFAULT);
	}

	// 根据路径获得图片并压缩，返回bitmap用于显示
	public static Bitmap getSmallBitmap(String filePath) {
	
		final BitmapFactory.Options options = new BitmapFactory.Options();
		options.inJustDecodeBounds = true;
		BitmapFactory.decodeFile(filePath, options);

		// Calculate inSampleSize
		options.inSampleSize = calculateInSampleSize(options, 480, 800);

		// Decode bitmap with inSampleSize set
		options.inJustDecodeBounds = false;

		return BitmapFactory.decodeFile(filePath, options);
	}

	// 计算图片的缩放值
	public static int calculateInSampleSize(BitmapFactory.Options options,
			int reqWidth, int reqHeight) {
		final int height = options.outHeight;
		final int width = options.outWidth;
		int inSampleSize = 1;

		if (height > reqHeight || width > reqWidth) {
			final int heightRatio = Math.round((float) height
					/ (float) reqHeight);
			final int widthRatio = Math.round((float) width / (float) reqWidth);
			inSampleSize = heightRatio < widthRatio ? heightRatio : widthRatio;
		}
		return inSampleSize;
	}

	/**
	 * 自定义图片名，获取照片的file
	 */
	public static File createImgFile(Context context) {
		// 确定文件名
		String path = Environment.getExternalStorageDirectory() + File.separator +"fafaDesign"; //获取路径  
		String fileName = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date())+".jpg";//定义文件名  
		File file = new File(path,fileName);  
		if(!file.getParentFile().exists()){//文件夹不存在  
		    file.getParentFile().mkdirs();  
		} 
		// 获取文件路径
		photoPath = file.getAbsolutePath();
		return file;
	}
	public static String getImageUri(){
		return photoPath;
	}

	/**
	 * Compress image by size, this will modify image width/height. Used to get
	 * thumbnail
	 * 
	 * @param image
	 * @param pixelW
	 *            target pixel of width
	 * @param pixelH
	 *            target pixel of height
	 * @return
	 */
	public static Bitmap ratio(Bitmap image, float pixelW, float pixelH) {
		ByteArrayOutputStream os = new ByteArrayOutputStream();
		image.compress(Bitmap.CompressFormat.JPEG, 100, os);
		if (os.toByteArray().length / 1024 > 1024) {// 判断如果图片大于1M,进行压缩避免在生成图片（BitmapFactory.decodeStream）时溢出
			os.reset();// 重置baos即清空baos
			image.compress(Bitmap.CompressFormat.JPEG, 100, os);// 这里压缩50%，把压缩后的数据存放到baos中
		}
		ByteArrayInputStream is = new ByteArrayInputStream(os.toByteArray());
		BitmapFactory.Options newOpts = new BitmapFactory.Options();
		// 开始读入图片，此时把options.inJustDecodeBounds 设回true了
		newOpts.inJustDecodeBounds = true;
		newOpts.inPreferredConfig = Config.RGB_565;
		Bitmap bitmap = BitmapFactory.decodeStream(is, null, newOpts);
		newOpts.inJustDecodeBounds = false;
		int w = newOpts.outWidth;
		int h = newOpts.outHeight;
		float hh = pixelH;// 设置高度为240f时，可以明显看到图片缩小了
		float ww = pixelW;// 设置宽度为120f，可以明显看到图片缩小了
		// 缩放比。由于是固定比例缩放，只用高或者宽其中一个数据进行计算即可
		int be = 1;// be=1表示不缩放
		if (w > h && w > ww) {// 如果宽度大的话根据宽度固定大小缩放
			be = (int) (newOpts.outWidth / ww);
		} else if (w < h && h > hh) {// 如果高度高的话根据宽度固定大小缩放
			be = (int) (newOpts.outHeight / hh);
		}
		if (be <= 0)
			be = 1;
		newOpts.inSampleSize = be;// 设置缩放比例
		// 重新读入图片，注意此时已经把options.inJustDecodeBounds 设回false了
		is = new ByteArrayInputStream(os.toByteArray());
		bitmap = BitmapFactory.decodeStream(is, null, newOpts);
		// 压缩好比例大小后再进行质量压缩
		return bitmap;
	}
	/**
	 * png照片转jpg
	 * @param pngFilePath png 路径
	 * @param jpgFilePath jpg 路径
	 */
	 @TargetApi(Build.VERSION_CODES.KITKAT) 
	public static void convertToJpg(String pngFilePath, String jpgFilePath) {
	        Bitmap bitmap = BitmapFactory.decodeFile(pngFilePath);
	        try (BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(jpgFilePath))) {
	            if (bitmap.compress(Bitmap.CompressFormat.JPEG, 80, bos)) {
	                bos.flush();
	            }
	        } catch (IOException e) {
	            e.printStackTrace();
	        }
     }
	 /**
	  * 将图片镜像反转生成在新的路径
	  * @param pngFilePath
	  * @param jpgFilePath
	  */
	 @TargetApi(Build.VERSION_CODES.KITKAT) 
	 public static void toPictureMirror(String pngFilePath, String jpgFilePath){
		    Bitmap bitmap = BitmapFactory.decodeFile(pngFilePath);
            Bitmap modBm = Bitmap.createBitmap(bitmap.getWidth(),bitmap.getHeight(),bitmap.getConfig());
          //处理画布  重新绘制图形  形成镜像
	        Canvas canvas = new Canvas(modBm);
	        Paint paint = new Paint();
	        //绘制矩阵  Matrix主要用于对平面进行平移(Translate)，缩放(Scale)，旋转(Rotate)以及斜切(Skew)操作。
	        Matrix matrix = new Matrix();
	        //镜子效果：
	        matrix.setScale(-1,1);//翻转
	        matrix.postTranslate(bitmap.getWidth(),0);
	        canvas.drawBitmap(bitmap,matrix,paint);
		    if (pngFilePath.contains(".png")) {
		    	try (BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(jpgFilePath))) {
		            if (modBm.compress(Bitmap.CompressFormat.PNG, 80, bos)) {
		                bos.flush();
		            }
		        } catch (IOException e) {
		            e.printStackTrace();
		        }
			}else{
				try (BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(jpgFilePath))) {
		            if (modBm.compress(Bitmap.CompressFormat.JPEG, 80, bos)) {
		                bos.flush();
		            }
		        } catch (IOException e) {
		            e.printStackTrace();
		        }
			}       
	 }

	 

}
