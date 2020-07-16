//
// Created by white on 2019/3/18.
//

#include "DXRiskInter.h"

#ifndef AS_RISK_DXRISKMANAGER_H
#define AS_RISK_DXRISKMANAGER_H


class DXRiskInterManager {
private:

    map<string, DXRiskInter*> m_allRiskMap;

    DXRiskInterManager();

    ~DXRiskInterManager();

    bool Contants(const string &appId);

public:

    static DXRiskInterManager* GetInstance();

    void Remove(const string &appId);

    DXRiskInter* Get(const string &appId);


};



#endif //AS_RISK_DXRISKMANAGER_H
