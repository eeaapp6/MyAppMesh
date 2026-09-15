/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKAbsGeoModelOperCompound.h
 * @brief   复合体操作抽象类。

 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-09-06
 *********************************************************************/

#ifndef  FITKABSGEOMODELOPERCOMPOUND_H
#define  FITKABSGEOMODELOPERCOMPOUND_H

#include "FITKAbsGeoModelSolid.h"

#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief   复合体操作抽象类。
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-09-06
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelOperCompound : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelOperCompound);
    public:
        /**
         * @brief   构造函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-06
         */
        FITKAbsGeoModelOperCompound() = default;

        /**
         * @brief   析构函数。[虚函数]
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-06
         */
        virtual ~FITKAbsGeoModelOperCompound() = default;

        /**
         * @brief   获取几何命令类型。[重写]
         * @return  命令类型
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-06
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief   设置复合对象数据ID。
         * @param   cmdId：模型数据ID
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-06
         */
        void setInputCmdId(Interface::VirtualShape cmdId);

        /**
         * @brief   获取复合对象的数据ID。
         * @return  模型数据ID
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-06
         */
        Interface::VirtualShape& getInputCmdId();

        /**
         * @brief   设置操作的复合对象中的s。
         * @param   cmdIds：ID列表
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-06
         */
        void setShapeCmdIds(QList<Interface::VirtualShape> cmdIds);

        /**
         * @brief   获取 操作的复合对象中的IDs。
         * @return  边虚拓扑ID列表
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-06
         */
        QList<Interface::VirtualShape>& getShapeCmdIds();

    protected:
        /**
         * @brief   复合对象的数据。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-06
         */
        Interface::VirtualShape m_solidCmdId{};

        /**
         * @brief   添加或删除 复合对象中的数据对象。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-06
         */
        QList<Interface::VirtualShape> m_solidCmdIds{};
    };
}

#endif // !FITKABSGEOMODELOPERCOMPOUND_H
