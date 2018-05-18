package com.example.falylibrary.utile;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.sqlite.SQLiteDatabase;

import android.net.Uri;

import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.widget.Toast;

import com.alibaba.fastjson.JSONObject;
import com.example.falylibrary.R;
import com.example.falylibrary.WebH5Activity;
import com.example.falylibrary.db.CrashDB;
import com.example.falylibrary.db.DbUtile;
import com.example.falylibrary.db.RecordSQLiteOpenHelper;
import com.mob.MobSDK;

import java.io.File;
import java.util.HashMap;
import java.util.List;

import cn.sharesdk.framework.Platform;
import cn.sharesdk.framework.PlatformActionListener;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.wechat.friends.Wechat;
import cn.sharesdk.wechat.moments.WechatMoments;

/**
 * Created by min on 2017/12/8.
 */

public class AppLibrary {
	public static AppLibrary mAppLibrary = new AppLibrary();
	private static Context mContext;
	/* 数据库变量 */
	private static RecordSQLiteOpenHelper helper;
	private static SQLiteDatabase db;

	private AppLibrary() {
	};

	public static AppLibrary getInstence(Context context) {
		mContext = context;
		helper = new RecordSQLiteOpenHelper(context);
		return mAppLibrary;
	}

	public void initLibrary(String key, String value) {

		MobSDK.init(mContext, key, value);
	}

	public void goWeb(String url) {
		Intent intent = new Intent(mContext, WebH5Activity.class);
		intent.putExtra("url", url);
		mContext.startActivity(intent);
	}

	/**
	 * 好友分享
	 * 
	 * @param strTitle
	 *            分享标题
	 * @param strText
	 *            分享文本
	 * @param imgUrl
	 *            网络图片rul
	 * @param strUrl
	 *            网友点进链接后，可以看到分享的详情
	 */
	public void weChatFavorite(String strTitle, String strText, String imgUrl,
			String strUrl) {
		if (!AppUtile.isWeixinAvilible(mContext)) {
			DialogUtile.showDialog(mContext);
		} else {

			Platform.ShareParams sp = new Platform.ShareParams();
			sp.setShareType(Platform.SHARE_WEBPAGE);// 非常重要：一定要设置分享属性
			sp.setTitle(strTitle); // 分享标题
			sp.setText(strText); // 分享文本
			sp.setImageUrl(imgUrl);// 网络图片rul
			sp.setUrl(strUrl); // 网友点进链接后，可以看到分享的详情

			// 3、非常重要：获取平台对象
			Platform wechat = ShareSDK.getPlatform(Wechat.NAME);
			wechat.setPlatformActionListener(new PlatformActionListener() {
				@Override
				public void onComplete(Platform platform, int i,
						HashMap<String, Object> hashMap) {
					Log.e("ShareSDK", "onComplete ---->  分享成功");
					platform.isClientValid();
					platform.getName();
					Toast.makeText(mContext, "分享成功", Toast.LENGTH_LONG).show();
				}

				@Override
				public void onError(Platform platform, int i,
						Throwable throwable) {
					Log.e("ShareSDK", "onError ---->  分享失败"
							+ throwable.getStackTrace().toString());
					Log.e("ShareSDK",
							"onError ---->  分享失败" + throwable.getMessage());
					Toast.makeText(mContext, "分享失败", Toast.LENGTH_LONG).show();
					throwable.getMessage();
					throwable.printStackTrace();
				}

				@Override
				public void onCancel(Platform platform, int i) {
					Log.e("ShareSDK", "onCancel ---->  分享取消");
					Toast.makeText(mContext, "分享取消", Toast.LENGTH_LONG).show();
				}
			});
			wechat.share(sp);
		}

	}

	/**
	 * 朋友圈分享
	 * 
	 * @param strTitle
	 *            分享标题
	 * @param strText
	 *            分享文本
	 * @param imgUrl
	 *            网络图片rul
	 * @param strUrl
	 *            网友点进链接后，可以看到分享的详情
	 */
	public void weChatMoments(String strTitle, String strText, String imgUrl,
			String strUrl) {
		if (!AppUtile.isWeixinAvilible(mContext)) {
			DialogUtile.showDialog(mContext);
		} else {
			Platform.ShareParams sp = new Platform.ShareParams();
			sp.setShareType(Platform.SHARE_WEBPAGE); // 非常重要：一定要设置分享属性
			sp.setTitle(strTitle); // 分享标题
			sp.setText(strText); // 分享文本
			sp.setImageUrl(imgUrl);// 网络图片rul
			sp.setUrl(strUrl); // 网友点进链接后，可以看到分享的详情

			// 3、非常重要：获取平台对象
			Platform wechatMoments = ShareSDK.getPlatform(WechatMoments.NAME);
			wechatMoments
					.setPlatformActionListener(new PlatformActionListener() {
						@Override
						public void onComplete(Platform platform, int i,
								HashMap<String, Object> hashMap) {
							Log.e("ShareSDK", "onComplete ---->  分享成功");
							platform.isClientValid();
							platform.getName();
							Toast.makeText(mContext, "分享成功", Toast.LENGTH_LONG)
									.show();
						}

						@Override
						public void onError(Platform platform, int i,
								Throwable throwable) {
							Log.e("ShareSDK", "onError ---->  分享失败"
									+ throwable.getStackTrace().toString());
							Log.e("ShareSDK",
									"onError ---->  分享失败"
											+ throwable.getMessage());
							Toast.makeText(mContext, "分享失败", Toast.LENGTH_LONG)
									.show();
							throwable.getMessage();
							throwable.printStackTrace();
						}

						@Override
						public void onCancel(Platform platform, int i) {
							Log.e("ShareSDK", "onCancel ---->  分享取消");
							Toast.makeText(mContext, "分享取消", Toast.LENGTH_LONG)
									.show();
						}
					});
			wechatMoments.share(sp);
		}
	}

	public void openIntentUri(String uri) {

		Intent intent = new Intent();
		intent.setAction("android.intent.action.VIEW");
		Uri content_url = Uri.parse(uri);
		intent.setData(content_url);
		mContext.startActivity(intent);
	}

	/**
	 * 获取报错信息
	 * 
	 * @param context
	 * @return
	 */
	public String getErrorInfoDesgin() {
		RecordSQLiteOpenHelper helper=new RecordSQLiteOpenHelper(mContext);
		
		List<JSONObject> mCrashDB = DbUtile.selectInfo(helper);
		if (mCrashDB != null && mCrashDB.size() > 0) {
			return mCrashDB.toString();
		} else {
			return "";
		}
	}

	/**
	 * 清除报错信息
	 * 
	 * @param context
	 * @return
	 */
	public void cleanErrorInfoDesgin() {
		DbUtile.delete(helper, db);
	}

}
