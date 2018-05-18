package com.example.falylibrary.db;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;
import android.widget.Toast;

/**
 * Created by Carson_Ho on 16/11/15.
 */
//SQLiteOpenHelper子类用于打开数据库并进行对用户搜索历史记录进行增删减除的操作
public class RecordSQLiteOpenHelper extends SQLiteOpenHelper {

    private static String name = "fafadesgin.db";
    private static Integer version = 1;
    Context context;
    public RecordSQLiteOpenHelper(Context context) {
    	
        super(context, name, null, version);
        this.context=context;
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        
        //打开数据库，建立了一个叫errorInfo的表，来存储错误信息记录：
        db.execSQL("create table fafadesginfo(id integer primary key autoincrement,name varchar(1000))");
        
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {

    }


}
