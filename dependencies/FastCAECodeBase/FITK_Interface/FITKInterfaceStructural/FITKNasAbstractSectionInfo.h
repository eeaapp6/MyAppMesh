/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_NAS_ABSTRACTSECTION_INFO_H___
#define _FITK_NAS_ABSTRACTSECTION_INFO_H___ 
 
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"
#include "FITKInterfaceStructuralAPI.h"

namespace Interface
{
    class  FITKInterfaceStructuralAPI FITKNasAbstractSectionInfo : 
        public Interface::FITKAbstractSectionInfo
    {
        Q_OBJECT
    public:
        enum NasSectionType
        {
            NasSectionNone,
            NasSectionPBAR,
            NasSectionPSHELL,
        };
        Q_ENUM(NasSectionType)

    public:
        explicit FITKNasAbstractSectionInfo() = default;
        virtual ~FITKNasAbstractSectionInfo() = 0;

        virtual NasSectionType getNasSectionType() =0;

        /**
         * @brief 获取类型
         * @return SectionType
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        virtual FITKAbstractSectionInfo::SectionType getSectionType() override;
     
    };
}


#endif
