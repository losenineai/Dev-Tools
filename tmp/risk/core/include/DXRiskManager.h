//
// Created by white on 2019/3/18.
//

#include "DXRisk.h"

#ifndef AS_RISK_DXRISKMANAGER_H
#define AS_RISK_DXRISKMANAGER_H


class DXRiskManager {
private:

    map<string, DXRisk*> m_allRiskMap;

    DXRiskManager();

    ~DXRiskManager();

    bool Contants(const string &appId);

public:

    static DXRiskManager* GetInstance();

    void Remove(const string &appId);

    DXRisk* Get(const string &appId);


};



#endif //AS_RISK_DXRISKMANAGER_H