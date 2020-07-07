
#ifndef __PKCS7__
#define __PKCS7__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unzip.h"
#include "zip.h"

#ifdef  _WIN32
#	define 	STRCASECMP		stricmp
#	define  FUNCTION_NAME	__FUNCTION__
#else
#	define 	STRCASECMP 		strcasecmp
#	define  FUNCTION_NAME  __func__
#endif

#ifdef  _WIN32
#pragma warning(disable:4996)
#endif 

#define TAG_INTEGER 	0x02
#define TAG_BITSTRING	0x03
#define TAG_OCTETSTRING 0x04
#define TAG_OBJECTID	0x06
#define TAG_UTCTIME		0x17
#define TAG_GENERALIZEDTIME 0x18   
#define TAG_SEQUENCE	0x30
#define TAG_SET			0x31

#define TAG_OPTIONAL	0xA0


#define NAME_LEN 	63

#define CERT_FILE_MAX_COUNT  10

typedef struct element {
	unsigned char tag;
	char name[NAME_LEN];
	int begin;
	int len;
	int level;
	struct element *next;
}element;

class pkcs7 {

public:
	pkcs7();
	~pkcs7();

	int get_cert_count();
	bool open_file(char *file_name);
	int get_count();
	int get_MD5_hex(char ret_md5[33], int index);

private:
	int  len_num(unsigned char lenbyte);
	int  num_from_len(int len);
	int  tag_offset(element *p, int index);

	int  get_length(unsigned char lenbyte, int pos, int index);

	bool get_from_apk(char *file_name);
	bool get_content(char *file_name);

	int create_element(unsigned char tag, const char *name, int level, int index);

	bool parse_content(int level, int index);
		bool parse_pkcs7();
		bool parse_certificate(int level, int index);
		bool parse_signerInfo(int level, int index);


		
		
		
	private:
//		unsigned char *	m_content;
//		int 			m_length;
//		int 			m_pos;
//		struct element *head;
//		struct element *tail;
//		struct element *p_cert;
//		struct element *p_signer;
//
		char *apk_file;
		char *cert_file;


		int				m_cert_file_count;
		unsigned char * m_cert_content[CERT_FILE_MAX_COUNT];
		int 			m_cert_file_length[CERT_FILE_MAX_COUNT];
		int 			m_cert_pos[CERT_FILE_MAX_COUNT];
		struct element *m_cert_head[CERT_FILE_MAX_COUNT];
		struct element *m_cert_tail[CERT_FILE_MAX_COUNT];
		struct element *m_p_cert_cert[CERT_FILE_MAX_COUNT];


		char* m_cert_file[CERT_FILE_MAX_COUNT];


};



#endif //__PKCS7__