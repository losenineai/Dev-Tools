
# -*- encoding: utf-8 -*-

import frida
import sys
import os
import glob


# _dex_path = "./dump_cls.dex";

def delete_files(rootdir, pattern):
    for file in glob.glob(os.path.join(rootdir, pattern)):
        print("delete file:"+file)
        os.remove(file)

def read_file_as_str(file_path):
    if not os.path.isfile(file_path):
        raise TypeError(file_path + " does not exist")

    with open(file_path, "rb") as f:
        all_the_text = f.read().decode("utf-8")
    # all_the_text = open(file_path, "r", encoding='UTF-8').read()
    # print type(all_the_text)
    return all_the_text

def write_messages(message, data):
    # print("out_name:"+message['payload'])
    out_name = message['payload']
    
    if(out_name.startswith("start:")):
        file = out_name.split(":")[1]
        if(os.path.exists(file)):
            os.remove(file)
            print("remove " + file + " success")
    else:
        dexfile = open(out_name, "ab+") 
        dexfile.write(data)
    

def hook_define_class():
    # get js text 
    hook_js = read_file_as_str("./hook.js")

    return hook_js

def main(apk):
    
    # if(os.path.exists(_dex_path)):
        # os.remove(_dex_path)

    delete_files(".", "*.dex")

    device = frida.get_usb_device(10)
    pid = device.spawn([apk])
    session = device.attach(pid)
    device.resume(pid)
    script = session.create_script(hook_define_class())
    script.on('message', write_messages)
    script.load()
    sys.stdin.read()
    session.detach()

if __name__ == '__main__':
    # TODO need to modify 
    # apk pkgname
    main("com.fhlcf.lyzg.yyjsqq")

    sys.exit(0)

    