/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostData.h
 * @brief  后处理数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKCFDPostData_H
#define _FITKCFDPostData_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace Interface 
{
    class FITKCFDPost3DManager;
    class FITKCFDPostLightManager;
    class FITKCFDPostColorLibaryDataManager;
    /**
     * @brief  后处理数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostData : public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        FITKCFDPostData();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        ~FITKCFDPostData();
        /**
         * @brief    获取后处理数据管理器
         * @return   FITKCFDPost3DManager* 后处理管理器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        FITKCFDPost3DManager* getPostDataManager();
        /**
         * @brief    获取灯光管理器
         * @return   FITKCFDPostLightManager* 灯光管理器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        FITKCFDPostLightManager* getPostLightManager();
        /**
         * @brief    获取色库管理器
         * @return   FITKCFDPostColorLibaryDataManager *
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        FITKCFDPostColorLibaryDataManager* getPostColorLibManager();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        void init();
    private:
        /**
         * @brief  后处理管理器对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        FITKCFDPost3DManager* _manager = nullptr;
        /**
         * @brief  后处理灯光管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-16
         */
        FITKCFDPostLightManager* _postLightManager =  nullptr;
        /**
         * @brief  后处理色库管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-25
         */
        FITKCFDPostColorLibaryDataManager* _colorLibManager = nullptr;
    };
}

#endif
