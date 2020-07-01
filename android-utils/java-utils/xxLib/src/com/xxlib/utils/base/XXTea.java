package com.xxlib.utils.base;


public class XXTea
{
	// 封装后的XXTea加密算法接口
	public static byte[] XXDoTeaEncrypt(final String src, final String key){
		if( src == null )
			return null;

		if( (key == null) || (key.length() != 16) )
			return null;

		byte[] srcByte = src.getBytes();
		return XXTeaEncrypt(srcByte, srcByte.length, key.getBytes());
	}
	
	// XXTea算法的原始加密接口
	public static byte[] XXTeaEncrypt(final byte[] src, int srcLen, final byte[] key){
		if(src == null || src.length <= 0 || srcLen <= 0)
			return null;
		
		if(key == null || key.length != 16)
			return null;
	
		// +4 用于存放转换的字节长度
		int n = ((srcLen+3)&(~3)) + 4;
		// 将srcLen个byte拷贝到新的buffer,并以0填充
		byte[] enc_ch = new byte[n];
		
		System.arraycopy(src, 0, enc_ch, 0, srcLen);
		int enc_len = 0;
		for(enc_len = srcLen; enc_len < n; enc_len++)
			enc_ch[enc_len] = 0;
		
		// 源数据转换为int型数组
		int[] v = uByteToIntArray(enc_ch, 0, enc_len);
		int[] k = uByteToIntArray(key, 0, key.length);
		n = enc_len >>> 2;	
		// 存放本次转换的字节长度
		v[n-1] = srcLen; 
		
		// 开始进行加密
		int z = v[n-1];
		int y = v[0];
		int q = 6 + 52/n;
		int sum = 0;
		int mx, e, i;
		// n-1方便循环时对数组的定位
		n--;
		
		if(n > 0){
			while(0 < q--){
				sum += 0x9E3779B9;
				e = (sum >>> 2) & 3;
				
				for(i = 0; i < n; i++){
					y = v[i + 1];
					mx = (z >>> 5 ^ y << 2) + (y >>> 3 ^ z << 4) ^ (sum ^ y) + (k[(i) & 3 ^ e] ^ z);
					z = v[i] = v[i] + mx ;
				}
				y = v[0];
				mx = (z >>> 5 ^ y << 2) + (y >>> 3 ^ z << 4) ^ (sum ^ y) + (k[(i) & 3 ^ e] ^ z);
				z = v[n] = v[n] + mx & 0xffffffff;
			}
		} else{
			for (i = 0; i < 4; i++){
				y = (((y & 0x0000ffff) << 16) | ((y & 0xffff0000) >>> 16)) ^ 0x9E3779B9;
				y += (k[i] ^ 0x9E3779B9);
			}
			v[0] = y;
		}
	
		return intToByteArray(v, v.length << 2);
	}
	
	public static byte[] XXDoTeaDecrypt(final byte[] src, int srcLen, final byte[] key){
		return XXTeaDecrypt(src, srcLen, key);
	}
	
	public static byte[] XXTeaDecrypt(final byte[] src, int srcLen, final byte[] key){
		if(src == null || src.length <= 0 || srcLen <= 0)
			{
			return null;}
		
		if(key == null || key.length != 16)	{return null;}
		
		if((srcLen&3) != 0)  {return null;}
		
		int[] v = uByteToIntArray(src, 0, srcLen);
		int[] k = uByteToIntArray(key, 0, key.length);
		
		int n = srcLen >>> 2;
		int z;
		int y = v[0];
		int q = 6 + 52/n;
		int sum = q * 0x9E3779B9;
		int i, mx, e;
		
		n--;
		if(n > 0){
			while(sum != 0){
				e = (sum >>> 2) & 3;
				for (i = n; i > 0; i--){
					z = v[i - 1];
					mx = (z >>> 5 ^ y << 2) + (y >>> 3 ^ z << 4)^ (sum ^ y) + (k[i & 3 ^ e] ^ z);
					y = v[i] -= mx;
				}
				z = v[n];
				mx = (z >>> 5 ^ y << 2) + (y >>> 3 ^ z << 4)^ (sum ^ y) + (k[i & 3 ^ e] ^ z);
				y = v[0] -= mx;
				sum -= 0x9E3779B9;
			}
		}else{
			for (i = 3; i >= 0; i--){
				y -= (k[i] ^ 0x9E3779B9);
				y = (((y & 0x0000ffff) << 16) | ((y & 0xffff0000) >>> 16)) ^ 0x79B99E37;
			}
			v[0] = y;
		}
		return intToByteArray(v, v[n]);
	}
	
	
	private static int[] uByteToIntArray(byte[] buf, int offset, int len){
		if(buf == null)
			return null;
		
		if( (offset < 0) || (len <= 0) || (offset + len > buf.length))
			return null;
		
		int n = ( (len + 3) & ~3) >>> 2;
		int[] nResult = new int[n];
		
		n = len;
        for (int i = 0; i < n; i++) 
            nResult[i >>> 2] |= (0x000000ff & buf[i+offset]) << ((i & 3) << 3);

        return nResult;
	}
	
	// byteLen: 转换后的byte[]长度，若byteLen > data.length << 2,其余位以0填充
	private static byte[] intToByteArray(int[] data, int byteLen) {
        int n = data.length << 2;
        byte[] result = new byte[byteLen];
        
        n = (n <= byteLen ? n : byteLen);
        for (int i = 0; i < n; i++) 
            result[i] = (byte) ((data[i >>> 2] >>> (byte) ((i & 3) << 3)) +256 );
          
        for (int i = n; i < byteLen; i++)
        	result[i] = '\0';
       
        return result;
    }
}
