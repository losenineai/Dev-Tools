//
//  DXBaseViewController.swift
//  Prototype
//
//  Created by Roger on 2020/3/24.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit
import SnapKit

class DXBaseViewController: UIViewController {
    
    let backBtn = UIButton()
    let navBarView = UIView()
    let messageBtn = UIButton()
    var titleLabel:UILabel?
    
    var clickBtn:(()->Void)?=nil
    
    override func viewDidLoad() {
        super.viewDidLoad()

        /// 恢复系统左划功能
        self.navigationController?.interactivePopGestureRecognizer?.delegate = self
    }
    
   
    
    
    
    /// 子类调用方法
    ///
    /// - Parameter text: 中间title
    func setTitle(text: String,btnImage:UIImage?=nil,clickBtn:(()->Void)?=nil) {
        navBarView.removeFromSuperview()
        view.addSubview(navBarView)
        navBarView.backgroundColor = UIColor(hex:0x11143B)
        navBarView.snp.makeConstraints { (make) in
            make.top.equalTo(self.view.snp.top)
            make.top.left.right.equalTo(0)
            make.height.equalTo(64 + navBarHeight)
        }

        titleLabel = UILabel(ch_text: text, fontSize: 17 * hScale, color: .white , alignment: .center)
        navBarView.addSubview(titleLabel!)
        titleLabel!.snp.makeConstraints { (make) in
            make.centerX.equalTo(navBarView)
            make.bottom.equalTo(-11)
        }

        if let nav = DXViewTool.getCurrentVC()?.navigationController,nav.viewControllers.count > 1 {
            backBtn.setImage(#imageLiteral(resourceName: "back_Btn"), for: .normal)
            backBtn.setTitle("         ", for: .normal)
            navBarView.addSubview(backBtn)
            backBtn.snp.makeConstraints { (make) in
                make.centerY.equalTo(titleLabel!)
                make.left.equalTo(13 * wScale)
            }
            backBtn.tag = 100
            backBtn.addTarget(self, action: #selector(click(btn:)), for: .touchUpInside)
        }
        
        messageBtn.setImage(btnImage, for: .normal)
        navBarView.addSubview(messageBtn)
        messageBtn.snp.makeConstraints { (make) in
            make.right.equalTo(-17 * wScale)
            make.centerY.equalTo(titleLabel!)
        }
        messageBtn.tag = 200
        messageBtn.addTarget(self, action: #selector(click(btn:)), for: .touchUpInside)
        self.clickBtn = clickBtn
    }
    
    @objc private func click(btn:UIButton) {
        switch btn.tag {
        case 100:
            self.navigationController?.popViewController(animated: true)
        case 200:
            self.clickBtn?()
        default:
            break
        }
    }

  

}


extension DXBaseViewController:UIGestureRecognizerDelegate {
    /// UIGestureRecognizerDelegate 代理方法
    ///
    /// - Parameter gestureRecognizer: 阻止根式图左划
    /// - Returns: ture or  false
    func gestureRecognizerShouldBegin(_ gestureRecognizer: UIGestureRecognizer) -> Bool {
        if navigationController?.viewControllers.count == 1 {
            return false
        } else {
            return true
        }
    }
}

