//
//  DXDecisionEngineController.swift
//  Prototype
//
//  Created by Roger on 2020/3/24.
//  Copyright © 2020 test. All rights reserved.
//

import UIKit

class DXDecisionEngineController: DXBaseViewController {
    
    private var tableView       :   UITableView!
    private var noDataImageView :   UIImageView!
    private var checkRiskBtn   :   UIButton!
    private var noticeView      :   DXNoticeView!

    private let cellIdentifier  =   "dxDecisionCellIdentifier"
    
    private var dataArr:Array<(key: String, value: Any)>  =   []

    override func viewDidLoad() {
        super.viewDidLoad()

        initUI()
    }
    

    private func initUI() {
        view.backgroundColor = .white
        setTitle(text: "决策引擎")
        
        noDataImageView = UIImageView.init(image: #imageLiteral(resourceName: "nodata"))
        view.addSubview(noDataImageView)
        noDataImageView.snp.makeConstraints { (make) in
            make.top.equalTo(navBarView.snp.bottom).offset(48 * hScale)
            make.centerX.equalTo(view)
            make.size.equalTo(CGSize(width: 157 * wScale, height: 208 * hScale))
        }
        
        checkRiskBtn = UIButton(ch_title: "发送请求")
        checkRiskBtn.backgroundColor = UIColor(hex: 0x0FBDA0)
        checkRiskBtn.addTarget(self, action: #selector(click(_:)), for: .touchUpInside)
        checkRiskBtn.tag = 1000
        view.addSubview(checkRiskBtn)
        checkRiskBtn.snp.makeConstraints { (make) in
            make.top.equalTo(navBarView.snp.bottom).offset(310 * hScale)
            make.centerX.equalTo(view)
            make.size.equalTo(CGSize(width: 343 * wScale, height: 50 * hScale))
        }
        checkRiskBtn.layer.cornerRadius = 4 * hScale
        checkRiskBtn.layer.masksToBounds = true
        
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
            self.checkRiskBtn.isEnabled = true
        })
    }
    
    private func showFaileToast() {
        let toastView = DXToastView(frame: CGRect(x: 0, y: 0, width: 154 * wScale, height: 132 * hScale))
        self.navigationController?.view.showToast(toastView, duration: 2.0, point: CGPoint(x: 188 * wScale, y: 258 * hScale + navBarHeight + 44 * hScale), completion: { (isTaped) in
            self.checkRiskBtn.isEnabled = true
        })
    }
    
    @objc
    private func click(_ btn:UIButton) {
        if btn.tag == 1000 {
            self.navigationController?.view.makeToastActivity(.center)
            getTokeningTips()
            checkRiskBtn.isEnabled = false
            dataReset()
            if let token = DXNetworkManager.getToken() {
                tableView.isHidden = false
                noDataImageView.isHidden = true
                noticeView.isHidden = !noDataImageView.isHidden
                mockTokenTips()
                let dataDic = ["const_id":token]
                let eventCode = UserDefaults.standard.string(forKey: dxEventCode) ?? ""
                DXNetworkManager.checkRisk(token, param: ["eventCode":eventCode,"data":dataDic,"flag":"test"]) { (response) in
                    self.navigationController?.view.hideToastActivity()
                    self.getTokenSuccessTips()
                    if let resp = response?.content as? Dictionary<String, Any> {
                         if let stateCode = resp["status"] {
                            if (stateCode as! String) == "SUCCESS" {
                                if let data = resp["result"]  as? Dictionary<String, Any> {
                                    let keys = data.sorted(by: {$0.0<$1.0})
                                    self.dataArr.append(contentsOf: keys)
                                    self.tableView.reloadData()
                                    self.checkRiskBtn.isEnabled = true
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
           checkRiskBtn.snp.remakeConstraints { (make) in
               make.bottom.equalTo (-20 * hScale - tabBarHeight - 40 * hScale)
               make.centerX.equalTo(self.view)
               make.size.equalTo(CGSize(width: 343 * wScale, height: 50 * hScale))
           }
           checkRiskBtn.setTitle("重新发送请求", for: .normal)
           checkRiskBtn.isSelected = true
           view.layoutIfNeeded()
           
    }
       
    private func dataReset() {
        let resetDic:Dictionary<String, Any> = ["字段名":"数据"]
        dataArr = [resetDic.first!]
    }

}


extension DXDecisionEngineController:UITableViewDelegate,UITableViewDataSource {
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
        return cell
    }


    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        return
    }
}
