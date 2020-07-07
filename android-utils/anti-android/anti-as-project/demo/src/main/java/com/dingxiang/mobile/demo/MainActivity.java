package com.dingxiang.mobile.demo;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.util.concurrent.Semaphore;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("DxAntiDemo");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI(this));

        try{
            Thread.sleep(4000);
            Log.e("Anti-log", "====detect-log");
            Log.e("DXAnti", "java print detect log");
        }catch (Exception e){
            e.printStackTrace();
        }

        Button btn = findViewById(R.id.btn);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                testJniHook();
            }
        });


        findViewById(R.id.test_hookbtn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                testChangeHook();
//                Toast.makeText(MainActivity.this,  testChangeHook(),Toast.LENGTH_LONG).show();
            }
        });


    }


    private void testJniHook() {
        String st = testHook(this);
        ((TextView)findViewById(R.id.sample_text)).setText(st);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI(Context context);

    public native String testHook(Context context);

    public native void testChangeHook();

    @Override
    public void onBackPressed() {
        super.onBackPressed();
//        Process.killProcess(Process.myPid());
    }
}
