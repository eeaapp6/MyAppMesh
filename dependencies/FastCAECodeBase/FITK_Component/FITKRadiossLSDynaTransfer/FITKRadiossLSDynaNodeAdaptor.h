/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossLSDynaNodeAdaptor.h
 * @brief  LS-DYNA节点数据适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2026-05-07
 *********************************************************************/
#ifndef _FITKRadiossLSDynaNodeAdaptor_H_
#define _FITKRadiossLSDynaNodeAdaptor_H_

#include "FITKRadiossLSDynaAbstractAdaptor.h"
#include "FITKRadiossLSDynaTransferAPI.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRadiossNodes;
    /**
     * @brief  LS-DYNA节点数据适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2026-05-07
     */
    class FITKRadiossLSDynaTransferAPI FITKRadiossLSDynaNodeAdaptor : public FITKRadiossLSDynaAbstractAdaptor
    {
    public:
        explicit FITKRadiossLSDynaNodeAdaptor();
        virtual ~FITKRadiossLSDynaNodeAdaptor() = default;

        QString getAdaptorClass() override;
        /**
         * @brief    读取
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        bool adaptR() override;
        /**
         * @brief    写出
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        bool adaptW() override;

    private:

        bool readNodeBlock();
        bool writeNodeBlock();

    private:
        FITKRadiossNodes* m_nodeData = nullptr;
    };
    Register2FITKIOAdaptorFactory(k, Radioss::FITKRadiossNodes, FITKRadiossLSDynaNodeAdaptor)
    RegLSDynaAdaptor(*NODE, FITKRadiossLSDynaNodeAdaptor)
}

#endif
