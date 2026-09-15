/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKOCCModelOperImprintSolid.h
 * @brief   抽象压印体操作类
 *
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-09-05
 *********************************************************************/

#ifndef  FITKOCCMODELOPERIMPRINTSOLID_H
#define  FITKOCCMODELOPERIMPRINTSOLID_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperImprintSolid.h"


namespace OCC
{
    /**
     * @brief  抽象压印体操作类
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2024-09-05
     */
    class FITKGeoCompOCCAPI FITKOCCModelOperImprintSolid : public Interface::FITKAbsGeoModelOperImprintSolid, public OCCShapeAgent
    {
    public:
        FITKOCCModelOperImprintSolid();
        virtual ~FITKOCCModelOperImprintSolid() override = default;

        /**
         * @brief   更新模型
         * @return  true
         * @return  false
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-05
         */
        virtual bool update() override;

    private:

        /**
         * @brief   压印体
         * @param   shapes：操作形状列表
         * @return  true
         * @return  false
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-05
         */
        bool updateImprintSolid(QList<TopoDS_Shape> shapes);
    };
}

#endif // !FITKABSGEOMODELOPERLINE_H