/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKVolumeMeshingParameter.h
 * @brief 几何网格区域尺寸生成器
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-12-17
 */
#ifndef _FITKVOLUME_MESHING_PARAMETR__H___
#define _FITKVOLUME_MESHING_PARAMETR__H___

#include "FITKAbaqusDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Interface/FITKInterfaceStructural/FITKAbaMeshPartVTK.h"

#include <QObject>
#include <QMetaEnum>

namespace AbaqusData
{
    /**
     * @brief 体网格划分参数抽象类
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-12-17
     */
    class FITKAbaqusDataAPI FITKVolumeMeshingParameter : public Core::FITKAbstractDataObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new FITKVolumeMeshingParameter object
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        explicit FITKVolumeMeshingParameter() = default;
        /**
         * @brief Destroy the FITKVolumeMeshingParameter object
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        virtual ~FITKVolumeMeshingParameter() = default;
        /**
         * @brief    设置模型ID
         * @param[i] modelID                    模型ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-07-02
         */
        void setModelID(int modelID);
        /**
         * @brief    获取模型ID
         * @return   int                        模型ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-07-02
         */
        int getModelID();
        /**
         * @brief       设置高阶标记。
         * @param[in]   flag：是否划分高阶单元
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-05
         */
        void setQuadraticFlag(bool flag);

        /**
         * @brief       获取高阶标记。
         * @return      是否划分高阶单元
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-05
         */
        bool getQuadraticFlag();
        /**
         * @brief       设置六面体网格划分单元类型。
         * @param[in]   type：六面体单元类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        void setHexElementType(Interface::FITKAbaMeshPartVTK::AbaEleType type);

        /**
         * @brief       获取六面体网格划分单元类型。
         * @return      六面体单元类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        Interface::FITKAbaMeshPartVTK::AbaEleType getHexElementType();

        /**
         * @brief       设置楔形网格划分单元类型。
         * @param[in]   type：楔形单元类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        void setWedgeElementType(Interface::FITKAbaMeshPartVTK::AbaEleType type);

        /**
         * @brief       获取楔形网格划分单元类型。
         * @return      楔形单元类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        Interface::FITKAbaMeshPartVTK::AbaEleType getWedgeElementType();

        /**
         * @brief       设置四面体网格划分单元类型。
         * @param[in]   type：四面体单元类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        void setTetElementType(Interface::FITKAbaMeshPartVTK::AbaEleType type);

        /**
         * @brief       获取四面体网格划分单元类型。
         * @return      四面体单元类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        Interface::FITKAbaMeshPartVTK::AbaEleType getTetElementType();
    protected:
        /**
         * @brief  模型ID
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-07-02
         */
        int _modelID = -1;
        /**
         * @brief       高阶标记。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-05
         */
        bool _isQuadratic = false;
        /**
         * @brief       指定的六面体单元类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        Interface::FITKAbaMeshPartVTK::AbaEleType _hexEType = Interface::FITKAbaMeshPartVTK::C3D8R;

        /**
         * @brief       指定的楔形单元类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        Interface::FITKAbaMeshPartVTK::AbaEleType _wedgeEType = Interface::FITKAbaMeshPartVTK::C3D6;

        /**
         * @brief       指定的四面体单元类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        Interface::FITKAbaMeshPartVTK::AbaEleType _tetEType = Interface::FITKAbaMeshPartVTK::C3D4;

    };
}
#endif
