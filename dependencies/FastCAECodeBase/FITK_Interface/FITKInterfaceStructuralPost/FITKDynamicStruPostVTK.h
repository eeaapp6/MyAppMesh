/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKDynamicStruPostVTK.h
 * @brief 动态分析结果
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-05-19
 *
 */
#ifndef __FITKDYNAMICSTRUPOST_H_VTK_H__
#define __FITKDYNAMICSTRUPOST_H_VTK_H__

#include "FITKAbstractStruPostVTK.h"
#include <QStringList>

namespace Interface
{
    /**
     * @brief 动态分析结果
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-19
     */
    class FITKInterfaceStructuralPostAPI FITKDynamicStruPostVTK
        : public FITKAbstractStructuralPostVTK
    {
    public:
        /**
         * @brief    构造函数
         * @param[i] type                  文件类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-25
         */
        explicit FITKDynamicStruPostVTK(StruPostFileType type);
        /**
         * @brief Destroy the FITKDynamicStruPostVTK object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        virtual ~FITKDynamicStruPostVTK() override;
        /**
         * @brief 获取后处理分析类型
         * @return StruPostType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        StruPostType getPostStructualType() override;
        /**
         * @brief 设置后处理文件列表
         * @param files
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        void setFiles(const QStringList & files);
        /**
         * @brief    设置H5网格和结果文件
         * @param[i] h5网格文件
         * @param[i] h5结果文件
         * @author   WuYizhen (wyz_hanhan@163.com)
         * @date     2026-01-23
         */
        void setFiles(const QString & fileMesh, const QString & fileResult);
        /**
         * @brief    追加文件
         * @param[i] fileName                   文件名称
         * @param[i] isUpdate                   是否更新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-02-10
         */
        void appendFile(const QString& fileName, bool isUpdate = false);
        /**
         * @brief 下一帧
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-21
         */
        virtual void nextFrame() override;
        /**
         * @brief 上一帧
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-21
         */
        virtual void previousFrame() override;
        /**
         * @brief 第一帧
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-21
         */
        virtual void firstFrame() override;
        /**
         * @brief 最后一帧
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-21
         */
        virtual void lastFrame() override;
        /**
         * @brief    设置当前帧数（初始帧数为0）
         * @param[i] curFrame                   当前帧数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-16
         */
        virtual void setCurFrame(int curFrame) override;
        /**
         * @brief    获取总帧数
         * @return   int                        总帧数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-16
         */
        virtual int frameNum() override;
        /**
         * @brief    获取当前帧数
         * @return   int                        当前帧数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-17
         */
        virtual int getCurFrame();
        /**
         * @brief    更新文本[override]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        virtual void updateText() override;
    private:
        /**
         * @brief 设置文件索引
         * @param[i]  index          文件索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-20
         */
        void setFileIndex(const int index);
        /**
         * @brief 获取当前文件索引
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-21
         */
        int getFileIndex();

    private:
        /**
         * @brief 文件列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        QStringList _fileList{};
        /**
         * @brief  区分是不是读取h5文件的标志位
         * @author WuYizhen (wyz_hanhan@163.com)
         * @date   2026-01-23
         */
        bool _isH5 = false;
        /**
         * @brief  网格文件
         * @author WuYizhen (wyz_hanhan@163.com)
         * @date   2026-01-23
         */
        QString _meshFile{};
        /**
         * @brief  结果文件
         * @author WuYizhen (wyz_hanhan@163.com)
         * @date   2026-01-23
         */
        QString _resultFile{};
    };
}


#endif
