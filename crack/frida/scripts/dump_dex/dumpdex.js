// created by white

// dumpdex by DefineClass form libart.so

// descripor Adndroid 8.1.0:
// mirror::Class* ClassLinker::DefineClass(Thread* self,
//                                         const char* descriptor,
//                                         size_t hash,
//                                         Handle<mirror::ClassLoader> class_loader,
//                                         const DexFile& dex_file,
//                                         const DexFile::ClassDef& dex_class_def)


var doDumpDex = function(dexBase, outName) {

	console.log("[-----------------------------]");
	console.log("[+] start dump dex:" + dexBase + " out:" + outName);

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
	var dex_cls_begin = dexBase.toInt32() + 4; // DexFile -> *begin_
	var dexaddr = Memory.readUInt(ptr(dex_cls_begin)) // real memaddr
	
	console.log("maigc:");
	console.log(Memory.readByteArray(ptr(dexaddr), 8));
	console.log("chechsum:");
	console.log(Memory.readByteArray(ptr(dexaddr + 8), 4));
	console.log("signature:");
	console.log(Memory.readByteArray(ptr(dexaddr + 8 + 4), 20));

	var fsize = Memory.readUInt(ptr(dexaddr + 8 + 4 + 20));
	console.log("file_size: " + fsize); // fileSize
	console.log("[+] begin to dump dex ...");
	
	// console.log(hexdump(ptr(dexaddr), {
	// 	offset: 0,
	// 	length: fsize,
	// 	header: true,
	// 	ansi: true
	// }));
	
	var buf_len = 10240;
	// var i = 0;
	var count = parseInt(fsize / buf_len);
	var lastbuf_len = parseInt(fsize % buf_len);
	
	if (lastbuf_len > 0) {
		count += 1;
	} 
	
	console.log("[+] send count: " + count);
	console.log("[+] send lastbuf_len: " + lastbuf_len);
	
	send("start:" + outName);
	for (var i = 0; i < count ; i++) {
		var offset = i * buf_len;
		if ((lastbuf_len > 0) && (i == (count - 1))) {
			console.log("[*] mem off: " + offset + ", len: " + lastbuf_len);
			send(outName, Memory.readByteArray(ptr(dexaddr + offset) , lastbuf_len));
		}  else {
			console.log("[*] mem off: " + offset + ", len: " + buf_len);
			send(outName, Memory.readByteArray(ptr(dexaddr + offset) , buf_len));
			
		}
	}
	console.log("[+] finish to dump dex ...");
	console.log("[-----------------------------]");
}

var out_i = 0;
var debugMode = false;
var dexList = [];
var dumpedList = [];

// for debug
// debugMode = true;
// TODO need to change
var classList = [
				"Lde/robv/android/xposed/callbacks/XCallback$Param$SerializeWrapper;",
				"Les/dmoral/toasty/R$drawable;",
				"Lcom/stub/StubApp;",
				];


// TODO need to change class form diff android version
var defineClass = Module.findExportByName("libart.so", "_ZN3art11ClassLinker11DefineClassEPNS_6ThreadEPKcjNS_6HandleINS_6mirror11ClassLoaderEEERKNS_7DexFileERKNS9_8ClassDefE");
Interceptor.attach(defineClass, {
	onEnter: function (args) {
		
		var classname = ptr(args[2]).readUtf8String();
		var base = args[5];
		console.log("[on] DefindClass: " + classname);

		if(debugMode){
			console.log("[on] Dex Addr: " + base);

			dexList[base] = classname;
			for(var dex in dexList){
				console.log("key:" + dex + " value:" + dexList[dex]);
			}
		}
		else{
			for(var i in classList){
				// console.log("cls:" + classList[i]);
				if(classname == classList[i]){
					var out_name = "classes" + (out_i == 0 ? "" : out_i) +".dex";
					console.log("[on] dumpedList[base]: " + dumpedList[base]);
					// avoid to dump same dex
					if(dumpedList[base]){
						out_name = dumpedList[base];
					}
					else{
						out_i++;
					}
					doDumpDex(base, out_name);
					dumpedList[base] = out_name;

					console.log("[on] out_name: " + out_name + " clas:"+classname);
				}
			}
		}
				
	},
		

	onLeave: function (retval) {
		// console.log("[*] leave to dump dex ...");
   	}
});

	

