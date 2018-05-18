package com.example.falylibrary.db;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.io.Writer;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringEscapeUtils;

import com.example.falylibrary.Constant;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

public class CrashHandler implements Thread.UncaughtExceptionHandler {
	
	   public static final String TAG = "CrashHandler";
        //系统默认的UncaughtException处理类
	    private Thread.UncaughtExceptionHandler mDefaultHandler;
	    //CrashHandler实例
	    private static CrashHandler INSTANCE = new CrashHandler();
	    //程序的Context对象
	    private Context mContext;
	    /*数据库变量*/
	    private RecordSQLiteOpenHelper helper ;
	    private SQLiteDatabase db;
	    //用来存储设备信息和异常信息
	    private Map<String, String> infos = new HashMap<String, String>();
	    private  List<String> mListInfo=new ArrayList<>();

	    //用于格式化日期,作为日志文件名的一部分
	    private DateFormat formatter = new SimpleDateFormat("yyyy-MM-dd-HH-mm-ss");

	    /** 保证只有一个CrashHandler实例 */
	    private CrashHandler() {
	    }

	    /** 获取CrashHandler实例 ,单例模式 */
	    public static CrashHandler getInstance() {
	        return INSTANCE;
	    }
	    /**
	     * 初始化
	     *
	     * @param context
	     */
	    public void init(Context context) {
	        this.mContext = context;
	        //获取系统默认的UncaughtException处理器
	        mDefaultHandler = Thread.getDefaultUncaughtExceptionHandler();
	        //实例化数据库SQLiteOpenHelper子类对象
	        helper = new RecordSQLiteOpenHelper(context);
	        //设置该CrashHandler为程序的默认处理器
	        Thread.setDefaultUncaughtExceptionHandler(this);
            Log.e("zhoujia11111111","22222222222222222");
	    }

	   /**
     * 当UncaughtException发生时会转入该函数来处理
     */
    @Override
    public void uncaughtException(Thread thread, Throwable ex) {

        if (!handleException(ex) && mDefaultHandler != null) {
            //如果用户没有处理则让系统默认的异常处理器来处理
            mDefaultHandler.uncaughtException(thread, ex);
        } else {
            try {
                Thread.sleep(3000);
               
            } catch (InterruptedException e) {
                Log.e(TAG, "error : ", e);
            }
            System.exit(0);
            mDefaultHandler.uncaughtException(thread, ex);
        }
    }
    /**
     * 自定义错误处理,收集错误信息 发送错误报告等操作均在此完成.
     *
     * @param ex
     * @return true:如果处理了该异常信息;否则返回false.
     */
    private boolean handleException(Throwable ex) {
          Log.e("zhoujia2222222222","22222222222222222");
        if (ex == null) {
            return false;
        }
          Log.e("zhoujia33333333333","22222222222222222");
        //保存日志文件
        saveCrashInfo2File(ex);
        return true;
    }

    /**
     * 保存错误信息到文件中
     *
     * @param ex
     * @return  返回文件名称,便于将文件传送到服务器
     */
    private String saveCrashInfo2File(Throwable ex) {

        StringBuffer sb = new StringBuffer();
        Writer writer = new StringWriter();
        PrintWriter printWriter = new PrintWriter(writer);
        ex.printStackTrace(printWriter);
        Throwable cause = ex.getCause();
        while (cause != null) 
        {
            cause.printStackTrace(printWriter);
            cause = cause.getCause();
        }
        printWriter.close();
        String result = writer.toString();
        sb.append(result);
         Log.e("zhoujia33333333333",result);
        setErrorInfo(mContext,result);
        try {
            long timestamp = System.currentTimeMillis();
            String time = formatter.format(new Date());
            String fileName = "crash-" + time + "-" + timestamp + ".log";
            if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
                String path = "/sdcard/FAFAcrash/";
                File dir = new File(path);
                if (!dir.exists()) {
                    dir.mkdirs();
                }
                FileOutputStream fos = new FileOutputStream(path + fileName);
                fos.write(sb.toString().getBytes());
                sendCrashLog2PM(path + fileName);
                fos.close();
            }
            return fileName;
        } catch (Exception e) {
            Log.e(TAG, "an error occured while writing file...", e);
        }
        return null;
    }
    /**
     * 将捕获的导致崩溃的错误信息发送给开发人员
     *
     * 目前只将log日志保存在sdcard 和输出到LogCat中，并未发送给后台。
     */
    private void sendCrashLog2PM(String fileName){
        if(!new File(fileName).exists()){
            Toast.makeText(mContext, "日志文件不存在！", Toast.LENGTH_SHORT).show();
            return;
        }
        FileInputStream fis = null;
        BufferedReader reader = null;
        String s = null;
        try {
            fis = new FileInputStream(fileName);
            reader = new BufferedReader(new InputStreamReader(fis, "GBK"));
            while(true){
                s = reader.readLine();
                if(s == null) break;
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally{   // 关闭流
            try {
                reader.close();
                fis.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * 组装错误信息
     * @param context
     * @param exceptionInfo
     */
    private void setErrorInfo(Context context,String exceptionInfo){
        String textInfo=StringEscapeUtils.escapeSql(exceptionInfo);
        CrashDB um=new CrashDB();
        um.setAppVersion(Constant.AppVersion);
        um.setAppType("FAFA_SOFT_APP");
        um.setSource("FAFA_SOFT_APP_ANDROID");
        um.setLevel("ERROR");
        um.setExceptionInfo(textInfo);
        um.setOccurTime(SystemUtil.getTimeDate());
        um.setAccount(Constant.AppAccount);//用户id
        um.setOs("ANDROID");
        um.setModel(android.os.Build.MODEL);
        um.setVersion(android.os.Build.VERSION.RELEASE + "_" + android.os.Build.VERSION.SDK_INT);
        um.setDeviceCode(SystemUtil.getDeviceId(mContext));
        um.setApiVersion(Constant.ApiVersion);
        um.setCpuType("");
        um.setMemory(SystemUtil.getTotalMemory(context));
        um.setMemoryFree(SystemUtil.getAvailMemory(context));
        um.setStorage(SystemUtil.getTotalInternalMemorySize(context)+ "_" +SystemUtil.getTotalExternalMemorySize(context));
        um.setStorageFree(SystemUtil.getAvailableInternalMemorySize(context)+ "_" +SystemUtil.getAvailableExternalMemorySize(context));
        insertData(CrashDB.getJsonString(um));
    }
  
    /*插入数据*/
    private void insertData(String tempName) {
    	try {
    		    db = helper.getWritableDatabase();
    	        db.execSQL("replace into fafadesginfo(name) values('" + tempName + "')");
    	        db.close();
		} catch (Exception e) {
			// TODO: handle exception
			e.getMessage();
		}
       
    }
 

}
