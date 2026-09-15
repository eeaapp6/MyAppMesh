/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGmshGeoScriptWrite.h
 * @brief  Gmsh Geo脚本写出
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-28
 *********************************************************************/
#ifndef _FITK_GMESHEXEC_GEOSCRIPT_WRITE_H___
#define _FITK_GMESHEXEC_GEOSCRIPT_WRITE_H___

#include "FITK_Interface/FITKInterfaceIO/FITKAbstractIO.h"

class QTextStream;

namespace Interface
{
    class FITKAbstractRegionMeshSize;
}

namespace GmshExe
{
    /**
     * @brief Gmsh Geo脚本写出
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-27
     */
    class FITKGmshGeoScriptWrite : public Interface::FITKAbstractIO
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKGmshGeoScriptWrite object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        explicit FITKGmshGeoScriptWrite(bool* _success = nullptr);
        /**
         * @brief Destroy the FITKGmshGeoScriptWrite object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual ~FITKGmshGeoScriptWrite();
        /**
         * @brief    设置模型文件路径
         * @param[i] shapeFilePath 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        void setShapeFromFile(const QString shapeFilePath);
        /**
         * @brief    设置网格文件
         * @param[i] meshFilePath 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        void setMeshFile(const QString meshFilePath);
        /**
         * @brief    重写run函数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        void run() override;
        /**
         * @brief    打印控制台消息
         * @param[i] level 打印级别 1 normal 2 warning 3error
         * @param[i] message 数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        void consoleMessage(int level, const QString& message) override;

    private:
        /**
         * @brief    写出geo脚本文件
         * @param[i] stream 
         * @return   bool 状态 true:成功 false:失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        bool writeGeoScript(QTextStream* stream);
        /**
         * @brief    几何几何种子点信息
         * @param[i] stream 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-06
         */
        bool writeGeometryEdgeSeedPoint(QTextStream* stream);
        /**
         * @brief    写出几何处理配置信息
         * @param[i] stream 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        bool writeGeometryProcessConfig(QTextStream* stream);
        /**
         * @brief    写出网格区域加密配置信息
         * @param[i] stream 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        bool writeRefinedMeshConfig(QTextStream* stream);
        /**
         * @brief    添加box加密区域
         * @param[i] stream 
         * @param[i] tag
         * @param[i] regionMesh
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-18
         */
        bool addGmshMeshFieldBox(QTextStream* stream, int tag, Interface::FITKAbstractRegionMeshSize* regionMesh);
        /**
         * @brief    添加Cylinder加密区域
         * @param[i] stream 
         * @param[i] tag
         * @param[i] regionMesh
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-18
         */
        bool addGmshMeshFieldCylinder(QTextStream* stream, int tag, Interface::FITKAbstractRegionMeshSize* regionMesh);
        /**
         * @brief    添加Sphere加密区域
         * @param[i] stream 
         * @param[i] tag
         * @param[i] regionMesh
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-18
         */
        bool addGmshMeshFieldSphere(QTextStream* stream, int tag, Interface::FITKAbstractRegionMeshSize* regionMesh);
        /**
         * @brief    写出网格生成配置信息
         * @param[i] stream 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        bool writeMeshConfig(QTextStream* stream);
        /**
         * @brief    2D算法配置
         * @param[i] algorithm2D algorithm2D
         * @return   int 返回配置
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        int algorithm2dDisposition(int algorithm2D);
        /**
         * @brief    3D算法配置
         * @param[i] algorithm3D
         * @return   int 返回配置
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        int algorithm3dDisposition(int algorithm3D);
        /**
         * @brief    2D重组算法配置
         * @param[i] reAlgorithm2D
         * @return   int 返回配置
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        int reAlgorithm2dDisposition(int reAlgorithm2D);
        /**
         * @brief    细化算法
         * @param[i] subAlgorithm
         * @return   int 返回配置
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        int subdivisionAlgorithmDisposition(int subAlgorithm);

    private:
        bool* m_isWriteOK{ nullptr };
        /**
         * @brief  几何形状路径
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        QString m_shapeFilePath{};
        /**
         * @brief  网格文件
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        QString m_meshFilePath{};
    };
}


#endif
