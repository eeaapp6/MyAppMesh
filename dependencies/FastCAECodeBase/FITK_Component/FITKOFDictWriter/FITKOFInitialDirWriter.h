/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFInitialDirWriter.h
 * @brief  初始值 0文件夹写出
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-29
 *********************************************************************/
#ifndef __FITKFLOWOFINITIALDIRWRITER_H__
#define __FITKFLOWOFINITIALDIRWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKOFAbstractDictWriter.h"

namespace Interface 
{
    class FITKOFAbsBoundaryType;
    class FITKOFBoundary;
}

namespace DICT
{
    class FITKDictVector;
    class FITKDictArray;
    class FITKDictGroup;
}

namespace Core
{
    class FITKAbstractEasyParam;
}

namespace IO
{
    class FITKOFChangeDictionaryDictW;
    /**
     * @brief ControlMeshDict 文件写出器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFInitialDirWriter : public FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFInitialDirWriter();
        virtual ~FITKOFInitialDirWriter();

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
        void dictObjectFoamFile(QString location, QString vName);
        /**
         * @brief    基础初始化数据写出
         * @param[i] filePath 
         * @param[i] dirName 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        bool basicDataBoundaryW(QString filePath, QString dirName, int regionMeshID);
        /**
         * @brief    区域数据写出
         * @param[i] filePath 
         * @param[i] dirName 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        bool regionPropDataW(QString filePath, QString dirName, int regionMeshID);
        /**
         * @brief    初始化边界参数字典文件写出
         * @param[i] vName 
         * @param[i] vPara 
         * @param[i] regionMeshID 
         * @param[i] isBasic 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-04
         */
        bool initialFileWrite(QString vName, Core::FITKAbstractEasyParam* vPara, int regionMeshID, bool isBasic = true);

        bool dimensionsW(QString vName);
        /**
         * @brief    初始化写出共轭传热的P初始化文件
         * @param[i] regionMeshID 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-07
         */
        bool initialCHTMultiRegionPFile(int regionMeshID);

    private:
        /**
         * @brief 文件名
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString m_fileName{};
        /**
         * @brief  边界写出的工具
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-30
         */
        FITKOFChangeDictionaryDictW* m_boundaryTool;
    };
}



#endif
