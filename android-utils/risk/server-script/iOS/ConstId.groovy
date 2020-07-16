def checkIdfaOrIdfv = {
        value ->
            if (StringUtils.isBlank(value)) {
                return false
            }
            if (StringUtils.length(value) != 36) {
                return false
            }
            // if (!Pattern.matches('^([0-9a-zA-Z]{1,})(([/\\s-][0-9a-zA-Z]{1,}){4})$', value)) {
            //     return false
            // }
            if (StringUtils.equals(value, '00000000-0000-0000-0000-000000000000')) {
                return false
            }
            return true
    }
    String mac = data['K1']
    if (mac != null && mac.length() == 17 && !'02:00:00:00:00:00'.equals(mac)) {
        device['macAddress'] = mac
    }
    device['idfa'] = data['K4']
    device['idfv'] = data['K5']
    device['uuid'] = data['K6']
    device['netType'] = data['K15']
    device['bootTime'] = data['K25']
    device['aslr'] = data['K24']
    device['inet'] = data['K27']
    device['brand'] = 'Apple'
    device['packageName'] = data['K46']
    device['system'] = 'iOS ' + data['K47']
    device['model'] = data['K48']
    device['gi'] = data['K70']
    device['injectFrame'] = data['K139']
    device['collectCostTime'] = data['K300']
    String jbprint = data['K42']
    //device['appVersion'] = data['app_version']
    //关联维度提取
    def index = []
    if (device['uuid'] != null) {
      println 'uuid:'+device['uuid']
        index << device['uuid']
    }
    if (device['macAddress'] != null) {
      println 'macAddress:'+device['macAddress']
        index << device['macAddress']
    }
    if (checkIdfaOrIdfv(device['idfa'])) {
        println 'idfa:'+device['idfa']
        index << device['idfa']
    }
    if (checkIdfaOrIdfv(device['idfv'])) {
        println 'idfv:'+device['idfv']
        index << device['idfv']
    }


    
    String bootTimeAslr = StringUtils.EMPTY
    if (StringUtils.isNotBlank(device['bootTime'])) {
        bootTimeAslr += device['bootTime']
    }
    if (StringUtils.isNotBlank(device['aslr'])) {
        bootTimeAslr += device['aslr']
    }
    if (StringUtils.isNotBlank(bootTimeAslr)) {
        println 'bootTimeAslr:'+bootTimeAslr
        index << bootTimeAslr
    }

    // for poc 改机指纹不变。jbprint关联
    // if (jbprint != null) {
    //     println 'jbprint:'+jbprint
    //     index << jbprint
    // }
    // for poc 刷机指纹不变。内外网ip关联
    // String ipId = StringUtils.EMPTY
    // if (StringUtils.isNotBlank(device['inet'])) {
    //     ipId += device['inet']
    // }
    // if (StringUtils.isNotBlank(device['createIp'])) {
    //     ipId += device['createIp']
    // }
    // if (StringUtils.isNotBlank(ipId)) {
    //     println 'ipId:'+ipId
    //     index << ipId
    // }

    device['index'] = index

