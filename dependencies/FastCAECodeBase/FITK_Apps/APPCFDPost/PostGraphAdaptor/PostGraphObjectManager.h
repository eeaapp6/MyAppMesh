/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PostGraphObjectManager.h
 * @brief  后处理渲染对象管理器
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef __PostGraphObjectManager_H__
#define __PostGraphObjectManager_H__

#include "PostGraphObjectAgent.h"
#include "PostGraphObjectLight.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"

#include <QMutex>
#include <QMutexLocker>
#include <QList>

namespace Interface
{
    class FITKAbstractCFDPostData;
    class PostGraphObjectBase;
    class PostGraphObjectLight;
    /**
     * @brief  灯光管理器
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-16
     */
    class PostGraphAdaptorAPI PostGraphLightManager : public Core::FITKAbstractDataManager<PostGraphObjectLight>
    {
    public:
        PostGraphLightManager() = default;
        ~PostGraphLightManager() = default;
    };

    /**
     * @brief  后处理渲染对象管理器
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class PostGraphAdaptorAPI PostGraphObjectManager : public Core::FITKAbstractDataManager<PostGraphObjectAgent>
    {
        /**
         * @brief    单例声明
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-10
         */
        DeclSingleton(PostGraphObjectManager);
    public:
        /**
         * @brief 获取后处理数据对应的渲染对象数据
         * @param[i]  postData             后处理数据
         * @return PostGraphObjectBase*    渲染对象数据
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-12
         */
        PostGraphObjectBase* getGraphObjectBase(FITKAbstractCFDPostData* postData);
        /**
         * @brief    获取后处理数据对应的渲染对象数据
         * @param[i] PostDataID             后处理数据ID
         * @return   PostGraphObjectBase*   渲染对象数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        PostGraphObjectBase* getGraphObjectBase(int PostDataID);
        /**
         * @brief    获取灯光管理器
         * @return   PostGraphLightManager*  灯光管理器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        PostGraphLightManager* getGraphLightManager();
    private:
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        void initialize();
        /**
         * @brief 结束，析构函数调用
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        void finalize();
    private:
        PostGraphLightManager* _postLightManager = nullptr;
    };
}
#endif