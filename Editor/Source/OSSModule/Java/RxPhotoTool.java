package com.oss.dachengshidai;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.ContentUris;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.MediaStore;

import com.drew.imaging.ImageMetadataReader;
import com.drew.imaging.ImageProcessingException;
import com.drew.metadata.Directory;
import com.drew.metadata.Metadata;
import com.drew.metadata.exif.ExifIFD0Directory;

import java.io.File;
import java.io.IOException;

import static com.oss.dachengshidai.RxFileTool.getDataColumn;
import static com.oss.dachengshidai.RxFileTool.isDownloadsDocument;
import static com.oss.dachengshidai.RxFileTool.isExternalStorageDocument;
import static com.oss.dachengshidai.RxFileTool.isGooglePhotosUri;
import static com.oss.dachengshidai.RxFileTool.isMediaDocument;

/**
 * Created by min on 2017/11/22.
 * 需要两个文件权限
 *  <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"></uses-permission>
 * <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"></uses-permission>
 */

public class RxPhotoTool {
    public static final int GET_IMAGE_FROM_PHONE = 5002;

    public static void openLocalImage(final Activity activity) {
        Intent intent = new Intent();
        intent.setType("image/*");
        intent.setAction(Intent.ACTION_GET_CONTENT);
        activity.startActivityForResult(intent, GET_IMAGE_FROM_PHONE);
    }
    /**
     * 根据Uri获取图片绝对路径，解决Android4.4以上版本Uri转换
     * @param context
     * @param imageUri
     */
    @TargetApi(19)
    public static String getImageAbsolutePath(Context context, Uri imageUri) {
        if (context == null || imageUri == null)
            return null;
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.KITKAT && DocumentsContract.isDocumentUri(context, imageUri)) {
            if (isExternalStorageDocument(imageUri)) {
                String docId = DocumentsContract.getDocumentId(imageUri);
                String[] split = docId.split(":");
                String type = split[0];
                if ("primary".equalsIgnoreCase(type)) {
                    return Environment.getExternalStorageDirectory() + "/" + split[1];
                }
            } else if (isDownloadsDocument(imageUri)) {
                String id = DocumentsContract.getDocumentId(imageUri);
                Uri contentUri = ContentUris.withAppendedId(Uri.parse("content://downloads/public_downloads"), Long.valueOf(id));
                return getDataColumn(context, contentUri, null, null);
            } else if (isMediaDocument(imageUri)) {
                String docId = DocumentsContract.getDocumentId(imageUri);
                String[] split = docId.split(":");
                String type = split[0];
                Uri contentUri = null;
                if ("image".equals(type)) {
                    contentUri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
                } else if ("video".equals(type)) {
                    contentUri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
                } else if ("audio".equals(type)) {
                    contentUri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                }
                String selection = MediaStore.Images.Media._ID + "=?";
                String[] selectionArgs = new String[]{split[1]};
                String string=getDataColumn(context, contentUri, selection, selectionArgs);
                if (!string.equals("")){
                    return string;
                }else{
                    return "";
                }
            }
        } // MediaStore (and general)
        else if ("content".equalsIgnoreCase(imageUri.getScheme())) {
            // Return the remote address
            if (isGooglePhotosUri(imageUri))
                return imageUri.getLastPathSegment();
            return getDataColumn(context, imageUri, null, null);
        }
        // File
        else if ("file".equalsIgnoreCase(imageUri.getScheme())) {
            return imageUri.getPath();
        }
        return null;
    }

    /**
     * 0-水平参数 1-垂直参数
     * @param source
     * @return
     */
    public static int[] getImageInfo(File source) {
        int [] ints=new int[2];
        try {
            Metadata metadata = ImageMetadataReader.readMetadata(source);
            for (Directory directory : metadata.getDirectories()) {
                if("ExifIFD0Directory".equalsIgnoreCase( directory.getClass().getSimpleName() )){
                    String strX=directory.getString(ExifIFD0Directory.TAG_X_RESOLUTION);
                    String strY=directory.getString(ExifIFD0Directory.TAG_Y_RESOLUTION);
                    if (strX==null){
                        ints[0]=300;
                    }else{
                        ints[0]=Integer.valueOf(strX);
                    }
                    if (strY==null){
                        ints[1]=300;

                    }else{
                        ints[1]=Integer.valueOf(strY);
                    }
                    return ints;
                }
            }
        } catch (ImageProcessingException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return new int[]{300,300};
    }
    /**
     * 将Uri转换成File
     * @param context
     * @param uri
     * @return
     */
    public static File getFilePhotoFromUri(Activity context, Uri uri) {
        return new File(RxPhotoTool.getImageAbsolutePath(context, uri));
    }
}
