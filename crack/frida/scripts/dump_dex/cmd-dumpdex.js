// dumpdex by cmd
// cmd:
// $ frida -U -f <pkg_name> -l cmd-dumpdex --no-pause
if(Java.available) {
	Java.perform(function () {
		// dumpdex by DefineClass form libart.so
		// descripor Adndroid 8.1.0:
		// mirror::Class* ClassLinker::DefineClass(Thread* self,
		//                                         const char* descriptor,
		//                                         size_t hash,
		//                                         Handle<mirror::ClassLoader> class_loader,
		//                                         const DexFile& dex_file,
		//                                         const DexFile::ClassDef& dex_class_def)

		// TODO need to change class form diff android version
		var defineClass = Module.findExportByName("libart.so", "_ZN3art11ClassLinker11DefineClassEPNS_6ThreadEPKcjNS_6HandleINS_6mirror11ClassLoaderEEERKNS_7DexFileERKNS9_8ClassDefE");
		console.log("[*] define class addr: " + defineClass);
		Interceptor.attach(defineClass, {
			onEnter: function (args) {
				
				var classname = args[2].readUtf8String();
				// TODO need to modify
				if(classname == "Lcom/dx/app/DXApplication;") { 
						console.log("[+] class name: " + classname);
						console.log("[*] this obj addr: " + args[0]); // this
						console.log("[*] thread addr: " + args[1]); // thread
						console.log("[*] classname addr: " + args[2]); // classname
						console.log("[*] uint value: " + args[3]); // unit
						console.log("[*] classloader addr: " + args[4]); // classloader 
						console.log("[*] dex file: " + args[5]); // dex_file 
						console.log("[*] dex classdef addr: " + args[6]); // dex_class_def 
						
						
						// struct Header {
							// unsigned char magic_[8];
							// unsigned int checksum_; // See also location_checksum_
							// unsigned char signature_[20];
							// unsigned int file_size_; // size of entire file
							// unsigned int header_size_; // offset to start of next section
							// unsigned int endian_tag_;
							// unsigned int link_size_; // unused
							// unsigned int link_off_; // unused
							// unsigned int map_off_; // unused
							// unsigned int string_ids_size_; // number of StringIds
							// unsigned int string_ids_off_; // file offset of StringIds array
							// unsigned int type_ids_size_; // number of TypeIds, we don't support more than 65535
							// unsigned int type_ids_off_; // file offset of TypeIds array
							// unsigned int proto_ids_size_; // number of ProtoIds, we don't support more than 65535
							// unsigned int proto_ids_off_; // file offset of ProtoIds array
							// unsigned int field_ids_size_; // number of FieldIds
							// unsigned int field_ids_off_; // file offset of FieldIds array
							// unsigned int method_ids_size_; // number of MethodIds
							// unsigned int method_ids_off_; // file offset of MethodIds array
							// unsigned int class_defs_size_; // number of ClassDefs
							// unsigned int class_defs_off_; // file offset of ClassDef array
							// unsigned int data_size_; // unused
							// unsigned int data_off_; // unused
						// };
			
						// args[5] == art::DexFile
						// DexFile + 0 = vtable addr
						// DexFile + 4 = *begin_ = *DexHeader
						var dex_begin = args[5].toInt32() + 4; // *begin
						var dp = Memory.readUInt(ptr(dex_begin)) // real memaddr
						
						console.log(Memory.readByteArray(ptr(dp), 16));
						
						var fsize = Memory.readUInt(ptr(dp + 32));
						console.log("dex file size : " + fsize); // fileSize
						console.log("[+] begin to dump dex ...");
						
						// console.log(hexdump(ptr(dp), {
						  // offset: 0,
						  // length: fsize,
						  // header: true,
						  // ansi: true
						// }));
						
						
						var mapx_buf_len = 10240;
						var i = 0;
						var count = parseInt(fsize / mapx_buf_len);
						var lastbuf_len = parseInt(fsize % mapx_buf_len);
						
						if (lastbuf_len > 0) {
							count += 1;
						} 
						

						console.log("[+] send count: " + count);
						console.log("[+] send lastbuf_len: " + lastbuf_len);
						
						for (; i < count ; i++) {
							if ((lastbuf_len > 0) && (i == (count - 1))) {
								console.log("[*] mem off: " + i*mapx_buf_len);
								console.log("[*] mem last off: " + lastbuf_len);
								send("Send dex file", Memory.readByteArray(ptr(dp + i*mapx_buf_len) , lastbuf_len));
							}  else {
								console.log("[*] mem off: " + i*mapx_buf_len);
								send("Send dex file", Memory.readByteArray(ptr(dp + i*mapx_buf_len) , mapx_buf_len));
								
							}
						}
						console.log("[+] finish to dump dex ...");
					}
				},
				
				onLeave: function (retval) {
					console.log("[*] leave to dump dex ...");
			   }
		});

		


	});
}
