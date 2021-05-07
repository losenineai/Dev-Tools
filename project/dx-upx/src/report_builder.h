#include<iostream>
#include <vector>

class report_builder{
public:
    report_builder(const char* _report_name, const char* _pic_dir, const char* _zip_name, const char* _vendor_path, const char* _arg0);
    int make_report();
    ~report_builder();
    bool store_u_len_info(const char* file_path);
    bool store_filenames(const char* file_path);
    bool store_c_len_info(const char* file_path);
    bool store_tasks(bool upx, int signature_check, int integrity_check, int anti_ida, int anti_debuging, int anti_dump);
    bool store_protected_info(int info);
private:
    char* getpath(const char* vendor);
    std::string get_value(std::string key);
    bool is_gen_report();
    bool write_line(std::string line);
    std::string report_name;
    std::string zip_name;
    std::string arg0;
    int fd;
    std::vector<std::string> filenames;
    std::vector<int> u_lens;
    std::vector<int> c_lens;
    std::vector<int> tasks;
    std::vector<int> protected_info;
    std::string pic_dir;
    FILE* vendor_fd;
};