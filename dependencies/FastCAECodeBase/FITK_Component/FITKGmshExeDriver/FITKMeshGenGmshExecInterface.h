/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKMeshGenGmshExecInterface.h
 * @brief  Gmsh exe网格生成
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-04-21
 *********************************************************************/
#ifndef _FITKMESHGENGMSHEXECINTERFACE_H_
#define _FITKMESHGENGMSHEXECINTERFACE_H_

#include "FITKGmshExeDriverAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

namespace Gmsh
{
    /**
     * @brief Gmsh网格生成
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-03-04
     */
    class FITKGmshExeDriverAPI FITKMeshGenGmshExecInterface :
        public AppFrame::FITKComponentInterface
    {
    public:
        /**
         * @brief Construct a new FITKMeshGenOFInterface object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        explicit FITKMeshGenGmshExecInterface();
        /**
         * @brief Destroy the FITKMeshGenOFInterface object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        virtual ~FITKMeshGenGmshExecInterface() = default;

        /**
        * @brief 获取部件名称，不能重复  return "FITKMeshGenOF"
        * @return QString
        * @author liuzhonghua (liuzhonghuaszch@163.com)
        * @date 2024-03-04
        */
        virtual QString getComponentName() override;

        /**
         * @brief 执行读取或写出
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-03-07
         */
        virtual bool exec(const int indexPort) override;

        /**
         * @brief       获取组件内不需要重置的数据对象ID。[虚函数][重写]
         * @return      ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-28
         */
        virtual QList<int> getUnResetDataObj() override;

    };
}

//例：Gmsh驱动实例

//#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
//#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
//#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
//#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshProcessor.h"
//#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
//#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
//#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
//
//class GmshMesher_Example : public QObject
//{
//    Q_OBJECT;
//public:
//    /**
//     * @brief    网格划分驱动参数
//     * @return   void
//     * @author   liuzhonghua (liuzhonghuaszch@163.com)
//     * @date     2026-01-30
//     */
//    void Example_ConfigPara()
//    {
//        //获取网格生成接口
//        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
//        if (!mf) return;
//        //获取所需的全局网格划分配置接口
//        Interface::FITKGlobalMeshSizeInfo* sizeInfo = mf->getGlobalMeshSizeInfo("GmshExec");
//        Interface::FITKGlobalMeshGenerateAlgorithmInfo* algorithmInfo = mf->getGlobalMeshGenerateAlgorithmInfo("GmshExec");
//        if (!sizeInfo || !algorithmInfo) return;
//        //配置所需设置
//        sizeInfo->setMaxSize(0.5);
//        sizeInfo->setMinSize(0.5);
//        algorithmInfo->setMeshGenerateDimension(3);
//    }
//    /**
//     * @brief    开始驱动网格划分
//     * @return   void
//     * @author   liuzhonghua (liuzhonghuaszch@163.com)
//     * @date     2026-01-30
//     */
//    void Example_startMesher()
//    {
//        int meshGenMethod = 0;//Gmsh网格划分驱动方式，可修改
//        //获取网格生成接口
//        Interface::FITKMeshGenInterface* meshGenInterface = Interface::FITKMeshGenInterface::getInstance();
//        if (!meshGenInterface) return;
//        Interface::FITKAbstractMesherDriver* mesher = meshGenInterface->getMesherDriver("GmshExec");//获取Gmsh网格生成驱动
//        if (!mesher) return;
//        //根据所选驱动方式不同，配置所必须的参数
//        switch (meshGenMethod)
//        {
//        case 0: {//自动生成脚本驱动网格划分：模型文件、指定网格生成路径即可，该方式会自动根据FITK网格划分参数数据结构自动生成脚本
//            QString shapeFile = "D:/geometry.brep", meshFile = "D:/mesh.msh";
//            //设置参数
//            this->Example_ConfigPara();
//            mesher->setValue("Method", 0);
//            mesher->setValue("MeshFile", meshFile);
//            mesher->setValue("ShapeFile", shapeFile);
//            break;
//        }
//        case 1: {//自动调用组件内部网格划分配置窗口驱动网格划分：指定网格生成路径即可，该方式会自动根据配置窗口（拾取几何数据与配置网格划分信息）自动生成几何文件与脚本文件
//            QString meshFile = "D:/mesh.msh";
//            //设置参数
//            mesher->setValue("Method", 1);
//            mesher->setValue("MeshFile", meshFile);
//            break;
//        }
//        case 2: {//直接驱动网格划分：模型文件、脚本文件、指定网格生成文件即可，该方式通过外部导入模型和划分脚本，直接驱动网格划分
//            QString shapeFile = "D:/geometry.brep", meshFile = "D:/mesh.msh", scriptFile = "D:/script.geo";
//            //设置参数
//            mesher->setValue("Method", 2);
//            mesher->setValue("MeshFile", meshFile);
//            mesher->setValue("ShapeFile", shapeFile);
//            mesher->setValue("ScriptFile", scriptFile);
//            break;
//        }
//        case 3: {//多模型并行驱动网格划分：模型文件、指定网格生成路径即可，该方式会自动根据FITK网格划分参数数据结构自动生成脚本
//            QStringList shapeFiles = QStringList() << "D:/geometry1.brep" << "D:/geometry2.brep" << "D:/geometry3.brep" << "D:/geometry4.brep"
//                , meshFiles = QStringList() << "D:/mesh1.msh" << "D:/mesh2.msh" << "D:/mesh3.msh" << "D:/mesh4.msh";
//            //设置参数
//            mesher->setValue("Method", 3);
//            mesher->setValue("ProcessNum", 2);//进程数：不设置会根据电脑的最大逻辑处理数为进程数
//            mesher->setValue("MeshFiles", meshFiles);
//            mesher->setValue("ShapeFiles", shapeFiles);
//            break;
//        }
//        default:
//            break;
//        }
//        //根据需求配置参数
//        mesher->setValue("EnableSendMessage", true);//是否将网格驱动信息发送到控制台：默认true -> 启动发送网格驱动信息
//        mesher->setValue("ThreadNum", 1);           //驱动网格划分线程数
//        //绑定完成信号
//        connect(mesher, &Interface::FITKAbstractMesherDriver::mesherFinished, this, &GmshMesher_Example::Example_MeshGenFinished);
//        //开始划分网格
//        mesher->startMesher();
//    }
//
//    /**
//     * @brief  网格划分完成槽
//     * @return void
//     * @author liuzhonghua (liuzhonghuaszch@163.com)
//     * @date   2026-01-30
//     */
//    void Example_MeshGenFinished()
//    {
//        QString meshFile = "D:/mesh.msh";
//        //获取网格数据
//        Interface::FITKUnstructuredMesh* mesh = new Interface::FITKUnstructuredMesh;
//        //获取网格生成接口
//        Interface::FITKMeshGenInterface* meshGenInterface = Interface::FITKMeshGenInterface::getInstance();
//        if (!meshGenInterface) return;
//        Interface::FITKAbstractMeshProcessor* processor = meshGenInterface->getMeshProcessor("GmshExec");//获取Gmsh网格生成驱动
//        if (!processor) return;
//        //必须参数
//        processor->setValue("File", meshFile);
//        processor->insertDataObject("Mesh", mesh);
//        processor->insertDataObject("ComponentManager", mesh->getComponentManager());
//        //根据需求配置参数
//        processor->setValue("FilterDim", QList<QVariant>() << 0 << 1);//设置读取时过滤掉的单元维度。默认为空链表，所有维度都不过滤
//        processor->setValue("FilterUnusedNode", false);//设置是否过滤掉未使用的节点。默认为false
//        processor->setValue("FilterDimElement", false);//设置是否过滤高维单元包含的低维单元。默认为false，该设置与几何映射有冲突
//        int geoCmdId = 0;
//        processor->setValue("ExpGeoCmdId", geoCmdId);//根据需求可生成几何映射。几何命令ID
//        //执行网格获取
//        processor->start(QStringList() << "MSH");
//    }
//};

#endif
