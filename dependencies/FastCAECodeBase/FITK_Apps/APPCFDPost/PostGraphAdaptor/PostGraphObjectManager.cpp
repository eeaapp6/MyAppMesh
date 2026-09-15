/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphObjectManager.h"
#include "PostGraphObjectAgent.h"
#include "PostGraphObjectBase.h"

#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"

namespace Interface 
{
    PostGraphObjectManager* PostGraphObjectManager::_instance = nullptr;
    QMutex PostGraphObjectManager::m_mutex;
    
    PostGraphObjectBase* PostGraphObjectManager::getGraphObjectBase(FITKAbstractCFDPostData* postData)
    {
        if (postData == nullptr)return nullptr;
        for (int i = 0; i < this->getDataCount(); i++) {
            PostGraphObjectAgent* agent = this->getDataByIndex(i);
            if (agent == nullptr)continue;
            PostGraphObjectBase* base = agent->getGraphObject();
            if(base == nullptr)continue;
            if (base->getPostID() == postData->getDataObjectID()) {
                return base;
            }
        }
        return nullptr;
    }

    PostGraphObjectBase* PostGraphObjectManager::getGraphObjectBase(int PostDataID)
    {
        if (PostDataID == -1)return nullptr;
        for (int i = 0; i < this->getDataCount(); i++) {
            PostGraphObjectAgent* agent = this->getDataByIndex(i);
            if (agent == nullptr)continue;
            PostGraphObjectBase* base = agent->getGraphObject();
            if (base == nullptr)continue;
            if (base->getPostID() == PostDataID) {
                return base;
            }
        }
        return nullptr;
    }

    PostGraphLightManager* PostGraphObjectManager::getGraphLightManager()
    {
        return _postLightManager;
    }

    void PostGraphObjectManager::initialize()
    {
        _postLightManager = new PostGraphLightManager();
    }

    void PostGraphObjectManager::finalize()
    {
        if (_postLightManager) {
            delete _postLightManager;
            _postLightManager = nullptr;
        }
    }
}
