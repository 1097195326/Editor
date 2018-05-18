package com.example.falylibrary.db;

import org.json.JSONObject;

import java.io.Serializable;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by min on 2017/8/28.
 */
public class CrashDB implements Serializable {
    private String source;          //来源
    private String level;           //级别
    private String exceptionInfo;   //异常信息
    private String occurTime;    //异常发生时间
    private String account;         //用户
    private String os;              //系统
    private String version;         //系统版本
    private String cpuType;         //cpu类型
    private String memory;          //内存总量
    private String storage;         //存储总量
    private String memoryFree;      //内存剩余量
    private String storageFree;     //存储剩余量
    private String model;           //设备型号
    private String deviceCode;      //设备编码 唯一标识
    private String appVersion;      //app版本
    private String apiVersion;      //api版本
    private String appType;         //app类型

    public String getSource() {
        return source;
    }

    public void setSource(String source) {
        this.source = source;
    }

    public String getAppType() {
        return appType;
    }

    public void setAppType(String appType) {
        this.appType = appType;
    }

    public String getApiVersion() {
        return apiVersion;
    }

    public void setApiVersion(String apiVersion) {
        this.apiVersion = apiVersion;
    }

    public String getAppVersion() {
        return appVersion;
    }

    public void setAppVersion(String appVersion) {
        this.appVersion = appVersion;
    }

    public String getDeviceCode() {
        return deviceCode;
    }

    public void setDeviceCode(String deviceCode) {
        this.deviceCode = deviceCode;
    }

    public String getModel() {
        return model;
    }

    public void setModel(String model) {
        this.model = model;
    }

    public String getMemoryFree() {
        return memoryFree;
    }

    public void setMemoryFree(String memoryFree) {
        this.memoryFree = memoryFree;
    }

    public String getStorageFree() {
        return storageFree;
    }

    public void setStorageFree(String storageFree) {
        this.storageFree = storageFree;
    }

    public String getStorage() {
        return storage;
    }

    public void setStorage(String storage) {
        this.storage = storage;
    }

    public String getMemory() {
        return memory;
    }

    public void setMemory(String memory) {
        this.memory = memory;
    }

    public String getCpuType() {
        return cpuType;
    }

    public void setCpuType(String cpuType) {
        this.cpuType = cpuType;
    }

    public String getVersion() {
        return version;
    }

    public void setVersion(String version) {
        this.version = version;
    }

    public String getOs() {
        return os;
    }

    public void setOs(String os) {
        this.os = os;
    }

    public String getAccount() {
        return account;
    }

    public void setAccount(String account) {
        this.account = account;
    }

    public String getOccurTime() {
        return occurTime;
    }

    public void setOccurTime(String occurTime) {
        this.occurTime = occurTime;
    }

    public String getExceptionInfo() {
        return exceptionInfo;
    }

    public void setExceptionInfo(String exceptionInfo) {
        this.exceptionInfo = exceptionInfo;
    }

    public String getLevel() {
        return level;
    }

    public void setLevel(String level) {
        this.level = level;
    }


    public static String getJsonString(CrashDB upShop) {
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("source", upShop.getSource());
        map.put("level", upShop.getLevel());
        map.put("exceptionInfo", upShop.getExceptionInfo());
        map.put("occurTime", upShop.getOccurTime());
        map.put("account", upShop.getAccount());
        map.put("os", upShop.getOs());
        map.put("version", upShop.getVersion());
        map.put("cpuType", upShop.getCpuType());
        map.put("memory", upShop.getMemory());
        map.put("storage", upShop.getStorage());
        map.put("memoryFree", upShop.getMemoryFree());
        map.put("storageFree", upShop.getStorageFree());
        map.put("model", upShop.getModel());
        map.put("deviceCode", upShop.getDeviceCode());
        map.put("appVersion", upShop.getAppVersion());
        map.put("apiVersion", upShop.getApiVersion());
        map.put("appType", upShop.getAppType());
        JSONObject jsonObject = new JSONObject(map);
        return jsonObject.toString();
    }
  
}
