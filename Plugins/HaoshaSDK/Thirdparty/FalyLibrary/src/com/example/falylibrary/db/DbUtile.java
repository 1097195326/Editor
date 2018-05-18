package com.example.falylibrary.db;

import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by min on 2017/8/30.
 */
public class DbUtile {
    /**
     * 删除本地库缓存的信息
     * @param helper
     * @param db
     */
    public static void delete(RecordSQLiteOpenHelper helper,SQLiteDatabase db){
        db = helper.getWritableDatabase();
        db.execSQL("delete from fafadesginfo");
        db.close();
    }

    /**
     * 查询本地表里全部信息
     * @param helper
     * @param db
     * @return
     */
    public static List<JSONObject> selectInfo(RecordSQLiteOpenHelper helper){
        List<JSONObject> mCrashDB = new ArrayList<>();
        Cursor cursor = helper.getReadableDatabase().rawQuery("select name from fafadesginfo", null);
        if (cursor != null) {
            while (cursor.moveToNext()) {
                if (!cursor.getString(cursor.getColumnIndexOrThrow("name")).equals("")) {
                	String str=cursor.getString(cursor.getColumnIndexOrThrow("name"));
                	 JSONObject jsonObject=JSON.parseObject(str);

                    mCrashDB.add(jsonObject);
                }
            }
        }
     return mCrashDB;
    }
}
