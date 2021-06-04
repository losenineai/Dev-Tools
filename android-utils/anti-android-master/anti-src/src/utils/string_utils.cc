//
// Created by white on 2018/5/16.
//

#include <cstring>
#include <regex>
#include <src/utils/include/utils_anti_log.h>
#include <dx-libc-ext/dx_libc_stdio.h>
#include <dx-libc-ext/dx_libc_utils.h>
#include "include/string_utils.h"
#include "md5.h"

string anti_str_replaceAll(const string &s, const string &search, const string &replace){
    string ns = s;

    for (size_t pos = 0;; pos += replace.length()) {
        pos = ns.find(search, pos);

        if (pos == string::npos) {
            break;
        }

        ns.erase(pos, search.length());
        ns.insert(pos, replace);
    }

    return ns;
}

void anti_str_splitString(const string &s, vector<string> &v, const string &c){
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;

    while (string::npos != pos2) {
        v.insert(v.end(), s.substr(pos1, pos2 - pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }

    if (pos1 != s.length()) {
        v.insert(v.end(), s.substr(pos1));
    }
}

string anti_str_format(const char *fmt, ...){
    char *buf = NULL;

    va_list vl;
    va_start(vl, fmt);
    vasprintf(&buf, fmt, vl);
    va_end(vl);

    string result(buf);
    delete_not_array(buf);

    return result;
}


string anti_str_MD5(const string &src) {
    uint8_t signature[16];

    only_md5((const unsigned char *) src.data(), src.size(), signature);

    string result((char *) signature, 16);

    return anti_str_charToHexString((unsigned char *)result.c_str(), result.size());
}

string anti_str_toHexString(const string &input){

    char *buf = new char[input.length() * 2 + 1];
    memset(buf, 0, input.length() * 2 + 1);

    for (int i = 0; i < input.size(); i++) {
        sprintf(buf + i * 2, "%02x", (uint8_t) input.at(i));
    }

    string res(buf, input.size() * 2);
    delete_array(buf)
    return res;
}

int anti_str_start_with(const char *str, const char *suffix){

    if (!str || !suffix)
        return 0;
    size_t lenA = strlen(str), lenB = strlen(suffix);
    if (lenB > lenA){
        return 0;
    }

    return strncmp(str, suffix, lenB) == 0;
}

int anti_str_end_with(const char *str, const char *suffix){

    if (!str || !suffix)
        return 0;
    size_t lenA = strlen(str);
    size_t lenB = strlen(suffix);
    if (lenB > lenA){
        return 0;
    }

    return strncmp(str + lenA - lenB, suffix, lenB) == 0;

}



void anti_str_containWithFilter(vector<string> &filters, vector<string> &src, vector<string> &result){
    if (src.size() == 0) {
        return;
    }
    for (auto iterInSrc = src.begin(); iterInSrc != src.end(); iterInSrc++) {

        string strLine = *iterInSrc;
        const char *line = strLine.c_str();
        size_t len = strlen(line);
        if (len <= 0) {
            continue;
        }
        for(auto iterInFilters = filters.begin(); iterInFilters != filters.end(); iterInFilters++){
            string filter = *iterInFilters;
            if (strstr(line, filter.c_str()) != NULL) {
                result.insert(result.end(), strLine);
                break;
            } else {

            }
        }
    }
}




string anti_str_charToHexString(unsigned char *pSrc, int nL) {

    char buf[nL * 2 + 1];
    string dest;

    memset((char *) buf, 0, sizeof(buf));

    unsigned char hb;
    unsigned char lb;

    for (int i = 0; i < nL; i++) {
        hb = (pSrc[i] & 0xf0) >> 4;

        if (hb >= 0 && hb <= 9)
            hb += 0x30;
        else if (hb >= 10 && hb <= 15)
            hb = hb - 10 + 'a';
        else
            return dest;

        lb = pSrc[i] & 0x0f;
        if (lb >= 0 && lb <= 9)
            lb += 0x30;
        else if (lb >= 10 && lb <= 15)
            lb = lb - 10 + 'a';
        else
            return dest;

        buf[i * 2] = hb;
        buf[i * 2 + 1] = lb;
    }

    dest = buf;

    return dest;
}

void anti_str_ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void anti_str_rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void anti_str_trim(std::string &s) {
    anti_str_ltrim(s);
    anti_str_rtrim(s);
}

// trim from start (copying)
std::string anti_str_ltrim_copy(std::string s) {
    anti_str_ltrim(s);
    return s;
}

// trim from end (copying)
std::string anti_str_rtrim_copy(std::string s) {
    anti_str_rtrim(s);
    return s;
}

// trim from both ends (copying)
std::string anti_str_trim_copy(std::string s) {
    anti_str_trim(s);
    return s;
}


void anti_str_trim_c(char *src) {
    // left
    int len = (int )strlen(src);
    char *p = src;
    while (*p != '\0' && isspace(*p) && len > 0){
        ++p;
        --len;
    }
    if (src != p) {
        memmove(src, p, len + 1);
    }

    // right
    int i = 0;
    for (i = (int )strlen(src) - 1; i > 0 && isspace(src[i]); --i);
    if (isspace(src[i])) {
        src[i] = '\0';
    }
    else {
        src[i + 1] = '\0';
    }

}