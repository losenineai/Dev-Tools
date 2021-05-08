setImmediate(function() {

	// hook java
	// if(Java.available){
	//     Java.perform(function(){

	//         var cls = Java.use("com.bun.miitmdid.core.JLibrary");
	//         cls.o0o0o0o0o0.implementation=function(obj, str, str2, list){
	//         	console.log('obj:'+obj.toString());
	//         	console.log('str:'+str);
	//         	console.log('str2:'+str2);
	//         	console.log('list:'+list);

	//             return cls.o0o0o0o0o0(obj, str, str2, list);        
	//         }
	//     });
	// }


	// hook native
	// Interceptor.attach(Module.findExportByName("libc.so" , "open"), {

 //    	onEnter: function (args) {
	// 	    console.log('call open(' + 'path="' + args[0].readUtf8String() + '"' + ', oflag=' + args[1] + ')');
	// 	},
 //    	onLeave:function(retval){
 //    		// console.log('open result:' + retval);
 //    	}
	// });


	// hook native
	Interceptor.attach(Module.findExportByName("libc.so" , "dlsym"), {

    	onEnter: function (args) {
		    console.log('call dlsym(' + 'handle="' + args[0] + '"' + ', sym=' + args[1].readUtf8String() + ')');


		    if (args[1].readUtf8String() == "Java_com_gamesec_DataCollector_getVer"){
		    	Interceptor.attach(Module.findExportByName("libdc.so", "Java_com_gamesec_DataCollector_getVer"), {
			    	onEnter: function (args) {
					    console.log('call Java_com_gamesec_DataCollector_getVer'); 
					},
			    	onLeave:function(retval){
			    	}
				});
		    }




		    if (args[1].readUtf8String() == "JNI_Onload"){
	    		Interceptor.attach(Module.findExportByName("libnative-lib.so", "JNI_OnLoad"), {
			    	onEnter: function (args) {
					    console.log('call JNI_OnLoad');
					},
			    	onLeave:function(retval){
			    		// console.log('open result:' + retval);
			    	}
				});
		    }




		    if (args[1].readUtf8String() == "luaL_loadbufferx"){
		    		Interceptor.attach(Module.findExportByName("libnative-lib.so", "luaL_loadbufferx"), {

			    	onEnter: function (args) {
					    console.log('call luaL_loadbufferx(' 
					    	+ 'args[0]="' + args[0] + '"' 
					    	+ 'args[1]="' + args[1] + '"' 
					    	+ 'args[2]="' + args[2] + '"' 
					    	+ 'args[3]="' + args[3] + '"' 
					    	+ ', args[4]=' + args[4] + ')');
					    	// log('state:'+state);
					},
			    	onLeave:function(retval){
			    	}
				});
		    }



		},
    	onLeave:function(retval){
    		// console.log('open result:' + retval);
    	}
	});




	// Interceptor.attach(Module.findExportByName("libnative-lib.so" , "lua_newstate"), {

 //    	onEnter: function (args) {
	// 	    console.log('call lua_newstate'); 
	// 	},
 //    	onLeave:function(retval){
 //    		console.log('lua_newstate result:' + retval);
 //    	}
	// });




});