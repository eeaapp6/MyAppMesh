/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAbsGeoModelExportTopos.h
 * @brief  导出几何部分形状
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-08-26
 *********************************************************************/
#ifndef _FITKAbsGeoModelExportTopos_
#define _FITKAbsGeoModelExportTopos_
 
#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"
#include <QString>

namespace Interface
{
    /**
     * @brief  导出几何部分形状
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-08-26
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelExportTopos :public  FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelExportTopos);
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-26
         */
        explicit FITKAbsGeoModelExportTopos();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-26
         */
        virtual ~FITKAbsGeoModelExportTopos();
        /**
         * @brief    设置文件名称
         * @param[i] fileName                   文件名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-26
         */
        void setFileName(const QString& fileName);
        /**
         * @brief    获取文件名称
         * @return   QString                    文件名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-26
         */
        QString getFileName() const;

        /**
         * @brief       设置是否启用缝合功能。
         * @param[in]   enabled：是否启用
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-10
         */
        void setEnableStitch(bool enabled);

        /**
         * @brief       获取是否启用缝合功能。
         * @return      是否启用
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-10
         */
        bool getEnableStitch();

        /**
         * @brief       设置缝合容差。
         * @param[in]   tol：容差
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-10
         */
        void setStitchTolerance(double tol);

        /**
         * @brief       获取缝合容差。
         * @return      容差
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-10
         */
        double getStitchTolerance();

        /**
         * @brief       设置被导出的虚拓扑数据列表。
         * @param[in]   edgeTopos：虚拓扑列表[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-23
         */
        void setExportTopos(QList<VirtualShape> edgeTopos);

        /**
         * @brief       获取被导出的虚拓扑数据列表。
         * @return      虚拓扑列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-23
         */
        const QList<VirtualShape> & getExportTopos();

        /**
         * @brief    添加需要导出的Topoid
         * @param[i] topoId                     拓扑id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-26
         */
        void appendExportTopo(const VirtualShape & topo);

        /**
         * @brief       清除导出拓扑信息列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-23
         */
        void clearExportTopos();

        /**
         * @brief    获取几何命令类型
         * @return   FITKGeoEnum::FITKGeometryComType       几何命令类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-26
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    protected:
        /**
         * @brief  导出文件名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-08-26
         */
        QString _exportFileName = "";

        /**
         * @brief       导出拓扑列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-23
         */
        QList<VirtualShape> _exportTopos = {};

        /**
         * @brief       是否启用缝合。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-10
         */
        bool _enableStitch = false;

        /**
         * @brief       缝合容差。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-10
         */
        double _stitchTol = 1e-3;

    };
    
}


#endif
