/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKStaticStruPostVTK.h
 * @brief 静力分析后处理结果
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-05-19
 *
 */
#ifndef __FITKSTATICSTRUPOST_H_VTK_H__
#define __FITKSTATICSTRUPOST_H_VTK_H__

#include "FITKAbstractStruPostVTK.h"

namespace Interface
{
    /**
     * @brief 静力分析后处理
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-19
     */
    class FITKInterfaceStructuralPostAPI FITKStaticStruPostVTK
        : public FITKAbstractStructuralPostVTK
    {
    public:
        /**
         * @brief    构造函数
         * @param[i] type                   文件类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-25
         */
        explicit FITKStaticStruPostVTK(StruPostFileType type);
        /**
         * @brief Destroy the FITKStaticStruPostVTK object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        virtual ~FITKStaticStruPostVTK() override;
        /**
         * @brief 获取后处理分析类型
         * @return StruPostType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        StruPostType getPostStructualType() override;
        /**
         * @brief 设置文件名称
         * @param[i]  file           文件名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-21
         */
        void setFile(const QString & file, const QString & fileResult = "");

    private:
        /**
         * @brief 文件名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-21
         */
        QString _fileName{};

        /**
         * @brief  第二文件名称
         * @author WuYizhen (wyz_hanhan@163.com)
         * @date   2026-01-23
         */
        QString _resultFileName{};

    };
}


#endif
