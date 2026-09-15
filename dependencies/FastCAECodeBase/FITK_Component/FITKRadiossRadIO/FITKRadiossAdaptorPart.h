/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorPart.h
 * @brief  Part读取适配器，读取Part及Element
 * @author zhangjingx (hiter25@outlook.com)
 * @date   2025-07-11
 *********************************************************************/
#ifndef __FITKRADIOSSADAPTORPART_H__
#define __FITKRADIOSSADAPTORPART_H__
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRadiossPart;
    /**
     * @brief  Part读取适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-11
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorPart : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorPart() = default;
        ~FITKRadiossAdaptorPart() = default;
        /**
         * @brief
         * @return   QString
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        bool adaptW() override;
    private:
        /**
         * @brief    写出单元
         * @param[i] part 
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-13
         */
        bool writeElement(FITKRadiossPart* part);
       /**
        * @brief    读取单元
        * @param[i] part 
        * @return   bool
        * @author   zhangjingx (hiter25@outlook.com)
        * @date     2025-07-11
        */
        bool readElement(FITKRadiossPart* part);
        /**
         * @brief    读取part
         * @param[i] part 
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-11
         */
        bool readPart(FITKRadiossPart* part);
    };
    /**
     * @brief  注册适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-09
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRadiossPart, FITKRadiossAdaptorPart);
    //Element
    //Beams and Trusses
    RegRadiossRadAdaptor(/BEAM, FITKRadiossAdaptorPart);
    //Springs 
    RegRadiossRadAdaptor(/SPRING, FITKRadiossAdaptorPart, 2);
    //Shells
    RegRadiossRadAdaptor(/SHELL, FITKRadiossAdaptorPart, 4);
    RegRadiossRadAdaptor(/SH3N, FITKRadiossAdaptorPart, 5);
    //Solid
    RegRadiossRadAdaptor(/BRICK, FITKRadiossAdaptorPart,7); 
    RegRadiossRadAdaptor(/TETRA10, FITKRadiossAdaptorPart, 8);
    RegRadiossRadAdaptor(/TETRA4, FITKRadiossAdaptorPart, 9);
    //Part
    RegRadiossRadAdaptor(/PART, FITKRadiossAdaptorPart,15);
    //粒子
    RegRadiossRadAdaptor(/SPHCEL, FITKRadiossAdaptorPart, 20);

}
#endif //!__FITKRADIOSSADAPTORPART_H__
