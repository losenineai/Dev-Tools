//
// Created by white on 2019/3/18.
//

#include "DXRiskInterManager.h"


DXRiskInterManager::DXRiskInterManager():m_allRiskMap() {

}

DXRiskInterManager::~DXRiskInterManager(){

}

DXRiskInterManager* DXRiskInterManager::GetInstance() {
    static DXRiskInterManager manager;
    return &manager;
}

void DXRiskInterManager::Remove(const string &appId){
    if (Contants(appId)) {
        DXRiskInter *risk = m_allRiskMap.at(appId);
        delete (risk);
        m_allRiskMap.erase(appId);
    }
}

DXRiskInter* DXRiskInterManager::Get(const string &appId){
    if (!Contants(appId)) {
       DXRiskInter *risk = new DXRiskInter();
       m_allRiskMap[appId] = risk;
    }

    return m_allRiskMap.at(appId);
}

bool DXRiskInterManager::Contants(const string &appId){
    return m_allRiskMap.find(appId) != m_allRiskMap.end();
}
