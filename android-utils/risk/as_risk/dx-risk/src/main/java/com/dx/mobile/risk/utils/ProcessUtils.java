package com.dx.mobile.risk.utils;

import java.io.BufferedReader;
import java.io.InputStreamReader;

/**
 * Created by blue on 2018/9/20.
 */

public class ProcessUtils {

    public static String exec(String[] cmd) {

        Process cmdProcess = null;
        BufferedReader reader = null;
        String retStr = "";

        try {
            cmdProcess = Runtime.getRuntime().exec(cmd);
            reader = new BufferedReader(new InputStreamReader(cmdProcess.getInputStream()));
            retStr = reader.readLine();
            return retStr;
        } catch (Throwable e) {
            return null;
        } finally {
            try {
                reader.close();
            } catch (Throwable e) {
            }

            ProcessUtils.closeProcess(cmdProcess);
        }
    }

    public static String exec(String cmd) {

        Process cmdProcess = null;
        BufferedReader reader = null;
        String retStr = "";

        try {
            cmdProcess = Runtime.getRuntime().exec(cmd);
            reader = new BufferedReader(new InputStreamReader(cmdProcess.getInputStream()));
            retStr = reader.readLine();
            return retStr;
        } catch (Throwable e) {
            return "";
        } finally {
            try {
                reader.close();
            } catch (Throwable e) {
            }

            ProcessUtils.closeProcess(cmdProcess);
        }
    }

    public static String exec_get_all(String[] cmd) {

        String result = "";
        String line = "";
        Process proc = null;

        try {
            proc = Runtime.getRuntime().exec(cmd);
            InputStreamReader is = new InputStreamReader(proc.getInputStream());
            BufferedReader br = new BufferedReader (is);

            while ((line = br.readLine ()) != null) {
                result += line + "\n";
            }

        } catch(Exception e) {
            //e.printStackTrace();
        }

        ProcessUtils.closeProcess(proc);

        return result;
    }

    public static String exec(String cmd, String filter) {

        String result = "";
        String line = "";
        Process proc = null;

        try {
            proc = Runtime.getRuntime().exec(cmd);
            InputStreamReader is = new InputStreamReader(proc.getInputStream());
            BufferedReader br = new BufferedReader (is);

            while ((line = br.readLine ()) != null && line.contains(filter)== false) {
                result += line;
            }

        } catch(Exception e) {
            //e.printStackTrace();
        }

        ProcessUtils.closeProcess(proc);

        return result;
    }

    public static void closeProcess(Process p) {

        BufferedReader reader = null;

        try {
            reader = new BufferedReader(new InputStreamReader(p.getErrorStream()));
            while ( reader.readLine() != null);

            if (p != null)
                p.exitValue();

        } catch (Exception e) {

            try {
                p.destroy();
            } catch (Throwable throwable) {
            }
        } finally {
            try {
                reader.close();
            } catch (Throwable e) {
            }
        }
    }
}
