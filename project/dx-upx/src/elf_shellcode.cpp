#include "elf_shellcode.h"

#include "anti_debugging.h"
#include "anti_dump.h"
#include "sign_verify.h"
#include "integrity_check.h"

#include "conf.h"
#include "log.h"

bool elf_shellcode::add_sign_verify_shellcode(int relhashcode, int reldebug_hashcode) {
    struct _SIGNVERIFY_MODULE* mod = NULL;
    size_t adden = 0;
    if (this->elf.get_type() == PLATFORM_ARM) {
        LOGI("ARM, %dbit", this->elf.get_mode());
        mod = &signature_verify::signverify_modules[0];
        adden = 1;
    } else if (this->elf.get_type() == PLATFORM_ARM64) {
        LOGI("ARM64, %dbit", this->elf.get_mode());
        mod = &signature_verify::signverify_modules[1];
    } else if (this->elf.get_type() == PLATFORM_X86) {
        LOGI("X86, %dbit", this->elf.get_mode());
        mod = &signature_verify::signverify_modules[2];
    } else {
        LOGE("in signature verify no match type");
        return false;
    }

    size_t align = 0x4;
    if(this->cur_offset % align != 0){
        this->cur_offset += (align - this->cur_offset%align);
    }
    this->sign_verify_addr = (this->xct_vaddr - this->xct_offset) + this->cur_offset;
    
    long original_hook_func = elf.hook("JNI_OnLoad", this->sign_verify_addr + adden);
    if(original_hook_func == 0){
        LOGE("in signature verify hook error");
        return false;
    }

    long rel_onload2syscall = 0;
    long rel_onload2checksign = 0;

    if(this->elf.get_type() == PLATFORM_ARM){
        if((original_hook_func & 1) == 1){
            original_hook_func &= 0xfffffffe;
        }
        rel_onload2syscall = 0xa74;
        rel_onload2checksign = 0xac;
    }else if(this->elf.get_type() == PLATFORM_ARM64){
        rel_onload2syscall = 0x1534;
        rel_onload2checksign = 0x12c;
    }else if(this->elf.get_type() == PLATFORM_X86){
        rel_onload2syscall = 0x1814;
        rel_onload2checksign = 0x130;    //using bin2hex tools to calculate it
    }else{
        LOGE("unsupport flatform");
        return false;
    }

    if(!signature_verify::update_signature_verify(mod->shellcode, 
        mod->length, 
        rel_onload2syscall,
            rel_onload2checksign,
            original_hook_func,
            relhashcode,
            reldebug_hashcode,
            elf.get_mode(),
            elf.get_arch()))
    {
        LOGE("update signagure verify fail");
        return false;
    }
    
    if(!elf.attach_shellcode(this->cur_offset, mod->shellcode, mod->length)){
        LOGE("attach shellcode fail");
        return false;
    }

    if(mod->length > 0x1000){
        LOGI("warning length of shellcode is :%zu", mod->length);
    }
    this->cur_offset += mod->length;

    return true;
}

bool elf_shellcode::add_integerity_check_shellcode() {
    struct _INTEGRITY_MODULE* mod = NULL;
    size_t adden = 0;
    if (this->elf.get_type() == PLATFORM_ARM) {
        //LOGD("ARM, %dbit", this->elf.getMode());
        adden = 1;
        mod = &integrity_check::integrity_modules[0];
    } else if (this->elf.get_type() == PLATFORM_ARM64) {
        //LOGD("ARM64, %dbit", this->elf.getMode());
        mod = &integrity_check::integrity_modules[1];
    } else if (this->elf.get_type() == PLATFORM_X86) {
        //LOGD("X86, %dbit", this->elf.getMode());
        mod = &integrity_check::integrity_modules[2];
    } else {
        LOGE("in signature verify no match type");
        return false;
    }

    size_t align = 0x4;
    if(this->cur_offset % align != 0){
        this->cur_offset += (align - this->cur_offset%align);
    }
    this->integrity_check_addr = (this->xct_vaddr - this->xct_offset) + this->cur_offset;
    
    // LOGI("integrity_check addr:%ld", this->integrity_check_addr);

    long cur2init_offset = 0;
    
    cur2init_offset = this->hook_DT_INIT(this->integrity_check_addr+adden);

    long relCurfun2Syscall = 0x00;
    long relCurfun2IntegrityCheck = 0x00;
    long relCurfun2init = cur2init_offset;

    if(this->elf.get_type() == PLATFORM_ARM){
        if((relCurfun2init & 1) == 1){
            relCurfun2init &= 0xfffffffe;
        }
        relCurfun2Syscall = 0x754;
        relCurfun2IntegrityCheck = 0xa0;
    }else if(this->elf.get_type() == PLATFORM_ARM64){
        relCurfun2Syscall = 0xc54;
        relCurfun2IntegrityCheck = 0x124;
    }else if(this->elf.get_type() == PLATFORM_X86){
        relCurfun2Syscall = 0xd34;
        relCurfun2IntegrityCheck = 0x140;    //using bin2hex tools to calculate it
    }else{
        LOGE("unsupport flatform");
        return false;
    }

    if(!integrity_check::update_integrity_check(mod->shellcode,
        mod->length,
        relCurfun2Syscall,
        relCurfun2IntegrityCheck,
        relCurfun2init,
        this->ifilename,
        this->elf.get_mode())){
        LOGE("update integrity check fail");
        return false;
    }
    
    if(!elf.attach_shellcode(this->cur_offset, mod->shellcode, mod->length)){
        LOGE("attach shellcode fail");
        return false;
    }

    if(mod->length > 0x1000){
        LOGI("warning length of shellcode is :%zu", mod->length);
    }

    this->cur_offset += mod->length;

    return true;
}

bool elf_shellcode::add_anti_dump_shellcode(){
    struct _ANTIDUMP_MODULE* mod = NULL;
    size_t adden = 0;
    if (this->elf.get_type() == PLATFORM_ARM) {
        //LOGD("ARM, %dbit", this->elf.getMode());
        mod = &anti_dump::anti_modules[0];
        adden = 1;
    } else if (this->elf.get_type() == PLATFORM_ARM64) {
        //LOGD("ARM64, %dbit", this->elf.getMode());
        mod = &anti_dump::anti_modules[1];
    } else if (this->elf.get_type() == PLATFORM_X86) {
        //LOGD("X86, %dbit", this->elf.getMode());
        mod = &anti_dump::anti_modules[2];
    } else {
        LOGE("in signature verify no match type");
        return false;
    }

    size_t align = 0x4;
    if(this->cur_offset % align != 0){
        this->cur_offset += (align - this->cur_offset%align);
    }
    this->anti_dump_addr = (this->xct_vaddr - this->xct_offset) + this->cur_offset;

    long cur2init_offset = this->hook_DT_INIT(this->anti_dump_addr + adden);
    long relCurfun2init = cur2init_offset;
    
    if(this->elf.get_type() == PLATFORM_ARM){
        if((relCurfun2init & 1) == 1){
            relCurfun2init &= 0xfffffffe;
        }
    }

    if(!anti_dump::update_anti_dump(mod->shellcode, 
        mod->length, 
        relCurfun2init, 
        this->anti_dump_addr + 0x1000,  //when upx
        this->elf.get_mode())){
            return false;
        }

    if(!elf.attach_shellcode(this->cur_offset, mod->shellcode, mod->length)){
        return false;
    }

    this->cur_offset += mod->length;

    if(mod->length > 0x1000){
        LOGI("warning length of shellcode is :%zu", mod->length);
    }

    return true;
}

bool elf_shellcode::add_anti_debugging_shellcode(){
    struct _ANTI_MODULE* mod =  NULL;
    size_t adden = 0;
    if (this->elf.get_type() == PLATFORM_ARM) {
        //LOGD("ARM, %dbit", this->elf.getMode());
        mod = &anti_debugging::anti_modules[0];
        adden = 1;
    } else if (this->elf.get_type() == PLATFORM_ARM64) {
        //LOGD("ARM64, %dbit", this->elf.getMode());
        mod = &anti_debugging::anti_modules[1];
    } else if (this->elf.get_type() == PLATFORM_X86) {
        //LOGD("X86, %dbit", this->elf.getMode());
        mod = &anti_debugging::anti_modules[2];
    } else {
        LOGI("in anti debugging no match type");
        return false;
    }

    size_t align = 0x4;
    if(this->cur_offset % align != 0){
        this->cur_offset += (align - this->cur_offset%align);
    }
    this->anti_debug_addr = (this->xct_vaddr - this->xct_offset) + this->cur_offset;
    // LOGI("anti debug xct_addr : %l", this->anti_debug_addr);

    long cur2init_offset = this->hook_DT_INIT(this->anti_debug_addr + adden);
    long relCurfun2init = cur2init_offset;
    long relCurfun2antifun = 0;
    if(this->elf.get_type() == PLATFORM_ARM){
        if((relCurfun2init & 1) == 1){
            relCurfun2init &= 0xfffffffe;
        }
        relCurfun2antifun = 0x60;
    }else if(this->elf.get_type() == PLATFORM_ARM64){
        relCurfun2antifun = 0xd8;
    }else if(this->elf.get_type() == PLATFORM_X86){
        relCurfun2antifun = 0xe0;
    }else{
        LOGE("no match type");
        return false;
    }
    // LOGI("anti debug curfun2init:%d, curfun2antifun:%d, length:%d", relCurfun2init, relCurfun2antifun, mod->length);
    if(!anti_debugging::update_anti_debugging(mod->shellcode, 
        mod->length, 
        relCurfun2init, 
        relCurfun2antifun,
        this->elf.get_mode())){
            return false;
        }

    if(!elf.attach_shellcode(this->cur_offset, mod->shellcode, mod->length)){
        return false;
    }
    
    this->cur_offset += mod->length;

    if(mod->length > 0x1000){
        LOGI("warning length of shellcode is :%zu", mod->length);
    }

    return true;
}


bool elf_shellcode::update() {
    return this->elf.update_file();
}

bool elf_shellcode::encrypt_shellcode(){
    this->elf.get_buffer();
    size_t s_offset = 0;
    size_t s_size = 0;
    if(elf.get_mode() == ARCH_BIT_32){
        Elf32_Phdr* phdr = (Elf32_Phdr*)this->elf.get_pt_load(3);
        if(phdr == NULL){
            LOGE("can not find 3th ptload");
            return false;
        }
        s_offset = phdr->p_offset;
        s_size = phdr->p_filesz;
    }else if(elf.get_mode() == ARCH_BIT_64){
        Elf64_Phdr* phdr = (Elf64_Phdr*)this->elf.get_pt_load(3);
        if(phdr == NULL){
            LOGE("can not find 3th ptload");
            return false;
        }
        s_offset = phdr->p_offset;
        s_size = phdr->p_filesz;
    }else{
        return false;
    }

    unsigned char* shellcode = this->elf.get_buffer() + s_offset;

    this->enc_dec_data(shellcode, s_size);

    this->elf.update_file();

    return true;
}

extern char shellcode_key[10];

void elf_shellcode::enc_dec_data(void* data, int length){
    const char* key = shellcode_key;
    int keylen = 8;
    unsigned char* pMessage = (unsigned char*)data;
    int ia = 0;
    unsigned char i=0,j=0,t;
    unsigned char s[256];
    unsigned char k[256];
    for(ia=0;ia<=255;ia++,i++)
        s[ia]=i+5;
    for(ia=0;ia<=255;ia++)
        k[ia]=(key)[ia%keylen];

    for(ia=i=j=0;ia<=255;ia++,i++){
        j=(j+s[i]+k[i])%256;
        t=s[i];s[i]=s[j];s[j]=t;
    }
    for(ia=i=j=0;ia<=length-1;ia++){
        i=(i+1)%256;
        j=(j+s[i])%256;
        t=s[i];s[i]=s[j];s[j]=t;
        t=(s[i]+s[j])%256;
        (pMessage)[ia]=s[t]^(pMessage)[ia];
   }

}


long elf_shellcode::hook_DT_INIT(size_t new_addr){
    long orig_addr = 0;
    int dt_val = DT_INIT;
    orig_addr = this->elf.get_dt_func_val(dt_val);
    
    if(orig_addr < 0){
        return orig_addr;
    }
    this->elf.set_dt_func_val(dt_val, new_addr);
    if(orig_addr == 0){
        return 0;
    }

    return new_addr - orig_addr;
}

