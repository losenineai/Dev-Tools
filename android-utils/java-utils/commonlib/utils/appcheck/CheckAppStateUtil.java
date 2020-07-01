package com.auric.intell.commonlib.utils.appcheck;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

import com.auric.intell.commonlib.utils.LogTool;

public class CheckAppStateUtil {
    public static final int APP_NO_RUNNING = 1;
    public static final int APP_RUNNING_FRONT = 2;
    public static final int APP_RUNNING_BACKGROUND = 3;
    public static final int APP_CHECK_ERROR = 4;
    private static final String TAG = "CheckAppStateUtil";

    // top命令
    public static final String[] TOP = { "/system/bin/top", "-n", "1", "-d", "0" };

    private static boolean IS_DEBUG = CheckAppRunningUtil.isDebugRunning;


    public static String checkRunningByTopCmd(String pkgName) {
        return run(TOP, pkgName);
    }


    public static int checkByTopResult(String result) {
        if (IS_DEBUG)
            LogTool.d(TAG, result);
        String[] resultRoot = result.split("@@");
        int min = 100;
        if (result != null && !result.trim().equals("")) {
            for (String runResult : resultRoot) {
                String pid = null;
                try {
                    pid = getStringPid(runResult);
                } catch (Throwable t) {
                    LogTool.w(TAG, t.toString());
                }

                int oom_adj_value = getOomAdjByPid(pid);
                LogTool.d(TAG, "checkByTopResult pid:" + pid + " oom_adj_value :" + oom_adj_value);
                if (oom_adj_value < min) {
                    min = oom_adj_value;
                }
            }
            // 前台进程
            if (min == 0) {
                return APP_RUNNING_FRONT;
            } else if (min < 0) {
                return APP_CHECK_ERROR;
            } else {
                return APP_RUNNING_BACKGROUND;
            }
        } else {
            return APP_NO_RUNNING;
        }
    }

    /**
     * @description 根据包名通过top获得pids，如果没有获取到任何pid，返回的数组size == 0;
     */
    public static ArrayList<Integer> getPidsByPackageName(String pkgName) {
        ArrayList<Integer> pids = new ArrayList<Integer>();
        try {
            String result = run(TOP, pkgName);
            String[] resultRoot = result.split("@@");
            if (result != null && !result.trim().equals("")) {
                for (String runResult : resultRoot) {
                    String pid = getStringPid(runResult);
                    int pid_t = Integer.parseInt(pid);
                    pids.add(pid_t);
                }
            }
        } catch (Throwable t) {
            t.printStackTrace();
        }
        return pids;
    }

    private static String getStringPid(String result) {
        result.trim();
        String[] results = result.split(" ");
        String pid = "";
        for (String temp : results) {
            if (!temp.trim().equals("")) {
                pid = temp;
                break;
            }
        }
        if (IS_DEBUG)
            LogTool.d(TAG, "pid : " + pid);
        return pid;
    }

    /**
     * 获取一个oom_adj最小的进程pid。一般在前台的游戏进程是0
     */
    public static int getMiniOomAdjPid(List<Integer> pids) {
        if (null == pids || pids.size() == 0) {
            if (IS_DEBUG) {
                LogTool.d(TAG, "pids is null or empty");
            }
            return 0;
        }
        int minOomAdjPid = 0;
        if (null != pids.get(0) && pids.get(0) > 0) {
            minOomAdjPid = pids.get(0);     // 避免下面获oom_adj都获取不到（默认值是999），导致正确的pid没办法返回。
        }
        int minOomAdj = 100;
        for (Integer pid : pids) {
            int oomAdjTmp = getOomAdjByPid(String.valueOf(pid));
            if (IS_DEBUG) {
                LogTool.d(TAG, "pid " + pid + " oom_adj is " + oomAdjTmp);
            }
            if (oomAdjTmp < minOomAdj) {
                minOomAdj = oomAdjTmp;
                minOomAdjPid = pid;
            }
        }
        return minOomAdjPid;
    }

    protected static int getOomAdjByPid(String pid) {
        String checkRunningResult = "";
        try {
            if (IS_DEBUG) {
                LogTool.d(TAG, " getOomAdjByPid pid " + pid);
            }
            checkRunningResult = readConsole("cat /proc/" + pid + "/oom_adj", false);
            if (IS_DEBUG)
                LogTool.d(TAG, "oom_adj value:" + (checkRunningResult == null ? "null" : checkRunningResult));
            checkRunningResult.trim();
            int valueOfOOMAdj = Integer.valueOf(checkRunningResult);
            if (IS_DEBUG)
                LogTool.d(TAG, "valueOfOOMAdj : " + valueOfOOMAdj);
            return valueOfOOMAdj;
        } catch (Throwable e) {
            if (IS_DEBUG)
                LogTool.d(TAG, "getOomAdjByPid excption" + e.toString());
            // e.printStackTrace();
            return 999;
        }
    }

    public static boolean isCanGetProcessOomadj() {
        int initOomadj = getOomAdjByPid("1");
        LogTool.d(TAG, "initOomadj " + initOomadj);
        return initOomadj != 999;
    }

    // 获取指定应用的top命令获取的信息
    // PID CPU% S #THR VSS RSS PCY UID Name // 进程属性
    // 如果当前应用不在运行则返回null
    private static synchronized String run(String[] cmd, String pkgName) {
        String line = "";
        InputStream is = null;
        String temp = null;
        try {
            Runtime runtime = Runtime.getRuntime();
            Process proc = runtime.exec(cmd);
            is = proc.getInputStream();
            // 换成BufferedReader
            BufferedReader buf = new BufferedReader(new InputStreamReader(is));
            do {
                temp = buf.readLine();
                // 读取到相应pkgName跳出循环（或者未找到）
                if (null == temp) {
                    break;
                } else {
                    if (temp.endsWith(pkgName)) {
                        if (!line.equals(""))
                            line += "@@";
                        line += temp;
                    }
                }
            } while (true);
            if (is != null) {
                buf.close();
                is.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return line;
    }

    public static String readConsole(String cmd, Boolean isPrettify) throws IOException {
        StringBuffer cmdout = new StringBuffer();
        Process process = Runtime.getRuntime().exec(cmd);
        InputStream fis = process.getInputStream();
        BufferedReader br = new BufferedReader(new InputStreamReader(fis));
        String line = null;
        if (isPrettify == null || isPrettify) {
            while ((line = br.readLine()) != null) {
                cmdout.append(line);
            }
        } else {
            while ((line = br.readLine()) != null) {
                cmdout.append(line).append(System.getProperty("line.separator"));
            }
        }
        return cmdout.toString().trim();
    }

}
