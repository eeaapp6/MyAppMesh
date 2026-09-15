/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFIOHDF5Interface.h
 * @brief  流体hdf5文件读写接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-07
 *********************************************************************/
#ifndef _FITKFLOWOFHDF5INTERFACE_H__
#define _FITKFLOWOFHDF5INTERFACE_H__

#include <QObject>
#include "FITKFlowOFIOHDF5API.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

ForwardDeclarNS(Core, FITKAbstractDataObject);
ForwardDeclarNS(Interface, FITKAbstractIO);
ForwardDeclarNS(H5, Group);
ForwardDeclarNS(H5, DataType);
ForwardDeclarNS(H5, H5File);

namespace IO
{
    /**
     * @brief  HDF5文件读写接口
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-07
     */
    class FITKFLOWOFIOHDF5API FITKFlowOFIOHDF5Interface : public QObject, public AppFrame::FITKComponentInterface
    {

        Q_OBJECT

    public:
        using AppFrame::FITKComponentInterface::FITKComponentInterface;
        virtual ~FITKFlowOFIOHDF5Interface() = default;

        /**
         * @brief 设置文件名称
         * @param fileName 文件名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        void setFileName(const QString& fileName);

        /**
         * @brief 打开文件流
         * @param state 1 读取 2 写出
         * @return 是否打开成功
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        bool openFileStream(int state);

        /**
         * @brief 设置HDF5文件指针
         * @param h5File HDF5文件指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        void setHDF5Root(H5::H5File* h5File);

        /**
         * @brief 关闭文件流
         * @return 是否关闭成功
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        bool closeFileStream();

        /**
         * @brief 获取组件名称，不能重复  return "FITKFlowOFHDF5IO"
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        virtual QString getComponentName() override;

        /**
         * @brief 执行读取或写出
         * @param indexPort  1：读取 2：写出
         * @return
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        virtual bool exec(const int indexPort) override;

        /**
         * @brief 获取是否执行成功标记
         * @return bool
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        bool isSuccess();

        /**
         * @brief 是否在执行中
         * @return true 执行中 false未在执行
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        bool isRunning();
        /**
         * @brief 设置是否在独立的线程运行。默认true，为在独立线程执行
         * @param r 是否在独立线程运行
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        void setRunInThread(bool r = true);


    signals:
        /**
         * @brief 读取线程结束发送信号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        void ioThreadFinishedSig();

    private slots:
        /**
         * @brief 读取线程结束处理槽函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        void ioThreadFinishedSlot();

    private:
        /**
         * @brief 文件名
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        QString _fileName{};

        /**
         * @brief H5的文件指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        H5::H5File* _h5File{};

        /**
         * @brief 是否执行成功标记
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        bool _success{ false };
        /**
         * @brief 读取线程
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        Interface::FITKAbstractIO * _io = nullptr;

        /**
         * @brief 是否在线程运行
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        bool _runInThread{ true };
        /**
         * @brief 是否打开文件流 用于标记H5File指针是否传进来的参数 false表示传进来的指针，true表示自己打开的文件流
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        bool _openFile{ false };
    };
}


#endif
