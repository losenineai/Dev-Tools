//
//  DXTool.swift
//  Prototype
//
//  Created by Roger on 2020/3/26.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit

let keyArr = ["server","appid","appSecret","appCode","productCode"]

let dxServer        = "UserDefaults.server"
let dxAppid         = "UserDefaults.appid"
let dxAppSecret     = "UserDefaults.appSecret"
let dxEventCode     = "UserDefaults.eventCode"
let dxProductCode   = "UserDefaults.productCode"
let dxCache         = "UserDefaults.Cache"

class DXTool: NSObject {
    class func lightFeedback() {
        let feedBackGenertor = UIImpactFeedbackGenerator(style: .light)
        feedBackGenertor.impactOccurred()
    }
    
    class func mediumFeedback() {
        let feedBackGenertor = UIImpactFeedbackGenerator(style: .medium)
        feedBackGenertor.impactOccurred()
    }
    
    class func heavyFeedback() {
        let feedBackGenertor = UIImpactFeedbackGenerator(style: .heavy)
        feedBackGenertor.impactOccurred()
    }
    
    class func softFeedback() {
        var feedBackGenertor:UIImpactFeedbackGenerator!
        if #available(iOS 13.0, *) {
            feedBackGenertor = UIImpactFeedbackGenerator(style: .soft)
        } else {
            feedBackGenertor = UIImpactFeedbackGenerator(style: .heavy)
        }
        feedBackGenertor.impactOccurred()
    }
    
    
}
