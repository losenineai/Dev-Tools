package com.xxlib.config;

/**
 * 系统配置文件
 * @author wxj
 *
 */
public interface DanMuChattConfig {

	/**
	 * 语音录制模块
	 */
	public static final String DIR_AUDIO_FILES = "/inputsystem/record";			// 语音文件存放的文件夹,在应用sd的工作目录下
	public static final int MAX_LEVEL_MIC_VOLUME = 100;							// 麦克风音量最高等级,用于UI级别显示
	// Handler回调事件常量
	public static final int RESULT_RECORD_FAILED = 0;				// 录音失败
	public static final int RESULT_RECORD_VOLUME = 1;				// 录音音量
	public static final int RESULT_RECORD_PREPARE_STOP = 2;			// 录音准备停止
	public static final int RESULT_RECORD_STOP = 3;					// 录音停止
	public static final int RESULT_PLAY_FILE_FAILED = 4;			// 播放失败
	public static final int RESULT_MEDIA_PLAY_COMPLETED = 5;		// 多媒体播放完成
	public static final int MAX_RECORD_TIME = 10;					// 最长的语音录制时间，秒
	public static final int PROMPT_TIME_BEFORE_RECORD_STOP = 3;		// 语音强制停止前的提示时间，秒
	
	
	/**
	 * emoji表情模块
	 */
	public static final int MAX_EXPRESSION_COUNT = 51;				// 最多的表情数量
	public static final String EXPRESSION_CODE_PREFIX = "[e";		// emoji表情编码前缀
	public static final String EXPRESSION_CODE_SUFFIX = "&]";		// emoji表情编码后缀
	public static final String EXPRESSION_PATTERN = "\\[e[^]]*\\&]";		// 匹配emoji编码的正则
	public static final String EXPRESSION_DELETE_ICON = "icon_emoji_delete";		// emoji删除icon
    public static final String EXPRESSION_COMMON_PATTERN =  "[\\ud83c\\udc00-\\ud83c\\udfff]|[\\ud83d\\udc00-\\ud83d\\udfff]|[\\u2600-\\u27ff]";
	
	
	/**
	 * 选择图片模块
	 */
	public static final String DIR_PHOTO = "/inputsystem";					// 父级目录
	public static final int MAX_COUNT_SEND_IMAGE = 6;						// 最多发送的图片数目
	public static final String EXTRA_KEY_PHOTO_PATH = "photoPathExtraKey";	// Extra key图片路径
	public static final String EXTRA_KEY_PHOTO_PATH_LIST = "photoPathListExtraKey";	// Extra key图片路径列表
	public static final String EXTRA_KEY_MAX_SELECT_COUNT = "maxSelectCountExtraKey";	// Extra key最大的选择数
	/*
	 * 调用相机和相册请求码
	 */
	public static final int REQUEST_CODE_FROM_CAMERA = 0;					// 获得相机图片的请求码
	public static final int REQUEST_CODE_FROM_VALLERY = 1;					// 获取相册图片的请求码
	public static final int REQUEST_CODE_IF_SELECT_CAMERA_PHOTO = 2; 		// 是否选择相机图片的请求码
	public static final int REQUEST_CODE_IF_DELETE_PHOTO = 3; 				// 是否移除图片的请求码
	
}
