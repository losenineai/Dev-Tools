//
// Created by white on 2019/3/18.
//

#include "DXRiskManager.h"


DXRiskManager::DXRiskManager():m_allRiskMap() {

}

DXRiskManager::~DXRiskManager(){

}

DXRiskManager* DXRiskManager::GetInstance() {
    static DXRiskManager manager;
    return &manager;
}

void DXRiskManager::Remove(const string &appId){
    if (Contants(appId)) {
        DXRisk *risk = m_allRiskMap.at(appId);
        delete (risk);
        m_allRiskMap.erase(appId);
    }
}

DXRisk* DXRiskManager::Get(const string &appId){
    if (!Contants(appId)) {
       DXRisk *risk = new DXRisk();
       m_allRiskMap[appId] = risk;
    }

    return m_allRiskMap.at(appId);
}

bool DXRiskManager::Contants(const string &appId){
    return m_allRiskMap.find(appId) != m_allRiskMap.end();
}