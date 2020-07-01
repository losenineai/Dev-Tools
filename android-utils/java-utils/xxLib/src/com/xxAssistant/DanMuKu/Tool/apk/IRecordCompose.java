package com.xxAssistant.DanMuKu.Tool.apk;

import java.util.List;

public interface IRecordCompose {

	boolean composeVideoAudio(String videoPath, String audioPath, String destPath);

	void mergeMp4List(final List<String> mp4FilePaths, final String outputPath, final MergeListener mergeListener);

	long getMp4Duration(String filePath);

	boolean convertVideoToH264Format(String srcPath, String outPath);

}
