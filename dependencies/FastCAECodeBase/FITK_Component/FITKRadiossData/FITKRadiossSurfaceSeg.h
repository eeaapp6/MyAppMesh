/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKRadiossSurfaceSeg.h
 * @brief Radioss表面分段类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-09-18
 * 
 */ 
#ifndef FITK_RADIOSS_SURFACE_SEG_H
#define FITK_RADIOSS_SURFACE_SEG_H

#include "FITKAbstractRadiossSurface.h"
#include <QHash>
#include <QList>

namespace Radioss
{
    /**
    * @class FITKRadiossSurfaceRadIOInfo
    * @author wangning (2185896382@qq.com)
    * @date   2025-10-27
    */
    class FITKRadiossDataAPI FITKRadiossSurfaceRadIOInfo
    {
        friend class FITKRadiossSurfaceSeg;

    public:
        explicit FITKRadiossSurfaceRadIOInfo()=default;
        virtual ~FITKRadiossSurfaceRadIOInfo()=default;

        /**
         * @brief 设置Radioss关键字
         * @param[i] keyWords 关键字
         * @author LiBaojun
         */
        void setSurfaceType(FITKAbstractRadiossSurface::RadiossSurfaceType type) { _type = type; }
        FITKAbstractRadiossSurface::RadiossSurfaceType getSurfaceType() { return _type; }
        /**
         * @brief 设置Radioss ID列表
         * @param[i] ids ID列表
         * @author LiBaojun
         */
        void setRadiossIDs(const QList<int>& ids) { _radiossIDs= ids; }
        QList<int> getRadiossIDs() { return _radiossIDs; }

    private:
        /**
         * @brief rad读取临时类型
         * @author wangning (2185896382@qq.com)
         * @date   2025-10-27
         */
        FITKAbstractRadiossSurface::RadiossSurfaceType _type = FITKAbstractRadiossSurface::RadiossSurfaceType::RST_None;
        QList<int> _radiossIDs;
    };

    /**
     * @brief Radioss表面分段类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-09-18
     */
    class FITKRadiossDataAPI FITKRadiossSurfaceSeg : public FITKAbstractRadiossSurface
    {
        friend class RadReadInformationMapper;
        friend class FITKRadiossAdaptorSurfaceOther;
        Q_OBJECT
        FITKCLASS(Radioss, FITKRadiossSurfaceSeg)
        RadiossKeyWord(FITKRadiossSurfaceSeg, /SURF/SEG)

    public:
        /**
         * @brief Construct a new FITKRadiossSurfaceSeg object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-18
         */
        explicit FITKRadiossSurfaceSeg();
        /**
         * @brief Destroy the FITKRadiossSurfaceSeg object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-18
         */
        virtual ~FITKRadiossSurfaceSeg() = default;
        /**
         * @brief 获取Radioss表面类型
         * @return RadiossSurfaceType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-18
         */
        virtual FITKAbstractRadiossSurface::RadiossSurfaceType getRadiossSurfaceType() override;
        /**
         * @brief 添加表面
         * @param[i]  surfNodesIDs  表面节点ID列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-18
         */
        void addSurface(QList<int> surfNodesIDs);
        /**
         * @brief 获取表面节点ID列表
         * @return QList<int>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-18
         */
        QList< QList<int> > getSurface(int element, const bool & lowerOrder= false);
        /**
         * @brief 获取所有单元ID
         * @return QList<int>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-18
         */
        QList<int> getAllElement() const;

    private:
        FITKRadiossSurfaceRadIOInfo* getRadIOInfo();
        void transformRadIOInfo(FITKRadiossMeshModel* model);
    private:
        ///< Radioss RadIO信息
        FITKRadiossSurfaceRadIOInfo* _radIOInfo{ nullptr };
    };

} // namespace Radioss

#endif // FITK_RADIOSS_SURFACE_SEG_H