package com.xxlib.utils;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.media.MediaScannerConnection;
import android.net.Uri;
import android.text.TextUtils;
import android.util.Log;

import com.xxlib.utils.base.LogTool;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.StringReader;
import java.io.StringWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

/**
 * File Utils
 * <ul>
 * Read or write file
 * <li>{@link #readFile(String)} read file</li>
 * <li>{@link #readFileToList(String)} read file to string list</li>
 * <li>{@link #writeFile(String, String, boolean)} write file from String</li>
 * <li>{@link #writeFile(String, String)} write file from String</li>
 * <li>{@link #writeFile(String, List, boolean)} write file from String List</li>
 * <li>{@link #writeFile(String, List)} write file from String List</li>
 * <li>{@link #writeFile(String, InputStream)} write file</li>
 * <li>{@link #writeFile(String, InputStream, boolean)} write file</li>
 * <li>{@link #writeFile(File, InputStream)} write file</li>
 * <li>{@link #writeFile(File, InputStream, boolean)} write file</li>
 * </ul>
 * <ul>
 * Operate file
 * <li>{@link #copyFile(String, String)}</li>
 * <li>{@link #getFileExtension(String)}</li>
 * <li>{@link #getFileName(String)}</li>
 * <li>{@link #getFileNameWithoutExtension(String)}</li>
 * <li>{@link #getFileSize(String)}</li>
 * <li>{@link #deleteFile(String)}</li>
 * <li>{@link #isFileExist(String)}</li>
 * <li>{@link #isFolderExist(String)}</li>
 * <li>{@link #makeFolders(String)}</li>
 * <li>{@link #makeDirs(String)}</li>
 * </ul>
 * 
 * @author <a href="http://www.trinea.cn" target="_blank">Trinea</a> 2012-5-12
 */
public class FileUtils {

	public final static String FILE_EXTENSION_SEPARATOR = ".";
    private static final String TAG = "FileUtils";

	public static boolean isFileExist(String filePath) {
		if (StringUtils.isBlank(filePath)) {
			return false;
		}

		File file = new File(filePath);
		return (file.exists() && file.isFile());
	}


	/**
	 * write file, the bytes will be written to the begin of the file
	 * 
	 * @param filePath
	 * @param stream
	 * @return
	 * @throws IOException
	 * @see {@link #writeFile(String, InputStream, boolean)}
	 */
	public static boolean writeFile(String filePath, InputStream stream) throws IOException {
		return writeFile(filePath, stream, false);
	}

	/**
	 * write file
	 * 
	 * @param file
	 *            the file to be opened for writing.
	 * @param stream
	 *            the input stream
	 * @param append
	 *            if <code>true</code>, then bytes will be written to the end of
	 *            the file rather than the beginning
	 * @return return true
	 * @throws IOException
	 * @throws RuntimeException
	 *             if an error occurs while operator FileOutputStream
	 */
	public static boolean writeFile(String filePath, InputStream stream, boolean append) throws IOException {
		return writeFile(filePath != null ? new File(filePath) : null, stream, append);
	}

	/**
	 * write file
	 * 
	 * @param file
	 *            the file to be opened for writing.
	 * @param stream
	 *            the input stream
	 * @param append
	 *            if <code>true</code>, then bytes will be written to the end of
	 *            the file rather than the beginning
	 * @return return true
	 * @throws IOException
	 * @throws RuntimeException
	 *             if an error occurs while operator FileOutputStream
	 */
	public static boolean writeFile(File file, InputStream stream, boolean append) throws IOException {

		OutputStream o = null;
		try {
			makeDirs(file.getAbsolutePath());
			o = new FileOutputStream(file, append);
			byte data[] = new byte[1024];
			int length = -1;
			while ((length = stream.read(data)) != -1) {
				o.write(data, 0, length);
			}
			o.flush();
			return true;
		} catch (FileNotFoundException e) {
			throw new RuntimeException("FileNotFoundException occurred. ", e);
		} catch (IOException e) {
			throw new IOException();
		} finally {
			if (o != null) {
				try {
					o.close();
					stream.close();
				} catch (IOException e) {
					throw new RuntimeException("IOException occurred. ", e);
				}
			}
		}
	}

	public static String getFolderName(String filePath) {

		if (StringUtils.isEmpty(filePath)) {
			return filePath;
		}

		int filePosi = filePath.lastIndexOf(File.separator);
		return (filePosi == -1) ? "" : filePath.substring(0, filePosi);
	}

	public static boolean makeDirs(String filePath) {
		String folderName = getFolderName(filePath);
		if (StringUtils.isEmpty(folderName)) {
			return false;
		}

		File folder = new File(folderName);
		return (folder.exists() && folder.isDirectory()) ? true : folder.mkdirs();
	}

	/**
	 * copy file
	 * 
	 * @param sourceFilePath
	 * @param destFilePath
	 * @return
	 * @throws IOException
	 * @throws RuntimeException
	 *             if an error occurs while operator FileOutputStream
	 */
	public static boolean copyFile(String sourceFilePath, String destFilePath) throws IOException {
		InputStream inputStream = null;
		inputStream = new FileInputStream(sourceFilePath);

		return writeFile(destFilePath, inputStream);
	}

	/**
	 * read file
	 * 
	 * @param filePath
	 * @param charsetName
	 *            The name of a supported {@link java.nio.charset.Charset
	 *            </code>charset<code>}
	 * @return if file not exist, return null, else return content of file
	 * @throws RuntimeException
	 *             if an error occurs while operator BufferedReader
	 */
	public static StringBuilder readFile(String filePath, String charsetName) {
		File file = new File(filePath);
		StringBuilder fileContent = new StringBuilder("");
		if (file == null || !file.isFile()) {
			return null;
		}

		BufferedReader reader = null;
		try {
			InputStreamReader is = new InputStreamReader(new FileInputStream(file), charsetName);
			reader = new BufferedReader(is);
			String line = null;
			while ((line = reader.readLine()) != null) {
				if (!fileContent.toString().equals("")) {
					fileContent.append("\r\n");
				}
				fileContent.append(line);
			}
			reader.close();
			return fileContent;
		} catch (IOException e) {
			throw new RuntimeException("IOException occurred. ", e);
		} finally {
			if (reader != null) {
				try {
					reader.close();
				} catch (IOException e) {
					throw new RuntimeException("IOException occurred. ", e);
				}
			}
		}
	}

	/**
	 * write file
	 * 
	 * @param filePath
	 * @param content
	 * @param append
	 *            is append, if true, write to the end of file, else clear
	 *            content of file and write into it
	 * @return return false if content is empty, true otherwise
	 * @throws RuntimeException
	 *             if an error occurs while operator FileWriter
	 */
	// public static boolean writeFile(String filePath, String content, boolean
	// append) {
	// if (StringUtils.isEmpty(content)) {
	// return false;
	// }
	//
	// FileWriter fileWriter = null;
	// try {
	// makeDirs(filePath);
	// fileWriter = new FileWriter(filePath, append);
	// fileWriter.write(content);
	// fileWriter.close();
	// return true;
	// } catch (IOException e) {
	// throw new RuntimeException("IOException occurred. ", e);
	// } finally {
	// if (fileWriter != null) {
	// try {
	// fileWriter.close();
	// } catch (IOException e) {
	// throw new RuntimeException("IOException occurred. ", e);
	// }
	// }
	// }
	// }
	//
	// /**
	// * write file
	// *
	// * @param filePath
	// * @param contentList
	// * @param append is append, if true, write to the end of file, else clear
	// content of file and write into it
	// * @return return false if contentList is empty, true otherwise
	// * @throws RuntimeException if an error occurs while operator FileWriter
	// */
	// public static boolean writeFile(String filePath, List<String>
	// contentList, boolean append) {
	// if (ListUtils.isEmpty(contentList)) {
	// return false;
	// }
	//
	// FileWriter fileWriter = null;
	// try {
	// makeDirs(filePath);
	// fileWriter = new FileWriter(filePath, append);
	// int i = 0;
	// for (String line : contentList) {
	// if (i++ > 0) {
	// fileWriter.write("\r\n");
	// }
	// fileWriter.write(line);
	// }
	// fileWriter.close();
	// return true;
	// } catch (IOException e) {
	// throw new RuntimeException("IOException occurred. ", e);
	// } finally {
	// if (fileWriter != null) {
	// try {
	// fileWriter.close();
	// } catch (IOException e) {
	// throw new RuntimeException("IOException occurred. ", e);
	// }
	// }
	// }
	// }
	//
	// /**
	// * write file, the string will be written to the begin of the file
	// *
	// * @param filePath
	// * @param content
	// * @return
	// */
	// public static boolean writeFile(String filePath, String content) {
	// return writeFile(filePath, content, false);
	// }
	//
	// /**
	// * write file, the string list will be written to the begin of the file
	// *
	// * @param filePath
	// * @param contentList
	// * @return
	// */
	// public static boolean writeFile(String filePath, List<String>
	// contentList) {
	// return writeFile(filePath, contentList, false);
	// }
	//
	// /**
	// * write file, the bytes will be written to the begin of the file
	// *
	// * @param filePath
	// * @param stream
	// * @return
	// * @see {@link #writeFile(String, InputStream, boolean)}
	// */
	// public static boolean writeFile(String filePath, InputStream stream) {
	// return writeFile(filePath, stream, false);
	// }
	//
	// /**
	// * write file
	// *
	// * @param file the file to be opened for writing.
	// * @param stream the input stream
	// * @param append if <code>true</code>, then bytes will be written to the
	// end of the file rather than the beginning
	// * @return return true
	// * @throws RuntimeException if an error occurs while operator
	// FileOutputStream
	// */
	// public static boolean writeFile(String filePath, InputStream stream,
	// boolean append) {
	// return writeFile(filePath != null ? new File(filePath) : null, stream,
	// append);
	// }
	//
	// /**
	// * write file, the bytes will be written to the begin of the file
	// *
	// * @param file
	// * @param stream
	// * @return
	// * @see {@link #writeFile(File, InputStream, boolean)}
	// */
	// public static boolean writeFile(File file, InputStream stream) {
	// return writeFile(file, stream, false);
	// }
	//
	// /**
	// * write file
	// *
	// * @param file the file to be opened for writing.
	// * @param stream the input stream
	// * @param append if <code>true</code>, then bytes will be written to the
	// end of the file rather than the beginning
	// * @return return true
	// * @throws RuntimeException if an error occurs while operator
	// FileOutputStream
	// */
	// public static boolean writeFile(File file, InputStream stream, boolean
	// append) {
	// OutputStream o = null;
	// try {
	// makeDirs(file.getAbsolutePath());
	// o = new FileOutputStream(file, append);
	// byte data[] = new byte[1024];
	// int length = -1;
	// while ((length = stream.read(data)) != -1) {
	// o.write(data, 0, length);
	// }
	// o.flush();
	// return true;
	// } catch (FileNotFoundException e) {
	// throw new RuntimeException("FileNotFoundException occurred. ", e);
	// } catch (IOException e) {
	// throw new RuntimeException("IOException occurred. ", e);
	// } finally {
	// if (o != null) {
	// try {
	// o.close();
	// stream.close();
	// } catch (IOException e) {
	// throw new RuntimeException("IOException occurred. ", e);
	// }
	// }
	// }
	// }
	//
	// /**
	// * copy file
	// *
	// * @param sourceFilePath
	// * @param destFilePath
	// * @return
	// * @throws RuntimeException if an error occurs while operator
	// FileOutputStream
	// */
	// public static boolean copyFileDonNotClose(String sourceFilePath, String
	// destFilePath) {
	// InputStream inputStream = null;
	// try {
	// inputStream = new FileInputStream(sourceFilePath);
	// } catch (FileNotFoundException e) {
	// throw new RuntimeException("FileNotFoundException occurred. ", e);
	// }
	// return writeFile(destFilePath, inputStream);
	// }
	//
	// /**
	// * read file to string list, a element of list is a line
	// *
	// * @param filePath
	// * @param charsetName The name of a supported {@link
	// java.nio.charset.Charset </code>charset<code>}
	// * @return if file not exist, return null, else return content of file
	// * @throws RuntimeException if an error occurs while operator
	// BufferedReader
	// */
	// public static List<String> readFileToList(String filePath, String
	// charsetName) {
	// File file = new File(filePath);
	// List<String> fileContent = new ArrayList<String>();
	// if (file == null || !file.isFile()) {
	// return null;
	// }
	//
	// BufferedReader reader = null;
	// try {
	// InputStreamReader is = new InputStreamReader(new FileInputStream(file),
	// charsetName);
	// reader = new BufferedReader(is);
	// String line = null;
	// while ((line = reader.readLine()) != null) {
	// fileContent.add(line);
	// }
	// reader.close();
	// return fileContent;
	// } catch (IOException e) {
	// throw new RuntimeException("IOException occurred. ", e);
	// } finally {
	// if (reader != null) {
	// try {
	// reader.close();
	// } catch (IOException e) {
	// throw new RuntimeException("IOException occurred. ", e);
	// }
	// }
	// }
	// }
	//
	// /**
	// * get file name from path, not include suffix
	// *
	// * <pre>
	// * getFileNameWithoutExtension(null) = null
	// * getFileNameWithoutExtension("") = ""
	// * getFileNameWithoutExtension("   ") = "   "
	// * getFileNameWithoutExtension("abc") = "abc"
	// * getFileNameWithoutExtension("a.mp3") = "a"
	// * getFileNameWithoutExtension("a.b.rmvb") = "a.b"
	// * getFileNameWithoutExtension("c:\\") = ""
	// * getFileNameWithoutExtension("c:\\a") = "a"
	// * getFileNameWithoutExtension("c:\\a.b") = "a"
	// * getFileNameWithoutExtension("c:a.txt\\a") = "a"
	// * getFileNameWithoutExtension("/home/admin") = "admin"
	// * getFileNameWithoutExtension("/home/admin/a.txt/b.mp3") = "b"
	// * </pre>
	// *
	// * @param filePath
	// * @return file name from path, not include suffix
	// * @see
	// */
	// public static String getFileNameWithoutExtension(String filePath) {
	// if (StringUtils.isEmpty(filePath)) {
	// return filePath;
	// }
	//
	// int extenPosi = filePath.lastIndexOf(FILE_EXTENSION_SEPARATOR);
	// int filePosi = filePath.lastIndexOf(File.separator);
	// if (filePosi == -1) {
	// return (extenPosi == -1 ? filePath : filePath.substring(0, extenPosi));
	// }
	// if (extenPosi == -1) {
	// return filePath.substring(filePosi + 1);
	// }
	// return (filePosi < extenPosi ? filePath.substring(filePosi + 1,
	// extenPosi) : filePath.substring(filePosi + 1));
	// }
	//
	// /**
	// * get file name from path, include suffix
	// *
	// * <pre>
	// * getFileName(null) = null
	// * getFileName("") = ""
	// * getFileName("   ") = "   "
	// * getFileName("a.mp3") = "a.mp3"
	// * getFileName("a.b.rmvb") = "a.b.rmvb"
	// * getFileName("abc") = "abc"
	// * getFileName("c:\\") = ""
	// * getFileName("c:\\a") = "a"
	// * getFileName("c:\\a.b") = "a.b"
	// * getFileName("c:a.txt\\a") = "a"
	// * getFileName("/home/admin") = "admin"
	// * getFileName("/home/admin/a.txt/b.mp3") = "b.mp3"
	// * </pre>
	// *
	// * @param filePath
	// * @return file name from path, include suffix
	// */
	// public static String getFileName(String filePath) {
	// if (StringUtils.isEmpty(filePath)) {
	// return filePath;
	// }
	//
	// int filePosi = filePath.lastIndexOf(File.separator);
	// return (filePosi == -1) ? filePath : filePath.substring(filePosi + 1);
	// }
	//
	// /**
	// * get folder name from path
	// *
	// * <pre>
	// * getFolderName(null) = null
	// * getFolderName("") = ""
	// * getFolderName("   ") = ""
	// * getFolderName("a.mp3") = ""
	// * getFolderName("a.b.rmvb") = ""
	// * getFolderName("abc") = ""
	// * getFolderName("c:\\") = "c:"
	// * getFolderName("c:\\a") = "c:"
	// * getFolderName("c:\\a.b") = "c:"
	// * getFolderName("c:a.txt\\a") = "c:a.txt"
	// * getFolderName("c:a\\b\\c\\d.txt") = "c:a\\b\\c"
	// * getFolderName("/home/admin") = "/home"
	// * getFolderName("/home/admin/a.txt/b.mp3") = "/home/admin/a.txt"
	// * </pre>
	// *
	// * @param filePath
	// * @return
	// */
	// public static String getFolderName(String filePath) {
	//
	// if (StringUtils.isEmpty(filePath)) {
	// return filePath;
	// }
	//
	// int filePosi = filePath.lastIndexOf(File.separator);
	// return (filePosi == -1) ? "" : filePath.substring(0, filePosi);
	// }
	//
	// /**
	// * get suffix of file from path
	// *
	// * <pre>
	// * getFileExtension(null) = ""
	// * getFileExtension("") = ""
	// * getFileExtension("   ") = "   "
	// * getFileExtension("a.mp3") = "mp3"
	// * getFileExtension("a.b.rmvb") = "rmvb"
	// * getFileExtension("abc") = ""
	// * getFileExtension("c:\\") = ""
	// * getFileExtension("c:\\a") = ""
	// * getFileExtension("c:\\a.b") = "b"
	// * getFileExtension("c:a.txt\\a") = ""
	// * getFileExtension("/home/admin") = ""
	// * getFileExtension("/home/admin/a.txt/b") = ""
	// * getFileExtension("/home/admin/a.txt/b.mp3") = "mp3"
	// * </pre>
	// *
	// * @param filePath
	// * @return
	// */
	// public static String getFileExtension(String filePath) {
	// if (StringUtils.isBlank(filePath)) {
	// return filePath;
	// }
	//
	// int extenPosi = filePath.lastIndexOf(FILE_EXTENSION_SEPARATOR);
	// int filePosi = filePath.lastIndexOf(File.separator);
	// if (extenPosi == -1) {
	// return "";
	// }
	// return (filePosi >= extenPosi) ? "" : filePath.substring(extenPosi + 1);
	// }
	//
	// /**
	// * Creates the directory named by the trailing filename of this file,
	// including the complete directory path required
	// * to create this directory. <br/>
	// * <br/>
	// * <ul>
	// * <strong>Attentions:</strong>
	// * <li>makeDirs("C:\\Users\\Trinea") can only create users folder</li>
	// * <li>makeFolder("C:\\Users\\Trinea\\") can create Trinea folder</li>
	// * </ul>
	// *
	// * @param filePath
	// * @return true if the necessary directories have been created or the
	// target directory already exists, false one of
	// * the directories can not be created.
	// * <ul>
	// * <li>if {@link FileUtils#getFolderName(String)} return null, return
	// false</li>
	// * <li>if target directory already exists, return true</li>
	// * <li>return {@link java.io.File#makeFolder}</li>
	// * </ul>
	// */
	// public static boolean makeDirs(String filePath) {
	// String folderName = getFolderName(filePath);
	// if (StringUtils.isEmpty(folderName)) {
	// return false;
	// }
	//
	// File folder = new File(folderName);
	// return (folder.exists() && folder.isDirectory()) ? true :
	// folder.mkdirs();
	// }
	//
	// /**
	// * @param filePath
	// * @return
	// * @see #makeDirs(String)
	// */
	// public static boolean makeFolders(String filePath) {
	// return makeDirs(filePath);
	// }
	//
	// /**
	// * Indicates if this file represents a file on the underlying file system.
	// *
	// * @param filePath
	// * @return
	// */
	// public static boolean isFileExist(String filePath) {
	// if (StringUtils.isBlank(filePath)) {
	// return false;
	// }
	//
	// File file = new File(filePath);
	// return (file.exists() && file.isFile());
	// }
	//
	// /**
	// * Indicates if this file represents a directory on the underlying file
	// system.
	// *
	// * @param directoryPath
	// * @return
	// */
	// public static boolean isFolderExist(String directoryPath) {
	// if (StringUtils.isBlank(directoryPath)) {
	// return false;
	// }
	//
	// File dire = new File(directoryPath);
	// return (dire.exists() && dire.isDirectory());
	// }
	//
	// /**
	// * delete file or directory
	// * <ul>
	// * <li>if path is null or empty, return true</li>
	// * <li>if path not exist, return true</li>
	// * <li>if path exist, delete recursion. return true</li>
	// * <ul>
	// *
	// * @param path
	// * @return
	// */
	// public static boolean deleteFile(String path) {
	// if (StringUtils.isBlank(path)) {
	// return true;
	// }
	//
	// File file = new File(path);
	// if (!file.exists()) {
	// return true;
	// }
	// if (file.isFile()) {
	// return file.delete();
	// }
	// if (!file.isDirectory()) {
	// return false;
	// }
	//
	// for (File f : file.listFiles()) {
	// if (f.isFile()) {
	// f.delete();
	// } else if (f.isDirectory()) {
	// deleteFile(f.getAbsolutePath());
	// }
	// }
	// return file.delete();
	// }
	// /**
	// * delete file or directory
	// * <ul>
	// * <li>if path is null or empty, return true</li>
	// * <li>if path not exist, return true</li>
	// * <li>if path exist, delete recursion. return true</li>
	// * <ul>
	// *
	// * @param path
	// * @return
	// */
	// public static long deleteFileAndGetSize(String path) {
	// long totalSize = 0;
	// if (StringUtils.isBlank(path)) {
	// return 0;
	// }
	//
	// File file = new File(path);
	// if (!file.exists()) {
	// return 0;
	// }
	// if (file.isFile()) {
	// totalSize = file.length();
	// if(file.delete()){
	// return totalSize;
	// }else{
	// return 0;
	// }
	//
	// }
	// if (!file.isDirectory()) {
	// return 0;
	// }
	//
	// for (File f : file.listFiles()) {
	// long size = 0;
	// if (f.isFile()) {
	// size = f.length();
	// if(f.delete()){
	// totalSize += size;
	// }else{
	// totalSize += 0;
	// }
	// } else if (f.isDirectory()) {
	// totalSize += deleteFileAndGetSize(f.getAbsolutePath());
	// }
	// }
	// return totalSize;
	// }
	//
	// /**
	// * 获得指定目录的大小(递归)
	// * @param path
	// * @return
	// */
	// public static long getDirSize(String path) {
	// long totalSize = 0;
	// if (StringUtils.isBlank(path)) {
	// return 0;
	// }
	//
	// File file = new File(path);
	// if (!file.exists()) {
	// return 0;
	// }
	// if (file.isFile()) {
	// totalSize = file.length();
	// return totalSize;
	// }
	// if (!file.isDirectory()) {
	// return 0;
	// }
	//
	// for (File f : file.listFiles()) {
	// long size = 0;
	// if (f.isFile()) {
	// size = f.length();
	// totalSize += size;
	// }
	// else if (f.isDirectory()) {
	// totalSize += getDirSize(f.getAbsolutePath());
	// }
	// }
	// return totalSize;
	// }
	//
	// /**
	// * get file size
	// * <ul>
	// * <li>if path is null or empty, return -1</li>
	// * <li>if path exist and it is a file, return file size, else return
	// -1</li>
	// * <ul>
	// *
	// * @param path
	// * @return returns the length of this file in bytes. returns -1 if the
	// file does not exist.
	// */
	// public static long getFileSize(String path) {
	// if (StringUtils.isBlank(path)) {
	// return -1;
	// }
	//
	// File file = new File(path);
	// return (file.exists() && file.isFile() ? file.length() : -1);
	// }
	//
	/**
	 * wxj， 转化成字节数组
	 * 
	 * @param file
	 *            源文件
	 * @return
	 */
	public static byte[] toBytes(File file) {
		if (file == null || !file.exists()) {
			return null;
		}

		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		FileInputStream fis = null;
		try {
			fis = new FileInputStream(file);
			byte[] buffer = new byte[1024];
			int len = -1;
			while ((len = fis.read(buffer)) != -1) {
				baos.write(buffer, 0, len);
			}
		} catch (IOException err) {
			return null;
		} finally {
			try {
				baos.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			try {
				fis.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		return baos.toByteArray();
	}

	public static byte[] toBytes(InputStream fis) {
		if (fis == null) {
			return null;
		}

		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		try {
			byte[] buffer = new byte[1024];
			int len = -1;
			while ((len = fis.read(buffer)) != -1) {
				baos.write(buffer, 0, len);
			}
		} catch (IOException err) {
			return null;
		} finally {
			try {
				baos.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			try {
				fis.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		return baos.toByteArray();
	}

	// 默认字符编码
	public static final String CHARSET = "utf-8";

	/**
	 * 保存图片
	 * 
	 * @param bitmap
	 *            图片
	 * @param destFile
	 *            目标文件
	 * @return
	 */
	public static boolean saveImage(Bitmap bitmap, File destFile) {
		if (bitmap == null || destFile == null) {
			return false;
		}

		FileOutputStream fos = null;
		try {
			fos = new FileOutputStream(destFile);
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			bitmap.compress(Bitmap.CompressFormat.JPEG, 100, baos);
			// int options = 100;
			// 如果大于80kb则再次压缩,最多压缩三次
			// while (baos.toByteArray().length / 1024 > 50 && options != 10) {
			// // 清空baos
			// baos.reset();
			// // 这里压缩options%，把压缩后的数据存放到baos中
			// bitmap.compress(Bitmap.CompressFormat.JPEG, options, baos);
			// options -= 30;
			// }
			fos.write(baos.toByteArray());
			fos.close();
			baos.close();
			// 保存到文件之后，销毁bitmap
			// destroyBitmap(bitmap);
			return true;
		} catch (Exception e) {
			// Log.e("wxj", "saveImage error:" + e);
		} finally {
			try {
				fos.close();
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		return false;
	}

	/**
	 * 删除文件
	 * 
	 * @param file
	 *            被删除的文件
	 * @return
	 */
	public static boolean deleteFile(File file) {
		if (file == null || !file.exists()) {
			return false;
		}
		file.delete();
		return true;
	}

	/**
	 * 销毁位图
	 * 
	 * @param bitmap
	 */
	public static void destroyBitmap(Bitmap bitmap) {
		if (bitmap == null) {
			return;
		}

		if (!bitmap.isRecycled()) {
			bitmap.recycle();
			bitmap = null;
			System.gc();
			// Log.e("wxj", "销毁bitmap成功");
		}
	}

	/**
	 * 销毁绘图
	 * 
	 * @param drawable
	 *            可以来自ImageView中的drawable，且是BitmapDrawable的实例
	 */
	public static void destroyDrawable(Drawable drawable) {
		if (drawable == null) {
			return;
		}
		BitmapDrawable bd = null;
		try {
			bd = (BitmapDrawable) drawable;
		} catch (Exception e) {
			return;
		}

		destroyBitmap(bd.getBitmap());

	}

	/**
	 * 保存字符串内容
	 * 
	 * @param content
	 *            字符串内容
	 * @param destFile
	 *            保存到的文件
	 * @param charset
	 *            字符编码，可null，默认为utf-8
	 */
	public static boolean saveContent(String content, File destFile, String charset) {
		if (content == null || destFile == null) {
			return false;
		}

		StringReader sr = new StringReader(content);
		OutputStreamWriter osw = null;
		try {
			osw = new OutputStreamWriter(new FileOutputStream(destFile), charset == null ? CHARSET : charset);
			char[] buf = new char[1024];
			int len = 0;
			while ((len = sr.read(buf)) != -1) {
				osw.write(buf, 0, len);
			}

			return true;
		} catch (Exception err) {
			err.printStackTrace();
		} finally {
			sr.close();
			try {
				osw.flush();
				osw.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		return false;
	}

	/**
	 * 读取文件
	 * 
	 * @param file
	 *            要读取的文件
	 * @param charset
	 *            字符编码，可null，默认为utf-8
	 * @return 返回字符串
	 */
	public static String readFile(File file, String charset) {
		if (file == null || !file.exists()) {
			return "";
		}

		StringWriter sw = new StringWriter();
		InputStreamReader isr = null;
		try {
			isr = new InputStreamReader(new FileInputStream(file), charset == null ? CHARSET : charset);
			char[] buf = new char[1024];
			int len = 0;
			while ((len = isr.read(buf)) != -1) {
				sw.write(buf, 0, len);
			}

			return sw.toString();

		} catch (Exception err) {
			err.printStackTrace();
		} finally {
			try {
				isr.close();
			} catch (IOException e) {
			}
			try {
				sw.close();
			} catch (IOException e) {
			}
		}

		return "";
	}

	/**
	 * 读取源文件内容
	 * 
	 * @param filename
	 *            String 文件路径
	 * @throws IOException
	 * @return byte[] 文件内容
	 */
	public static byte[] readFile(String filename) throws IOException {

		File file = new File(filename);
		if (filename == null || filename.equals("")) {
			throw new NullPointerException("无效的文件路径");
		}
		long len = file.length();
		byte[] bytes = new byte[(int) len];

		BufferedInputStream bufferedInputStream = new BufferedInputStream(new FileInputStream(file));
		int r = bufferedInputStream.read(bytes);
		if (r != len)
			// throw new IOException("读取文件不正确");
			bufferedInputStream.close();

		return bytes;

	}

	/**
	 * 将数据写入文件
	 * 
	 * @param data
	 *            byte[]
	 * @throws IOException
	 */
	public static void writeFile(byte[] data, String filename) throws IOException {
		File file = new File(filename);
		file.getParentFile().mkdirs();
		BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(new FileOutputStream(file));
		bufferedOutputStream.write(data);
		bufferedOutputStream.close();

	}

	/**
	 * 从jar文件里读取class
	 * 
	 * @param filename
	 *            String
	 * @throws IOException
	 * @return byte[]
	 */
	public byte[] readFileJar(String filename) throws IOException {
		BufferedInputStream bufferedInputStream = new BufferedInputStream(getClass().getResource(filename).openStream());
		int len = bufferedInputStream.available();
		byte[] bytes = new byte[len];
		int r = bufferedInputStream.read(bytes);
		if (len != r) {
			bytes = null;
			throw new IOException("读取文件不正确");
		}
		bufferedInputStream.close();
		return bytes;
	}

	/**
	 * 读取网络流，为了防止中文的问题，在读取过程中没有进行编码转换，而且采取了动态的byte[]的方式获得所有的byte返回
	 * 
	 * @param bufferedInputStream
	 *            BufferedInputStream
	 * @throws IOException
	 * @return byte[]
	 */
	public byte[] readUrlStream(BufferedInputStream bufferedInputStream) throws IOException {
		byte[] bytes = new byte[100];
		byte[] bytecount = null;
		int n = 0;
		int ilength = 0;
		while ((n = bufferedInputStream.read(bytes)) >= 0) {
			if (bytecount != null)
				ilength = bytecount.length;
			byte[] tempbyte = new byte[ilength + n];
			if (bytecount != null) {
				System.arraycopy(bytecount, 0, tempbyte, 0, ilength);
			}

			System.arraycopy(bytes, 0, tempbyte, ilength, n);
			bytecount = tempbyte;

			if (n < bytes.length)
				break;
		}
		return bytecount;
	}

	/**
	 * 删除单个文件
	 * 
	 * @param filePath
	 *            被删除文件的文件名
	 * @return 文件删除成功返回true，否则返回false
	 */
	public static boolean deleteFile(String filePath) {
		File file = new File(filePath);
		if (file.isFile() && file.exists()) {
			return file.delete();
		}
		return false;
	}

    public static void createDir(String dir) {
        if (TextUtils.isEmpty(dir)) {
            return;
        }
        try {
            File file = new File(dir);
            if (file.exists()) {
                if (!file.isDirectory()) {
                    file.delete();
                    new File(dir).mkdirs();
                }
            } else {
                file.mkdirs();
            }
        } catch (Throwable t)  {
            LogTool.w(TAG, t.toString());
        }
    }

	/**
	 * 删除文件夹以及目录下的文件
	 * 
	 * @param filePath
	 *            被删除目录的文件路径
	 * @return 目录删除成功返回true，否则返回false
	 */
	public static boolean deleteDirectory(String filePath) {
		boolean flag = false;
		// 如果filePath不以文件分隔符结尾，自动添加文件分隔符
		if (!filePath.endsWith(File.separator)) {
			filePath = filePath + File.separator;
		}
		File dirFile = new File(filePath);
		if (!dirFile.exists() || !dirFile.isDirectory()) {
			return false;
		}
		flag = true;
		File[] files = dirFile.listFiles();
		// 遍历删除文件夹下的所有文件(包括子目录)
		for (int i = 0; i < files.length; i++) {
			if (files[i].isFile()) {
				// 删除子文件
				flag = deleteFile(files[i].getAbsolutePath());
				if (!flag)
					break;
			} else {
				// 删除子目录
				flag = deleteDirectory(files[i].getAbsolutePath());
				if (!flag)
					break;
			}
		}
		if (!flag)
			return false;
		// 删除当前空目录
		return dirFile.delete();
	}

	/**
	 * 根据路径删除指定的目录或文件，无论存在与否
	 * 
	 * @param filePath
	 *            要删除的目录或文件
	 * @return 删除成功返回 true，否则返回 false。
	 */
	public static boolean DeleteFolder(String filePath) {
		File file = new File(filePath);
		if (!file.exists()) {
			return false;
		} else {
			if (file.isFile()) {
				// 为文件时调用删除文件方法
				return deleteFile(filePath);
			} else {
				// 为目录时调用删除目录方法
				return deleteDirectory(filePath);
			}
		}
	}

	/**
	 * @description
	 * @param sourceDir
	 *            最后不需要加斜杠 /
	 * @param targetDir
	 *            最后不需要加斜杠 /
	 * @throws IOException
	 */
	public static void copyDirectiory(String sourceDir, String targetDir) throws IOException {
		// 新建目标目录
		(new File(targetDir)).mkdirs();

		// 获取源文件夹当下的文件或目录
		File[] file = (new File(sourceDir)).listFiles();
		if (file != null) {
			for (int i = 0; i < file.length; i++) {
				if (file[i].isFile()) {
					// 源文件
					File sourceFile = file[i];
					// 目标文件
					File targetFile = new File(new File(targetDir).getAbsolutePath() + File.separator + file[i].getName());
					copyFile(sourceFile.getAbsolutePath(), targetFile.getAbsolutePath());
				}

				if (file[i].isDirectory()) {
					// 准备复制的源文件夹
					String dir1 = sourceDir + File.separator + file[i].getName();
					// 准备复制的目标文件夹
					String dir2 = targetDir + File.separator + file[i].getName();
					copyDirectiory(dir1, dir2);
				}
			}
		}
	}

	public static boolean downLoadBitmap(String imageUrl, String savePath) {
		HttpURLConnection con = null;
		FileOutputStream fos = null;
		BufferedOutputStream bos = null;
		BufferedInputStream bis = null;
		File imageFile = null;
		try {
			URL url = new URL(imageUrl);
			con = (HttpURLConnection) url.openConnection();
			con.setConnectTimeout(5 * 1000);
			con.setReadTimeout(15 * 1000);
			con.setDoInput(true);
			bis = new BufferedInputStream(con.getInputStream());
			imageFile = new File(savePath);
			fos = new FileOutputStream(imageFile);
			bos = new BufferedOutputStream(fos);
			byte[] b = new byte[1024];
			int length;
			while ((length = bis.read(b)) != -1) {
				bos.write(b, 0, length);
				bos.flush();
			}
			return true;
		} catch (Exception e) {
			if (imageFile != null)
				imageFile.delete();
			return false;
		} finally {
			try {
				if (bis != null) {
					bis.close();
				}
				if (bos != null) {
					bos.close();
				}
				if (con != null) {
					con.disconnect();
				}
			} catch (IOException e) {
			}
		}
	}
	
	public static String readFile(InputStream inputStream, String charset) {
		if (null == inputStream) {
			return "";
		}
		StringWriter sw = new StringWriter();
		InputStreamReader isr = null;
		try {
			isr = new InputStreamReader(inputStream, charset == null ? CHARSET : charset);
			char[] buf = new char[1024];
			int len = 0;
			while ((len = isr.read(buf)) != -1) {
				sw.write(buf, 0, len);
			}

			return sw.toString();

		} catch (Exception err) {
			err.printStackTrace();
		} finally {
			try {
				isr.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			try {
				sw.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		return "";
	}
	
	public static boolean saveContent(String content, FileOutputStream outputStream, String charset) {
		if (null == outputStream) {
			return false;
		}
		StringReader sr = new StringReader(content);
		OutputStreamWriter osw = null;
		try {
			osw = new OutputStreamWriter(outputStream, charset == null ? CHARSET : charset);
			char[] buf = new char[1024];
			int len = 0;
			while ((len = sr.read(buf)) != -1) {
				osw.write(buf, 0, len);
			}

			return true;
		} catch (Exception err) {
			err.printStackTrace();
		} finally {
			sr.close();
			try {
				osw.flush();
				osw.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		return false;
	}

	public static ArrayList<String> getFileLines(File file) {
		try {
			FileReader fileReader = new FileReader(file);
			BufferedReader bufferedReader = new BufferedReader(fileReader);
			ArrayList<String> lines = new ArrayList<String>();
			String content = null;
			while ((content = bufferedReader.readLine()) != null) {
				lines.add(content);
			}
			return lines;
		} catch (IOException e) {
			e.printStackTrace();
		}

		return null;
	}

	public static void copyFileDonNotClose(InputStream inputStream, OutputStream outputStream) {
		try {
			byte[] buffer = new byte[2048];
			while (inputStream.read(buffer) != -1) {
				outputStream.write(buffer);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void copyFileClose(InputStream inputStream, OutputStream outputStream) {
		try {
			copyFileDonNotClose(inputStream, outputStream);
			inputStream.close();
			outputStream.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void copyFileClose(File inFile, File outFile) {
		try {
			InputStream inputStream = new FileInputStream(inFile);
			OutputStream outputStream = new FileOutputStream(outFile);
			copyFileClose(inputStream, outputStream);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void copyFileClose(InputStream inputStream, File outFile) {
		try {
			OutputStream outputStream = new FileOutputStream(outFile);
			copyFileClose(inputStream, outputStream);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

    /**=============================start xmod method=======================================**/
    /**
     * 保存图片PNG无损
     *
     * @param bitmap   图片
     * @param destFile 目标文件
     * @return
     */
    public static boolean saveImageByPng(Bitmap bitmap, File destFile) {
        if (bitmap == null || destFile == null) {
            return false;
        }

        FileOutputStream fos = null;
        try {
            fos = new FileOutputStream(destFile);
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            bitmap.compress(Bitmap.CompressFormat.PNG, 100, baos);
            fos.write(baos.toByteArray());
            fos.close();
            baos.close();
            return true;
        } catch (Exception e) {
            Log.e("wxj", "saveImage error:" + e);
        } finally {
            try {
                fos.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return false;
    }

    public static void notifySystemMediaScanner(Context context, File file) {
        String filePath = file.getAbsolutePath();
        MediaScannerConnection.scanFile(context, new String[]{filePath}, null,
                new MediaScannerConnection.OnScanCompletedListener() {
                    @Override
                    public void onScanCompleted(String path, Uri uri) {
                        LogTool.i("MediaScanWork", "file " + path
                                + " was scanned seccessfully: " + uri);
                    }
                });
    }
    /**=============================end xmod method=======================================**/
}
