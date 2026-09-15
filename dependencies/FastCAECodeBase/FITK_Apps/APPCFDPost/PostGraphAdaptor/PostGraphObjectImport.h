/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PostGraphObjectImport.h
 * @brief  导入渲染数据对象
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-10
 *********************************************************************/
#ifndef _PostGraphObjectImport_H
#define _PostGraphObjectImport_H

#include "PostGraphAdaptorAPI.h"
#include "PostGraphObjectBase.h"

namespace Interface 
{
    class FITKAbstractCFDPostData;
    /**
     * @brief  导入渲染数据对象
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-10
     */
    class PostGraphAdaptorAPI PostGraphObjectImport : public PostGraphObjectBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] postData     后处理数据对象 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-10
         */
        explicit PostGraphObjectImport(FITKAbstractCFDPostData* postData, Comp::FITKGraph3DWindowVTK* graph3DWidget);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-10
         */
        virtual ~PostGraphObjectImport();
    };
}

#endif
