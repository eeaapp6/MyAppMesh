/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKMesherDriverGmshExecExec.h
 * @brief  Gmsh Exec网格驱动器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-10-15
 *********************************************************************/
#ifndef __FITKMESHERDRIVERGMSHEXEC_H___
#define __FITKMESHERDRIVERGMSHEXEC_H___

#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"

class TopoDS_Shape;
class QTimer;

namespace Interface
{
    class FITKAbstractRegionMeshSize;
}

namespace Core
{
    class FITKAbstractDataObject;
}

namespace AppFrame
{
    class FITKExecProgramDriver;
}

namespace Gmsh
{
    /**
     * @brief Gmsh网格驱动器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-10-15
     */
    class FITKMesherDriverGmshExec : public Interface::FITKAbstractMesherDriver
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKMesherDriverGmshExec object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-15
         */
        explicit FITKMesherDriverGmshExec();
        /**
         * @brief Destroy the FITKMesherDriverGmshExec object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-15
         */
        virtual ~FITKMesherDriverGmshExec();
        /**
         * @brief    启动网格划分程序
         * @param[i] info           附加信息
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-07-18
         */
        virtual void startMesher(QStringList info = QStringList()) override;
        /**
         * @brief 结束网格划分程序
         * @param[i]  info           附加信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-18
         */
        virtual void stopMesher(QStringList info = QStringList()) override;

    protected:
        /**
         * @brief    进行gmsh网格生成设置-弹出网格设置窗口
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-17
         */
        void gmshMeshGenSetting();
        /**
         * @brief    开始驱动Gmsh进行网格划分
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-17
         */
        void startGmshMeshGenerationDriven(bool isGenScript = true);
        /**
         * @brief    开始驱动多模型网格划分
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-02
         */
        void startMultiModelGenDriven();

    private:
        /**
         * @brief    创建Gmsh程序驱动
         * @param[i] shapeFile 
         * @param[i] meshFile 
         * @param[i] geoScriptFile 
         * @param[i] isGenScript 
         * @return   AppFrame::FITKExecProgramDriver *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-02
         */
        AppFrame::FITKExecProgramDriver* createGmshExecProgramDriver(const QString& shapeFile, const QString& meshFile, const QString& geoScriptFile, bool isGenScript = true);

    private slots:
        /**
         * @brief  写出几何文件模型与geo脚本文件
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-20
         */
        void writerModelGeoScriptFileDriven();
        /**
         * @brief    网格生成完成槽
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-02
         */
        void meshGenDriven_Solt();

    protected:
        /**
         * @brief  用于记录未开始与未完成的进程数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-02-02
         */
        int _processNumRecord{ 0 };
        /**
         * @brief  多进程
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-02-02
         */
        QList<AppFrame::FITKExecProgramDriver*> _process{};
    };
}


#endif
