//
//  DXExtension.swift
//  Prototype
//
//  Created by Roger on 2020/3/24.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit

extension UIColor {
    // UIColor(hexString: 0x50E3C2, alpha: 1.0)
    public convenience init(hex: UInt32, alpha: CGFloat = 1.0) {
        let red     = CGFloat((hex & 0xFF0000) >> 16) / 255.0
        let green   = CGFloat((hex & 0x00FF00) >> 8 ) / 255.0
        let blue    = CGFloat((hex & 0x0000FF)      ) / 255.0
        self.init(red: red, green: green, blue: blue, alpha: alpha)
    }

    public convenience init(r: CGFloat, g: CGFloat, b: CGFloat, alpha: CGFloat = 1.0) {
        let red     = r / 255.0
        let green   = g / 255.0
        let blue    = b / 255.0

        self.init(red: red, green: green, blue: blue, alpha: alpha)
    }

    /// 生成随机颜色
    ///
    /// - returns: UIColor
    class func ch_randomColor() -> UIColor {
        return UIColor(r: CGFloat(arc4random() % 256), g: CGFloat(arc4random() % 256), b: CGFloat(arc4random() % 256)).withAlphaComponent(0.4)
    }
}

extension UILabel {

    /// 创建 UILabel
    ///
    /// - parameter text:      text
    /// - parameter fontSize:  fontSize，默认 15
    /// - parameter color:     color，默认 黑色
    /// - parameter alignment: alignment，默认左对齐
    ///
    /// - returns: UILabel
    convenience init(ch_text text: String?,
                     fontSize: CGFloat = 15,
                     color: UIColor = UIColor.black,
                     alignment: NSTextAlignment = .natural) {

        self.init()
        self.text = text
        self.textColor = color
        self.font = UIFont(name: "PingFangSC-Regular", size: fontSize)
//        self.font = UIFont.systemFont(ofSize: fontSize)
        self.textAlignment = alignment

        self.numberOfLines = 0

        // 自动调整大小
        sizeToFit()
    }

    convenience init(ch_text text: String?,
                     fontSize: CGFloat = 15,
                     colorInt: UInt32,
                     alignment: NSTextAlignment = .natural) {

        self.init(ch_text: text, fontSize: fontSize, color: UIColor(hex: colorInt), alignment: alignment)
    }
}

extension UIView {
    /// 划线
    @discardableResult func drawLine(frame: CGRect, color: UIColor) -> CALayer{
        let bottomBorder = CALayer()
        bottomBorder.frame = frame
        bottomBorder.backgroundColor = color.cgColor
        bottomBorder.cornerRadius = min(frame.width, frame.height) / 2
        self.layer.addSublayer(bottomBorder)
        return bottomBorder
    }
}

extension Array {
    func index(_ i:Int) -> Element? {
        return i >= self.count ? nil : self[i]
    }
}

extension String {
    /// 要是用md5要创建oc桥接文件并#import <CommonCrypto/CommonDigest.h>
    func md5() -> String {
        let str = self.cString(using: String.Encoding.utf8)
        let strLen = CUnsignedInt(self.lengthOfBytes(using: String.Encoding.utf8))
        let digestLen = Int(CC_MD5_DIGEST_LENGTH)
        let result = UnsafeMutablePointer<CUnsignedChar>.allocate(capacity: digestLen)
        CC_MD5(str!, strLen, result)
        let hash = NSMutableString()
        for i in 0 ..< digestLen {
            hash.appendFormat("%02x", result[i])
        }
        return String(format: hash as String)
    }
    
    func base64() -> String? {
        let data = self.data(using: .utf8)
        let base64String = data?.base64EncodedString()
        return base64String
    }
}

extension Dictionary {
//    func dic2String() -> String {
//        var dicStr = ""
//        for key in self.keys {
//            if let value = self[key] as? String {
//                dicStr += key as! String + value
//            }else if let dic = self[key] as? Dictionary {
//                dicStr += dic.dic2String()
//            }
//        }
//        return dicStr
//    }
    
    func dic2String() -> String {
        let dic = self as! Dictionary<String, Any>
        let dicStr = "eventCode" + (dic["eventCode"] as! String) + "flag" + (dic["flag"] as! String) + "const_id" + ((dic["data"] as! Dictionary<String, Any>)["const_id"] as! String)
        return dicStr
    }
    
    func dic2JsonString() -> String? {
        let data = try! JSONSerialization.data(withJSONObject: self, options: .prettyPrinted)
        var str = String(data: data, encoding: .utf8)
        /// 去掉空格
        str = str?.replacingOccurrences(of: " ", with: "")
        str = str?.replacingOccurrences(of: "\n", with: "")
        str = str?.replacingOccurrences(of: "\\", with: "")
        return str
    }
    
    mutating func apped(_ otherDic:Dictionary) {
        for (key,value) in otherDic.reversed() {
            self[key] = value
        }
    }
    
    mutating func pancakeDic() -> Dictionary{
        for key in self.keys {
            if var value = self[key] as? Dictionary {
                self.removeValue(forKey: key)
                let dic = value.pancakeDic()
                self.apped(dic)
            }
        }
        return self
    }
}
