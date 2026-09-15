/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorTimerHistory.h
 * @brief  时间历程读写适配器
 * @author wangning (2185896382@qq.com)
 * @date   2025-09-29
 *********************************************************************/
#ifndef __FITKRadiossAdaptorTimerHistory_H__
#define __FITKRadiossAdaptorTimerHistory_H__

#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKAbstractTimeHistory;
    class FITKTimeHistoryACCEL;
    class FITKTimeHistoryINTER;
    class FITKTimeHistoryRBODY;
    class FITKTimeHistoryRWALL;
    class FITKTimeHistorySECTIO;
    /**
     * @brief  时间历程读写适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-29
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorTimerHistory : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorTimerHistory() = default;
        ~FITKRadiossAdaptorTimerHistory() = default;
        /**
         * @brief
         * @return   QString
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool adaptW() override;

    private:
        /**
         * @brief    读取文件参数并赋值给对象
         * @param[i] lines
         * @param[i] accel
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool readParam(const QStringList& lines, FITKAbstractTimeHistory* th);

        //读取ACCEL参数
        bool readACCEL(FITKAbstractTimeHistory* th, QStringList& keyWords);
        //读取INTER参数
        bool readINTER(FITKAbstractTimeHistory* th, QStringList& keyWords,QString _keyWord_);
        //读取RWALL参数
        bool readRWALL(FITKAbstractTimeHistory* th, QStringList& keyWords);
        //读取RBODY参数
        bool readRBODY(FITKAbstractTimeHistory* th, QStringList& keyWords);
        //读取SECTIO参数
        bool readSECTIO(FITKAbstractTimeHistory* th, QStringList& keyWords);
        //读取SPHCEL参数
        bool readSPHCEL(FITKAbstractTimeHistory* th, QStringList& keyWords, QString idAndName);
    };
    /**
     * @brief  注册适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-15
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKAbstractTimeHistory, FITKRadiossAdaptorTimerHistory);
    RegRadiossRadAdaptor(/TH/ACCEL, FITKRadiossAdaptorTimerHistory);
    RegRadiossRadAdaptor(/TH/INTER, FITKRadiossAdaptorTimerHistory,1);
    RegRadiossRadAdaptor(/TH/RBODY, FITKRadiossAdaptorTimerHistory,2);
    RegRadiossRadAdaptor(/TH/RWALL, FITKRadiossAdaptorTimerHistory,3);
    RegRadiossRadAdaptor(/TH/SECTIO, FITKRadiossAdaptorTimerHistory,4);
    RegRadiossRadAdaptor(/TH/SPHCEL, FITKRadiossAdaptorTimerHistory,5);

    class FITKRadiossRadIOAPI FITKRadiossAdaptorTimerHistoryACCEL : public FITKRadiossAdaptorTimerHistory{};
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKTimeHistoryACCEL, FITKRadiossAdaptorTimerHistoryACCEL);

    class FITKRadiossRadIOAPI FITKRadiossAdaptorTimerHistoryINTER : public FITKRadiossAdaptorTimerHistory{};
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKTimeHistoryINTER, FITKRadiossAdaptorTimerHistoryINTER);

    class FITKRadiossRadIOAPI FITKRadiossAdaptorTimerHistoryRBODY : public FITKRadiossAdaptorTimerHistory{};
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKTimeHistoryRBODY, FITKRadiossAdaptorTimerHistoryRBODY);

    class FITKRadiossRadIOAPI FITKRadiossAdaptorTimerHistoryRWALL : public FITKRadiossAdaptorTimerHistory{};
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKTimeHistoryRWALL, FITKRadiossAdaptorTimerHistoryRWALL);

    class FITKRadiossRadIOAPI FITKRadiossAdaptorTimerHistorySECTIO : public FITKRadiossAdaptorTimerHistory{};
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKTimeHistorySECTIO, FITKRadiossAdaptorTimerHistorySECTIO);

    class FITKRadiossRadIOAPI FITKRadiossAdaptorTimerHistorySPHCEL : public FITKRadiossAdaptorTimerHistory{};
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKTimeHistorySPHCEL, FITKRadiossAdaptorTimerHistorySPHCEL);
}
#endif //!__FITKRADIOSSADAPTORPROPSHELLVALUE_H__
