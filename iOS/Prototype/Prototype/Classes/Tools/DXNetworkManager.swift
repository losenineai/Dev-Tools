//
//  DXNetworkManager.swift
//  Prototype
//
//  Created by Roger on 2020/3/27.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit

class DXNetworkManager: NSObject {
    
    class func getToken() -> String? {
        if DXRiskManager.setup() {
            let appid = UserDefaults.standard.string(forKey: dxAppid)
            let serverUrl = YSHttpConfigure.shareInstance().generalServer
            var dic = [DXRiskManagerKeyUserId: "123456" , DXRiskManagerKeyDelayMsTime:"4000",DXRiskManagerKeyURL:serverUrl + "/udid/m1","PRIVATE_CLEAR_TOKEN":"clear"]
            if UserDefaults.standard.bool(forKey: dxCache) {
                dic.removeValue(forKey: "PRIVATE_CLEAR_TOKEN")
            }
            let riskStr = DXRiskManager.getToken(appid, extendsParams: dic)
            return riskStr
        }
        return nil
    }
    
    class func getDeviceInfo(_ token:String,respone:@escaping YSHttpResponseBlock) {
        let appid = UserDefaults.standard.string(forKey: dxAppid) ?? ""
        let appSecret = UserDefaults.standard.string(forKey: dxAppSecret) ?? ""
        let sign = (appSecret + token + appSecret).md5()
        YSHttpManager.share().sendRequest(configBlock: { (request) in
            request?.requestMethod = .get
            request?.requestHeader = ["Accept":"*/*","User-Agent":"STEE-SDK" , "connection":"keep-Alive"]
            request?.requestURL = "/udid/api/getDeviceInfo"
            request?.urlParams = ["appId":appid,"token":token,"sign":sign]
        }, complete: respone)
    }
    
    class func verifyHost(_ url:String,respone:@escaping YSHttpResponseBlock) {
        YSHttpManager.share().sendRequest(configBlock: { (request) in
            request?.requestMethod = .get
            request?.baseURL = url
        }, complete: respone)
    }
    
    class func checkRisk(_ token:String,param:Dictionary<String, Any> ,respone:@escaping YSHttpResponseBlock) {
           let appid = UserDefaults.standard.string(forKey: dxAppid) ?? ""
           let appSecret = UserDefaults.standard.string(forKey: dxAppSecret) ?? ""
           let paramStr = param.dic2String()
           let sign = (appSecret + paramStr + appSecret).md5()
           let bodyStr = param.dic2JsonString()?.base64()
           
           YSHttpManager.share().sendRequest(configBlock: { (request) in
               request?.requestMethod = .post
               request?.requestHeader = ["Accept":"*/*","User-Agent":"STEE-SDK","connection":"keep-Alive","Content-type":"application/octet-stream"]
               request?.requestURL = "/ctu/event.do"
               request?.urlParams = ["appKey":appid,"sign":sign,"version":"1"]
               request?.normalParams = bodyStr
           }, complete: respone)
       }

}


