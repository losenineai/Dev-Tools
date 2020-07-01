//
//  DXTabBarController.swift
//  Prototype
//
//  Created by Roger on 2020/3/24.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit

class DXTabBarController: UITabBarController {

    override func viewDidLoad() {
        super.viewDidLoad()

        tabBar.backgroundColor = UIColor(hex: 0xFCFCFC)
        addChildVC(childVC: DXRiskController(), title: "设备指纹", image: #imageLiteral(resourceName: "risk"), selectedImage: #imageLiteral(resourceName: "risk_selected"), tag: 0)
        addChildVC(childVC: DXDecisionEngineController(), title: "决策引擎", image: #imageLiteral(resourceName: "DecisionEngine_normal"), selectedImage: #imageLiteral(resourceName: "DecisionEngine_selected"), tag: 1)
        addChildVC(childVC: DXSetViewController(), title: "设置", image: #imageLiteral(resourceName: "set"), selectedImage: #imageLiteral(resourceName: "set_selected"), tag: 2)

        selectedIndex = 0
    }
    

     private func addChildVC(childVC: UIViewController, title: String?, image: UIImage, selectedImage: UIImage, tag: Int) {
        /// 导航栏
        let navc = DXNavigationController(rootViewController: childVC)

        let tabItem = UITabBarItem(title: title, image: image, selectedImage: selectedImage)
        tabItem.selectedImage = selectedImage.withRenderingMode(.alwaysOriginal)
        tabItem.image = image.withRenderingMode(.alwaysOriginal)
        /**  设置文字距离底部的距离 */
//        let offset = UIOffset(horizontal: 0, vertical: )
//        tabItem.titlePositionAdjustment = offset

        tabBar.tintColor = UIColor(hex: 0x14A7A9)
        // 添加角标
        //        self.addRedCornerrMark()
        navc.tabBarItem = tabItem

        addChild(navc)
            
    }

}
