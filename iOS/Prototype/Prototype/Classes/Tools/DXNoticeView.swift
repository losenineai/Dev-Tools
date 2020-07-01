//
//  DXNoticeView.swift
//  Prototype
//
//  Created by Roger on 2020/3/26.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit

class DXNoticeView: UIView {
    
    var tipsImage:UIImage! {
        didSet {
            imageView.image = tipsImage
        }
    }
    
    var tipsText:String! {
        didSet {
            titleLabel.text = tipsText
        }
    }
    
    private var imageView: UIImageView!
    private var titleLabel: UILabel!

    override init(frame: CGRect) {
        super.init(frame: frame)
        addImageView()
        addTitleLabel()
    }
    
    private func addImageView() {
        imageView = UIImageView()
        imageView.clipsToBounds = true
        addSubview(imageView)
        imageView.snp.makeConstraints { (make) in
            make.left.equalTo(16 * wScale)
            make.top.equalTo(10 * hScale)
            make.size.equalTo(CGSize(width: 14 * wScale, height: 14 * hScale))
        }
    }
    
    private func addTitleLabel() {
        titleLabel = UILabel(ch_text: "", fontSize: 12 * hScale, color: UIColor.black.withAlphaComponent(0.45), alignment: .left)
        titleLabel.clipsToBounds = true
        addSubview(titleLabel)
        titleLabel.snp.makeConstraints { (make) in
            make.centerY.equalTo(imageView)
            make.left.equalTo(imageView.snp.right).offset(8 * wScale)
        }
    }
    
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    

}
