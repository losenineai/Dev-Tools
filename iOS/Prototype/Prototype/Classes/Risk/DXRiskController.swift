//
//  ViewController.swift
//  Prototype
//
//  Created by Roger on 2020/3/24.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit
import Toast_Swift


class DXRiskController: DXBaseViewController {
    
    private var tableView       :   UITableView!
    private var noDataImageView :   UIImageView!
    private var getRiskBtn      :   UIButton!
    private var noticeView      :   DXNoticeView!
    private let cellIdentifier  =   "dxRiskCellIdentifier"
    private var dataArr:Array<(key: String, value: Any)>  =   []


    override func viewDidLoad() {
        super.viewDidLoad()
        initUI()
        configNetWork()
        test()
    }
    
    private func test() {
        let eventCode       = "constid_demo"
        let server          = "https://rcs-demo.dingxiang-inc.com"
        let appid           = "77ca0992a99095d64e40f33dee0c1acc"
        let appsecret       = "224ce365af802f6d93af32a6b1bef518"
//        let server          = "https://dingxiang-inc.com"
//        let appid           = "0fd68c9f8f253db0b163b0aa345da747"
//        let appsecret       = "bf69ee3d8994e90d1eb3e95cc9ac8832"
        
        
//        let server          = "http://10.1.2.7:7776"
//        let appid           = "e41ce0545c23fb81d66eabe38425cbb9"
//        let appsecret       = "33e31b5c0acb601219d2f17661800a55"

        UserDefaults.standard.set(server, forKey: dxServer)
        UserDefaults.standard.set(appid, forKey: dxAppid)
        UserDefaults.standard.set(appsecret, forKey: dxAppSecret)
        UserDefaults.standard.set(eventCode, forKey: dxEventCode)
        YSHttpConfigure.shareInstance().generalServer = server

    }
    
    private func configNetWork() {
        YSHttpConfigure.shareInstance().generalServer = UserDefaults.standard.string(forKey: dxServer) ?? ""
    }
    
    private func initUI() {
        view.backgroundColor = .white
        setTitle(text: "设备指纹")
        
        noDataImageView = UIImageView.init(image: #imageLiteral(resourceName: "nodata"))
        view.addSubview(noDataImageView)
        noDataImageView.snp.makeConstraints { (make) in
            make.top.equalTo(navBarView.snp.bottom).offset(48 * hScale)
            make.centerX.equalTo(view)
            make.size.equalTo(CGSize(width: 157 * wScale, height: 208 * hScale))
        }
        
        getRiskBtn = UIButton(ch_title: "获取设备指纹")
        getRiskBtn.backgroundColor = UIColor(hex: 0x0FBDA0)
        getRiskBtn.addTarget(self, action: #selector(click(_:)), for: .touchUpInside)
        getRiskBtn.tag = 1000
        view.addSubview(getRiskBtn)
        getRiskBtn.snp.makeConstraints { (make) in
            make.top.equalTo(310 * hScale + navBarHeight + 64 * hScale)
            make.centerX.equalTo(view)
            make.size.equalTo(CGSize(width: 343 * wScale, height: 50 * hScale))
        }
        getRiskBtn.layer.cornerRadius = 4 * hScale
        getRiskBtn.layer.masksToBounds = true
        
        noticeView = DXNoticeView()
        noticeView.tipsText = "请按顺序填写，每一项内容用换行分割"
        noticeView.tipsImage = #imageLiteral(resourceName: "going")
        view.addSubview(noticeView)
        noticeView.snp.makeConstraints { (make) in
            make.left.right.equalTo(0)
            make.top.equalTo(navBarView.snp.bottom).offset(8 * hScale)
            make.height.equalTo(34 * hScale)
        }
        noticeView.isHidden = true
        
        tableView = UITableView(frame: .zero, style: .plain)
        view.addSubview(tableView)
        tableView.snp.makeConstraints { (make) in
            make.left.equalTo(16 * wScale)
            make.right.equalTo(-16 * wScale)
            make.top.equalTo(navBarView.snp.bottom).offset(45 * hScale)
            make.height.equalTo(440 * hScale)
        }
        tableView.register(DXRiskCell.classForCoder(), forCellReuseIdentifier: cellIdentifier)
        tableView.delegate = self
        tableView.dataSource = self
        tableView.backgroundColor = UIColor(hex: 0xF8F9FB)
        tableView.separatorStyle = .none
        tableView.layer.cornerRadius = 8 * hScale
        tableView.isHidden = true
        
    }
    
    private func mockTokenTips() {
        noticeView.tipsText = getCurrentTime() +  "模拟业务端获取信息..."
        noticeView.tipsImage = #imageLiteral(resourceName: "going")
    }
    
    private func getTokeningTips() {
        noticeView.tipsText = getCurrentTime() +  "获取token..."
        noticeView.tipsImage = #imageLiteral(resourceName: "going")
    }
    
    private func getTokenSuccessTips() {
        noticeView.tipsText = getCurrentTime() +  "设备指纹获取成功"
        noticeView.tipsImage = #imageLiteral(resourceName: "finish")
    }
    
    private func getCurrentTime() -> String {
        let formatter = DateFormatter()
        formatter.dateFormat = "YYYY.MM.dd  HH:mm:ss   "
        return formatter.string(from: Date())
    }
    
    private func showSuccessToast() {
        let toastView = DXToastView(frame: CGRect(x: 0, y: 0, width: 154 * wScale, height: 114 * hScale))
        self.navigationController?.view.showToast(toastView, duration: 2.0, point: CGPoint(x: 188 * wScale, y: 258 * hScale + navBarHeight + 44 * hScale), completion: { (isTaped) in
            self.getRiskBtn.isEnabled = true
        })
    }
    
    private func showFaileToast() {
        let toastView = DXToastView(frame: CGRect(x: 0, y: 0, width: 154 * wScale, height: 132 * hScale))
        self.navigationController?.view.showToast(toastView, duration: 2.0, point: CGPoint(x: 188 * wScale, y: 258 * hScale + navBarHeight + 44 * hScale), completion: { (isTaped) in
            self.getRiskBtn.isEnabled = true
        })
    }
    
    @objc private func click(_ btn:UIButton) {
        if btn.tag == 1000 {
            self.navigationController?.view.makeToastActivity(.center)
            getTokeningTips()
            getRiskBtn.isEnabled = false
            dataReset()
            if let token = DXNetworkManager.getToken() {
                tableView.isHidden = false
                noDataImageView.isHidden = true
                noticeView.isHidden = !noDataImageView.isHidden
                mockTokenTips()
                DXNetworkManager.getDeviceInfo(token) { (response) in
                    self.navigationController?.view.hideToastActivity()
                    self.getTokenSuccessTips()
                    if let resp = response?.content as? Dictionary<String, Any> {
                        if let stateCode = resp["stateCode"] {
                            if (stateCode as! Int) == 200 {
                                if var data = resp["data"] as? Dictionary<String, Any> {
                                    print(data)
                                    let keys = data.pancakeDic().sorted(by: {$0.0<$1.0})
                                    self.dataArr.append(contentsOf: keys)
                                    self.tableView.reloadData()
                                    self.getRiskBtn.isEnabled = true
                                    self.updateBtnFrame()
                                    return
                                }
                            }
                        }
                    }
                    if response?.status == .error {
                        self.showFaileToast()
                    }
                }
            }else {
                self.navigationController?.view.hideToastActivity()
                showFaileToast()
            }
        }
    }
    
    private func updateBtnFrame() {
        getRiskBtn.snp.remakeConstraints { (make) in
            make.bottom.equalTo (-20 * hScale - tabBarHeight - 40 * hScale)
            make.centerX.equalTo(self.view)
            make.size.equalTo(CGSize(width: 343 * wScale, height: 50 * hScale))
        }
        getRiskBtn.setTitle("重新获取设备指纹", for: .normal)
        getRiskBtn.isSelected = true
        view.layoutIfNeeded()
        
    }
    
    private func dataReset() {
        let resetDic:Dictionary<String, Any> = ["字段名":"数据"]
        dataArr = [resetDic.first!]
    }
}

extension DXRiskController:UITableViewDelegate,UITableViewDataSource {
    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        return 40 * hScale
    }
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return dataArr.count
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: cellIdentifier, for: indexPath) as! DXRiskCell
        cell.titleText = dataArr[indexPath.row].key
        cell.dataText = dataArr[indexPath.row].value as? String
        if let data = dataArr[indexPath.row].value as? Int {
            cell.dataText = data == 1 ? "是" : "否"
        }
        return cell
    }


    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        return
    }
}
