/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorOpenRadiossFormatConversion.h
 * @brief  OpenRadioss格式转换
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-08-20
 *********************************************************************/
#ifndef _OPERATOROPENRADIOSSFORMATCONVERSION_H
#define _OPERATOROPENRADIOSSFORMATCONVERSION_H

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKExecProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramDriverFactory.h"

namespace ModelOper
{
    /**
     * @brief  OpenRadioss 格式转换操作器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-18
     */
    class OperatorOpenRadiossFormatConversion : public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        explicit OperatorOpenRadiossFormatConversion() = default;
        virtual ~OperatorOpenRadiossFormatConversion() = default;
        /**
         * @brief    界面逻辑，生成UI交互
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-18
         */
        virtual bool execGUI() override;
        /**
         * @brief    业务处理逻辑，在execGUI后执行
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-18
         */
        virtual bool execProfession() override;

    private slots:


    private:
        /**
         * @brief    格式转换VTK
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-20
         */
        void formatAnimToVTK();
        /**
         * @brief    格式转换CSV
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-20
         */
        void formatTHToCSV();
    };
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionFormatAnimToVTK, OperatorOpenRadiossFormatConversion);
    Register2FITKOperatorRepo(actionFormatTHToCSV, OperatorOpenRadiossFormatConversion);

    /**
     * @brief  OpenRadioss 格式转换vtk可执行程序驱动
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-18
     */
    class FITKOpenRadiossToVTKFormatExecProgramDriver : public AppFrame::FITKExecProgramDriver
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKOpenRadiossToVTKFormatExecProgramDriver object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        explicit FITKOpenRadiossToVTKFormatExecProgramDriver();
        /**
         * @brief Destroy the FITKOpenRadiossToVTKFormatExecProgramDriver object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual ~FITKOpenRadiossToVTKFormatExecProgramDriver();
        /**
         * @brief 获取程序类型，抽象值
         * @return int
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual int getProgramType() override;
        /**
         * @brief 获取程序名称，任务管理器的名称，不是文件路径
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual QString getProgramName() override;
        /**
         * @brief    是否存在可执行程序
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-31
         */
        bool isExistExeProgram();
        /**
         * @brief    等待时间
         * @param[i] time -1:无限等待
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-20
         */
        void waitForFinished(int time);
    };
    //驱动器注册相关操作
    Register2FITKProgramDriverFactory(3, FITKOpenRadiossToVTKFormatExecProgramDriver, FITKOpenRadiossToVTKFormatExecProgramDriver);

    /**
     * @brief  OpenRadioss 格式转换csv可执行程序驱动
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-18
     */
    class FITKOpenRadiossToCSVFormatExecProgramDriver : public AppFrame::FITKExecProgramDriver
    {
    public:
        /**
         * @brief Construct a new FITKOpenRadiossToCSVFormatExecProgramDriver object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        explicit FITKOpenRadiossToCSVFormatExecProgramDriver();
        /**
         * @brief Destroy the FITKOpenRadiossToCSVFormatExecProgramDriver object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual ~FITKOpenRadiossToCSVFormatExecProgramDriver();
        /**
         * @brief 获取程序类型，抽象值
         * @return int
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual int getProgramType() override;
        /**
         * @brief 获取程序名称，任务管理器的名称，不是文件路径
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual QString getProgramName() override;
        /**
         * @brief    是否存在可执行程序
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-31
         */
        bool isExistExeProgram();
    };
    //驱动器注册相关操作
    Register2FITKProgramDriverFactory(3, FITKOpenRadiossToCSVFormatExecProgramDriver, FITKOpenRadiossToCSVFormatExecProgramDriver);

    /**
     * @brief  OpenRadioss格式转换vtk可执行程序驱动输入信息
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-18
     */
    class FITKOpenRadiossToVTKExecProgramInputInfo : public AppFrame::FITKProgramInputInfo
    {
    public:
        /**
         * @brief Construct a new FITKOpenRadiossToVTKExecProgramInputInfo object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        explicit FITKOpenRadiossToVTKExecProgramInputInfo();
        /**
         * @brief Destroy the FITKOpenRadiossToVTKExecProgramInputInfo object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual ~FITKOpenRadiossToVTKExecProgramInputInfo();
        /**
         * @brief    设置输入文件
         * @param[i] solverConfigFile
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        void setInputFile(QString inputFile);
        /**
         * @brief    设置输出文件
         * @param[i] outputFile 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-20
         */
        void setOutputFile(QString outputFile);
        /**
         * @brief 获取启动参数
         * @return QStringList
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual QStringList args() override;

    private:
        /**
         * @brief  输入文件
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        QString _inputFile{};
        /**
         * @brief  输出文件
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        QString _outputFile{};
    };

    /**
     * @brief  OpenRadioss格式转换csv可执行程序驱动输入信息
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-18
     */
    class FITKOpenRadiossToCSVExecProgramInputInfo : public AppFrame::FITKProgramInputInfo
    {
    public:
        /**
         * @brief Construct a new FITKOpenRadiossToCSVExecProgramInputInfo object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        explicit FITKOpenRadiossToCSVExecProgramInputInfo();
        /**
         * @brief Destroy the FITKOpenRadiossToCSVExecProgramInputInfo object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual ~FITKOpenRadiossToCSVExecProgramInputInfo();
        /**
         * @brief    设置输入文件
         * @param[i] solverConfigFile
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        void setInputFile(QString inputFile);
        /**
         * @brief 获取启动参数
         * @return QStringList
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual QStringList args() override;

    private:
        /**
         * @brief  输入文件
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        QString _inputFile{};
    };
}

#endif