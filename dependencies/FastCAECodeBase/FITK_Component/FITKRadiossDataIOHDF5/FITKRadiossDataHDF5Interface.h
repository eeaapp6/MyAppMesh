/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataHDF5Interface.h
 * @brief  Radioss数据HDF5组件
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-28
 *********************************************************************/

#ifndef  __FITKRadiossDataHDF5Interface_H__
#define  __FITKRadiossDataHDF5Interface_H__

#include <QObject>
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITKRadiossDataIOHDF5API.h"

ForwardDeclarNS(Core, FITKAbstractDataObject);
ForwardDeclarNS(Interface, FITKAbstractIO);
ForwardDeclarNS(H5, Group);
ForwardDeclarNS(H5, DataType);
ForwardDeclarNS(H5, H5File);

namespace IO
{
    /**
     * @brief  Radioss数据HDF5组件
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-29
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataHDF5Interface : public QObject, public AppFrame::FITKComponentInterface
    {
        Q_OBJECT
    public:

        FITKRadiossDataHDF5Interface() = default;
        virtual ~FITKRadiossDataHDF5Interface() = default;

        virtual bool exec(const int indexPort) override;


        virtual QString getComponentName() override;
        /**
         * @brief    设置文件名称
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        void setFileName(const QString& fileName);
        /**
         * @brief    设置HDF5文件指针
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        void setHDF5Root(H5::H5File* h5File);
        /**
         * @brief    文件读写时的数据对象
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        void setDataObject(Core::FITKAbstractDataObject* dataObject);
        /**
         * @brief    设置是否在独立的线程运行。默认true，为在独立线程执行
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        void setRunInThread(bool r = true);
        /**
         * @brief    获取是否执行成功标记
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        bool isSuccess();
        /**
         * @brief    是否在执行中
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        bool isRunning();
        /**
         * @brief    打开文件流
         * @param[i] state 1 读取 2 写出
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        bool openFileStream(int state);
        /**
         * @brief    关闭文件流
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        bool closeFileStream();
    signals:
        /**
         * @brief  读取线程结束发送信号
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-28
         */
        void ioThreadFinishedSig();

    private slots:
        /**
         * @brief  读取线程结束处理槽函数
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-28
         */
        void ioThreadFinishedSlot();

    private:
        /**
         * @brief  文件名
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-28
         */
        QString _fileName{};
        /**
         * @brief  H5的文件指针
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-28
         */
        H5::H5File* _h5File{};
        /**
         * @brief  数据对象
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-28
         */
        Core::FITKAbstractDataObject* _dataObject{};
        /**
         * @brief  是否执行成功标记
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-28
         */
        bool _success{ false };
        /**
         * @brief  读取线程
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-28
         */
        Interface::FITKAbstractIO * _io = nullptr;
        /**
         * @brief  是否在线程运行
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-28
         */
        bool _runInThread{ true };
        /**
         * @brief  是否打开文件流 用于标记H5File指针是否传进来的参数 false表示传进来的指针，true表示自己打开的文件流
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-28
         */
        bool _openFile{ false };

    };
}

#endif // !__FITKACISIOHDF5INTERFACE_H__
