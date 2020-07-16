def getIndex ={
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

    // 作弊器检测
    def pkglist=data['K1500']
    def applog=data['K1501']
    def cheatAppList=''
    if(pkglist != null){
        cheatAppList+=pkglist.contains("net.hackerbot.hbsiteapp") == true ? 'Hacker Bot,':''
        cheatAppList+=pkglist.contains("ru.qQxnLnBd.UdzzaLcEz") == true ? 'Lucky Patcher,':''
        cheatAppList+=pkglist.contains("com.xxAssistant") == true ? '叉叉助手,':''
        cheatAppList+=pkglist.contains("com.touchsprite.android") == true ? '触动精灵,':''
        cheatAppList+=pkglist.contains("com.zhangkongapp.joke.bamenshenqi") == true ? '八门神器,':''
        cheatAppList+=pkglist.contains("com.huluxia.gametools") == true ? '葫芦侠,':''
        cheatAppList+=pkglist.contains("org.sbtools.gamehack") == true ? '烧饼游戏修改器,':''
        cheatAppList+=pkglist.contains("com.cyjh.gundam") == true ? '游戏蜂窝,':''
        cheatAppList+=pkglist.contains("com.lizimodifier") == true ? '栗子单机游戏修改器,':''
        cheatAppList+=pkglist.contains("com.xiongmaoxia.gameassistant") == true ? '熊猫侠游戏修改器,':''
        cheatAppList+=pkglist.contains("com.paojiao.youxia") == true ? '泡椒修改器t,':''
        cheatAppList+=pkglist.contains("com.joke.bamenshenqi.mgame") == true ? '圈圈游戏助手,':''
        cheatAppList+=pkglist.contains("cc.madkite.freedom") == true ? 'Freedom APK,':''
        cheatAppList+=pkglist.contains("com.sj4399.terrariapeaid") == true ? '4399泰拉瑞亚助手,':''
    }

    if(applog != null){
        cheatAppList+=applog.contains("android") == true ? 'GameGuardian,':''
    }

    if(pkglist || applog){
        device['cheatApps']=cheatAppList
    }

    // 字段篡改检测
    def notMatch=""

    def nativeMac=data['K1800']
    def javaMac=data['K75']
    if(nativeMac != null && javaMac != null){
        if(nativeMac != javaMac){
            notMatch+="|mac"
        }
    }

    def nativeSerial=data['K1131']
    def javaSerial=data['K57']
    if(nativeSerial != null && javaSerial != null){
        if(nativeSerial != javaSerial){
            notMatch+="|serial"
        }
    }

    def nativeBootloader=data['K1090']
    def javaBootloader=data['K26']
    if(nativeBootloader != null && javaBootloader != null){
        if(nativeBootloader != javaBootloader){
            notMatch+="|bootloader";
        }
    }

    def nativeAbiList=data['K1116']
    def javaAbiList=data['K21']
    if(nativeAbiList != null && javaAbiList != null){
        if(nativeAbiList != javaAbiList){
            notMatch+="|abilist"
        }
    }

    def nativeProduct=data['K1110']
    def javaProduct=data['K20']
    if(nativeProduct != null && javaProduct != null){
        if(nativeProduct != javaProduct){
            notMatch+="|product"
        }
    }

    def nativeModel=data['K1111']
    def javaModel=data['K18']
    if(nativeModel != null && javaModel != null){
        if(nativeModel != javaModel){
            notMatch+="|model"
        }
    }

    if(notMatch!=""){
        device['notMatch']=notMatch
    }
