//
//  AppDelegate.swift
//  Prototype
//
//  Created by Roger on 2020/3/24.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit
import IQKeyboardManagerSwift

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?
    let tabBarController = DXTabBarController()


    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        self.window = UIWindow.init(frame: UIScreen.main.bounds)
        self.window?.rootViewController = tabBarController
        self.window?.makeKeyAndVisible()
        DXRiskManager.setup()
        setIQKeyBoard()
        return true
    }
    
    /// 键盘
    private func setIQKeyBoard() {
        let keyboardManager = IQKeyboardManager.shared
        /**  开启功能 */
        keyboardManager.enable = true
        /**  点击空白处回收键盘 */
        keyboardManager.shouldResignOnTouchOutside = true
        /**  shouldShowTextFieldPlaceholder */
        keyboardManager.shouldShowToolbarPlaceholder = true
        keyboardManager.keyboardDistanceFromTextField = 70 * hScale
        keyboardManager.enableAutoToolbar = false
        
    }

}

