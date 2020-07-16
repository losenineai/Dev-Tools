constantId{{def getIndex ={
	   str,arr ->
	   if(StringUtils.isNotBlank(str)){
	   arr << str
	   }
	}
	device['uuid']=data['K58']
    device['deviceId']=data['K1700'] != null ? data['K1700']:data['K50']
    device['mac']=data['K1800'] != null ? data['K1800']:data['K75']
    device['serial']=data['K1131'] != null ? data['K1131']:(data['K1091'] != null ? data['K1091']:data['K57'])
    device['aid']=data['K56']
    //设备标签信息提取
    device['model']=data['K18']
    device['brand']=data['K19']
    device['userId']=data['user_U1']
    //补充内网ip和gpu信息
    device['inet']=data['K79']
    device['gi']=data['K62']
    device['packageName']=data['K2']
    device['system']=data['K11']
    device['appVersion']=data['K4']
    device['injectFrame']=data['K139']
    device['collectCostTime']=data['K300']
    //关联维度提取
    def index = []
	getIndex(GroovyUtils.getIndex(device['serial'],device['deviceId'],device['mac']),index)
	getIndex(GroovyUtils.getIndex(device['aid'],device['deviceId'],device['mac']),index)
	getIndex(GroovyUtils.getIndex(device['uuid']),index)
    getIndex(GroovyUtils.getIndex(device['mac']),index)
    device['index'] = index

    
}}risk{{ 

    def checkRouteProxy ={
        ipstrs ->
            def iplist = ipstrs.tokenize('|')

            def reg_pattern = ~'^1(((0|27)(.(([1-9]?|1[0-9])[0-9]|2([0-4][0-9]|5[0-5])))|(72.(1[6-9]|2[0-9]|3[01])|92.168))(.(([1-9]?|1[0-9])[0-9]|2([0-4][0-9]|5[0-5]))){2})$'
            //私有ip正则匹配

            def publicIpNum=0
            def isRoute=false

            iplist.each{
                ipaddr->
                    def single_ip = ipaddr
                    
                    if(reg_pattern.matcher(single_ip).matches() == false){ //公有ip
                        publicIpNum++
                    }
            }

            if(publicIpNum >= 2){
                isRoute = true
            }else{
                isRoute = false
            }
            return isRoute
    }

    def checkFake ={
        left,right ->
            if(StringUtils.isBlank(left) || StringUtils.isBlank(right)){
                return false
            }

            return (left != right)
    }

    def checkEmulator = {
        emulator,fingerPrint->
            if (StringUtils.isNotBlank(fingerPrint) && 'redfinger'.equals(fingerPrint.toLowerCase())) {
                return true
            }
            if (StringUtils.isBlank(emulator)) {
                return false
            }
           
            try {
                String[] arr = emulator.split("\\|")
                /** 解析模拟器信息 **/
                for (int i = 0; i < arr.length; i++) {
                    try {
                        Integer id = Integer.valueOf(arr[i])
                        if (id > 0 && id < 100 && id != 10) {
                            return true
                        }
                    } catch (Exception e) {
                    }
                    
                }
            } catch (Exception e) {
                return false
            }
            return false
    }
    risk['isEmulator'] = checkEmulator.call(data['K130'], data['K13'])
    risk['isInject'] = StringUtils.isNotBlank(data['K131'])
    risk['isHook'] = StringUtils.isNotBlank(data['K131'])
    risk['isDump'] = StringUtils.isNotBlank(data['K132'])
    risk['isDebug'] = StringUtils.isNotBlank(data['K133'])
    risk['isMultirun'] = StringUtils.isNotBlank(data['K134'])
    risk['isFlawJanus'] = StringUtils.isNotBlank(data['K135'])
    risk['isRoot'] = StringUtils.isNotBlank(data['K136'])
    risk['isVpn'] = StringUtils.isNotBlank(data['K72'])
    risk['isProxy'] = StringUtils.isNotBlank(data['K70'])
    risk['isFrameAttack'] = StringUtils.isNotBlank(data['K139'])


    // 作弊器检测
    def pkglist = data['K1500']
    def applog = data['K1501']
    def cheatAppList = ""
    if(pkglist != null){
        cheatAppList += pkglist.contains("net.hackerbot.hbsiteapp") == true ? 'Hacker Bot,':''
        cheatAppList += pkglist.contains("ru.qQxnLnBd.UdzzaLcEz") == true ? 'Lucky Patcher,':''
        cheatAppList += pkglist.contains("com.xxAssistant") == true ? '叉叉助手,':''
        cheatAppList += pkglist.contains("com.touchsprite.android") == true ? '触动精灵,':''
        cheatAppList += pkglist.contains("com.zhangkongapp.joke.bamenshenqi") == true ? '八门神器,':''
        cheatAppList += pkglist.contains("com.huluxia.gametools") == true ? '葫芦侠,':''
        cheatAppList += pkglist.contains("org.sbtools.gamehack") == true ? '烧饼游戏修改器,':''
        cheatAppList += pkglist.contains("com.cyjh.gundam") == true ? '游戏蜂窝,':''
        cheatAppList += pkglist.contains("com.lizimodifier") == true ? '栗子单机游戏修改器,':''
        cheatAppList += pkglist.contains("com.xiongmaoxia.gameassistant") == true ? '熊猫侠游戏修改器,':''
        cheatAppList += pkglist.contains("com.paojiao.youxia") == true ? '泡椒修改器t,':''
        cheatAppList += pkglist.contains("com.joke.bamenshenqi.mgame") == true ? '圈圈游戏助手,':''
        cheatAppList += pkglist.contains("cc.madkite.freedom") == true ? 'Freedom APK,':''
        cheatAppList += pkglist.contains("com.sj4399.terrariapeaid") == true ? '4399泰拉瑞亚助手,':''
    }
    if(applog != null){
        cheatAppList += applog.contains("android") == true ? 'GameGuardian,':''
    }
    if(pkglist || applog){
        risk['hasCheatApps'] = cheatAppList
    }

    // 字段篡改检测
    def fieldInfo = ""
    if(checkFake(data['K75'], data['K1800'])){
        fieldInfo += "|mac"
    }
    if(checkFake(data['K57'], data['K1131'])){
        fieldInfo += "|serial"
    }
    if(checkFake(data['K26'], data['K1090'])){
        fieldInfo += "|bootloader"
    }
    if(checkFake(data['K21'], data['K1116'])){
        fieldInfo += "|abilist"
    }
    if(checkFake(data['K20'], data['K1110'])){
        fieldInfo += "|product"
    }
    if(checkFake(data['K18'], data['K1111'])){
        fieldInfo += "|model"
    }
    if(fieldInfo != ""){
        risk['isFieldFake'] = fieldInfo
    }

    // GPS篡改检测
    risk['isGpsFake'] = checkFake(data['K125'], data['K2201'])

    // 基站篡改检测
    risk['isBsFake'] = (checkFake(data['K701'], data['K711'])
                        && checkFake(data['K702'], data['K712'])
                        && checkFake(data['K703'], data['K713'])
                        && checkFake(data['K704'], data['K714'])
                        && checkFake(data['K705'], data['K715'])
                        && checkFake(data['K706'], data['K716']))

    // WiFi定位篡改检测
    risk['isWiFiLocationFake'] = checkFake(data['K603'], data['K605'])

    // DNS篡改检测
    risk['isDNSFake'] = checkFake(data['K2300'], data['K2301'])

    // 路由器代理检测
    risk['isRouteProxy'] = checkRouteProxy(data['K15002'])

  }}label{{label['isTamperPck']='篡改包'
label['isEmulator']='模拟器'
label['isInject']='注入'
label['isHook']='Hook'
label['isDump']='内存Dump'
label['isDebug']='调试'
label['isMultirun']='多开'
label['isFlawJanus']='漏洞攻击'
label['isRoot']='Root'
label['isVpn']='VPN'
label['isProxy']='代理'
label['isFrameAttack']='框架攻击'}}return{{Map data = JSON.parseObject(device['data'], Map.class)
Set<String> risks = GroovyUtils.getRiskSet(device['riskCode'])
    Object value
    for (String e : retList) {
        if (e.startsWith('is')) {
            value = risks.contains(e) ? true : false
        } else {
            switch (e) {
                case 'producter':
                    value = data['K24']
                    break
                case 'deviceType':
                    value = 'Android'
                    break
                case 'macAddress':
                    value = data['kK75']
                    break
                case 'constId':
                    value = GroovyUtils.getConstId(device['hardId'])
                    break
                case 'osType':
                    value = 'Android'
                    break
                case 'osVersion':
                    value = data['K11']
                    break
                case 'score':
                    value = device['riskScore']
                    break
                case 'createIp':
                    value = device['createIp']
                    break
                case 'appVersion':
                    value = device['appVersion']
                    break
                default:
                    continue
            }
        }
        ret[e] = value

    }}}