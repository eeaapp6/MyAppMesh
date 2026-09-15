/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKPHengLEIerDriver.h
 * @brief PHengLEI驱动器抽象类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-18
 *
 */
#ifndef __FITK_ABSTRACT_MESHER_DRIVER_H___
#define __FITK_ABSTRACT_MESHER_DRIVER_H___

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"
#include <QObject>

namespace Core
{
    class FITKAbstractDataObject;
}

namespace Interface
{
	class FITKPHengLEIExecProgramDriver;
    class FITKPHengLEILinuxShellDriver;
    /**
     * @brief PHengLEI驱动器抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-18
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIerDriver : public QObject,
        public Core::FITKVarientParams 
    {
        Q_OBJECT
    public:
		enum taskType
		{
			None,
			MeshProcess,
			CFDCalculate,
		};
	public:
        /**
         * @brief Construct a new FITKPHengLEIerDriver object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        explicit FITKPHengLEIerDriver() = default;
        /**
         * @brief Destroy the FITKPHengLEIerDriver object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual ~FITKPHengLEIerDriver();
        /**
         * @brief 设置临时数据对象
         * @param[i]  data           临时数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual void setDataObject(Core::FITKAbstractDataObject* data);
        /**
         * @brief 设置启动参数
         * @param[i]  args           参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual void setArgs(const QStringList& args);
        /**
         * @brief 启动PHengLEI驱动器
         * @param[i]  info           附加信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual void startDriver(QStringList info = QStringList());
        /** 
         * @brief 停止PHengLEI驱动器
         * @param[i]  info           附加信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual void stopDriver(QStringList info = QStringList());
		/**
		 * @brief   设置驱动程序任务
		 * @param   type
		 * @author  WangKai (wang_starry@outlook.com)
		 * @date    2025-07-18
		 */
		void setTaskType(FITKPHengLEIerDriver::taskType type);
		/**
		 * @brief 判断进程是否正在运行
		 * @return true 进程正在运行
		 * @return false 进程未运行
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2025-07-18
		 */
		bool isProcessRunning() const;
    signals:
        /**
         * @brief 任务结束信号
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        void processFinished();
	public slots:
		/**
		 * @brief 启动网格转换
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-11
		 */
		void startMeshTransfer();
		/**
		 * @brief 启动网格分区
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-11
		 */
		void startMeshPartion();
		/**
		 * @brief 启动CFD计算驱动
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-09-07
		 */
		void startCFDCalculate();

		void onExecDriverFinished();
    protected:
        /**
         * @brief 临时数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        Core::FITKAbstractDataObject* _data{};
        /**
         * @brief 启动参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        QStringList _args{};

		taskType _taskType{None};

		FITKPHengLEIExecProgramDriver* _execDriver{};

        FITKPHengLEILinuxShellDriver* _linuxshellDriver{};
    };

}


#endif
