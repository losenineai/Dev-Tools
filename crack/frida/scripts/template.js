setImmediate(function() {

	// hook java
	if(Java.available){
	    Java.perform(function(){

	        var cls = Java.use("com.bun.miitmdid.core.JLibrary");
	        cls.o0o0o0o0o0.implementation=function(obj, str, str2, list){
	        	console.log('obj:'+obj.toString());
	        	console.log('str:'+str);
	        	console.log('str2:'+str2);
	        	console.log('list:'+list);

	            return cls.o0o0o0o0o0(obj, str, str2, list);        
	        }
	    });
	}


	// hook native
	Interceptor.attach(Module.findExportByName("libc.so" , "open"), {

    	onEnter: function (args) {
		    console.log('call open(' + 'path="' + args[0].readUtf8String() + '"' + ', oflag=' + args[1] + ')');
		    	// log('state:'+state);
		},
    	onLeave:function(retval){
    		console.log('open result:' + retval);
    	}
	});


});