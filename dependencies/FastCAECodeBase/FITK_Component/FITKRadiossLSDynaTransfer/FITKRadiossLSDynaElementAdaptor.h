/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossLSDynaElementAdaptor.h
 * @brief  LS-DYNA单元数据适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2026-05-08
 *********************************************************************/
#ifndef _FITKRadiossLSDynaElementAdaptor_H_
#define _FITKRadiossLSDynaElementAdaptor_H_

#include "FITKRadiossLSDynaAbstractAdaptor.h"
#include "FITKRadiossLSDynaTransferAPI.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"

namespace Radioss
{
    class FITKRadiossPart;
    class FITKRadiossMeshModel;
    /**
     * @brief  LS-DYNA单元数据适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2026-05-08
     */
    class FITKRadiossLSDynaTransferAPI FITKRadiossLSDynaElementAdaptor : public FITKRadiossLSDynaAbstractAdaptor
    {
    public:
        explicit FITKRadiossLSDynaElementAdaptor();
        virtual ~FITKRadiossLSDynaElementAdaptor() = default;

        QString getAdaptorClass() override;
        /**
         * @brief    读取
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        bool adaptR() override;
        /**
         * @brief    写出
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        bool adaptW() override;

    private:
        /**
         * @brief    读取单元块
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        bool readElementBlock();
        /**
         * @brief    写出单元块
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        bool writeElementBlock();
        /**
         * @brief    读梁单元
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        bool readBeamElements();
        /**
         * @brief    读壳单元
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        bool readShellElements();
        bool readSH3NElements();
        /**
         * @brief    读体单元
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        bool readSolidElements();
        /**
         * @brief    写梁单元
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        bool writeBeamElements();
        /**
         * @brief    写壳单元
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        bool writeShellElements();
        bool writeSH3NElements();
        /**
         * @brief    写体单元
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        bool writeSolidElements();
        /**
         * @brief    读离散弹簧单元
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-09
         */
        bool readDiscreteElements();
        /**
         * @brief    写离散弹簧单元
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-09
         */
        bool writeDiscreteElements();
        /**
         * @brief    写六面体单元
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-09
         */
        bool writeHex8Elements();
        /**
         * @brief    写4面体4节点单元
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-09
         */
        bool writeTet4Elements();
        /**
         * @brief    获取全部单元ID
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-08
         */
        void getElementIDs();
        /**
         * @brief    获取或创建Part（延迟初始化支持）
         * @param[in] pid Part ID
         * @return   FITKRadiossPart* Part指针，如果Part不存在则自动创建占位对象
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-09
         */
        FITKRadiossPart* getOrCreatePart(int pid);

    private:
        QHash<Interface::FITKModelEnum::FITKEleType, QList<int>> _eleTypeIds{};
        QList<int> _discreteEleIds{};

    };
    RegLSDynaAdaptor(*ELEMENT_SHELL, FITKRadiossLSDynaElementAdaptor,0)
    RegLSDynaAdaptor(*ELEMENT_SOLID, FITKRadiossLSDynaElementAdaptor,1)
    RegLSDynaAdaptor(*ELEMENT_BEAM, FITKRadiossLSDynaElementAdaptor,2)
    RegLSDynaAdaptor(*ELEMENT_DISCRETE, FITKRadiossLSDynaElementAdaptor,3)

}

#endif
