//
//  DXSetCell.swift
//  Prototype
//
//  Created by Roger on 2020/3/25.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit

class DXSetCell: UITableViewCell {
    
    var fieldText:String? {
        get {
            return textField.text!
        }
        set {
            textField.text = newValue
        }
    }
    var switchIsOn:Bool! {
        get {
            return switchBtn.isOn
        }
        set {
            switchBtn.isOn = newValue
        }
    }
    
    var isShowError:Bool! {
        didSet {
            errorLabel.isHidden = !isShowError
        }
    }
    
    var titleText:String! {
        didSet {
            var attrString = NSMutableAttributedString()
            let lastCharacter = titleText.last
            if lastCharacter == "*" {
                titleText.removeLast()
                let attributedString1 = NSAttributedString(string: titleText, attributes: [.foregroundColor :UIColor.black.withAlphaComponent(0.65), .font : UIFont.systemFont(ofSize: 14)])
                let attributedString2 = NSAttributedString(string: "*", attributes: [.foregroundColor : UIColor(hex: 0xEA0D0D), .font : UIFont.systemFont(ofSize: 14)])
                attrString.append(attributedString1)
                attrString.append(attributedString2)
            }else {
                attrString = NSMutableAttributedString(string: titleText)
            }
            titleLabel.attributedText = attrString
        }
    }
    
    var errorText:String! {
        didSet {
            errorLabel.text = errorText
        }
    }
    
    var placeholderText:String! {
        didSet {
            textField.placeholder = placeholderText
        }
    }
    
    var needSwitch:Bool=false {
        didSet {
            switchBtn.isHidden = !needSwitch
            explainLabel.isHidden = !needSwitch
            textField.isHidden = needSwitch
            drawLine(frame: CGRect(x: 16 * wScale, y: 69 * hScale + (needSwitch ? 20 * hScale : 0), width: 343 * wScale, height: 1 * hScale), color: UIColor(hex: 0xC8C7CC))
            textField.font = UIFont.systemFont(ofSize: needSwitch ? 12 * hScale : 16 * hScale)
        }
    }
    
    private var titleLabel      :UILabel!
    private var errorLabel      :UILabel!
    private var textField       :UITextField!
    private var switchBtn       :UISwitch!
    private var explainLabel    :UILabel!
    
    
    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        initUI()
    }
    
    private func initUI() {
        selectionStyle = .none
        titleLabel = UILabel(ch_text: "", fontSize: 14 * hScale, color: UIColor.black.withAlphaComponent(0.65), alignment: .left)
        addSubview(titleLabel)
        titleLabel.snp.makeConstraints { (make) in
            make.left.equalTo(16 * wScale)
            make.top.equalTo(12 * hScale)
        }
        
        errorLabel = UILabel(ch_text: "", fontSize: 12 * hScale, color: UIColor(hex: 0xEA1111), alignment: .left)
        addSubview(errorLabel)
        errorLabel.snp.makeConstraints { (make) in
            make.left.equalTo(titleLabel.snp.right).offset(8 * wScale)
            make.centerY.equalTo(titleLabel)
        }
        errorLabel.isHidden = true
        
        textField = UITextField()
        addSubview(textField)
        textField.snp.makeConstraints { (make) in
            make.left.equalTo(titleLabel)
            make.bottom.equalTo(-12 * hScale)
            make.width.equalTo(343 * wScale)
        }
        textField.font = UIFont.systemFont(ofSize: 16 * hScale)
        textField.textColor = UIColor.black.withAlphaComponent(0.85)
        
        switchBtn = UISwitch()
        addSubview(switchBtn)
        switchBtn.snp.makeConstraints { (make) in
            make.centerY.equalTo(titleLabel)
            make.right.equalTo(-16 * wScale)
            
        }
        switchBtn.onTintColor = UIColor(hex: 0x0E7DF0)
        switchBtn.transform = CGAffineTransform(scaleX: 0.86, y: 0.86)
        
        explainLabel = UILabel(ch_text: "开启缓存后，设备指纹token超过有效期后会进行更新若未开启则每次获取设备指纹都会变成一个token", fontSize: 12 * hScale, color: UIColor.black.withAlphaComponent(0.25), alignment: .left)
        addSubview(explainLabel)
        explainLabel.snp.makeConstraints { (make) in
            make.left.equalTo(titleLabel)
            make.bottom.equalTo(-12 * hScale)
            make.width.equalTo(284 * wScale)
        }
        explainLabel.numberOfLines = 2
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func awakeFromNib() {
        super.awakeFromNib()
        // Initialization code
    }

    override func setSelected(_ selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)

        // Configure the view for the selected state
    }

}
