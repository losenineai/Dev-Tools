package com.auric.intell.commonlib.network.quality;


import com.auric.intell.commonlib.utils.LogUtils;


import java.io.BufferedInputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.Timer;
import java.util.TimerTask;
import javax.net.ssl.HttpsURLConnection;

/**
 * Created by sh on 2017/8/15.
 * 显示实时网速
 */

public class NetworkRealTimeSpeed {


        private static long speed;
        private static TimerTask task;
        private static Timer timer;

        public NetworkRealTimeSpeed(){

        }

        //开始计算网速
        public static void startCalNetSpeed( ){
            speed=0;
            stopCalNetSpeed();
            task= new TimerTask() {
                @Override
                public void run() {
                    speed=(new GetWebFile()).getFileFromUrl("http://www.people.com.cn/");
                }
            };
            timer=new Timer();
            timer.schedule(task, 0, 500);
        }
        //结束计算网速
        public static void stopCalNetSpeed(){
            if(timer!=null)
                timer.cancel();
            if(task!=null)
                task.cancel();
        }


        //获取网速
        public static long getNetSpeed(){
            return speed;
        }

    public static class GetWebFile
    {

        /**
         * <p>
         * Read file from web
         * </p>
         * @param url
         * @return
         */
        public long getFileFromUrl( String url)
        {
            int currentByte = 0;
            long startTime = 0;
            long maxInterval=500;
            long speed=0;
            String userAgent = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36";
            byte[] b = null;

            int bytecount = 0;
            int fileLength=0;
            URL urlx = null;
            HttpURLConnection con = null;
            BufferedInputStream bis = null;
            try
            {
                urlx = new URL( url );
                URLConnection uc = urlx.openConnection();
                con =(HttpURLConnection) uc;
                con.setConnectTimeout( 3000 );
                con.setDoOutput(true);
                con.setRequestProperty("User-Agent", userAgent);
                con.setRequestProperty("Accept-Encoding", "gzip, deflate, sdch, br");
                con.setRequestProperty("Accept-Language", "zh-CN,zh;q=0.8");
                con.setRequestProperty("Connection", "keep-alive");

                con.connect();
                // 得到响应状态码的返回值 responseCode
                int code = con.getResponseCode();
                LogUtils.d("NetworkRealTimeSpeed"+"code"+code);
                if(code!=200)
                    return 0;


                bis = new BufferedInputStream(con.getInputStream());
                startTime = System.currentTimeMillis();
                fileLength= con.getContentLength();
                LogUtils.d("NetworkRealTimeSpeed"+"NetworkRealTimeSpeedLength"+fileLength);
                if(fileLength<=0)
                    return 0;
                b = new byte[fileLength];
                    while ( ( currentByte = bis.read() ) != -1 )
                    {
                            b[bytecount++] = ( byte ) currentByte;
                        if((System.currentTimeMillis()-startTime)>maxInterval){
                            LogUtils.d("NetworkRealTimeSpeed"+"System.currentTimeMillis()"+System.currentTimeMillis());
                            LogUtils.d("NetworkRealTimeSpeed"+"startTime"+startTime);
                            speed=(1000*bytecount)/(((maxInterval)));
                            return speed;
                        }
                        if(bytecount>=fileLength){
                            LogUtils.d("NetworkRealTimeSpeed"+"System.currentTimeMillis()"+System.currentTimeMillis());
                            LogUtils.d("NetworkRealTimeSpeed"+"startTime"+startTime);
                            speed=(1000*bytecount)/(((System.currentTimeMillis()-startTime)));
                        }
                    }

            }
            catch ( Exception e )
            {
                LogUtils.e( "exception : "+e.getMessage() );
            }
            finally
            {
                try
                {
                    if( bis != null )
                    {
                        bis.close();
                    }
                }
                catch ( Exception e )
                {
                    LogUtils.e( "exception : "+e.getMessage() );
                }

            }
            return speed;
        }
    }
}
