#include "report_builder.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "conf.h"
#include "log.h"
#include "md5.h"
#if 1 && (ACC_OS_DOS32) && defined(__DJGPP__)
#include <crt0.h>
int _crt0_startup_flags = _CRT0_FLAG_UNIX_SBRK;
#endif

#if (ACC_OS_WIN32 || ACC_OS_WIN64) && (defined(__MINGW32__) || defined(__MINGW64__))
#include <windows.h>
#endif

report_builder::report_builder(const char* _report_name, const char* _pic_dir, const char* _zip_name, const char* _vendor_path, const char* _arg0){
    if(_report_name != NULL && _pic_dir != NULL && _zip_name != NULL && _arg0 != NULL){
        this->report_name = std::string(_report_name);
        this->pic_dir = std::string(_pic_dir);
        this->zip_name = std::string(_zip_name);
        this->arg0 = std::string(_arg0);
        char* vendor_path = this->getpath(_vendor_path);
        LOGD("vendor path : %s", vendor_path);
        this->vendor_fd = fopen(vendor_path, "r");
        if(this->vendor_fd == NULL){
            LOGE("open vendor fail");
        }
        this->fd = 0;
    }else{
        this->report_name = std::string("");
        this->fd = 0;
        this->pic_dir = std::string("");
        this->vendor_fd = NULL;
    }
}


bool report_builder::write_line(std::string line){
    if(this->fd<=0){
        return false;
    }
    ssize_t length = write(this->fd, line.c_str(), line.length());
    if(length != (ssize_t)line.length()){
        return false;
    }
    length = write(this->fd, "\n", strlen("\n"));
    if(length != 1){
        return false;
    }
    return true;
}

report_builder::~report_builder(){
    if(this->fd != 0){
        close(this->fd);
    }
    if(this->vendor_fd != NULL){
        fclose(this->vendor_fd);
    }
    this->fd = 0;
}

bool report_builder::store_u_len_info(const char* file_path){
    if(!is_gen_report()){
        return false;
    }
    int temp_fd = open(file_path, O_RDONLY, 0666);
    int file_len = lseek(temp_fd, 0, SEEK_END);
    close(temp_fd);
    temp_fd = 0;
    this->u_lens.push_back(file_len);

    return true;
}

bool report_builder::store_filenames(const char* file_path){
    if(!is_gen_report()){
        return false;
    }
    const char* name = strrchr(file_path, '/');
    if(name == NULL){
        name = strrchr(file_path, '\\');
        if(name == NULL){
            name = file_path;
        }else{
            name = name + 1;
        }
    }else{
        name = name + 1;
    }
    this->filenames.push_back(name);
    return true;
}

bool report_builder::store_c_len_info(const char* file_path){
    if(!is_gen_report()){
        return false;
    }
    
    int temp_fd = open(file_path, O_RDONLY, 0666);
    int file_len = lseek(temp_fd, 0, SEEK_END);
    close(temp_fd);
    temp_fd = 0;
    this->c_lens.push_back(file_len);

    return true;
}

bool report_builder::store_tasks(bool upx, int signature_check, int integrity_check, int anti_ida, int anti_debuging, int anti_dump){
    if(!is_gen_report()){
        return false;
    }
    
    int anti_static_analysis = 0;
    int anti_tamper = 0;
    int anti_running_time_attack = 0;
    int anti_running_debuging = 0;
    int hasupx = 0;
    if(upx || anti_ida){
        anti_static_analysis = 1;
    }
    if(upx && (signature_check || integrity_check)){
        anti_tamper = 2;
    }
    if(upx && anti_dump){
        anti_running_time_attack = 4;
    }
    if(upx && anti_debuging){
        anti_running_debuging = 8;
    }
    if(upx){
        hasupx = 16;
    }
    
    this->tasks.push_back(anti_static_analysis|anti_tamper|anti_running_time_attack|anti_running_debuging|hasupx);

    return true;
}

/*
** 返回值：0 --> 完成报告生成。
** 返回值：1 --> 无法生成报告。（so无法被保护，无需生成报告）
** 返回值：-1 --> 生成报告失败。（需要生成报告，但是由于无法预知原因而生成失败。）
** 返回值：2 --> 不生成报告。（即普通so保护，不需要生成报告）
*/
int report_builder::make_report(){
    if(!is_gen_report()){
        LOGD("do not making report");
        return 2;
    }
    
    #if (defined(__MINGW32__) || defined(__MINGW64__))
        this->fd = open(this->report_name.c_str(), O_TRUNC | O_CREAT | O_RDWR | O_BINARY, 0666);
    #else
        this->fd = open(this->report_name.c_str(), O_TRUNC | O_CREAT | O_RDWR, 0666);
    #endif

    if(this->fd <=0){
        LOGE("open report fail");
        return -1;
    }
    
    bool has_so_protected = false;

    std::string vendor_info;
    char buf[520];

    size_t title_index = this->zip_name.find_last_of('/');
    if(title_index==std::string::npos){
        title_index = this->zip_name.find_last_of('\\');
    }
    const char* title_filename = (this->zip_name.c_str()+title_index+1);
    sprintf(buf, "# <center>%s</center>", title_filename);
    this->write_line(buf);

    vendor_info = this->get_value("title");
    sprintf(buf, "# <center>%s</center>", vendor_info.c_str());
    this->write_line(buf);
    
    time_t now = time(0);
    tm *ltm = localtime(&now);

    vendor_info = this->get_value("vendor");
    sprintf(buf, "### <center>%d-%02d-%02d %s</center>", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, vendor_info.c_str());
    this->write_line(buf);
    
    vendor_info = this->get_value("taskinfo");
    sprintf(buf, "## %s", vendor_info.c_str());
    this->write_line(buf);
    int anti_static_analysis = 0;
    int anti_tamper = 0;
    int anti_running_time_attack = 0;
    int anti_running_debuging = 0;

    for(size_t i=0; i<this->tasks.size(); i++){
        int task_info = this->tasks.at(i);
        if(task_info & 1){
            vendor_info = this->get_value("task1");
            sprintf(buf, "* %s", vendor_info.c_str());
            this->write_line(buf);
            anti_static_analysis = 1;
            break;
        }
    }
    
    for(size_t i=0; i<this->tasks.size(); i++){
        int task_info = this->tasks.at(i);
        if(task_info & 2){
            vendor_info = this->get_value("task2");
            sprintf(buf, "* %s", vendor_info.c_str());
            this->write_line(buf);
            anti_tamper = 1;
            break;
        }
    }

    for(size_t i=0; i<this->tasks.size(); i++){
        int task_info = this->tasks.at(i);
        if(task_info & 4){
            vendor_info = this->get_value("task3");
            sprintf(buf, "* %s", vendor_info.c_str());
            this->write_line(buf);
            anti_running_time_attack = 1;
            break;
        }
    }

    for(size_t i=0; i<this->tasks.size(); i++){
        int task_info = this->tasks.at(i);
        if(task_info & 8){
            vendor_info = this->get_value("task4");
            sprintf(buf, "* %s", vendor_info.c_str());
            this->write_line(buf);
            anti_running_debuging = 1;
            break;
        }
    }
    this->write_line("");

    vendor_info = this->get_value("baseinfo");
    sprintf(buf, "## %s", vendor_info.c_str());
    this->write_line(buf);

    char* md5 = MD5_file(this->zip_name.c_str(), 32);
    int temp_fd = open(this->zip_name.c_str(), O_RDONLY, 0666);
    float file_len = lseek(temp_fd, 0, SEEK_END);
    close(temp_fd);
    temp_fd = 0;
    file_len /= (1024*1024);
    size_t index = this->zip_name.find_last_of('/');
    if(index==std::string::npos){
        index = this->zip_name.find_last_of('\\');
    }
    const char* filename = (this->zip_name.c_str()+index+1);

    LOGD("file name : %s", filename);

    sprintf(buf, "* 文件名：%s", filename);
    this->write_line(buf);
    sprintf(buf, "* 大小：%0.4fMB", file_len);
    this->write_line(buf);
    sprintf(buf, "* MD5：%s", md5);
    this->write_line(buf);

    this->write_line("");

    vendor_info = this->get_value("protectInfo");
    sprintf(buf, "## %s", vendor_info.c_str());
    this->write_line(buf);

    vendor_info = this->get_value("protectRecord");
    sprintf(buf, "### %s", vendor_info.c_str());
    this->write_line(buf);
    for(size_t i=0; i<this->tasks.size(); i++){
        std::string temp_name = this->filenames.at(i);
        if(this->protected_info.at(i) == 1){
            vendor_info = this->get_value("protect_fail1");
            sprintf(buf, "* `%s (%d-->%d)`，%s  ", temp_name.c_str(), this->u_lens.at(i), this->c_lens.at(i), vendor_info.c_str());
            this->write_line(buf);
        }else if(this->protected_info.at(i) == 2){
            vendor_info = this->get_value("protect_fail2");
            sprintf(buf, "* `%s (%d-->%d)`，%s  ", temp_name.c_str(), this->u_lens.at(i), this->c_lens.at(i), vendor_info.c_str());
            this->write_line(buf);
        }else if(this->protected_info.at(i) == 3){
            vendor_info = this->get_value("protect_fail3");
            sprintf(buf, "* `%s (%d-->%d)`，%s  ", temp_name.c_str(), this->u_lens.at(i), this->c_lens.at(i), vendor_info.c_str());
            this->write_line(buf);
        }else{
            sprintf(buf, "* `%s (%d-->%d)`  ", temp_name.c_str(), this->u_lens.at(i), this->c_lens.at(i));
            this->write_line(buf);
            has_so_protected = true;
        }
        bool has_task = false;
        if(tasks.at(i) & 1){
            vendor_info = this->get_value("task1");
            sprintf(buf, "  - %s", vendor_info.c_str());
            this->write_line(buf);
            has_task = true;
        }
        if(tasks.at(i) & 2){
            vendor_info = this->get_value("task2");
            sprintf(buf, "  - %s", vendor_info.c_str());
            this->write_line(buf);
            has_task = true;
        }
        if(tasks.at(i) & 4){
            vendor_info = this->get_value("task3");
            sprintf(buf, "  - %s", vendor_info.c_str());
            this->write_line(buf);
            has_task = true;
        }
        if(tasks.at(i) & 8){
            vendor_info = this->get_value("task4");
            sprintf(buf, "  - %s", vendor_info.c_str());
            this->write_line(buf);
            has_task = true;
        }
        if(!has_task){
            sprintf(buf, "  - 无");
            this->write_line(buf);
        }
        
        this->write_line("");
    }
    if(has_so_protected == false){
        return 1;
    }
    vendor_info = this->get_value("checkTool");
    sprintf(buf, "### %s", vendor_info.c_str());
    this->write_line(buf);
    vendor_info = this->get_value("tool1");
    sprintf(buf, "%s", vendor_info.c_str());
    this->write_line(buf);
    vendor_info = this->get_value("tool2");
    sprintf(buf, "%s", vendor_info.c_str());
    this->write_line(buf);
    vendor_info = this->get_value("tool3");
    sprintf(buf, "%s", vendor_info.c_str());
    this->write_line(buf);
    this->write_line("");
    if(anti_static_analysis == 1){
        vendor_info = this->get_value("task1");
        sprintf(buf, "### %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("checkTitle");
        sprintf(buf, "#### %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("checkProcess");
        sprintf(buf, "%s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task1_step1");
        sprintf(buf, "1. %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task1_step2");
        sprintf(buf, "2. %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task1_step3");
        sprintf(buf, "3. %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task1_step4");
        sprintf(buf, "4. %s", vendor_info.c_str());
        this->write_line(buf);
        
        this->write_line("");

        sprintf(buf, "![ida](%s/anti-ida.png)", this->pic_dir.c_str());
        this->write_line(buf);
        this->write_line("<center>[SO壳保护后在IDA下无法分析的效果图]</center>");
        this->write_line("");
    }
    
    if(anti_tamper == 1){
        vendor_info = this->get_value("task2");
        sprintf(buf, "### %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("checkTitle");
        sprintf(buf, "#### %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("checkProcess");
        sprintf(buf, "%s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task2_step1");
        sprintf(buf, "1. %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task2_step2");
        sprintf(buf, "2. %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task2_step3");
        sprintf(buf, "3. %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task2_step4");
        sprintf(buf, "4. %s", vendor_info.c_str());
        this->write_line(buf);
        this->write_line("");
    }
    
    if(anti_running_time_attack == 1){
        vendor_info = this->get_value("task3");
        sprintf(buf, "### %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("checkTitle");
        sprintf(buf, "#### %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("checkProcess");
        sprintf(buf, "%s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task3_step1");
        sprintf(buf, "1. %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task3_step2");
        sprintf(buf, "2. %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task3_step3");
        sprintf(buf, "3. %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task3_step4");
        sprintf(buf, "4. %s", vendor_info.c_str());
        this->write_line(buf);
        
        this->write_line("");
        sprintf(buf, "![ida](%s/anti-running-attack1.png)", this->pic_dir.c_str());
        this->write_line(buf);
        this->write_line("<center>[防dump功能示例图]</center>");
        this->write_line("");
    }
    

    if(anti_running_debuging == 1){
        vendor_info = this->get_value("task4");
        sprintf(buf, "### %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("checkTitle");
        sprintf(buf, "#### %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("checkProcess");
        sprintf(buf, "%s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task4_step1");
        sprintf(buf, "1. %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task4_step2");
        sprintf(buf, "2. %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task4_step3");
        sprintf(buf, "3. %s", vendor_info.c_str());
        this->write_line(buf);
        vendor_info = this->get_value("task4_step4");
        sprintf(buf, "4. %s", vendor_info.c_str());
        this->write_line(buf);

        this->write_line("");
    }
    
    close(this->fd);
    this->fd = 0;
    
    return 0;
}


std::string report_builder::get_value(std::string key){
    if(this->vendor_fd == NULL){
        return "";
    }
    char line[400];
    fseek(this->vendor_fd, 0, SEEK_SET);

    while(!feof(this->vendor_fd)){
        char* temp = fgets(line, 400-1, this->vendor_fd);
        (void)temp;
        std::string readline(line);
        if(std::string::npos != readline.find(key)){
            size_t start = readline.find(":\"");
            size_t end = readline.rfind('\"');
            std::string value = readline.substr(start+2, end-start-2);
            return value;
        }
        
    }
    return "";
}


char* report_builder::getpath(const char* vendor){
    #if (ACC_OS_WIN32 || ACC_OS_WIN64) && (defined(__MINGW32__) || defined(__MINGW64__))

    char* vendorPath = (char*)malloc(4096);
    GetModuleFileNameA(NULL, vendorPath, 4096);
    strrchr(vendorPath, '\\')[1] = '\0';
    strcat(vendorPath, vendor);
    return vendorPath;
#elif (ACC_OS_POSIX_LINUX) && (defined(__linux__) || defined(__LINUX__))
    char* vendorPath = (char*)malloc(4096);
    
    int cnt = readlink("/proc/self/exe", vendorPath, PATH_MAX);
    if(cnt<0 || cnt >= PATH_MAX){
        return NULL;
    }
    strrchr(vendorPath, '/')[1] = '\0';
    strcat(vendorPath, vendor);
    return vendorPath;
#else
//MACOSX
    char* vendorPath = (char*)malloc(4096);
    char cmdline[4096];
    snprintf(cmdline, 4096, "dirname %s", this->arg0.c_str());
    FILE *file;
	file = popen(cmdline, "r");
    fgets(vendorPath, 4096, file);
    int cnt = strlen(vendorPath);
    if(cnt<0 || cnt >= 4096){
        return NULL;
    }
    vendorPath[cnt-1] = '/';
    vendorPath[cnt] = '\0';
    strcat(vendorPath, vendor);
    return vendorPath;
#endif
}

bool report_builder::store_protected_info(int info){
    if(!is_gen_report()){
        return false;
    }
    this->protected_info.push_back(info);
    return true;
}

bool report_builder::is_gen_report(){
    if(this->report_name == "" || this->zip_name == "" || this->vendor_fd == NULL){
        return false;
    }
    return true;
}