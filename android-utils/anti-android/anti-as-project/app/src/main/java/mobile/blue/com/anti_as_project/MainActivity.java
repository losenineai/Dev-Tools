package mobile.blue.com.anti_as_project;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Process;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends Activity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("DxAntiTest");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI(this));

        /*new Thread() {
            @Override
            public void run() {

                try {
                    for(int i=0; i<5; i++) {
                        Log.e("DXAnti", "倒计时： " + (5-i));
                        Thread.sleep(2000);
                    }

                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                throw new RuntimeException("test exception");

            }
        }.start();*/
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI(Context context);

    public void onClickTest(View v){
        String str = stringFromJNI(this);
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(str);
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        android.os.Process.killProcess(Process.myPid());

    }
}
