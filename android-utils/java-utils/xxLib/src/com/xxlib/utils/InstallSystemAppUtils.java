package com.xxlib.utils;

import java.io.File;

import com.xxlib.utils.base.ShellTool;
import com.xxlib.utils.base.ShellTool.CommandResult;

public class InstallSystemAppUtils {

    public final static int INSTALL_SUCCESS = 1;
    public final static int INSTALL_FAILED_NO_PERMISSION = 2;
    public final static int CHANGE_WRITABLE_FAILED = 3;
    public final static int CHANGE_WRITABLE_SUCCESS = 4;
    public final static int MOVE_TO_SYSTEM_SUCCESS = 5;
    public final static int MOVE_TO_SYSTEM_FAILED = 6;
    public final static int CHANGE_READ_POWER_SUCCESS = 7;
    public final static int CHANGE_READ_POWER_FAILED = 8;
    public final static int REBOOT_SUCCESS = 9;
    public final static int REBOOT_FAILED = 10;
    public final static int REMOVE_FROM_SYSTEM_SUCCESS = 11;
    public final static int REMOVE_FROM_SYSTEM_FAILED = 12;

    /**
     * @param paths 可传入多个安装包的路径
     */
    public static int installSystemApp(String[] paths) {
        int result = 0;
        if (ShellTool.checkRootPermission()) {
            result = changeSystemDirWritable();
            if (result == CHANGE_WRITABLE_FAILED) {
                return result;
            }
            for (int i = 0; i < paths.length; i++) {
                result = moveToSystemDir(paths[i]);
                if (result == MOVE_TO_SYSTEM_FAILED) {
                    return result;
                }
                result = changeReadPower(paths[i]);
                if (result == CHANGE_READ_POWER_FAILED) {
                    return result;
                }
            }
            return INSTALL_SUCCESS;
        } else {
            return INSTALL_FAILED_NO_PERMISSION;
        }
    }

    /**
     * @param paths 可传入多个安装包的路径
     */
    public static int uninstallSystemApp(String[] paths) {
        int result = 0;
        if (ShellTool.checkRootPermission()) {
            result = changeSystemDirWritable();
            if (result == CHANGE_WRITABLE_FAILED) {
                return result;
            }
            for (int i = 0; i < paths.length; i++) {
                result = removeFromSystemDir(paths[i]);
                if (result == REMOVE_FROM_SYSTEM_FAILED) {
                    return result;
                }
            }
            return REMOVE_FROM_SYSTEM_SUCCESS;
        } else {
            return INSTALL_FAILED_NO_PERMISSION;
        }
    }

    /**
     * @param path
     */
    public static int installSystemApp(String path) {
        int result;
        if (ShellTool.checkRootPermission()) {
            result = changeSystemDirWritable();
            if (result == CHANGE_WRITABLE_FAILED) {
                return result;
            }
            result = moveToSystemDir(path);
            if (result == MOVE_TO_SYSTEM_FAILED) {
                return result;
            }
            result = changeReadPower(path);
            if (result == CHANGE_READ_POWER_FAILED) {
                return result;
            }

            return INSTALL_SUCCESS;
        } else {
            return INSTALL_FAILED_NO_PERMISSION;
        }
    }

    /**
     * @param path
     */
    public static int uninstallSystemApp(String path) {
        int result;
        if (ShellTool.checkRootPermission()) {
            result = changeSystemDirWritable();
            if (result == CHANGE_WRITABLE_FAILED) {
                return result;
            }
            result = removeFromSystemDir(path);
            if (result == REMOVE_FROM_SYSTEM_FAILED) {
                return result;
            }

            return REMOVE_FROM_SYSTEM_SUCCESS;
        } else {
            return INSTALL_FAILED_NO_PERMISSION;
        }
    }

    private static int changeSystemDirWritable() {
        String command = "mount -o remount,rw /system";
        CommandResult result = ShellTool.execCommand(command, true);
        if (result.result == 0) {
            return CHANGE_WRITABLE_SUCCESS;
        } else {
            return CHANGE_WRITABLE_FAILED;
        }
    }

    private static int moveToSystemDir(String path) {
        File file = new File(path);
        String command = "cat " + path + " > " + "/system/app/" + file.getName();
        CommandResult result = ShellTool.execCommand(command, true);
        if (result.result == 0) {
            return MOVE_TO_SYSTEM_SUCCESS;
        } else {
            return MOVE_TO_SYSTEM_FAILED;
        }
    }

    private static int changeReadPower(String path) {
        File file = new File(path);
        String command = "chmod 644 " + "/system/app/" + file.getName();
        CommandResult result = ShellTool.execCommand(command, true);
        if (result.result == 0) {
            return CHANGE_READ_POWER_SUCCESS;
        } else {
            return CHANGE_READ_POWER_FAILED;
        }
    }

    private static int removeFromSystemDir(String path) {
        File file = new File(path);
        String command = "rm " + "/system/app/" + file.getName();
        CommandResult result = ShellTool.execCommand(command, true);
        if (result.result == 0) {
            return REMOVE_FROM_SYSTEM_SUCCESS;
        } else {
            return REMOVE_FROM_SYSTEM_FAILED;
        }
    }

    public static int reboot() {
        String command = "reboot";
        CommandResult result = ShellTool.execCommand(command, true);
        if (result.result == 0) {
            return REBOOT_SUCCESS;
        } else {
            return REBOOT_FAILED;
        }
    }
}
