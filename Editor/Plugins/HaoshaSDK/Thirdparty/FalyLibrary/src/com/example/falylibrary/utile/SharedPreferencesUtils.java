package com.example.falylibrary.utile;


import android.content.Context;
import android.content.SharedPreferences;
import android.util.Base64;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

/**
 * sharepreference 存储工具类
 */
public class SharedPreferencesUtils {
	/**
	 * 保存在手机里面的名字
	 */
	public static final String FILE_NAME = "shared_data_info";
	/**
	 * 保存数据的方法，拿到数据保存数据的基本类型，然后根据类型调用不同的保存方法
	 * @param key
	 * @param object
	 */
	public static void saveData(Context context,String key,String object ){
		SharedPreferences sharedPreferences= context.getSharedPreferences(FILE_NAME, Context.MODE_PRIVATE);
		SharedPreferences.Editor editor= sharedPreferences.edit();
        editor.putString(key,object);
	    editor.commit();
	}

	/**
	 * 从文件中读取数据
	 * @param key
	 * @param defaultObject
	 */
	public static String getData(Context context,String key){
		SharedPreferences sharedPreferences= context.getSharedPreferences(FILE_NAME, Context.MODE_PRIVATE);
		SharedPreferences.Editor editor= sharedPreferences.edit();
        return sharedPreferences.getString(key,"");
		

	}

	/**
	 * 清除所有的数据
	 */
	public static void clear(Context context) {
		SharedPreferences sharedPreferences= context.getSharedPreferences(FILE_NAME, Context.MODE_PRIVATE);
		SharedPreferences.Editor editor= sharedPreferences.edit();
		editor.clear().commit();
	}
}

