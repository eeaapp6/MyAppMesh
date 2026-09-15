/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFMeshBoundaryW.h
 * @brief  网格边界文件 boundary
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef __FITKFLOWOFMESHBOUNDARYWRITER_H__
#define __FITKFLOWOFMESHBOUNDARYWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKOFAbstractDictWriter.h"

class QTextStream;

namespace IO
{
    /**
     * @brief FITKOFMeshBoundaryW 文件写出器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFMeshBoundaryW : public FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFMeshBoundaryW();
        virtual ~FITKOFMeshBoundaryW();

        /**
         * @brief 设置文件路径
         * @param[i] filePath
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        void setFilePath(const QString& filePath) override;
        /**
         * @brief 重写run函数
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-15
         */
        bool run() override;
        /**
         * @brief 获取文件
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString getFileName() override;

    private:
        /**
         * @brief 设置字典里的FoamFile
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        void dictObjectFoamFile(QString location);
        /**
         * @brief    网格边界写出
         * @param[i] meshRegionID 区域网格ID
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        bool meshBoundaryW(int meshRegionID);

    private:
        /**
         * @brief 文件名
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString m_fileName{};
        /**
         * @brief  文件流
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-06
         */
        QTextStream* m_stream{};
    };
}



#endif
