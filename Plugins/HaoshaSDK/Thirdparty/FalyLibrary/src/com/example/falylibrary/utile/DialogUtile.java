package com.example.falylibrary.utile;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;

public class DialogUtile {
	
	public static void showDialog(Context context){
		new AlertDialog.Builder(context)
		.setTitle("温馨提示")
		.setMessage("目前您的微信版本过低或未安装微信，需要安装微信才能使用。")
		.setNegativeButton("确定",new OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				// TODO Auto-generated method stub
				dialog.dismiss();
			}
		}).create()
		.show();
	}

}
