/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKNastranBDFIOInterface.h
 * @brief  Nastran bdf网格读写接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-02-10
 *********************************************************************/
#ifndef _FITK_NASTRAN_BDF_IO_INTERFACE_H___
#define _FITK_NASTRAN_BDF_IO_INTERFACE_H___

#include <QObject>
#include "FITKNastranBDFIOAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"

namespace Interface
{
    class FITKAbstractIO;
}

namespace Nastran
{
    /**
     * @brief  Nastran bdf网格读写接口
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2026-02-10
     */
    class FITKNastranBDFIOAPI FITKNastranBDFIOInterface : public QObject,
        public AppFrame::FITKComponentInterface
    {
        Q_OBJECT
    public:
        explicit FITKNastranBDFIOInterface() = default;
        virtual ~FITKNastranBDFIOInterface() = default;

        /**
         * @brief    执行组件
         * @param[i] indexPort 1:读取 2:写出
         * @return   bool 执行结果：不在线程中执行时有效
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        bool exec(const int indexPort) override;
        /**
         * @brief    设置尺寸因子（缩放比例）：缩放中心为模型中心点
         * @param[i] v 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-12
         */
        void setScaleFactor(double v);
        /**
         * @brief 设置文件名称
         * @param[i]  fileName      文件名称，要求包含绝对路径
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        void setFileName(const QString& fileName);
        /**
         * @brief 是否在独立线程执行读写
         * @param[i]  inThread       是否在独立线程执行读写操作
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        void runInThread(const bool & inThread = true);
        /**
         * @brief 是否正在执行
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        bool isRunning();
        /**
         * @brief 执行是否成功
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        bool isSuccess();
        /**
         * @brief 获取部件名称，不能重复  返回 NastranBDFIO
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        QString getComponentName() override;

    signals:
        /**
         * @brief 读写线程结束信号
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        void ioThreadFinishedSig();

    private:
        /**
         * @brief 文件名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        QString _fileName{};
        /**
         * @brief  尺寸因子（缩放比例）
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-12
         */
        double _scaleFactor{ 1.0 };
        /**
         * @brief 是否在线程运行
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        bool _runInThread{ true };
        /**
         * @brief 是否执行成功标记
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        bool _success{ false };
        /**
         * @brief io 对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-11
         */
        Core::FITKAbstractObjectContainer<Interface::FITKAbstractIO> _io{ nullptr };

    };
}


#endif
