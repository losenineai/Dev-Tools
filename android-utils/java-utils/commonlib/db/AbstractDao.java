package com.auric.intell.commonlib.db;

import android.util.Log;

import com.auric.intell.commonlib.utils.LogTool;
import com.j256.ormlite.android.apptools.OrmLiteSqliteOpenHelper;
import com.j256.ormlite.dao.Dao;
import com.j256.ormlite.stmt.QueryBuilder;
import com.j256.ormlite.support.ConnectionSource;
import com.j256.ormlite.support.DatabaseConnection;

import java.lang.reflect.ParameterizedType;
import java.sql.SQLException;
import java.sql.Savepoint;
import java.util.Collection;
import java.util.List;

/***
 * 
 * 基础sqlite DAO   
 * Created By Zhangxiliang
 * Date：2014年11月18日 
 * Version： 2.0   
 * Copyright (c) 2014 56.com Software corporation All Rights Reserved.     
 *
 */ 
public abstract class AbstractDao<T> {
	protected OrmLiteSqliteOpenHelper helper;
    protected Dao<T,Integer> baseDao;
	private static final String TAG="BaseDao";
	public AbstractDao() {
		Log.e(TAG,"in BaseDao ");

		helper = getOrmLiteDBHelper();
		Class< T > entityClass  =  (Class< T >) ((ParameterizedType) getClass().getGenericSuperclass()).getActualTypeArguments()[ 0 ];
		try {
			baseDao=helper.getDao(entityClass);
		} catch (SQLException e) {
			Log.e(TAG, Log.getStackTraceString(e));
		}
	}

	public abstract OrmLiteSqliteOpenHelper getOrmLiteDBHelper();

	public  void add(T entity) {
		try {
			baseDao.createOrUpdate(entity);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			Log.e(TAG, Log.getStackTraceString(e));
		}
	}


	
	public void delete(T entiry){
		try {
			baseDao.delete(entiry);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			Log.e(TAG, Log.getStackTraceString(e));
		}
	}
	
	public void update(T entity){
		try {
			baseDao.update(entity);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			Log.e(TAG, Log.getStackTraceString(e));
		}
	}


	public void patchAdd(List<T> entityList){
		try {
			ConnectionSource connectionSource=helper.getConnectionSource();
			DatabaseConnection connection=connectionSource.getReadWriteConnection();
			Savepoint savepoint=connection.setSavePoint(null);
			for (T entity:entityList ){
				baseDao.createOrUpdate(entity);
			}
			connection.commit(savepoint);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			Log.e(TAG, Log.getStackTraceString(e));
		}
	}


	public void patchDelete(List<T> entityList){
		try {
			ConnectionSource connectionSource=helper.getConnectionSource();
			DatabaseConnection connection=connectionSource.getReadWriteConnection();
			Savepoint savepoint=connection.setSavePoint(null);
			for (T entity:entityList ){
				baseDao.delete(entity);
			}
			connection.commit(savepoint);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			Log.e(TAG, Log.getStackTraceString(e));
		}
	}



	public void addList(Collection<T> collection) {
		if (collection == null && collection.size() == 0) {
			return;
		}
		
		try {
			for (T entity : collection) {
				baseDao.create(entity);
			}
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			Log.e("BaseDao", Log.getStackTraceString(e));
			e.printStackTrace();
		}
	}
	
	
	public List<T> queryAll(){
		List<T> list=null;
		try {
			list=baseDao.queryForAll();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return list;
	}

    /**
     * 查询数据
     * @return
     */
    public long queryCount() {
        LogTool.d(TAG, "queryCount");
        try {
            return baseDao.countOf(baseDao.queryBuilder().setCountOf(true).prepare());
        } catch (SQLException e) {
            LogTool.e(TAG, Log.getStackTraceString(e));
        }
        return 0L;
    }

    /**
     * 查询数据
     * @return
     */
    public boolean deleteAll() {
        LogTool.d(TAG, "deleteAll");
        try {
            int result = baseDao.deleteBuilder().delete();
            LogTool.d(TAG, "deleteAll result"+result);
            return result > 0;
        } catch (SQLException e) {
            LogTool.e(TAG, Log.getStackTraceString(e));
        }
        return false;
    }


    /**
     * 获取QueryBuilder，主要用于连表操作
     * @return
     */
    public QueryBuilder<T, Integer> getQueryBuilder() {
        return baseDao.queryBuilder();
    }


}
