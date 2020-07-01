//
//  DXRiskCell.swift
//  Prototype
//
//  Created by Roger on 2020/4/13.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit

class DXRiskCell: UITableViewCell {
    
    var titleText:String! {
        didSet {
            label.textColor = UIColor.black.withAlphaComponent(titleText == "字段名" ? 0.45 : 0.85)
            label.font = UIFont.systemFont(ofSize: titleText == "字段名" ? 14 * hScale : 15 * hScale)
            if let text = mapDic[titleText] {
                label.text = text
            }else {
                label.text = titleText
            }
        }
    }
    
    var dataText:String! {
        didSet {
            dataLabel.textColor = UIColor.black.withAlphaComponent(dataText == "数据" ? 0.45 : 0.85)
            dataLabel.font = UIFont.systemFont(ofSize: dataText == "数据" ? 14 * hScale : 15 * hScale)
            dataLabel.text = dataText ?? "-"
        }
    }
    
    private let mapDic      =   ["osVersion":"系统版本","isInject":"注入","isProxy":"代理","macAddress":"Mac地址","isMultirun":"多开","isDebug":"调试","isEmulator":"模拟器","constId":"设备指纹","osType":"OS版本","isFrameAttack":"工具攻击","isHook":"Hook","token":"Token","producter":"producter","deviceType":"deviceType","isJailBreak":"越狱","isVpn":"VPN"]
    private var label       :   UILabel!
    private var dataLabel   :   UILabel!
    
    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        initUI()
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func awakeFromNib() {
        super.awakeFromNib()
    }
    
    private func initUI() {
        self.backgroundColor = UIColor(hex: 0xF8F9FB)
        label = UILabel(ch_text: "", fontSize: 15 * hScale, color: UIColor.black.withAlphaComponent(0.85), alignment: .left)
        addSubview(label)
        label.snp.makeConstraints { (make) in
            make.left.equalTo(16 * wScale)
            make.centerY.equalTo(self)
            make.width.equalTo(80 * wScale)
        }
        label.numberOfLines = 1
        label.lineBreakMode = .byTruncatingMiddle
        
        dataLabel = UILabel(ch_text: "", fontSize: 15 * hScale, color: UIColor.black.withAlphaComponent(0.85), alignment: .left)
        addSubview(dataLabel)
        dataLabel.snp.makeConstraints { (make) in
            make.left.equalTo(125 * wScale)
            make.centerY.equalTo(self)
            make.width.equalTo(200 * wScale)
        }
        dataLabel.numberOfLines = 2
        
        drawLine(frame: CGRect(x: 0, y: 39 * hScale , width: 343 * wScale, height: 1 * hScale), color: .white)
        drawLine(frame: CGRect(x: 108 * wScale, y: 0, width: 1 * wScale, height: 40 * hScale), color: .white)

    }

    override func setSelected(_ selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)

        // Configure the view for the selected state
    }

}
