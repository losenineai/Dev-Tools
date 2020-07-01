//
//  DXToastView.swift
//  Prototype
//
//  Created by Roger on 2020/3/25.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit

class DXToastView: UIView {
    
    
    private var imageView:UIImageView!
    private var label:UILabel!

    override init(frame: CGRect) {
        super.init(frame: frame)

        initUI()
    }
    
    private func initUI() {
        let layerView = UIView()
        let width = bounds.size.width
        let height = bounds.size.height
        layerView.frame = CGRect(x: 0, y: 0, width: width, height: height)
        // fillCode
        let bgLayer1 = CALayer()
        bgLayer1.frame = layerView.bounds
        bgLayer1.backgroundColor = UIColor(red: 0, green: 0, blue: 0.17, alpha: 0.65).cgColor
        layerView.layer.addSublayer(bgLayer1)
        // shadowCode
        layerView.layer.shadowColor = UIColor(red: 0, green: 0, blue: 0, alpha: 0.15).cgColor
        layerView.layer.shadowOffset = CGSize(width: 0, height: 2)
        layerView.layer.shadowOpacity = 1
        layerView.layer.shadowRadius = 8
        layerView.layer.cornerRadius = 8
        layerView.layer.masksToBounds = true
        addSubview(layerView)
        
        let isWarning = height == 132 * wScale
        imageView = UIImageView()
        imageView.image = isWarning ? #imageLiteral(resourceName: "warning") : #imageLiteral(resourceName: "success")
        addSubview(imageView)
        imageView.snp.makeConstraints { (make) in
            make.centerX.equalTo(self)
            make.top.equalTo(20 * hScale + (isWarning ? 0 : 4 * hScale))
            make.size.equalTo(CGSize(width: 36 * wScale, height: 36 * wScale))
        }
        
        label = UILabel(ch_text: isWarning ? "服务器连接失败/鉴权失败" : "设置成功", fontSize: 14 * hScale, color: .white, alignment: .center)
        addSubview(label)
        label.snp.makeConstraints { (make) in
            make.top.equalTo(imageView.snp.bottom).offset(12 * hScale)
            make.centerX.equalTo(self)
            make.width.equalTo(isWarning ? 98 * wScale : 56 * wScale)
        }
        
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
}
