/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKOFBlockMeshDictWriter.h
 * @brief BlockMeshDict写出接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-07-19
 */
#ifndef __FITKFLOWOFDICTWRITER_H__
#define __FITKFLOWOFDICTWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKOFAbstractDictWriter.h"

namespace Interface
{
    class FITKAbstractRegionMeshSize;
}

namespace DICT
{
    class FITKDictVector;
    class FITKDictArray;
}

namespace IO
{
    /**
     * @brief BlockMeshDict 文件写出器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFBlockMeshDictWriter : public FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFBlockMeshDictWriter();
        virtual ~FITKOFBlockMeshDictWriter();

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
         * @brief 设置网格区域数据
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        bool blockMeshRegionMeshW();
        /**
         * @brief 设置box数据到blockMeshDict
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        bool blockMeshRegionMeshBox();
        /**
         * @brief 设置box点数据到blockMeshDict
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        bool vertexBox();
        /**
         * @brief 设置box边界数据到blockMeshDict
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        bool boundaryBox();
        /**
         * @brief 插入box面数据，通过面index
         * @param face
         * @param index
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        bool vectorDataBoxByFaceIndex(DICT::FITKDictVector* face, int index);
        /**
         * @brief 转化全局坐标系
         * @param[i] point
         * @param[o] global
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-25
         */
        void conversionglobalPoint(double point[], double global[]);
        /**
         * @brief 设置Cylinder数据到blockMeshDict
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        bool blockMeshRegionMeshCylinder();
        /**
         * @brief 设置Cylinder点数据到blockMeshDict
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        bool vertexCylinder();
        /**
         * @brief 设置Cylinder线数据到blockMeshDict
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-26
         */
        bool edgesCylinder();
        /**
         * @brief 设置Cylinder边界数据到blockMeshDict
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        bool boundaryCylinder();
        /**
         * @brief 插入Cylinder面数据，通过面index
         * @param face
         * @param index
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        bool vectorDataCylinderByFaceIndex(DICT::FITKDictArray* face, int index);
        /**
         * @brief 设置Cylinder数据到blockMeshDict
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        bool blockMeshRegionMeshSphere();
        /**
         * @brief 插入点数据
         * @param[i] array
         * @param[i] point
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-25
         */
        void insertPoint(DICT::FITKDictArray* array, double point[]);

    private:
        /**
         * @brief 文件名
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString m_fileName{};
        /**
         * @brief 局部区域尺寸数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        Interface::FITKAbstractRegionMeshSize* m_regionMeshSize{};
    };
}



#endif
