/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKGlobalMeshingParameter.h
 * @brief       通用网格划分参数。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-08-27
 *********************************************************************/

#ifndef __FITKGLOBALMESHINGPARAMETER__H__
#define __FITKGLOBALMESHINGPARAMETER__H__

#include "FITKAbaqusDataAPI.h"
#include "FITKLocalMeshRefinement.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

#include <QObject>
#include <QMetaEnum>

namespace AbaqusData
{
    /**
     * @brief       通用网格划分参数。
     */
    class FITKAbaqusDataAPI FITKGlobalMeshingParameter : public Core::FITKAbstractDataObject
    {
        Q_OBJECT;
    public:
        enum ElementType2D3D
        {
            ET_Quad = 0,
            ET_Tri
        };
        Q_ENUM(ElementType2D3D);

        explicit FITKGlobalMeshingParameter() = default;
        virtual ~FITKGlobalMeshingParameter() = default;

        void setMeshSizeFactor(double fac);
        double getMeshSizeFactor();

        void setMinMeshSize(double size);
        double getMinMeshSize();

        void setMaxMeshSize(double size);
        double getMaxMeshSize();

        void setHighOrderFlag(bool flag);
        bool getHighOrderFlag();

        void setElementType(ElementType2D3D type);
        ElementType2D3D getElementType();

        void setSizeConstraintEnabled(bool v);
        bool getSizeConstraintEnabled() const;

        void setSmoothRatio(double v);
        double getSmoothRatio() const;

        void setSmoothSteps(int v);
        int getSmoothSteps() const;

        void setOptimize(bool v);
        bool getOptimize() const;

        void setOptimizeNetgen(bool v);
        bool getOptimizeNetgen() const;

        void setCheckSurfaceNormal(bool v);
        bool getCheckSurfaceNormal() const;

        void setQualityType(int v);
        int getQualityType() const;

        void setOptimizeThreshold(double v);
        double getOptimizeThreshold() const;

        void setGeometryCoherence(bool v);
        bool getGeometryCoherence() const;

        void setGeometryStitch(bool v);
        bool getGeometryStitch() const;

        void setUseIncompleteElement(bool v);
        bool getUseIncompleteElement() const;

        void setRecombineAll(bool v);
        bool getRecombineAll() const;

        void setAlgorithm2D(int v);
        int getAlgorithm2D() const;

        void setAlgorithm3D(int v);
        int getAlgorithm3D() const;

        void setReAlgorithm2D(int v);
        int getReAlgorithm2D() const;

        void setLocalRefineEnabled(bool v);
        bool getLocalRefineEnabled() const;

        void setLocalRefineMethod(int v);
        int getLocalRefineMethod() const;

        void setLocalRefinements(const FITKLocalMeshRefinementList &list);
        FITKLocalMeshRefinementList getLocalRefinements() const;

        static QString localRefinementsToJson(const FITKLocalMeshRefinementList &list);
        static FITKLocalMeshRefinementList localRefinementsFromJson(const QString &json);

    protected:
        double m_sizeFactor = 1.;
        double m_minMeshSize = 1.;
        double m_maxMeshSize = 5.;
        bool m_highOrderFlag = false;
        ElementType2D3D m_eType = ElementType2D3D::ET_Quad;

        bool m_sizeConstraintEnabled = true;
        double m_smoothRatio = 1.8;
        int m_smoothSteps = 1;
        bool m_optimize = true;
        bool m_optimizeNetgen = false;
        bool m_checkSurfaceNormal = true;
        int m_qualityType = 2;
        double m_optimizeThreshold = 0.3;
        bool m_geometryCoherence = false;
        bool m_geometryStitch = true;
        bool m_useIncompleteElement = false;
        bool m_recombineAll = false;
        int m_algorithm2D = 2;
        int m_algorithm3D = 0;
        int m_reAlgorithm2D = 1;
        bool m_localRefineEnabled = false;
        int m_localRefineMethod = 0;
        FITKLocalMeshRefinementList m_localRefinements;
    };
}
#endif
