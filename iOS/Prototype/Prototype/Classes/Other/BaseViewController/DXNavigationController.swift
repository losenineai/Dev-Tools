//
//  DXNavigationController.swift
//  Prototype
//
//  Created by Roger on 2020/3/24.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit

class DXNavigationController: UINavigationController {

    override func viewDidLoad() {
        super.viewDidLoad()
        setStyle()
    }

    private func setStyle() {
        self.navigationBar.isHidden = true
    }

    override var preferredStatusBarStyle: UIStatusBarStyle {
        return .default
    }

    override func pushViewController(_ viewController: UIViewController, animated: Bool) {

        if viewControllers.count > 0 {// 此时push进来的viewController是第二个子控制器
            // 自动隐藏tabbar
            viewController.hidesBottomBarWhenPushed = true
        }
        // 调用父类pushViewController，self.viewControllers数组添加对象viewController
        super.pushViewController(viewController, animated: animated)
    }


}
