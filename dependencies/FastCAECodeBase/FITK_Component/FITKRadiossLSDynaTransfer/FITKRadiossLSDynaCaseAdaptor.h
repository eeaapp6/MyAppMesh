/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossLSDynaCaseAdaptor.h
 * @brief  LS-DYNA Case适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2026-05-07
 *********************************************************************/
#ifndef _FITKRadiossLSDynaCaseAdaptor_H_
#define _FITKRadiossLSDynaCaseAdaptor_H_

#include "FITKRadiossLSDynaAbstractAdaptor.h"
#include "FITKRadiossLSDynaTransferAPI.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"
#include <memory>

namespace Radioss
{
    class FITKRadiossCase;
    class FITKRadiossLSDynaNodeAdaptor;
    class FITKRadiossNodes;

    /**
     * @brief  LS-DYNA Case适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2026-05-07
     */
    class FITKRadiossLSDynaTransferAPI FITKRadiossLSDynaCaseAdaptor : public FITKRadiossLSDynaAbstractAdaptor
    {
    public:
        /**
         * @brief    构造函数
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        explicit FITKRadiossLSDynaCaseAdaptor() = default;
        /**
         * @brief    析构函数
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        ~FITKRadiossLSDynaCaseAdaptor() = default;

        /**
         * @brief    获取适配器数据类型名
         * @return   QString
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        QString getAdaptorClass() override;

        /**
         * @brief    适配器读取数据
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        virtual bool adaptR() override;

        /**
         * @brief    适配器写入数据
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-07
         */
        virtual bool adaptW() override;

    private:
        /**
         * @brief  算例数据
         * @author guqingtao (15598887859@163.com)
         * @date   2026-05-07
         */
        Radioss::FITKRadiossCase* _caseData{};

    private:
        /**
         * @brief  写入节点数据
         * @return bool
         * @author guqingtao (15598887859@163.com)
         * @date   2026-05-07
         */
        bool writeNodeData();
        bool writePartData();
        bool writeElementData();
    };

    Register2FITKIOAdaptorFactory(k, Radioss::FITKRadiossCase, FITKRadiossLSDynaCaseAdaptor)
    RegLSDynaAdaptor(*KEYWORD, FITKRadiossLSDynaCaseAdaptor)
    RegLSDynaAdaptor(*END, FITKRadiossLSDynaCaseAdaptor,0)
}

#endif
