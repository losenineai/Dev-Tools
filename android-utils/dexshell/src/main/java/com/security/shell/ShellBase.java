package com.security.shell;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Build;

import com.security.mobile.annotation.JXCStub;
import com.security.mobile.utils.CpuAbiTools;
import com.security.mobile.utils.DXLog;
import com.keep.DebugUtils;
import com.security.mobile.utils.RefInvoke;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import dalvik.system.DexFile;

public abstract class ShellBase {
    protected Context mCtx;
    protected String mPayLoadZipPath;
    protected String mExtractDir;
    protected String mReleaseDexsDir;
    protected String mOptDexDir;
    protected String mProcessAbi;

    protected int mDexCount;

    @JXCStub
    protected Object[] makeDexElements(ArrayList<File> files, ClassLoader loader, File optimizedDirectory) {
        // 4.0 - 4.3:   static Element[] makeDexElements(ArrayList<File> files, File optimizedDirectory)
        // 4.4 - 5.1:   static Element[] makeDexElements(ArrayList<File> files, File optimizedDirectory, ArrayList<IOException> suppressedExceptions)
        // 6.0      :   static Element[] makePathElements(List<File> files, File optimizedDirectory, List<IOException> suppressedExceptions)
        // 7.0 - 9.0:   static Element[] makeDexElements(List<File> files, File optimizedDirectory, List<IOException> suppressedExceptions, ClassLoader loader)

        Object result;
        int sdk_version = Build.VERSION.SDK_INT;

        String DexPathList_class = "dalvik.system.DexPathList";
        ArrayList<IOException> exceptions = new ArrayList<>();

        if( sdk_version >= Build.VERSION_CODES.ICE_CREAM_SANDWICH && sdk_version <= Build.VERSION_CODES.JELLY_BEAN_MR2){

            result = RefInvoke.invokeStaticMethod(DexPathList_class,
                    "makeDexElements",
                    new Class[] { ArrayList.class, File.class},
                    new Object[] { files, optimizedDirectory }
            );

        }else if(sdk_version >= Build.VERSION_CODES.KITKAT && sdk_version <= Build.VERSION_CODES.KITKAT_WATCH){

            result = RefInvoke.invokeStaticMethod(DexPathList_class,
                    "makeDexElements",
                    new Class[] { ArrayList.class, File.class, ArrayList.class},
                    new Object[] { files, optimizedDirectory, exceptions}
            );
        }else if(sdk_version >= Build.VERSION_CODES.LOLLIPOP && sdk_version <= Build.VERSION_CODES.M){

            result = makeDexElementsL2M(files);

        }else if(sdk_version >= Build.VERSION_CODES.N && sdk_version <= 27){

            result = RefInvoke.invokeStaticMethod(DexPathList_class,
                    "makeDexElements",
                    new Class[] { List.class, File.class, List.class, ClassLoader.class },
                    new Object[] {files, optimizedDirectory, exceptions, loader}
            );

        }else {
            result = RefInvoke.invokeStaticMethod(DexPathList_class,
                    "makeDexElements",
                    new Class[] { List.class, File.class, List.class, ClassLoader.class },
                    new Object[] {files, optimizedDirectory, exceptions, loader}
            );
        }

        return (Object[]) result;
    }

    @JXCStub
    protected Object[] makeDexElementsL2M(List<File> dexs) {

        Object[] elements = new Object[dexs.size()];

        String oat = String.format("%s/%s", mOptDexDir, "classes.oat");
        if (!getBooleanValue(mCtx, "opt-finish"))
            oat = null;

        int i=0;
        for (File dex : dexs){

            Class[] argTypes = new Class[]{String.class, String.class, int.class};
            Object[] argValue = new Object[]{dex.getAbsolutePath(), oat, 0};
            //??????openDexFile??????????????????????????????dex??????????????????oat???????????????cookie
            Object cookie =  RefInvoke.invokeStaticMethod("dalvik.system.DexFile", "openDexFile", argTypes, argValue);

            //????????????DexFile.loadDex???????????????dexFile??????????????????dexFile?????????mCookie?????????????????????cookie???
            //?????????????????????dex???????????????????????????loadDex?????????????????????dex?????????????????????????????????????????????????????????????????????????????????dexFile??????
            argTypes = new Class[]{String.class, String.class, int.class};
            argValue = new Object[]{dex.getPath(), null, 0};
            Object dexFile = RefInvoke.invokeStaticMethod(DexFile.class.getName(), "loadDex", argTypes, argValue);
            RefInvoke.setFieldOjbect(DexFile.class.getName(), "mCookie", dexFile, cookie);

            argTypes = new Class[]{File.class, boolean.class, File.class, DexFile.class};
            argValue = new Object[]{new File(""), false, null, dexFile};
            Object element = RefInvoke.newInstance("dalvik.system.DexPathList$Element", argTypes, argValue);
            elements[i] = element;
        }

        return elements;
    }

    @JXCStub
    private void patchClassLoader(ClassLoader loader, Object []dexElement) {
        try {
            Object pathList = RefInvoke.getFieldOjbect(dalvik.system.BaseDexClassLoader.class.getName(),
                loader, "pathList");
            Object []oldElements = (Object[])RefInvoke.getFieldOjbect(pathList.getClass().getName(), pathList, "dexElements");

            int sz = oldElements.length + dexElement.length;
            Class<?> eleType = Class.forName("dalvik.system.DexPathList$Element");
            Object array = Array.newInstance(eleType, sz);
            int i = 0;
            for (Object obj : oldElements) {
                Array.set(array, i++, obj);
            }
            //????????????dexElements???????????????
            //??????????????????????????????????????????????????????dex???9.0???org.apache?????????????????????????????????????????????????????????????????????????????????????????????
            //???????????????????????????????????????????????????org.apache?????????????????????????????????????????????????????????apache????????????????????????dex?????????????????????????????????apache?????????.
            for (Object obj : dexElement) {
                Array.set(array, i++, obj);
            }
            // ??????dexElements
            RefInvoke.setFieldOjbect(pathList.getClass().getName(), "dexElements", pathList, array);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    @JXCStub
    protected void normalMakeDexs(ArrayList<File> dexes, final ClassLoader loader, final File optDir) {
        long mTimeInit = System.currentTimeMillis();
        //Object pathList = RefInvoke.getFieldOjbect(dalvik.system.BaseDexClassLoader.class.getName(), loader, "pathList");
        DXLog.d("shell normalMakeDexs step 1:%d", System.currentTimeMillis()- mTimeInit);
        Object[] new_dexElementsxx = makeDexElements(dexes, loader, optDir);
        DXLog.d("shell normalMakeDexs step 2:%d", System.currentTimeMillis()- mTimeInit);
        // ??????dexElements
        patchClassLoader(loader, new_dexElementsxx);
        //RefInvoke.setFieldOjbect(pathList.getClass().getName(), "dexElements", pathList, new_dexElementsxx);
    }

    private static class ElementWithIndexs {
        public int id;
        public Object element;
        public ElementWithIndexs(int _id, Object _ele) {
            id = _id;
            element = _ele;
        }
    }
    @JXCStub
    protected void multiThreadMakeDexs(final ArrayList<File> files, final ClassLoader loader, final File optDir) {
        //??????makeDexsElements????????????dex????????????????????????makeDexElements

        long mTimeInit = System.currentTimeMillis();

        final ConcurrentLinkedQueue<ElementWithIndexs> outDexElems = new ConcurrentLinkedQueue<>();
        int sz = files.size();
        final CountDownLatch latch = new CountDownLatch(sz - 1);

        DXLog.i("begin muti made");

        int nThreads = Runtime.getRuntime().availableProcessors();
        if (nThreads > sz - 1) {
            nThreads = sz - 1;
        }

        DXLog.d("shell multiThreadMakeDexs step 1:%d", System.currentTimeMillis()- mTimeInit);


        if (sz > 1) {
            ExecutorService service = Executors.newFixedThreadPool(nThreads);
            for (int i = 1; i < sz; i++) {
                final int id = i;
                service.execute(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            ArrayList<File> dexs = new ArrayList<>();
                            dexs.add(files.get(id));
                            Object[] elements = makeDexElements(dexs, loader, optDir);
                            if (elements.length > 0) {
                                outDexElems.add(new ElementWithIndexs(id, elements[0]));
                            }
                            DXLog.i(String.format("thread id:%d return %s", id, elements.toString()));
                        }
                        catch (Exception e) {
                            e.printStackTrace();
                        }
                        latch.countDown();
                    }
                });
            }
        }

        DXLog.d("shell multiThreadMakeDexs step 2:%d", System.currentTimeMillis()- mTimeInit);

        ArrayList<File> dexs = new ArrayList<>();
        dexs.add(files.get(0));


        DXLog.d("shell multiThreadMakeDexs step 2-1:%d", System.currentTimeMillis()- mTimeInit);
        DXLog.d("before makeDexElement memory : %d", DebugUtils.getMemorySize());
        Object[] elements = makeDexElements(dexs, loader, optDir);
        DXLog.d("after makeDexElement memory : %d", DebugUtils.getMemorySize());
        DXLog.d("shell multiThreadMakeDexs step 2-2:%d", System.currentTimeMillis()- mTimeInit);

        if (elements.length > 0) {
            outDexElems.add(new ElementWithIndexs(0, elements[0]));
        }

        DXLog.d("shell multiThreadMakeDexs step 2-3:%d", System.currentTimeMillis()- mTimeInit);
        try {
            latch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        DXLog.d("shell multiThreadMakeDexs step 3:%d", System.currentTimeMillis()- mTimeInit);

        if (outDexElems.size() < sz) {
            DXLog.w("multithread make dex not all dex has made size:%d actual made %d", sz, outDexElems.size());
            //multithread make dex?????????n900???????????????????????????PR_SET_DUMPABLE?????????????????????????????????????????????????????????????????????make????????????
            //????????????????????????make?????????????????????.
            normalMakeDexs(files, loader, optDir);
        }
        else {
            try {
                Class<?> eleType = Class.forName("dalvik.system.DexPathList$Element");
                Object []array = (Object[])Array.newInstance(eleType, outDexElems.size());
                for (ElementWithIndexs elementWithIndexs : outDexElems) {
                    //??????????????????Dex File???????????????load dex,?????????????????????
                    int id = elementWithIndexs.id;
                    Object element = elementWithIndexs.element;
                    Array.set(array, id, element);
                }
                // ??????dexElements
                patchClassLoader(loader, array);
                //RefInvoke.setFieldOjbect(pathList.getClass().getName(), "dexElements", pathList, array);
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            }
            DXLog.i(" multi make end");
        }

        DXLog.d("shell multiThreadMakeDexs step 4:%d", System.currentTimeMillis()- mTimeInit);
    }

    @JXCStub
    protected void _copyTo(InputStream is, FileOutputStream fileOutputStream) throws IOException {
        DXLog.i("begin copyTo");
        byte[] arrayOfByte = new byte[1024*1024];
        while (true) {
            int len = is.read(arrayOfByte);

            if (len == -1)
                break;
            fileOutputStream.write(arrayOfByte, 0, len);
        }
        DXLog.i("end copyTo");
    }

    @JXCStub
    protected void setLongValue(Context ctx, String key, long value){
        SharedPreferences.Editor ed = ctx.getSharedPreferences("dx-config", Context.MODE_MULTI_PROCESS).edit();
        ed.putLong(key, value);
        ed.commit();
    }

    @JXCStub
    protected void setStringValue(Context ctx, String key, String value){
        SharedPreferences.Editor ed = ctx.getSharedPreferences("dx-config", Context.MODE_MULTI_PROCESS).edit();
        ed.putString(key, value);
        ed.commit();
    }

    @JXCStub
    protected long getLongValue(Context ctx, String key){
        SharedPreferences sp = ctx.getSharedPreferences("dx-config", Context.MODE_MULTI_PROCESS);
        return sp.getLong(key, -1);
    }

    @JXCStub
    protected void setBooleanValue(Context ctx, String key, boolean value){
        SharedPreferences.Editor ed = ctx.getSharedPreferences("dx-config", Context.MODE_MULTI_PROCESS).edit();
        ed.putBoolean(key, value);
        ed.commit();
    }

    @JXCStub
    protected boolean getBooleanValue(Context ctx, String key){
        SharedPreferences sp = ctx.getSharedPreferences("dx-config", Context.MODE_MULTI_PROCESS);
        return sp.getBoolean(key, false);
    }

    @JXCStub
    protected String getStringValue(Context ctx, String key){
        SharedPreferences sp = ctx.getSharedPreferences("dx-config", Context.MODE_MULTI_PROCESS);
        String val = sp.getString(key, "");
        return val;
    }

    protected ShellBase(Context ctx, String payLoadZipPath, String extractDir, int dexCount) {
        mCtx = ctx;
        mPayLoadZipPath = payLoadZipPath;
        mExtractDir = extractDir;
        mReleaseDexsDir = String.format("%s/.unzip/", extractDir);
        mProcessAbi = CpuAbiTools.getProcessAbi(ctx);

        mOptDexDir = String.format("%s/.opt", extractDir);
        int sdk_int = Build.VERSION.SDK_INT;
        if (sdk_int >= 26){
            mOptDexDir = String.format("%s/.unzip/oat/%s", extractDir, mProcessAbi);
        }
        mDexCount = dexCount;
    }

    protected abstract void extractDexs(String payLoadPath, int numOfDexs) throws IOException;

    protected abstract void init2();

    public void init() throws IOException {

        String payLoadPath = String.format("%s/.csn.dex", mExtractDir);
        extractDexs(payLoadPath, mDexCount);
        init2();
    }

    public abstract ClassLoader createClassLoader(ClassLoader parentLoader);

}
