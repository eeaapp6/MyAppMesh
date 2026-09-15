/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGmshWorkflowMeshSync.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshSizeInfoGenerator.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSize.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeBox.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeCylinder.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeSphere.h"

#include <QtMath>

namespace GmshExe
{
    namespace
    {
        static const QString kGmshExecId = QStringLiteral("GmshExec");

        QStringList splitNumericTokens(const QString &s)
        {
            QStringList out;
            for (const QString &p : s.split(QLatin1Char(','))) {
                const QString t = p.trimmed();
                if (!t.isEmpty())
                    out.append(t);
            }
            return out;
        }

        bool parseDoubles(const QString &s, int minCount, QVector<double> *out)
        {
            out->clear();
            for (const QString &p : splitNumericTokens(s)) {
                bool ok = false;
                const double v = p.toDouble(&ok);
                if (!ok)
                    return false;
                out->append(v);
            }
            return out->size() >= minCount;
        }
    }

    void FITKGmshWorkflowMeshSync::deriveAlgorithmsFromElementType(AbaqusData::FITKGlobalMeshingParameter *gp, int meshDim)
    {
        if (!gp)
            return;
        using Alg = Interface::FITKGlobalMeshGenerateAlgorithmInfo;
        if (gp->getElementType() == AbaqusData::FITKGlobalMeshingParameter::ET_Tri) {
            gp->setRecombineAll(false);
            gp->setAlgorithm2D(static_cast<int>(Alg::Algorithm2D::Delaunay2D));
            gp->setAlgorithm3D(static_cast<int>(Alg::Algorithm3D::Delaunay3D));
            gp->setReAlgorithm2D(static_cast<int>(Alg::ReAlgorithm2D::Blossom));
        } else {
            gp->setRecombineAll(true);
            gp->setAlgorithm2D(static_cast<int>(Alg::Algorithm2D::FrontalDelaunayForQuads));
            gp->setAlgorithm3D(static_cast<int>(Alg::Algorithm3D::Delaunay3D));
            gp->setReAlgorithm2D(static_cast<int>(Alg::ReAlgorithm2D::BlossomFullQuad));
        }
        Q_UNUSED(meshDim);
    }

    bool FITKGmshWorkflowMeshSync::parseRegionSpec(int method, const QString &spec, QString *errOut)
    {
        QVector<double> nums;
        const int need = (method == 0) ? 6 : (method == 1 ? 8 : 4);
        if (!parseDoubles(spec, need, &nums)) {
            if (errOut)
                *errOut = QObject::tr("Invalid region specification (expected %1 numbers).").arg(need);
            return false;
        }
        return true;
    }

    bool FITKGmshWorkflowMeshSync::applyPartParamsToGmshExec(AbaqusData::FITKGlobalMeshingParameter *gp, int meshDim,
        QString *errOut)
    {
        if (!gp || meshDim < 2 || meshDim > 3) {
            if (errOut)
                *errOut = QObject::tr("Invalid mesh parameters or dimension.");
            return false;
        }

        deriveAlgorithmsFromElementType(gp, meshDim);

        Interface::FITKMeshGenInterface *mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf) {
            if (errOut)
                *errOut = QObject::tr("Mesh generator interface is not available.");
            return false;
        }

        Interface::FITKGlobalMeshSizeInfo *sizeInfo = mf->getGlobalMeshSizeInfo(kGmshExecId);
        Interface::FITKGlobalMeshGenerateAlgorithmInfo *alg = mf->getGlobalMeshGenerateAlgorithmInfo(kGmshExecId);
        if (!sizeInfo || !alg) {
            if (errOut)
                *errOut = QObject::tr("Gmsh global mesh settings are not available.");
            return false;
        }

        if (gp->getSizeConstraintEnabled()) {
            sizeInfo->setSizeFactor(gp->getMeshSizeFactor());
            sizeInfo->setMinSize(gp->getMinMeshSize());
            sizeInfo->setMaxSize(qMax(gp->getMaxMeshSize(), 1e-6));
        } else {
            sizeInfo->setSizeFactor(gp->getMeshSizeFactor());
            sizeInfo->setMinSize(1e-12);
            sizeInfo->setMaxSize(1e12);
        }

        alg->setMeshGenerateDimension(meshDim);
        alg->setElementOrder(gp->getHighOrderFlag() ? 2 : 1);
        alg->setUseIncElem(gp->getUseIncompleteElement());
        alg->setSmoothStep(gp->getSmoothSteps());
        alg->setGeometryCoherence(gp->getGeometryCoherence());
        alg->setGeometryStitch(gp->getGeometryStitch());
        alg->setRecombineAllTriMeshes(gp->getRecombineAll());

        alg->set2DAlgorithm(static_cast<Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D>(gp->getAlgorithm2D()));
        alg->set3DAlgorithm(static_cast<Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D>(gp->getAlgorithm3D()));
        alg->set2DRecombinationAlgorithm(
            static_cast<Interface::FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D>(gp->getReAlgorithm2D()));

        if (gp->getElementType() == AbaqusData::FITKGlobalMeshingParameter::ET_Quad) {
            if (meshDim >= 3)
                alg->setSubdivisionAlgorithm(Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::AllHexas);
            else
                alg->setSubdivisionAlgorithm(Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::AllQuads);
        } else {
            alg->setSubdivisionAlgorithm(Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::None);
        }

        alg->setValue(QStringLiteral("MeshSmoothRatio"), gp->getSmoothRatio());
        alg->setValue(QStringLiteral("MeshOptimize"), gp->getOptimize());
        alg->setValue(QStringLiteral("MeshOptimizeNetgen"), gp->getOptimizeNetgen());
        alg->setValue(QStringLiteral("MeshOptimizeThreshold"), gp->getOptimizeThreshold());
        alg->setValue(QStringLiteral("MeshQualityType"), gp->getQualityType());
        alg->setValue(QStringLiteral("MeshCheckSurfaceNormal"), gp->getCheckSurfaceNormal());
        alg->setValue(QStringLiteral("FieldMergeMin"), true);

        Interface::FITKRegionMeshSizeManager *mgr = mf->getRegionMeshSizeMgr(kGmshExecId);
        if (!mgr)
            return true;
        mgr->clear();
        if (!gp->getLocalRefineEnabled())
            return true;

        Interface::FITKAbstractMeshSizeInfoGenerator *gen = mf->getMeshSizeGenerator(kGmshExecId);
        if (!gen)
            return true;

        const AbaqusData::FITKLocalMeshRefinementList regions = gp->getLocalRefinements();
        for (const AbaqusData::FITKLocalMeshRefinement &r : regions) {
            if (!parseRegionSpec(r.method, r.regionSpec, errOut))
                return false;

            QVector<double> nums;
            parseDoubles(r.regionSpec, 1, &nums);
            const double vIn = r.sizeIn;
            const double vTh = qMax(r.thickness, 1e-9);
            const double vOut = qMax(r.sizeOut, vIn);

            if (r.method == 0) {
                parseDoubles(r.regionSpec, 6, &nums);
                auto *box = dynamic_cast<Interface::FITKRegionMeshSizeBox *>(
                    gen->createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionBox));
                if (!box)
                    continue;
                double p1[3] = { nums[0], nums[1], nums[2] };
                double len[3] = { nums[3], nums[4], nums[5] };
                box->setPoint1(p1);
                box->setLength(len);
                box->setInternalSize(vIn);
                box->setExternalSize(vOut);
                box->setThickness(vTh);
                box->setValue(QStringLiteral("BackgroundRegion"), true);
                box->setDataObjectName(mgr->checkName(r.name.isEmpty() ? QStringLiteral("Region") : r.name));
                mgr->appendDataObj(box);
            } else if (r.method == 1) {
                parseDoubles(r.regionSpec, 8, &nums);
                auto *cy = dynamic_cast<Interface::FITKRegionMeshSizeCylinder *>(
                    gen->createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionCylinder));
                if (!cy)
                    continue;
                double loc[3] = { nums[0], nums[1], nums[2] };
                double axis[3] = { nums[3], nums[4], nums[5] };
                cy->setLocation(loc);
                cy->setDirection(axis);
                cy->setLength(nums[6]);
                cy->setRadius(nums[7]);
                cy->setInternalSize(vIn);
                cy->setExternalSize(vOut);
                cy->setThickness(vTh);
                cy->setValue(QStringLiteral("BackgroundRegion"), true);
                cy->setDataObjectName(mgr->checkName(r.name.isEmpty() ? QStringLiteral("Cylinder") : r.name));
                mgr->appendDataObj(cy);
            } else if (r.method == 2) {
                parseDoubles(r.regionSpec, 4, &nums);
                auto *sp = dynamic_cast<Interface::FITKRegionMeshSizeSphere *>(
                    gen->createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionSphere));
                if (!sp)
                    continue;
                double loc[3] = { nums[0], nums[1], nums[2] };
                sp->setLocation(loc);
                sp->setRadius(nums[3]);
                sp->setInternalSize(vIn);
                sp->setExternalSize(vOut);
                sp->setThickness(vTh);
                sp->setValue(QStringLiteral("BackgroundRegion"), true);
                sp->setDataObjectName(mgr->checkName(r.name.isEmpty() ? QStringLiteral("Sphere") : r.name));
                mgr->appendDataObj(sp);
            }
        }
        return true;
    }
}
