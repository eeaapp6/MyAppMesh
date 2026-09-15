/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGmshMshIOInterface.h
 * @brief  gmsh msh网格读写接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-28
 *********************************************************************/
#ifndef _FITK_GMSH_MEH_IO_INTERFACE_H___
#define _FITK_GMSH_MEH_IO_INTERFACE_H___

#include <QObject>
#include "FITKGmshMshIOAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"

namespace Interface
{
    class FITKAbstractIO;
}

namespace Gmsh
{
    class FITKGmshMshData;
    /**
     * @brief  读取Gmsh Msh文件配置参数（影响读取速率，根据需求修改）
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2026-02-03
     */
    struct ReaderGmshMshConfig
    {
        /**
         * @brief  是否将Gmsh msh数据提取到meshData数据结构中
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-03
         */
        bool _isExtractData{ true };
        /**
         * @brief  需要过滤的维度
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        QList<int> _filterDim{};
        /**
         * @brief  是否根据高维单元自动过滤它包含的低维单元(仅保留高维度)
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-02-03
         */
        bool _isFilterHighToLowCells{ false };
        /**
         * @brief  是否过滤掉未使用的节点
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-02-03
         */
        bool _isFilterUnusedNode{ false };
    };
    /**
     * @brief  gmsh msh网格读写接口
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2026-01-28
     */
    class FITKGmshMshIOAPI FITKGmshMshIOInterface : public QObject,
        public AppFrame::FITKComponentInterface
    {
        Q_OBJECT
    public:
        /**
         * @brief  构造函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        explicit FITKGmshMshIOInterface() = default;
        /**
         * @brief  析构函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        virtual ~FITKGmshMshIOInterface();
        /**
         * @brief  获取组件名称
         * @return 组件名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        QString getComponentName() override;
        /**
         * @brief  设置文件名称
         * @param  fileName 文件名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        void setFileName(const QString& fileName);
        /**
         * @brief    获取Gmsh msh数据
         * @return   FITKGmshMshData *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-28
         */
        FITKGmshMshData* getGmshMshData();
        /**
         * @brief    设置读取配置
         * @param[i] config 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-03
         */
        void setReaderConfig(const ReaderGmshMshConfig& config);
        /**
         * @brief  执行组件
         * @return bool 执行结果
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool exec(const int indexPort) override;
        /**
         * @brief    是否在独立线程执行读写
         * @param[i] inThread       是否在独立线程执行读写操作
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-19
         */
        void runInThread(const bool & inThread = true);
        /**
         * @brief  是否正在执行
         * @return true
         * @return false
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool isRunning();
        /**
         * @brief  执行是否成功
         * @return true
         * @return false
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool isSuccess();
        /**
         * @brief    清理Gmsh msh数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-28
         */
        void clearGmshMshData();

    signals:
        /**
         * @brief  读写线程结束信号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        void ioThreadFinishedSig();

    private:
        /**
         * @brief  文件名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        QString _fileName{};
        /**
         * @brief  是否在线程运行
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool _runInThread{ true };
        /**
         * @brief  是否执行成功标记
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool _success{ false };
        /**
         * @brief  读取配置
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-02-03
         */
        ReaderGmshMshConfig _readConfig{};
        /**
         * @brief  Gmsh Msh数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-01-28
         */
        Gmsh::FITKGmshMshData* _mshDataGmsh{ nullptr };
        /**
         * @brief  读写线程
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        Core::FITKAbstractObjectContainer<Interface::FITKAbstractIO> _io{ nullptr };
    };
}

//例：Gmsh Msh文件IO实例

//#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
//#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
//#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
//#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
//#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
////#include "FITK_Component/FITKGmshMshIO/FITKGmshMshIOInterface.h"
//#include "FITK_Component/FITKGmshMshIO/FITKGmshMshData.h"
//
//class GmshMshIO_Example : public QObject
//{
//    Q_OBJECT;
//public:
//    /**
//     * @brief    读取Gmsh msh文件
//     * @return   void
//     * @author   liuzhonghua (liuzhonghuaszch@163.com)
//     * @date     2026-02-03
//     */
//    void Example_ReadGmshMshFile()
//    {
//        //创建数据对象和获取msh文件路径
//        QString mshPath = "D:/1.msh";
//        Interface::FITKUnstructuredMesh* mesh = new Interface::FITKUnstructuredMesh;
//        Interface::FITKComponentManager* comp = mesh->getComponentManager();
//        //等待线程结束
//        Core::FITKThreadPool::getInstance()->wait();
//        //获取IO接口
//        Gmsh::FITKGmshMshIOInterface* mshIO = FITKAPP->getComponents()->
//            getComponentTByName<Gmsh::FITKGmshMshIOInterface>("GmshMshIO");
//        if (mshIO == nullptr) return;
//        //设置读取必须参数
//        mshIO->setFileName(mshPath);
//        mshIO->setDataObject("ComponentManager", comp);
//        mshIO->setDataObject("MeshData", mesh);
//        //根据需求设置可选参数
//        Gmsh::ReaderGmshMshConfig readConfig;
//        readConfig._filterDim = QList<int>() << 0 << 1;//设置读取时过滤掉的单元维度。默认为空链表，所有维度都不过滤
//        readConfig._isExtractData = true;//设置读取的Msh文件数据是否提取到mesh和comp数据结构中。默认为true
//        readConfig._isFilterHighToLowCells = false;//设置是否过滤高维单元包含的低维单元。默认为false
//        readConfig._isFilterUnusedNode = false;//设置读取的Msh文件数据是否过滤未使用的节点。默认为false
//        mshIO->setReaderConfig(readConfig);
//        mshIO->runInThread(true);//设置是否在线程中执行，默认为true
//        //仅在线程中读取时才关联槽函数
//        connect(mshIO, SIGNAL(ioThreadFinishedSig()), this, SLOT(Example_mshIOThreadFinishedSlot()), Qt::UniqueConnection);
//        //执行读取
//        mshIO->exec(1);
//    }
//    /**
//     * @brief    写出Gmsh msh文件
//     * @return   void
//     * @author   liuzhonghua (liuzhonghuaszch@163.com)
//     * @date     2026-02-03
//     */
//    void Example_WriteGmshMshFile()
//    {
//
//    }
//
//private slots:
//    /**
//     * @brief  线程读取完成，界面处理槽
//     * @author liuzhonghua (liuzhonghuaszch@163.com)
//     * @date   2026-02-03
//     */
//    void Example_mshIOThreadFinishedSlot()
//    {
//        //获取mesh数据
//        Interface::FITKUnstructuredMesh* mesh = nullptr;
//        if (mesh == nullptr) return;
//        Interface::FITKComponentManager* comp = mesh->getComponentManager();
//        //当_isExtractData接口设置的是false时,msh文件数据没有读进mesh数据结构中,执行界面层数据处理逻辑
//        if (!true)
//        {
//            //获取IO接口
//            Gmsh::FITKGmshMshIOInterface* mshIO = FITKAPP->getComponents()->
//                getComponentTByName<Gmsh::FITKGmshMshIOInterface>("GmshMshIO");
//            if (mshIO == nullptr) return;
//            Gmsh::FITKGmshMshData* mshData = mshIO->getGmshMshData();
//            if (mshData)
//            {
//                this->Example_GUIProcessData(mshData);
//                mshData->update(mesh, comp);
//            }
//        }
//        //处理GUI渲染与界面功能逻辑
//    }
//
//protected:
//    /**
//     * @brief    界面层数据处理(自定义处理msh文件数据)
//     * @return   void
//     * @author   liuzhonghua (liuzhonghuaszch@163.com)
//     * @date     2026-02-03
//     */
//    void Example_GUIProcessData(Gmsh::FITKGmshMshData* mshData)
//    {
//        //mshData数据结构是公有数据，可对数据直接操作
//        Q_UNUSED(mshData);
//    }
//};

#endif
