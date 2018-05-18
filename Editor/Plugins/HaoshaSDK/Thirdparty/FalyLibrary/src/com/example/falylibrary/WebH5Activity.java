package com.example.falylibrary;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.lang.ref.WeakReference;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Map;

import com.alipay.sdk.app.EnvUtils;
import com.alipay.sdk.app.PayTask;
import com.example.falylibrary.utile.AppUtile;
import com.example.falylibrary.utile.PhotoBitmapUtils;
import com.example.falylibrary.utile.PhotoUtils;
import com.example.falylibrary.utile.PhotoUtils.PhotoPathListener;
import com.example.falylibrary.utile.RxPhotoTool;


import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.provider.MediaStore;
import android.text.TextUtils;
import android.util.Base64;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;

import android.webkit.CookieManager;
import android.webkit.CookieSyncManager;
import android.webkit.JavascriptInterface;
import android.webkit.ValueCallback;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebSettings.LayoutAlgorithm;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

/**
 * Created by min on 2017/12/6.
 */

@SuppressWarnings("unused")
@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
@SuppressLint({ "SetJavaScriptEnabled", "SimpleDateFormat" })
public class WebH5Activity extends Activity{
	private ImageView imgBack;
	private ImageView imgClose;
	private TextView tvTitle;
	private WebView webView;
	private Context context;
	private static final String APP_CACAHE_DIRNAME = "/data/data/package_name/cache/webviewCache";
	private String token = "";
	public static final int SDK_PAY_FLAG_ZHIFU = 1;
	private RelativeLayout rl;
	MyHandler myHandler=null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.go_h5_layout);
		context = this;

		imgBack = (ImageView) findViewById(R.id.imgBack);
		imgClose = (ImageView) findViewById(R.id.imgClose);
		tvTitle = (TextView) findViewById(R.id.tvTitle);
		webView = (WebView) findViewById(R.id.webView);
		rl = (RelativeLayout) findViewById(R.id.rl);
		String url = getIntent().getStringExtra("url");
		token = getIntent().getStringExtra("token");
		if (url != null && !url.equals("")) {
			initWebViews(url, token);
		}

		imgBack.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if (webView != null) {
					if (webView.canGoBack()) {
						webView.goBack();
					} else {
						finish();
					}
				}

			}
		});
		imgClose.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				finish();
			}
		});
		myHandler=new MyHandler(WebH5Activity.this);

	}

	@SuppressWarnings("deprecation")
	@SuppressLint("NewApi")
	private void initWebViews(String url, final String token) {

		webView.loadUrl(url);
		webView.setWebViewClient(new WebViewClient() {

			@Override
			public void onPageStarted(WebView view, String url, Bitmap favicon) {
				// TODO Auto-generated method stub
				super.onPageStarted(view, url, favicon);
				

			}

			@Override
			public boolean shouldOverrideUrlLoading(final WebView view,
					final String scheme) {
				// 判断url链接中是否含有某个字段，如果有就执行指定的跳转（不执行跳转url链接），如果没有就加载url链接
				String url = AppUtile.spiltURL(scheme);
				if (url.contains("native")) {
					String[] spilte = url.split("-", 3);
					if (spilte[1].equals("cancel")) {
						finish();
					}
				} else {

					if (scheme.contains("tel:")) {
						String[] stel = scheme.split("tel:");
						call(context, stel[1]);
						return true;

					} else if (scheme.contains("img")) {
						selectImage();
						return true;
					} else if (scheme.startsWith("alipays:")
							|| scheme.startsWith("alipay")) {
						try {

							context.startActivity(new Intent(
									"android.intent.action.VIEW", Uri
											.parse(scheme)));

						} catch (Exception e) {
							new AlertDialog.Builder(context)
									.setMessage("未检测到支付宝客户端，请安装后重试。")
									.setPositiveButton(
											"立即安装",
											new DialogInterface.OnClickListener() {

												@Override
												public void onClick(
														DialogInterface dialog,
														int which) {
													Uri alipayUrl = Uri
															.parse("https://d.alipay.com");
													context.startActivity(new Intent(
															"android.intent.action.VIEW",
															alipayUrl));

												}
											}).setNegativeButton("取消", null)
									.show();
						}

					}else {
						view.loadUrl(scheme);

					}

				}
				return true;

			}

			@Override
			public void onPageFinished(WebView view, String url) {

				// 获取H5-title
				String title = view.getTitle();
				if (title.contains("order/cashier-desk.jhtml?")) {
					tvTitle.setText("支付宝支付");
				} else {
					tvTitle.setText(title);
				}
			}

			@Override
			public void onReceivedError(WebView view, int errorCode,
					String description, String failingUrl) {
				// TODO Auto-generated method stub
				super.onReceivedError(view, errorCode, description, failingUrl);
				rl.removeAllViews();
			}

		});
		// 声明WebSettings子类
		WebSettings webSettings = webView.getSettings();
		webSettings.setJavaScriptEnabled(true);

		// 设置缓存模式
		webView.getSettings().setCacheMode(WebSettings.LOAD_DEFAULT);
		String cacheDirPath = APP_CACAHE_DIRNAME;
		// 设置数据库缓存路径
		webView.getSettings().setDatabasePath(cacheDirPath);
		// 设置 应用 缓存目录
		webView.getSettings().setAppCachePath(cacheDirPath);
		// 开启 DOM 存储功能
		webView.getSettings().setDomStorageEnabled(true);
		// 开启 数据库 存储功能
		webView.getSettings().setDatabaseEnabled(true);
		
		webView.getSettings().setTextZoom(100);
		// 开启 应用缓存 功能
		webView.getSettings().setAppCacheEnabled(true);
		webView.addJavascriptInterface(new NativePay(context), "NativePay");
		webSettings.setLayoutAlgorithm(LayoutAlgorithm.NORMAL);
		webSettings.setUseWideViewPort(true);
		webSettings.setLoadWithOverviewMode(true);
		webSettings.setDomStorageEnabled(true);

	}

	public class NativePay {
		Context context;

		public NativePay(Context context) {
			this.context = context;
		}

		// 该方法将暴露给js调用,打来商品详情页
		@JavascriptInterface
		public void goPay(String payValue) {
			goNativePay((WebH5Activity) context, payValue);

		}
	}

	@Override
	protected void onDestroy() {

		super.onDestroy();

		CookieManager cookieManager = CookieManager.getInstance(); // the
		cookieManager.removeAllCookie();// Removes all cookies.
		CookieSyncManager.getInstance().sync(); // forces sync manager to sync
												// now
		
		if (webView != null) {
			rl.removeView(webView);
			webView.setWebChromeClient(null);
			webView.setWebViewClient(null);
			webView.getSettings().setJavaScriptEnabled(false);
			webView.clearCache(true);
			webView.destroy();
		}

	}

	private final void selectImage() {
		boolean flag = Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED);
		if (!flag) {
			Toast.makeText(this, "请插入手机存储卡再使用本功能", Toast.LENGTH_SHORT).show();
		} else {
			AlertDialog.Builder alertDialog = new AlertDialog.Builder(this);
			alertDialog.setTitle(R.string.options);
			alertDialog.setItems(R.array.options,
					new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							if (which == 0) {
								chosePic();
							} else {
								openCarcme();
							}
						}
					});
			alertDialog.show();

		}
	}

	protected void chosePic() {
		PhotoUtils.getPhotoUtils(context).openPic();
	}

	/**
	 * 打开照相机
	 */
	private void openCarcme() {
		PhotoUtils.getPhotoUtils(context).takePicture();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode,
			Intent intent) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, intent);
		if (requestCode == PhotoUtils.REQUEST_CODE_PICK_IMAGE) {
			// 相机
			if (resultCode == Activity.RESULT_OK) {
				new MyThree(AppUtile.getImageUri(),WebH5Activity.this).start();
			
			} else {
			
			}

		} else if (requestCode == PhotoUtils.FILECHOOSER_RESULTCODE) {
			// 相册
			if (resultCode == Activity.RESULT_OK) {
				if (PhotoUtils.getPhotoUtils(WebH5Activity.this).hasSdcard()) {

					String imgFile = RxPhotoTool.getImageAbsolutePath(WebH5Activity.this,
							intent.getData());
				
					new MyThree(imgFile,WebH5Activity.this).start();
					
				}
			} else {
				
			}
		}
	}
	private class MyThree extends Thread{
		private String imgFile;
		private Context context;
		@SuppressWarnings("unused")
		public MyThree(String imgFile,Context context){
			this.imgFile=imgFile;
			this.context=context;
		}
		@Override
		public void run() {
			// TODO Auto-generated method stub
			super.run();
			String imgUrl= PhotoBitmapUtils.amendRotatePhoto(imgFile, context);
			Message msg=new Message();
			msg.obj=imgUrl;
			myHandler.sendMessage(msg);
		}
		
	}
	
	private class MyHandler extends Handler{
		WeakReference<Activity>mWeakReference;
		public MyHandler(Activity activity){
			this.mWeakReference=new WeakReference<Activity>(activity);
		}
		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			if (mWeakReference.get()!=null) {
				
				if (msg.obj!=null) {
					String imgPath=(String) msg.obj;
					if (imgPath!=null&&!imgPath.equals("")) {
						String title = "";
						if (imgPath.contains(".png")) {
							title = "data:image/png;base64,";
						} else if (imgPath.contains(".icon")) {
							title = "data:image/icon;base64,";
						} else if (imgPath.contains(".gif")) {
							title = "data:image/gif;base64,";
						} else if (imgPath.contains(".jpg")) {
							title = "data:image/jpeg;base64,";
						}
						Bitmap mpBitmap = AppUtile.getSmallBitmap(imgPath);
						// Bitmap bitmap2 = AppUtile.ratio(mpBitmap, 1080,
						// 1920);
						String base64ImageUrl = AppUtile.bitmapTo(mpBitmap);
						if (base64ImageUrl != null
								&& !base64ImageUrl.equals("")) {
							if (webView != null) {
								webView.loadUrl("javascript:setPhoto('" + title
										+ base64ImageUrl + "')");
							}
						} else {
							Toast.makeText(context, "图片格式错误",
									Toast.LENGTH_SHORT).show();
						}
					}else{
						Toast.makeText(context, "图片资源获取失败",
								Toast.LENGTH_SHORT).show();
					}
				}
			}
		}
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			if (webView != null) {
				if (webView.canGoBack()) {
					webView.goBack();
				} else {
					finish();
				}
			}
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}

	/**
	 * 调用拨号界面
	 * 
	 * @param phone
	 *            电话号码
	 */
	public static void call(Context context, String phone) {

		if (phone != null) {
			// 拨号：激活系统的拨号组件
			Intent intent = new Intent(); // 意图对象：动作 + 数据
			intent.setAction(Intent.ACTION_CALL); // 设置动作
			Uri data = Uri.parse("tel:" + phone); // 设置数据
			intent.setData(data);
			context.startActivity(intent); // 激活Activity组件
		} else {
			Toast.makeText(context, "电话号为空", Toast.LENGTH_LONG).show();
		}

	}
	
	/**
	 * 走原生的支付
	 * 
	 * @param activity
	 * @param payValue
	 */
	private void goNativePay(final Activity activity, final String payValue) {

		try {
			Thread.sleep(300);
			// 支付宝
			Runnable payRunnable = new Runnable() {
				@Override
				public void run() {
					PayTask alipay = new PayTask(activity);
					Map<String, String> result = alipay.payV2(payValue, true);
					Message msg = new Message();
					msg.what = SDK_PAY_FLAG_ZHIFU;
					msg.obj = result;
					mHandler.sendMessage(msg);
				}
			};
			Thread payThread = new Thread(payRunnable);
			payThread.start();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

	}

	@SuppressLint("HandlerLeak")
	public Handler mHandler = new Handler() {
		@SuppressWarnings("unused")
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SDK_PAY_FLAG_ZHIFU: {
				PayResult payResult = new PayResult(
						(Map<String, String>) msg.obj);
				/**
				 * 对于支付结果，请商户依赖服务端的异步通知结果。同步通知结果，仅作为支付结束的通知。
				 */
				String resultInfo = payResult.getResult();// 同步返回需要验证的信息
				String resultStatus = payResult.getResultStatus();
				// 判断resultStatus 为9000则代表支付成功
				if (TextUtils.equals(resultStatus, "9000")) {
					// 该笔订单是否真实支付成功，需要依赖服务端的异步通知。

					try {
						Thread.sleep(200);
						if (webView != null) {

							webView.loadUrl("javascript:getData('"
									+ resultStatus + "')");
						}
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				} else {
					if (webView != null) {
						webView.loadUrl("javascript:getData('" + resultStatus
								+ "')");
					}
				}
				break;

			}
			default:
				break;
			}
		}

		;
	};


}
