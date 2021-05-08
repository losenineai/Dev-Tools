
// for java, print method invoke stack
function print_java_Stack() {
    Java.perform(function () {
        var Exception = Java.use("java.lang.Exception");
        var ins = Exception.$new("Exception");
        var straces = ins.getStackTrace();
        if (straces != undefined && straces != null) {
            var strace = straces.toString();
            var replaceStr = strace.replace(/,/g, "\r\n");
            console.log("=============================Stack strat=======================");
            console.log(replaceStr);
            console.log("=============================Stack end=======================\r\n");
            Exception.$dispose();
        }
    });
}


function hook_java_template(){
    Java.perform(function(){
		// overload
	    var URL = Java.use('java.net.URL');
	    URL.openConnection.overload().implementation = function(){
	    	console.log("Hooked URL.openConnection url:" + URL.toString.call(this));
	    	print_java_Stack()
	    	return URL.openConnection.call(this);
	    }
		
		// hook init
		var e = Java.use('my.shenghe.common.h.b.e');
	    e.$init.implementation = function(str, f){
	    	console.log("Hooked e.init str:" + str + " f:" + f);
	    	print_java_Stack()
	    	return this.$init(str, f);
		}
    });
}



function hook_JNI_Onload(){
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
}


function hook_HttpURLConnection(){
	Java.perform(function () {		
	    var URL = Java.use('java.net.URL');
	    URL.openConnection.overload().implementation = function(){
	    	console.log("Hooked URL.openConnection url:" + URL.toString.call(this));
	    	print_java_Stack()
	    	return URL.openConnection.call(this);
	    }

	    var HttpURLConnection = Java.use('com.android.okhttp.internal.huc.HttpURLConnectionImpl');
	    HttpURLConnection.connect.overload().implementation = function () {
	        console.log('Hooked HttpURLConnection->connect()');
	        HttpURLConnection.connect.call(this);
	    }

	    HttpURLConnection.getInputStream.overload().implementation = function () {
	        console.log('Hooked HttpURLConnection->getInputStream()');
	        return HttpURLConnection.getInputStream.call(this);
	    }

	    HttpURLConnection.getOutputStream.overload().implementation = function () {
	        console.log('Hooked HttpURLConnection->getOutputStream()');
	        return HttpURLConnection.getOutputStream.call(this);
	    }
	});
}



function hook_send(){
	Interceptor.attach(Module.findExportByName("libc.so" , "send"), {
		onEnter: function (args) {
			var fd = args[0].toInt32()
          	console.log( "send==> fd:" +fd + " len : " + args[2])
            var r = Socket.peerAddress(fd)
            var local = Socket.localAddress(fd)
            console.log("send ==> localIP:" +  local.ip + ":" + local.port +" => remoute IP:" + r.ip + ":" + r.port)
            console.log(hexdump(args[1], {
                offset: 0,
                length: args[2].toInt32(),
                header:true,
                ansi:true
            }))

		},

    	onLeave:function(retval){
    		// console.log('open result:' + retval);
    	}
	});
}


function hook_recv(){
	Interceptor.attach(Module.findExportByName("libc.so" , "send"), {
		onEnter: function (args) {
			var fd = args[0].toInt32()
		  	console.log( "send==> fd:" +fd + " len : " + args[2])
		    var r = Socket.peerAddress(fd)
		    var local = Socket.localAddress(fd)
		    console.log("send ==> localIP:" +  local.ip + ":" + local.port +" => remoute IP:" + r.ip + ":" + r.port)
		    console.log(hexdump(args[1], {
		        offset: 0,
		        length: args[2].toInt32(),
		        header:true,
		        ansi:true
		    }))

		},

		onLeave:function(retval){
			// console.log('open result:' + retval);
		}
	});
}





setImmediate(function() {


	hook_HttpURLConnection()


});


