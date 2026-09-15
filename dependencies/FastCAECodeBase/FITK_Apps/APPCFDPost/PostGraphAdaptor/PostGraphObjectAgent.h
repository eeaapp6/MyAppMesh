/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PostGraphObjectAgent.h
 * @brief  后处理渲染对象代理类（添加代理类是为了让系统的管理类识别）
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _PostGraphObjectAgent_H
#define _PostGraphObjectAgent_H

#include "PostGraphAdaptorAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace Interface 
{
    class PostGraphObjectBase;
    /**
     * @brief  后处理渲染对象代理类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class PostGraphAdaptorAPI PostGraphObjectAgent : public Core::FITKAbstractDataObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] postData   后处理实际渲染对象 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        explicit PostGraphObjectAgent(PostGraphObjectBase* postData);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        ~PostGraphObjectAgent();
        /**
         * @brief    获取实际渲染对象
         * @return   PostGraphObjectBase*  实际渲染对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        PostGraphObjectBase* getGraphObject();
    protected:
        /**
         * @brief  实际渲染对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        PostGraphObjectBase* _postData = nullptr;
    };
}

#endif
