/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorSolverSetting.h
 * @brief  Radioss适配器 用于求解设置的读写
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-10-29
 *********************************************************************/
#ifndef _FITKRADIOSSADAPTORSOLVERSETTING_H_
#define _FITKRADIOSSADAPTORSOLVERSETTING_H_
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRadiossCase;

    /**
     * @brief  Radioss RAD 读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-07-02
     */
    class FITKRadiossAdaptorSolverSetting : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorSolverSetting() = default;
        ~FITKRadiossAdaptorSolverSetting() = default;
        /**
         * @brief    获取适配器数据类型名
         * @return   QString 适配器数据类型名
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-02
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool true成功，false失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-02
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool true成功，false失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-02
         */
        bool adaptW() override;

    private:
        /**
         * @brief    读取打印配置
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool readPrint();
        /**
         * @brief    写出打印配置
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-10
         */
        bool writePrint();
        /**
         * @brief    读取运行配置
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool readRun();
        /**
         * @brief    写出运行配置
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-10
         */
        bool writeRun();
        /**
         * @brief    读取版本
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool readVersion();
        /**
         * @brief    写出版本
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-10
         */
        bool writeVersion();
        /**
         * @brief    读取监控
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool readMonitoring();
        /**
         * @brief    写出监控
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-10
         */
        bool writeMonitoring();
        /**
         * @brief    读取配置的重启文件信息.rst
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool readRestartFile();
        /**
         * @brief    写出配置的重启文件信息.rst
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool writeRestartFile();
        /**
         * @brief    读取并行配置
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-11
         */
        bool readParallel();
        /**
         * @brief    写出并行配置
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-11
         */
        bool writeParallel();
        /**
         * @brief    读取全局时间步设置
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool readGlobalTimeStep();
        /**
         * @brief    写出全局时间步设置
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool writeGlobalTimeStep();
        /**
         * @brief    读取停止控制
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-12
         */
        bool readStopControl();
        /**
         * @brief    写出停止控制
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-12
         */
        bool writeStopControl();
        /**
         * @brief    读取DT DT1关键字
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool readDataTimeStep();
        /**
         * @brief    读取点时间步参数
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-12
         */
        bool readNodeTimeParam();
        /**
         * @brief    写出点时间步参数
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-12
         */
        bool writeNodeTimeParam();
        /**
         * @brief    读取单元的时间步
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-11
         */
        bool readElemTimeParam();
        /**
         * @brief    写出单元的时间步
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-11
         */
        bool writeElemTimeParam();
        /**
         * @brief    读取收敛参数
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-12
         */
        bool readConvergenceCriteriaParam();
        /**
         * @brief    写出收敛参数
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-12
         */
        bool writeConvergenceCriteriaParam();
        /**
         * @brief    数据提取
         * @param[i] line 
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        QStringList getKeywordExtraction(QString line);
    };
    /**
     * @brief 注册适配器
     * @date   2025-07-07
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRadiossSolverSettings, FITKRadiossAdaptorSolverSetting);
    RegRadiossRadAdaptor(/PRINT, FITKRadiossAdaptorSolverSetting, 1);
    RegRadiossRadAdaptor(/RUN, FITKRadiossAdaptorSolverSetting, 2);
    RegRadiossRadAdaptor(/VERS, FITKRadiossAdaptorSolverSetting, 3);
    RegRadiossRadAdaptor(/MON, FITKRadiossAdaptorSolverSetting, 4);
    RegRadiossRadAdaptor(/RFILE, FITKRadiossAdaptorSolverSetting, 5);
    RegRadiossRadAdaptor(/DT, FITKRadiossAdaptorSolverSetting, 6);
    RegRadiossRadAdaptor(/DT1, FITKRadiossAdaptorSolverSetting, 7);
    RegRadiossRadAdaptor(/STOP, FITKRadiossAdaptorSolverSetting, 8);
    RegRadiossRadAdaptor(/NEGVOL, FITKRadiossAdaptorSolverSetting, 9);
    RegRadiossRadAdaptor(/PARITH, FITKRadiossAdaptorSolverSetting, 10);

}
#endif // !_FITKRADIOSSADAPTORSOLVERSETTING_H_