/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFTransportModelManager.h
 * @brief  Transport Model管理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-23
 *********************************************************************/
#ifndef _FITK_OF_TRANSPORT_MODEL_MANAGER_H___
#define _FITK_OF_TRANSPORT_MODEL_MANAGER_H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITKOFEnum.hpp"
#include <functional>
#include <QStringList>

namespace Interface
{
    class FITKAbsOFTransportModel;
}

typedef std::function<Interface::FITKAbsOFTransportModel*()>  CREATETRANSPORTMODELFUN;

namespace Interface
{

    struct FITKOFTransportModelInfo
    {
        QString _transModelName{};
        QList<FITKOFSolverTypeEnum::FITKOFSolverType> _enableSolverType;
        CREATETRANSPORTMODELFUN _createFuns{};
    };

    class FITKInterfaceFlowOFAPI FITKOFTransportModelManager
    {
    public:
        explicit FITKOFTransportModelManager();
        virtual ~FITKOFTransportModelManager();

        /**
         * @brief    过滤Transport Model类型
         * @param[i] solverType 
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        QStringList filterTransportModels(FITKOFSolverTypeEnum::FITKOFSolverType solverType);
        /**
         * @brief    获取Transport Model信息
         * @param[i] transModelName 
         * @return   FITKOFTransportModelInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        FITKOFTransportModelInfo getTransportModelInfo(QString transModelName);

    private:
        /**
         * @brief    初始化Transport Model数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void initTransportModels();

    private:
        /**
         * @brief  Transport Model数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-23
         */
        QList<FITKOFTransportModelInfo> m_transportModels{};
    };
}


#endif
