//
//  DXConfig.swift
//  Prototype
//
//  Created by Roger on 2020/3/24.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit

//func debugLog(_ items: Any) {
//    debugPrint(items)
//    #if DEBUG
//        NotificationCenter.default.post(name: NSNotification.Name(rawValue: "saveLog"), object: nil, userInfo: ["log":items])
//    #endif
//}

let naviBarBg = UIColor(hex: 0xF9F9FA)

// 屏幕变量
let kWidth  = UIScreen.main.bounds.size.width
let kHeight = UIScreen.main.bounds.size.height

///////****iphoneX适配参数&****//////////
let isIPX = kHeight >= 812 ? true : false
let wScale = isIPX ? 1 : kWidth / 375
let hScale = isIPX ? 1 : kHeight / 667

let tabBarHeight:CGFloat = isIPX ? 34 : 0
let navBarHeight:CGFloat = isIPX ? 24 : 0


let lineColor = UIColor(hex: 0xEEEEEE)

let APPDelegateInstance = UIApplication.shared.delegate! as! AppDelegate


class DXViewTool: NSObject {
    class func getCurrentVC() -> UIViewController? {
        var result: UIViewController?
        var window = UIApplication.shared.keyWindow
        if window?.windowLevel != UIWindow.Level.normal {
            let windows = UIApplication.shared.windows
            for tmpWin in windows where tmpWin.windowLevel == UIWindow.Level.normal {
                window = tmpWin
                break
            }
        }

        let frontView = window?.subviews.index(0)!
        let nextResponder = frontView!.next
        if nextResponder!.isKind(of: UIViewController.self) {
            result = nextResponder as? UIViewController
        } else {
            result = window?.rootViewController
        }
        if result!.isKind(of: UITabBarController.self) {
            result = ((result as! UITabBarController).selectedViewController as! DXNavigationController).topViewController
        }

        result?.navigationController?.isNavigationBarHidden = false

        return result
    }
    
}

extension UIButton {

    /// 创建 UIButton
    ///
    /// - parameter title:         title
    /// - parameter fontSize:      fontSize，默认 17
    /// - parameter color:         color，默认 darkGrayColor
    /// - parameter imageName:     imageName，默认 nil
    /// - parameter backImageName: backImageName，默认 nil
    ///
    /// - returns: UIButton
    convenience init(ch_title title: String?,
                     fontSize: CGFloat = 17 * hScale,
                     color: UIColor = .white,
                     imageName: String? = nil,
                     backImageName: String? = nil) {

        self.init()

        self.setTitle(title, for: .normal)
        self.titleLabel?.font = UIFont.systemFont(ofSize: fontSize)
        self.setTitleColor(color, for: .normal)

        if let imageName = imageName {
            self.setImage(UIImage(named: imageName), for: .normal)
            self.setImage(UIImage(named: imageName + "_highlighted"), for: .highlighted)
            self.setImage(UIImage(named: imageName + "_selected"), for: .selected)
        }

        if let backImageName = backImageName {
            self.setBackgroundImage(UIImage(named: backImageName), for: .normal)
            self.setBackgroundImage(UIImage(named: backImageName + "_highlighted"), for: .highlighted)
        }

        sizeToFit()
    }
}
