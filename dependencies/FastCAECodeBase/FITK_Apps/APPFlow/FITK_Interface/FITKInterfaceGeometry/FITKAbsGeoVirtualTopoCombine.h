/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoVirtualTopoCombine.h
 * @brief  虚拓扑合并
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2025-07-16
 */
#ifndef FITKABSGEOVIRTUALTOPOCOMBINE_H
#define FITKABSGEOVIRTUALTOPOCOMBINE_H


#include "FITKAbsGeoCommand.h"
#include "FITKInterfaceGeometryAPI.h"

namespace Interface {
    /**
     * @brief  虚拓扑合并.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-07-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoVirtualTopoCombine :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoVirtualTopoCombine);
    public:
        FITKAbsGeoVirtualTopoCombine() = default;
        virtual ~FITKAbsGeoVirtualTopoCombine() = default;

        void setCombineTopos(QList<VirtualShape> topos) { m_Topos = topos; }
        QList<VirtualShape> combineTopos() const { return m_Topos; }

        void setResultTopos(QList<VirtualShape> topos) { m_resultTopos = topos; }
        QList<VirtualShape> resultTopos() const { return m_resultTopos; }

        void setHiddenTopos(QList<VirtualShape> topos) { m_hiddenTopos = topos; }
        QList<VirtualShape> hiddenTopos() const { return m_hiddenTopos; }

        /**
         * @brief       从部件中移除命令处理操作。[虚函数][重写]
         * @param[in]   part：部件命令
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-07
         */
        virtual void removeFromPart(FITKAbsGeoCommand* part) override;

    protected:
        QList<VirtualShape> m_Topos{};

        /**
         * @brief       合并后被隐藏的拓扑。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-28
         */
        QList<VirtualShape> m_hiddenTopos;

        /**
         * @brief       合并结果拓扑。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-07
         */
        QList<VirtualShape> m_resultTopos;

    };

    /**
     * @brief  虚拓扑合并面.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-07-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoVirtualTopoCombineFaces :
        public FITKAbsGeoVirtualTopoCombine
    {
        FITKCLASS(Interface, FITKAbsGeoVirtualTopoCombineFaces);
    public:
        FITKAbsGeoVirtualTopoCombineFaces() = default;
        ~FITKAbsGeoVirtualTopoCombineFaces() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        void setSkipSharpAngle(bool skip) { m_SkipSharpAngle = skip; }
        bool skipSharpAngle() const { return m_SkipSharpAngle; }
        void setCombineBounding(bool skip) { m_CombineBounding = skip; }
        bool combineBounding() const { return m_CombineBounding; }

    protected:
        bool m_SkipSharpAngle{ true };
        bool m_CombineBounding{ true };
    };

    /**
     * @brief  虚拓扑合并边.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-07-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoVirtualTopoCombineEdges :
        public FITKAbsGeoVirtualTopoCombine
    {
        FITKCLASS(Interface, FITKAbsGeoVirtualTopoCombineEdges);
    public:
        FITKAbsGeoVirtualTopoCombineEdges() = default;
        ~FITKAbsGeoVirtualTopoCombineEdges() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        void setSkipSharpAngle(bool skip) { m_SkipSharpAngle = skip; }
        bool skipSharpAngle() const { return m_SkipSharpAngle; }

    protected:
        bool m_SkipSharpAngle{ true };

    };

}
#endif // !FITKABSGEOVIRTUALTOPOCOMBINE_H
