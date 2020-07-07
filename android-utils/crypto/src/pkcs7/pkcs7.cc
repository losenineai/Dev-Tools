/*PKCS7结构
*contentInfo : SEQUENCE
*	contentType : ObjectIdentifier  {data|signedData|envelopedData|signedAndEnvelopedData|digestedData|encryptedData}
* 	content		#内容由contentType决定
*
*contentInfo : SEQUENCE
*	contentType : ObjectIdentifier  {data}
	content : OCTETSTRING
*
*contentInfo : SEQUENCE
*	contentType : ObjectIdentifier  {signedData}
*	content[optional] : SEQUENCE 							#CERT.RSA是属于signedData类型
*		version : INTEGER 
*		digestAlgorithms : SET : DigestAlgorithmIdentifier  #消息摘要的算法
*		contentInfo : SEQUENCE   							#整个文件也是contentInfo结构
*		certificates[optional] : SEQUENCE 					#证书信息
*			tbsCertificate : SEQUENCE #
*				version : INTEGER
*				serialNumber : INTEGER  					#证书的序列号，由证书颁布者和序列号可以唯一确定证书
*				signature ： SEQUENCE : AlgorithmIdentifier
*				issuer : SET 								#证书颁布者
*				validity : SEQUENCE    						#证书的有效期
*				subject : SET #证书主体
*				subjectPublicKeyInfo : SEQUENCE 			#公钥相关信息，包含有加密算法和公钥
*				issuerUniqueID[optional] : BITSTRING 
*				subjectUniqueID[optional] : BITSTRING 
*				extensions[optional] : SEQUENCE  			#保存有证书扩展信息
*			signatureAlgorithm : AlgorithmIdentifier 		#签名算法 ，如常用的有 SHA256withRSA
*			signatureValue : BITSTRING 						#这是tbsCertificate部分的数字签名信息，防止tbsCertificate内容被修改
*		crls[optional] : SET 								#证书吊销列表
*		signerInfos : SET
			signerInfo : SEQUENCE							#签名者信息
*				version : INTEGER
*				issuerAndSerialNumber : SEQUENCE 			#证书的颁布者和序列号
*				digestAlgorithmId : SEQUENCE : DigestAlgorithmIdentifier #消息摘要的算法
*				authenticatedAttributes[optional] 
*				digestEncryptionAlgorithmId : SEQUENCE 			#签名算法
*				encryptedDigest : OCTETSTRING   			#私钥加密后的数据
*				unauthenticatedAttributes[optional] 
*
*每项的保存形式为{tag，length，content}
*/

#include "pkcs7.h"
#include "md5.h"
// #include "log.h"

/**
 * 构造函数，必须提供签名证书文件或者apk文件
 */
pkcs7::pkcs7()
{
	m_cert_file_count = 0;
	memset(m_cert_content, 0x0, sizeof(m_cert_content));
	memset(m_cert_file_length, 0x0, sizeof(m_cert_file_length));
	memset(m_cert_pos, 0x0, sizeof(m_cert_pos));
	memset(m_cert_head, 0x0, sizeof(m_cert_head));
	memset(m_cert_tail, 0x0, sizeof(m_cert_tail));
	memset(m_p_cert_cert, 0x0, sizeof(m_p_cert_cert));
	memset(m_cert_file, 0x0, sizeof(m_cert_file));

	apk_file = cert_file = NULL;
}

bool pkcs7::open_file(char *file_name)
{
	bool ret = get_content(file_name);

	if (ret == false) {
		// err_log("The file format is error!\n");
		return ret;
	}

	ret = parse_pkcs7();

	if (ret == false) {
		// err_log("parse the pkcs7 format error!\n");
		return ret;
	}
	return true;
}

pkcs7::~pkcs7()
{
	for(int i = 0; i < m_cert_file_count; i++) {
		element *p = m_cert_head[i];
		while (p != NULL) {
			m_cert_head[i] = p->next;
			free(p);
			p = m_cert_head[i];
		}

		free(m_cert_content[i]);
	}

	if (apk_file != NULL)
		free(apk_file);
	if (cert_file != NULL)
		free(cert_file);

}

/**
 * 该函数用于从apk中获取签名证书文件，
 * 若找到将该文件内容保存在m_content中，m_length为其长度
 *
 * 使用minizip库， 1）unzOpen64 打开apk文件；
 *                 2）unzGetGlobalInfo64 获取文件总数；
                   3）unzGoFirstFile 和 unzGoToNextFile 遍历文件；
				   4）unzGetCurrentFileInfo64 获取当前文件信息，对比找到签名证书文件；
				   5）unzOpenCurrentFilePass 打开当前文件；
				   6）unzReadCurrentFile 读取当前文件内容；
				   7）unzCloseCurrentFile 关闭当前文件；
				   8）unzClose 关闭apk文件。
 */
bool pkcs7::get_from_apk(char *file_name)
{
	unzFile uf = NULL;
	unz_file_info64 file_info;
	char filename_inzip[256];
	int err;
	
	uf = unzOpen64(file_name);
	if (uf == NULL) {
		// err_log("open apk file error!\n");
		return false;
	}

	apk_file = (char *)malloc(sizeof(char) * (strlen(file_name) + 1));
	strcpy(apk_file, file_name);

	unz_global_info64 gi;
	err = unzGetGlobalInfo64(uf, &gi); 
	if (err != UNZ_OK) {
		// err_log("error %d with zipfile in unzGetGlobalInfo \n", err);
		return false;
	}

	err = unzGoToFirstFile(uf);

	auto setContent = [&](int name_len) {

		int file_index = m_cert_file_count;

		m_cert_file[file_index] = (char *)malloc(sizeof(char) * (name_len + 1));
		strcpy(m_cert_file[file_index], filename_inzip);

		int err = unzOpenCurrentFilePassword(uf, NULL);
		if (err != UNZ_OK) {
			// err_log("open current error!\n");
			return  false;
		}

		/*获取文件内容*/
		m_cert_file_length[file_index] = file_info.uncompressed_size;
		if (m_cert_file_length[file_index] <= 0)
			return false;

		m_cert_content[file_index] = (unsigned char *)malloc((size_t)(m_cert_file_length[file_index]));
		err = unzReadCurrentFile(uf, m_cert_content[file_index], m_cert_file_length[file_index]);

		if (err != file_info.uncompressed_size) {
			// err_log("read content error!\n");
			return false;
		}

		m_cert_file_count++;

		return true;
	};

	bool bFindSignFile = false;
	for (int i = 0; i< gi.number_entry; i++) {

		if (err != UNZ_OK) {
			// err_log("get file error!\n");
			return false;
		}

		if (unzGetCurrentFileInfo64(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0))
		{
			// err_log("get file infomation error!\n");
			return false;
		}

		int name_len = strlen(filename_inzip);
		if (name_len != file_info.size_filename) {
			// err_log("file name length is not right!\n");
			return false;
		}

		if (name_len > 13) {// "META-INF/*.RSA"
			if ((!strncmp(filename_inzip, "META-INF/", 9)) &&
				(!STRCASECMP(filename_inzip + name_len - 4, ".RSA") ||
				!STRCASECMP(filename_inzip + name_len - 4, ".DSA") ||
				!STRCASECMP(filename_inzip + name_len - 3, ".EC")))
			{
				setContent(name_len);
				bFindSignFile = true;
			}
		}

		err = unzGoToNextFile(uf);
	}

	if (!bFindSignFile) {
		// err_log("cannot find the file!\n");
		return false;
	}

	unzCloseCurrentFile(uf);
	unzClose(uf);

	return true;
}

/**
 * 获取签名文件内容，支持：1）直接提供的是签名文件；2）apk压缩文件。
 */

bool pkcs7::get_content(char *file_name)
{
	int name_len = strlen(file_name);
	if (name_len < 4)
		return false;

	if (!STRCASECMP(file_name + name_len - 4, ".RSA") ||
		!STRCASECMP(file_name + name_len - 4, ".DSA") ||
		!STRCASECMP(file_name + name_len - 3, ".EC")) {

			FILE *f = fopen(file_name, "rb");
			if (f == NULL)
				return false;

			fseek(f, 0, SEEK_END);

			m_cert_file_length[0] = ftell(f);

			if (m_cert_file_length[0] == -1)
				return false;

			fseek(f, 0, SEEK_SET);
			m_cert_content[0] = (unsigned char *)malloc(sizeof(unsigned char) * m_cert_file_length[0]);

			if (fread(m_cert_content[0], 1, m_cert_file_length[0], f) != m_cert_file_length[0])
				return false;

			return true;
		}

	return get_from_apk(file_name);
}

/**
 * 根据lenbyte计算出 length所占的字节个数， 1）字节最高位为1，则低7位长度字节数；2）最高位为0，则lenbyte表示长度
 */
int pkcs7::len_num(unsigned char lenbyte)
{
	int num = 1;
	if (lenbyte & 0x80) {
		num += lenbyte & 0x7f;
	}
	return num;
}
/**
 * 将长度信息转化成ASN.1长度格式
 * len <= 0x7f       1
 * len >= 0x80       1 + 非零字节数
 */
int pkcs7::num_from_len(int len)
{
	int num = 0;
	int tmp = len;
	while (tmp) {
		num++;
		tmp >>= 8;
	}
	if ((num == 1 && len >= 0x80) || (num > 1))
		num += 1;
	return num;
}

/**
 *每个element元素都是{tag, length, data}三元组，tag和length分别由tag和len保存，data是由[begin, begin+len)保存。
 *
 *该函数是从data位置计算出到tag位置的偏移值
 */
int pkcs7::tag_offset(element *p, int index)
{
	if (p == NULL)
		return 0;

	int offset = num_from_len(p->len);

	if (m_cert_content[index][p->begin - offset - 1] == p->tag)
		return offset + 1;
	else	
		return 0;
}

/**
 * 根据lenbyte计算长度信息，算法是 lenbyte最高位为1， 则lenbyte & 0x7F表示length的字节长度，后续字节使用大端方式存放
 * 最高位为0， lenbyte直接表示长度
 *
 * 1)若 0x82 0x34 0x45 0x22 ....  0x82是lenbyte， 高位为1，0x82 & 0x7F == 2，则后续两个字节是高端存放的长度信息
    则长度信息为 0x3445
   2)若 lenbyte == 0x34， 最高位为0， 则长度信息是0x34
*/
int pkcs7::get_length(unsigned char lenbyte, int offset, int index)
{
	int len = 0, num;
	unsigned char tmp;
	if (lenbyte & 0x80) {
		num = lenbyte & 0x7f;
		if (num < 0 || num > 4) {
			// err_log("its too long !\n");
			return 0;
		}
		while (num) {
			len <<= 8;
			tmp = m_cert_content[index][offset++];
			len += (tmp & 0xff);
			num--;
		}
	} else {
		len = lenbyte & 0xff;
	}
	return len;
}

/**
 * 创建element.pkcs7中的每个元素都有对应element.
 */
int pkcs7::create_element(unsigned char tag, const char *name, int level, int index)
{
	unsigned char get_tag = m_cert_content[index][m_cert_pos[index]++];
	if (get_tag != tag) {
		m_cert_pos[index]--;
		return -1;
	}

	unsigned char lenbyte = m_cert_content[index][m_cert_pos[index]];
	int len = get_length(lenbyte, m_cert_pos[index] + 1, index);
	m_cert_pos[index] += len_num(lenbyte);
	
	element *node = (element *)malloc(sizeof(element));
	node->tag = get_tag;
	strcpy(node->name, name);
	node->begin = m_cert_pos[index];
	node->len = len;
	node->level = level;
	node->next = NULL;
	
	if (m_cert_head[index] == NULL) {
		m_cert_head[index] = m_cert_tail[index] = node;
	} else {
		m_cert_tail[index]->next = node;
		m_cert_tail[index] = node;
	}

	return len;
}

/**
 * 解析证书信息
 */
bool  pkcs7::parse_certificate(int level, int index)
{
	const char *names[] = {
			"tbsCertificate",
			"version",
			"serialNumber",
			"signature",
			"issuer",
			"validity",
			"subject",
			"subjectPublicKeyInfo",
			"issuerUniqueID-[optional]",
			"subjectUniqueID-[optional]",
			"extensions-[optional]",
			"signatureAlgorithm",
			"signatureValue" };

	int len = 0;
	unsigned char tag;
	bool have_version = false;

	len = create_element(TAG_SEQUENCE, names[0], level, index);
	if (len == -1 || m_cert_pos[index] + len > m_cert_file_length[index]) {
		return false;
	}

	//version
	tag = m_cert_content[index][m_cert_pos[index]];
	if (((tag & 0xc0) == 0x80) && ((tag & 0x1f) == 0)) {
		m_cert_pos[index] += 1;
		m_cert_pos[index] += len_num(m_cert_content[index][m_cert_pos[index]]);
		len = create_element(TAG_INTEGER, names[1], level + 1, index);

		if (len == -1 || m_cert_pos[index] + len > m_cert_file_length[index]) {
			return false;
		}

		m_cert_pos[index] += len;
		have_version = true;
	}

	for (int i = 2; i < 11; i++) {
		switch (i) {
			case 2: 
					tag = TAG_INTEGER;
					break;
			case 8:	
					tag = 0xA1;
					break;
			case 9:	
					tag = 0xA2;
					break;
			case 10:
					tag = 0xA3;
					break;
			default:
					tag = TAG_SEQUENCE;
		}
		len = create_element(tag, names[i], level + 1, index);

		if (i < 8 && len == -1) {
			return false;
		}

		if (len != -1)
			m_cert_pos[index] += len;
	}

	//signatureAlgorithm
	len = create_element(TAG_SEQUENCE, names[11], level, index);
	if (len == -1 || m_cert_pos[index] + len > m_cert_file_length[index]) {
		return false;
	}

	m_cert_pos[index] += len;

	//signatureValue
	len = create_element(TAG_BITSTRING, names[12], level, index);
	if (len == -1 || m_cert_pos[index] + len > m_cert_file_length[index]) {
		return false;
	}

	m_cert_pos[index] += len;

	return true;
}

/**
 * 解析签名者信息
 */
bool pkcs7::parse_signerInfo(int level, int index)
{
	const char *names[] = {
		"version",
		"issuerAndSerialNumber",
		"digestAlgorithmId",
		"authenticatedAttributes-[optional]",
		"digestEncryptionAlgorithmId",
		"encryptedDigest",
		"unauthenticatedAttributes-[optional]" };

	int len;
	unsigned char tag;

	for (int i = 0; i < sizeof(names)/sizeof(names[0]); i++) {
		switch (i) {
			case 0:
					tag = TAG_INTEGER;
					break;
			case 3:
					tag = 0xA0;
					break;
			case 5:
					tag = TAG_OCTETSTRING;
					break;
			case 6:
					tag = 0xA1;
					break;
			default:
					tag = TAG_SEQUENCE;
			
		}

		len = create_element(tag, names[i], level, index);
		if (len == -1 || m_cert_pos[index] + len > m_cert_file_length[index]) {
			if (i == 3 || i == 6)
				continue;
			return false;
		}
		m_cert_pos[index] += len;
	}

	// FIXME unused
	// int ret = (m_cert_pos[index] == m_cert_file_length[index] ? 1 : 0);

	return true;
}

/**
 * 解析 contentType == signedData 的content部分
 */
bool pkcs7::parse_content(int level, int index)
{

	const char *names[] = {"version",
					"DigestAlgorithms",
					"contentInfo",
					"certificates-[optional]",
					"crls-[optional]",
					"signerInfos",
					"signerInfo"};

	unsigned char tag;
	int len = 0;	
	//element *p = NULL;
	//version
	len = create_element(TAG_INTEGER, names[0], level, index);
	if (len == -1 || m_cert_pos[index] + len > m_cert_file_length[index]) {
		return false;
	}
	m_cert_pos[index] += len;

	//DigestAlgorithms
	len = create_element(TAG_SET, names[1], level, index);
	if (len == -1 || m_cert_pos[index] + len > m_cert_file_length[index]) {
		return false;
	}
	m_cert_pos[index] += len;

	//contentInfo
	len = create_element(TAG_SEQUENCE, names[2], level, index);
	if (len == -1 || m_cert_pos[index] + len > m_cert_file_length[index]) {
		return false;
	}
	m_cert_pos[index] += len;

	//certificates-[optional]
	tag = m_cert_content[index][m_cert_pos[index]];
	if (tag == TAG_OPTIONAL) {
		m_cert_pos[index]++;
		m_cert_pos[index] += len_num(m_cert_content[index][m_cert_pos[index]]);
		len = create_element(TAG_SEQUENCE, names[3], level, index);
		if (len == -1 || m_cert_pos[index] + len > m_cert_file_length[index]) {
			return false;
		}
		m_p_cert_cert[index] = m_cert_tail[index];
		bool ret = parse_certificate(level + 1, index);
		if (ret == false) {
			return ret;
		}
	}
	//crls-[optional]
	tag = m_cert_content[index][m_cert_pos[index]];
	if (tag == 0xA1) {
		m_cert_pos[index]++;
		m_cert_pos[index] += len_num(m_cert_content[index][m_cert_pos[index]]);
		len = create_element(TAG_SEQUENCE, names[4], level, index);
		if (len == -1 || m_cert_pos[index] + len > m_cert_file_length[index]) {
			return false;
		}
		m_cert_pos[index] += len;
	}

	//signerInfos
	tag = m_cert_content[index][m_cert_pos[index]];
	if (tag != TAG_SET) {
		return false;
	}

	len = create_element(TAG_SET, names[5], level, index);
	if (len == -1 || m_cert_pos[index] + len > m_cert_file_length[index]) {
		return false;
	}
	//signerInfo
	len = create_element(TAG_SEQUENCE, names[6], level + 1, index);
	if (len == -1 || m_cert_pos[index] + len > m_cert_file_length[index]) {
		return false;
	}

	return parse_signerInfo(level + 2, index);
}

/**
 * 解析文件开始函数
 */
bool pkcs7::parse_pkcs7()
{
	for(int i = 0; i < m_cert_file_count; i++) {

		unsigned char tag, lenbyte;
		int len = 0;
		int level = 0;

		tag = m_cert_content[i][m_cert_pos[i]++];

		if (tag != TAG_SEQUENCE) {
			// err_log("not found the Tag indicating an ASN.1!\n");
			continue;
		}

		lenbyte = m_cert_content[i][m_cert_pos[i]];
		len = get_length(lenbyte, m_cert_pos[i] + 1, i);
		m_cert_pos[i] += len_num(lenbyte);
		if (m_cert_pos[i] + len > m_cert_file_length[i])
			continue;

		//contentType
		len = create_element(TAG_OBJECTID, "contentType", level, i);
		if (len == -1) {
			// err_log("not found the ContentType!\n");
			continue;
		}

		m_cert_pos[i] += len;

		//optional
		tag = m_cert_content[i][m_cert_pos[i]];
		m_cert_pos[i] += 1;
		lenbyte = m_cert_content[i][m_cert_pos[i]];
		m_cert_pos[i] += len_num(lenbyte);

		//content-[optional]
		len = create_element(TAG_SEQUENCE, "content-[optional]", level, i);
		if (len == -1) {
			// err_log("not found the content!\n");
			continue;
		}

		parse_content(level + 1, i);

	}

	return true;
}

int pkcs7::get_count() {
    return m_cert_file_count;
}

/**
 * 获取证书信息的MD5
 */
int pkcs7::get_MD5_hex(char ret_md5[33], int index)
{
	memset(ret_md5, 0, 33);

	// warning: comparison of array 'this->m_p_cert_cert' equal to a null pointer is always false [-Wtautological-pointer-compare]
	// if (m_p_cert_cert == NULL)
	//	return 1;

	unsigned char md5_arr[16];

	int offset = tag_offset(m_p_cert_cert[index], index);
	if (offset == 0) {
		// err_log("get offset error!\n");
		return 1;
	}

	md5(m_cert_content[index] + m_p_cert_cert[index]->begin - offset, m_p_cert_cert[index]->len + offset, md5_arr);

	for (int i = 0; i < 16; i++) {
		unsigned char byte = md5_arr[i];
		char high = (((byte >> 4) >= 10) ? ((byte >> 4) - 10 + 'A') : ((byte >> 4) + '0'));
		char low = (((byte & 0x0F) >= 10) ? ((byte & 0x0F) - 10 + 'A') : ((byte & 0x0F) + '0'));
		ret_md5[i*2] = high;
		ret_md5[i*2+1] = low;
	}

	ret_md5[32] = '\0';
	return 0;
}
