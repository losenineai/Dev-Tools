//
//  DXSetViewController.swift
//  Prototype
//
//  Created by Roger on 2020/3/24.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit

class DXSetViewController: DXBaseViewController {
    
    
    private var tableView       :UITableView!
    private var userSetBtn      :UIButton!
    private let cellIdentifier  = "dxCellIdentifier"
    private let titlesArr       = ["服务器地址 *","AppID *","AppSecret *","产品Code","事件Code","缓存设备指纹 Token"]
    private let errorsArr       = ["服务器连接失败/请填写服务器地址","鉴权失败，请重新设置/请填写鉴权信息","鉴权失败，请重新设置/请填写鉴权信息"]
    private let placeholdersArr = ["请输入服务器地址","请输入AppID","请输入AppSecret","使用决策引擎Demo时填写","使用决策引擎Demo时填写"]
    private var textArr         = [String]()
    private let keyArr          = [dxServer,dxAppid,dxAppSecret,dxEventCode,dxProductCode]
    private var isShowError     = false
    
    override func viewDidLoad() {
        super.viewDidLoad()
        initUI()
    }

    private func initUI() {
        view.backgroundColor = .white
        setTitle(text: "设置", btnImage: #imageLiteral(resourceName: "update")) {
            print("next view")
            let configController = DXConfigController()
            configController.block = {(textArr) -> Void in
                self.textArr = textArr
                self.tableView.reloadData()
            }
            self.navigationController?.pushViewController(configController, animated: true)
            
        }
        for i in 0..<keyArr.count {
            let value = UserDefaults.standard.string(forKey: keyArr.index(i)!)
            textArr.append(value ?? "")
        }
        
        tableView = UITableView(frame: .zero, style: .plain)
        view.addSubview(tableView)
        tableView.snp.makeConstraints { (make) in
            make.left.right.equalTo(0)
            make.top.equalTo(navBarView.snp.bottom)
            make.height.equalTo(440 * hScale)
        }
        tableView.register(DXSetCell.classForCoder(), forCellReuseIdentifier: cellIdentifier)
        tableView.delegate = self
        tableView.dataSource = self
        tableView.isScrollEnabled = false
        tableView.separatorStyle = .none

        userSetBtn = UIButton(ch_title: "应用设置")
        userSetBtn.setTitle("应用设置", for: .selected)
        userSetBtn.backgroundColor = UIColor(hex: 0x0FBDA0)
        userSetBtn.addTarget(self, action: #selector(click(_:)), for: .touchUpInside)
        userSetBtn.tag = 1000
        view.addSubview(userSetBtn)
        userSetBtn.snp.makeConstraints { (make) in
            make.bottom.equalTo(-41 * hScale - 48 * hScale - tabBarHeight)
            make.centerX.equalTo(view)
            make.size.equalTo(CGSize(width: 343 * wScale, height: 50 * hScale))
        }
        userSetBtn.layer.cornerRadius = 4 * hScale
        userSetBtn.layer.masksToBounds = true
    }
    
    @objc
    private func click(_ btn:UIButton) {
        if btn.tag == 1000 {
            btn.isEnabled = false
            DXTool.lightFeedback()
            var textArr = [String]()
            var url:String!
            for i in 0..<6 {
                let cell = tableView.cellForRow(at: IndexPath(row: i, section: 0)) as! DXSetCell
                if i < 5 {
                    let text = cell.fieldText
                    if i == 0 { url = text }
                    textArr.append(text ?? "")
                }else {
                    let isOn = cell.switchIsOn
                    UserDefaults.standard.set(isOn, forKey: dxCache)
                }
            }
            
            DXNetworkManager.verifyHost(url, respone: { (response) in
                if response?.status == .success {
                    self.showSuccessToast()
                    self.saveSettings(textArr)
                    self.isShowError = false
                }else if response?.status == .error {
                    self.showFaileToast()
                    self.isShowError = true
                }
                self.tableView.reloadData()
            })
        }
    }
    
    private func showSuccessToast() {
        let toastView = DXToastView(frame: CGRect(x: 0, y: 0, width: 154 * wScale, height: 114 * hScale))
        self.navigationController?.view.showToast(toastView, duration: 2.0, point: CGPoint(x: 188 * wScale, y: 258 * hScale + navBarHeight + 44 * hScale), completion: { (isTaped) in
            self.userSetBtn.isEnabled = true
        })
    }
    
    private func showFaileToast() {
        let toastView = DXToastView(frame: CGRect(x: 0, y: 0, width: 154 * wScale, height: 132 * hScale))
        self.navigationController?.view.showToast(toastView, duration: 2.0, point: CGPoint(x: 188 * wScale, y: 258 * hScale + navBarHeight + 44 * hScale), completion: { (isTaped) in
            self.userSetBtn.isEnabled = true
        })
    }
    
    private func saveSettings(_ setArr:[String]) {
        for i in 0..<keyArr.count {
            UserDefaults.standard.set(setArr.index(i), forKey: keyArr.index(i)!)
        }
        YSHttpConfigure.shareInstance().generalServer = UserDefaults.standard.value(forKey: dxServer) as! String
    }
}


extension DXSetViewController:UITableViewDelegate,UITableViewDataSource {
    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        return indexPath.row == 5 ? 90 * hScale : 70 * hScale
    }
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return 6
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: cellIdentifier, for: indexPath) as! DXSetCell
        cell.titleText          = titlesArr.index(indexPath.row) ?? ""
        cell.errorText          = errorsArr.index(indexPath.row) ?? ""
        cell.placeholderText    = placeholdersArr.index(indexPath.row) ?? ""
        cell.needSwitch         = indexPath.row == 5
        cell.isShowError        = isShowError ? indexPath.row == 0 : false
        if indexPath.row < 5 {
            cell.fieldText      = textArr.index(indexPath.row) ?? ""
        }else {
            cell.switchIsOn     = UserDefaults.standard.bool(forKey: dxCache)
        }
        return cell
    }


    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        return
    }

}
