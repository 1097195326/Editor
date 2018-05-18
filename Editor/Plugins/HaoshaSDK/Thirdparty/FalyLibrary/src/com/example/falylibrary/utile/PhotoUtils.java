package com.example.falylibrary.utile;

import java.io.File;

import com.example.falylibrary.utile.AppUtile;
import com.example.falylibrary.utile.PhotoBitmapUtils;
import com.example.falylibrary.utile.RxPhotoTool;


import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;

import android.widget.Toast;

public class PhotoUtils {
	public static PhotoUtils mPhotoUtils = new PhotoUtils();
	public static Context mContext;
	public final static int FILECHOOSER_RESULTCODE = 0;// 相册
	public final static int REQUEST_CODE_PICK_IMAGE = 1;// 相机
	public static File fileUri = null;
	private static Uri cropImageUri;

	public static PhotoUtils getPhotoUtils(Context context) {
		mContext = context;
     	return mPhotoUtils;
	}
	private PhotoUtils(){
		
	}

	/**
	 * @param activity
	 *            当前activity
	 * @param imageUri
	 *            拍照后照片存储路径
	 * @param requestCode
	 *            调用系统相机请求码
	 */
	public void takePicture() {
		boolean flag = Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED);
		if (!flag) {
			Toast.makeText(mContext, "请插入手机存储卡再使用本功能", Toast.LENGTH_LONG)
					.show();

		} else {
			// 调用系统相机
			Intent intentCamera = new Intent();
			intentCamera.setAction(MediaStore.ACTION_IMAGE_CAPTURE);
			// 将拍照结果保存至photo_file的Uri中，不保留在相册中
			fileUri = AppUtile.createImgFile(mContext);
			cropImageUri = Uri.fromFile(fileUri);
			intentCamera.putExtra(MediaStore.EXTRA_OUTPUT, cropImageUri);
			((Activity) mContext).startActivityForResult(intentCamera,
					REQUEST_CODE_PICK_IMAGE);
		}
		
	}

	/**
	 * @param activity
	 *            当前activity
	 * @param requestCode
	 *            打开相册的请求码
	 */
	public void openPic() {
		boolean flag = Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED);
		if (!flag) {
			Toast.makeText(mContext, "请插入手机存储卡再使用本功能", Toast.LENGTH_LONG)
					.show();

		} else {
			Intent photoPickerIntent = new Intent(Intent.ACTION_GET_CONTENT);
			photoPickerIntent.setType("image/*");
			((Activity) mContext).startActivityForResult(photoPickerIntent,
					FILECHOOSER_RESULTCODE);
		}
		
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data,PhotoPathListener photoPathListener) {
		if (requestCode == REQUEST_CODE_PICK_IMAGE) {
			// 相机
			if (resultCode == Activity.RESULT_OK) {
				String filepath = PhotoBitmapUtils.amendRotatePhoto(
						AppUtile.getImageUri(), mContext);
				photoPathListener.imgPhotoPath(filepath);
			} else {
				Toast.makeText(mContext, "取消操作", Toast.LENGTH_SHORT).show();
			}

		} else if (requestCode == FILECHOOSER_RESULTCODE) {
			// 相册
			if (resultCode == Activity.RESULT_OK) {
				if (hasSdcard()) {

					String imgFile = RxPhotoTool.getImageAbsolutePath(mContext,
							data.getData());
					String filepath = PhotoBitmapUtils.amendRotatePhoto(
							imgFile, mContext);
					photoPathListener.imgPhotoPath(filepath);
				}
			} else {
				Toast.makeText(mContext, "取消操作", Toast.LENGTH_SHORT).show();
			}
		}

	}

	public PhotoPathListener mPhotoPathListener;

	public void setPhotoPathListener(PhotoPathListener mPhotoPathListener) {
		this.mPhotoPathListener = mPhotoPathListener;
	}

	public interface PhotoPathListener {

		void imgPhotoPath(String imgPath);

	}

	/**
	 * 检查设备是否存在SDCard的工具方法
	 */
	public boolean hasSdcard() {
		String state = Environment.getExternalStorageState();
		return state.equals(Environment.MEDIA_MOUNTED);
	}
}
