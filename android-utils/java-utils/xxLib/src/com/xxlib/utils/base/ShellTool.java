package com.xxlib.utils.base;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.List;

/**
 * ShellUtils
 * <ul>
 * <strong>Check root</strong>
 * <li>{@link ShellTool#checkRootPermission()}</li>
 * </ul>
 * <ul>
 * <strong>Execte command</strong>
 * <li>{@link ShellTool#execCommand(String, boolean)}</li>
 * <li>{@link ShellTool#execCommand(String, boolean, boolean)}</li>
 * <li>{@link ShellTool#execCommand(List, boolean)}</li>
 * <li>{@link ShellTool#execCommand(List, boolean, boolean)}</li>
 * <li>{@link ShellTool#execCommand(String[], boolean)}</li>
 * <li>{@link ShellTool#execCommand(String[], boolean, boolean)}</li>
 * </ul>
 * 
 * @author <a href="http://www.trinea.cn" target="_blank">Trinea</a> 2013-5-16
 */
public class ShellTool {

	public static final String COMMAND_SU = "su";
	public static final String COMMAND_SH = "sh";
	public static final String COMMAND_EXIT = "exit\n";
	public static final String COMMAND_LINE_END = "\n";

	private static String TAG = "ShellTool";

	/**
	 * check whether has root permission
	 * 
	 * @return
	 */
	public static boolean checkRootPermission() {
		// return execCommand("echo root", true, false, null).result == 0;
		LogTool.i(TAG, "checkRootPermission ,before execRoot(true)");
		return execRoot(true);
	}

	/**
	 * execute shell command, default return result msg
	 * 
	 * @param command
	 *            command
	 * @param isRoot
	 *            whether need to run with root
	 * @return
	 * @see ShellTool#execCommand(String[], boolean, boolean)
	 */
	public static CommandResult execCommand(String command, boolean isRoot) {
		return execCommand(new String[] { command }, isRoot, true);
	}

	/**
	 * execute shell command, default return result msg
	 * 
	 * @param command
	 *            command
	 * @param isRoot
	 *            whether need to run with root
	 * @return
	 * @see ShellTool#execCommand(String[], boolean, boolean)
	 */
	public static CommandResult execCommand(String command1, String command2, boolean isRoot) {
		return execCommand(new String[] { command1, command2 }, isRoot, true);
	}

	/**
	 * execute shell commands, default return result msg
	 * 
	 * @param commands
	 *            command list
	 * @param isRoot
	 *            whether need to run with root
	 * @return
	 * @see ShellTool#execCommand(String[], boolean, boolean)
	 */
	public static CommandResult execCommand(List<String> commands, boolean isRoot) {
		return execCommand(commands == null ? null : commands.toArray(new String[] {}), isRoot, true);
	}

	/**
	 * execute shell commands, default return result msg
	 * 
	 * @param commands
	 *            command array
	 * @param isRoot
	 *            whether need to run with root
	 * @return
	 * @see ShellTool#execCommand(String[], boolean, boolean)
	 */
	public static CommandResult execCommand(String[] commands, boolean isRoot) {
		return execCommand(commands, isRoot, true);
	}

	/**
	 * execute shell command
	 * 
	 * @param command
	 *            command
	 * @param isRoot
	 *            whether need to run with root
	 * @param isNeedResultMsg
	 *            whether need result msg
	 * @return
	 * @see ShellTool#execCommand(String[], boolean, boolean)
	 */
	public static CommandResult execCommand(String command, boolean isRoot, boolean isNeedResultMsg) {
		return execCommand(new String[] { command }, isRoot, isNeedResultMsg);
	}

	/**
	 * execute shell commands
	 * 
	 * @param commands
	 *            command list
	 * @param isRoot
	 *            whether need to run with root
	 * @param isNeedResultMsg
	 *            whether need result msg
	 * @return
	 * @see ShellTool#execCommand(String[], boolean, boolean)
	 */
	public static CommandResult execCommand(List<String> commands, boolean isRoot, boolean isNeedResultMsg) {
		return execCommand(commands == null ? null : commands.toArray(new String[] {}), isRoot, isNeedResultMsg);
	}

	/**
	 * execute shell commands
	 * 
	 * @param commands
	 *            command array
	 * @param isRoot
	 *            whether need to run with root
	 * @param isNeedResultMsg
	 *            whether need result msg
	 * @return <ul>
	 *         <li>if isNeedResultMsg is false, {@link CommandResult#successMsg}
	 *         is null and {@link CommandResult#errorMsg} is null.</li>
	 *         <li>if {@link CommandResult#result} is -1, there maybe some
	 *         excepiton.</li>
	 *         </ul>
	 */
	public static CommandResult execCommand(String[] commands, boolean isRoot, boolean isNeedResultMsg) {
		int result = -1;
		if (commands == null || commands.length == 0) {
			return new CommandResult(result, null, null);
		}

		StringBuilder successMsg = null;
		StringBuilder errorMsg = null;

		try {
			if (mRootProcess == null) {
				return new CommandResult(result, null, null);
			}

			StringBuilder cmdBuilder = new StringBuilder();
			for (String command : commands) {
				if (command == null) {
					continue;
				}
				cmdBuilder.append(command + "\n");
			}

			cmdBuilder.append("echo $?");
			// cmdBuilder.append(COMMAND_EXIT);
			String resultStr = mRootProcess.exec(cmdBuilder.toString());
			LogTool.i(TAG, cmdBuilder + " resultStr " + resultStr);

			if (resultStr != null && (resultStr.equals("0") || resultStr.equals("ready"))) {
				result = 0;
			}

		} catch (Exception e) {
			e.printStackTrace();
		}

		LogTool.i(TAG, "result:" + result + ", successMsg: " + (successMsg == null ? "null" : successMsg.toString())
				+ ", errorMsg:" + (errorMsg == null ? "null" : errorMsg.toString()));
		return new CommandResult(result, successMsg == null ? null : successMsg.toString(), errorMsg == null ? null
				: errorMsg.toString());
	}

	public static RootShellProcess mRootProcess;

	/**
	 * execute shell su
	 * 
	 */
	public static boolean execRoot(boolean isNeedResultMsg) {
		synchronized (ShellTool.class) {
			boolean result = false;
			try {
				if (mRootProcess != null) {
					LogTool.i(TAG, "mRootProcess != null, has root permission" + " cur id " + Thread.currentThread().getId());
					return true;
				}

				LogTool.i(TAG, "new RootShellProcess() 0 " + " cur id " + Thread.currentThread().getId());
				mRootProcess = new RootShellProcess();

				LogTool.i(TAG, "new RootShellProcess() 1" + " cur id " + Thread.currentThread().getId());
				String resultStr = mRootProcess.exec("echo $?");

				LogTool.i(TAG, "new RootShellProcess() 2 resultStr ：" + resultStr + " cur id " + Thread.currentThread().getId());
				if (resultStr != null && resultStr.equals("0")) {
					result = true;
				}

			} catch (Exception e) {
				e.printStackTrace();
			}

			if (mRootProcess != null && !result) {
				mRootProcess.destroy();
				mRootProcess = null;
			}
			LogTool.i(TAG, "new RootShellProcess() 3, result is " + result + " cur id " + Thread.currentThread().getId());

			return result;
		}
	}

	/**
	 * result of command
	 * <ul>
	 * <li>{@link CommandResult#result} means result of command, 0 means normal,
	 * else means error, same to excute in linux shell</li>
	 * <li>{@link CommandResult#successMsg} means success message of command
	 * result</li>
	 * <li>{@link CommandResult#errorMsg} means error message of command result</li>
	 * </ul>
	 * 
	 * @author <a href="http://www.trinea.cn" target="_blank">Trinea</a>
	 *         2013-5-16
	 */
	public static class CommandResult {

		/** result of command **/
		public int result;
		/** success message of command result **/
		public String successMsg;
		/** error message of command result **/
		public String errorMsg;

		public CommandResult(int result) {
			this.result = result;
		}

		public CommandResult(int result, String successMsg, String errorMsg) {
			this.result = result;
			this.successMsg = successMsg;
			this.errorMsg = errorMsg;
		}
	}

	/**
	 * 销毁root进程
	 * 
	 * @description
	 */
	public static void destroyRootProcess() {
		new Thread() {
			public void run() {
				synchronized (ShellTool.class) {
					if (mRootProcess != null) {
						try {
							mRootProcess.exec(COMMAND_EXIT);
							mRootProcess.destroy();
							mRootProcess = null;
						} catch (Exception e) {
							e.printStackTrace();
						}
					}
				}
			}
		}.start();
	}

	/**
	 * shell命令监听器
	 * 
	 * @Description
	 * 
	 *              版本号 作者 日期 相关修改 1.0 white 2014-12-15 创建
	 * 
	 * @author white 2014-12-15 -- 上午9:43:02 
	 */
	public interface IShellListener {
		public void onRootSuccess();
	}

	/**
	 * 有Root权限的shellProcess
	 * 
	 * @Description
	 * 
	 *              版本号 作者 日期 相关修改 1.0 white 2014-12-15 创建
	 * 
	 * @author white 2014-12-15 -- 下午5:24:38
	 */
	public static class RootShellProcess {
		private DataInputStream dis;
		private DataInputStream errDis;
		private DataOutputStream dos;
		private Process mProcess;

		public RootShellProcess() {
			try {
				mProcess = Runtime.getRuntime().exec(COMMAND_SU);
				dos = new DataOutputStream(mProcess.getOutputStream());
				dis = new DataInputStream(mProcess.getInputStream());
				errDis = new DataInputStream(mProcess.getErrorStream());
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		public synchronized String exec(String cmd) {
			try {
				if (null != dos) {
					dos.writeBytes(cmd + "\n");
					dos.flush();
					@SuppressWarnings("deprecation")
					String line = dis.readLine();
					return line;
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
			return null;
		}

		public synchronized Process getProcess() {
			return mProcess;
		}

		public synchronized void destroy() {
			if (mProcess != null) {
				try {
					if (dos != null)
						dos.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
				try {
					if (dis != null)
						dis.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
				try {
					if (errDis != null)
						errDis.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
				mProcess.destroy();
				mProcess = null;
			}
		}
	}

}
