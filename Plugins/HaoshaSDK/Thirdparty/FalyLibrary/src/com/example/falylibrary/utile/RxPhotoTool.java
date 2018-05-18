package com.example.falylibrary.utile;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.ContentUris;
import android.content.Context;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.MediaStore;

import static com.example.falylibrary.utile.RxFileTool.isDownloadsDocument;
import static com.example.falylibrary.utile.RxFileTool.getDataColumn;
import static com.example.falylibrary.utile.RxFileTool.isExternalStorageDocument;
import static com.example.falylibrary.utile.RxFileTool.isGooglePhotosUri;
import static com.example.falylibrary.utile.RxFileTool.isMediaDocument;


import java.io.File;


/**
 * Created by min on 2017/11/22.
 *
 *  <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"></uses-permission>
 * <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"></uses-permission>
 */

public class RxPhotoTool {
  
    
    @TargetApi(Build.VERSION_CODES.KITKAT) 
    public static String getImageAbsolutePath(Context context, Uri imageUri) {
        if (context == null || imageUri == null)
            return null;
        String pathHead = "file:///";
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
     * @param context
     * @param uri
     * @return
     */
    public static File getFilePhotoFromUri(Activity context, Uri uri) {
        return new File(RxPhotoTool.getImageAbsolutePath(context, uri));
    }
}
