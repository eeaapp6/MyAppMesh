/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKOFSnappyHexMeshDictWriter.h
 * @brief SnappyHexMeshDict写出接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-07-23
 */
#ifndef __FITKOFSNAPPYHEXMESHDICTWRITER_H__
#define __FITKOFSNAPPYHEXMESHDICTWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKOFAbstractDictWriter.h"

namespace Interface
{
    class FITKAbsGeoShapeAgent;
    class FITKGeometryMeshSizeManager;
}

namespace DICT
{
    class FITKDictGroup;
}

namespace IO
{
    /**
     * @brief SnappyHexMeshDict 文件写出器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFSnappyHexMeshDictWriter : public FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFSnappyHexMeshDictWriter();
        virtual ~FITKOFSnappyHexMeshDictWriter();

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
        void dictObjectFoamFile();
        /**
         * @brief 设置几何网格区域数据
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        bool snappyHexMeshGeometryMeshW();
        /**
         * @brief 设置几何区域数据
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        bool geometrySnappyHexMesh();
        /**
         * @brief 设置要解析的点与几何尺寸，边界
         * @return  状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-25
         */
        bool castellatedMeshControls();
        /**
         * @brief 几何数据stl
         * @param[i] geoModel 几何数据代理器对象
         * @param[i] stlGroup 字典数据用于存储几何数据
         * @param[i] name 几何名称
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-24
         */
        bool geoFaceGroupStl(Interface::FITKAbsGeoShapeAgent* geoModel, DICT::FITKDictGroup* stlGroup, QString name);
        /**
         * @brief snapControls
         * @return  状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-25
         */
        bool snapControls();
        /**
         * @brief 添加图层控件
         * @param[i] isAddLayers 是否添加层级
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-24
         */
        bool addLayersControls(bool isAddLayers);
        /**
         * @brief meshQualityControls
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-25
         */
        bool meshQualityControls();

    private:
        /**
         * @brief 文件名
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString m_fileName{};
        /**
         * @brief 几何网格区域尺寸
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-25
         */
        Interface::FITKGeometryMeshSizeManager* m_geometryMeshSizeManager{};
    };
}



#endif
