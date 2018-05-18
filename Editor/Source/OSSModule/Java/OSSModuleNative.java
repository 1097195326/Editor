package com.oss.dachengshidai;
public class OSSModuleNative {
    // 下载进度、失败、成功
    public static native void onDownloadProgress(int inFlag, long inCurrentSize, long inTotalSize);
    public static native void onDownloadFailed(int inFlag,int inErrorCode,String inReason);
    public static native void onDownloadSuccess(int inFlag);
    // 上传进度、失败、成功
    public static native void onUploadProgress(int inFlag, long inCurrentSize, long inTotalSize);
    public static native void onUploadFailed(int inFlag,int inErrorCode,String inReason);
    public static native void onUploadSuccess(int inFlag);

    // 下载失败
    public static native void onDirectoryCannotCreate(String inFileName);

	public static native void OSSLog(String tag, String msg);

	// 打开相册，获取上传的花型
	public static native void onOpenPattern(String inIconPath,String inPatternPath,int inDpiX,int inDpiY,int inResX, int inResY);
}
