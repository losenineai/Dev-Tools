//
//  DXConfigController.swift
//  Prototype
//
//  Created by Roger on 2020/3/25.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit
import ZWPlaceHolder

typealias setBackBlock = ([String])->Void

class DXConfigController: DXBaseViewController {
    
    
    var block:setBackBlock!
    private let textView = UITextView()
    private var placeHolderLabel:UILabel!
    private var updateBtn  :UIButton!

    override func viewDidLoad() {
        super.viewDidLoad()

        initUI()
    }
    
    private func initUI() {
        view.backgroundColor = .white
        setTitle(text: "设置")
        
        let noticeView = DXNoticeView()
        noticeView.tipsText = "请按顺序填写，每一项内容用换行分割"
        noticeView.tipsImage = #imageLiteral(resourceName: "tips")
        view.addSubview(noticeView)
        noticeView.snp.makeConstraints { (make) in
            make.left.right.equalTo(0)
            make.top.equalTo(navBarView.snp.bottom).offset(8 * hScale)
            make.height.equalTo(34 * hScale)
        }
        
        view.addSubview(textView)
        textView.snp.makeConstraints { (make) in
            make.centerX.equalTo(view)
            make.top.equalTo(navBarView.snp.bottom).offset(45 * hScale)
            make.size.equalTo(CGSize(width: 343 * wScale, height: 200 * hScale))
        }
        textView.backgroundColor = UIColor(hex: 0xF7F8FA)
        textView.font = UIFont.systemFont(ofSize: 16 * hScale)
        textView.layer.cornerRadius = 4 * hScale
        textView.zw_placeHolder =   """
                                    服务器地址
                                    AppID
                                    AppSecret
                                    产品Code
                                    事件Code
                                    """
        
        
        updateBtn = UIButton(ch_title: "上传设置信息")
        updateBtn.setTitle("上传设置信息", for: .selected)
        updateBtn.backgroundColor = UIColor(hex: 0x0FBDA0)
        updateBtn.addTarget(self, action: #selector(click(_:)), for: .touchUpInside)
        updateBtn.tag = 1000
        view.addSubview(updateBtn)
        updateBtn.snp.makeConstraints { (make) in
            make.top.equalTo(textView.snp.bottom).offset(24 * hScale)
            make.centerX.equalTo(view)
            make.size.equalTo(CGSize(width: 343 * wScale, height: 50 * hScale))
        }
        updateBtn.layer.cornerRadius = 4 * hScale
        updateBtn.layer.masksToBounds = true
        

    }
    
    @objc
    private func click(_ btn:UIButton) {
        if btn.tag == 1000 {
            print(textView.text ?? "")
            let setText = textView.text
            let textArr = setText?.components(separatedBy: "\n")
            block(textArr ?? [""])
            self.navigationController?.popViewController(animated: true)
        }
    }
    
    
    deinit {
        debugPrint("释放了\(self.classForCoder)")
    }

}
