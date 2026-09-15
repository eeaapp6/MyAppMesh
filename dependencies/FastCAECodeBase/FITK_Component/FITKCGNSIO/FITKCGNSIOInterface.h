/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCGNSIOInterface.h
 * @brief  CGNS IO接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-19
 *********************************************************************/

#ifndef _FITKFCGNSIOINTERFACE_H__
#define _FITKFCGNSIOINTERFACE_H__

#include "FITKCGNSIOAPI.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include <QObject>

ForwardDeclarNS(Interface, FITKAbstractIO);

namespace Interface {
    class FITKUnstructuredFluidMeshVTK;
    class FITKAbstractMesh;
    class FITKModelSet;
    class FITKComponentManager;
}
namespace IO
{
    /**
     * @brief  读写数据存储结构
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-04-15
     */
    struct CGNSDataIO
    {
        /**
         * @brief  网格数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-15
         */
        QList<Interface::FITKAbstractMesh*> _mesh{};
        /**
         * @brief  集合数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-15
         */
        QList < QList<Interface::FITKModelSet*> >_componentManager{};
        /**
         * @brief  创建网格函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-15
         */
        std::function<Interface::FITKAbstractMesh* ()> CreateMeshFun;
    };
    /**
     * @brief  CGNS 读写接口 
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-19
     */
    class FITKCGNSIOAPI FITKCGNSIOInterface : public QObject, public AppFrame::FITKComponentInterface
    {
        Q_OBJECT
    public:
        /**
         * @brief  构造函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        FITKCGNSIOInterface() = default;
        /**
         * @brief  析构函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        virtual ~FITKCGNSIOInterface() = default;
        /**
         * @brief 设置文件名称
         * @param fileName 文件名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        void setFileName(const QString& fileName);
        /**
         * @brief  获取组件名称
         * @return 组件名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        QString getComponentName() override;
        /**
         * @brief  执行组件
         * @return 执行结果
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool exec(const int indexPort) override;
        /**
         * @brief    设置CGNS的数据接口
         * @param[i] data 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-15
         */
        void setCGNSDataIO(CGNSDataIO* data);
        /**
         * @brief    获取CGNS的数据接口
         * @return   CGNSDataIO &
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-15
         */
        CGNSDataIO& getCGNSDataIO();
        /**
         * @brief 获取是否执行成功标记
         * @return bool
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool isSuccess();
        /**
         * @brief 是否在执行中
         * @return true 执行中 false未在执行
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool isRunning();
        /**
         * @brief 设置是否在独立的线程运行。默认true，为在独立线程执行
         * @param r 是否在独立线程运行
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        void setRunInThread(bool r = true);
        /**
         * @brief  清理CGNS数据但不会释放数据
         * @return void
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        void clearCGNSDataIO();

    signals:
        /**
         * @brief 读取线程结束发送信号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        void ioThreadFinishedSig();

    private slots:
        /**
         * @brief 读取线程结束处理槽函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        void ioThreadFinishedSlot();

    private:
        /**
         * @brief  文件名
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        QString m_fileName{};
        /**
         * @brief  CGNS文件读写数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-14
         */
        CGNSDataIO m_data{};
        /**
         * @brief 读写线程
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        Core::FITKAbstractObjectContainer<Interface::FITKAbstractIO> m_io{ nullptr };
        /**
         * @brief 是否执行成功标记
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool m_success{ false };
        /**
         * @brief 是否在线程运行
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        bool m_runInThread{ true };
    };

}

#endif