/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITKOCCHDF5ADAPTORSIMPLESHAPE_H__
#define __FITKOCCHDF5ADAPTORSIMPLESHAPE_H__

#include "FITKGeoOCCIOHDF5API.h"
#include "FITKOCCShapeHDF5IOHelper.h"
#include "FITKOCCHDF5AdaptorMacros.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModelBox.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModelCylinder.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModelSphere.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModelBool.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModelPoint.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModel.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModelTransform.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModelCurve.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModelThreePointsCurve.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModelFace.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModelSolid.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModelSketch.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModelSplitter.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoDatumModel.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoModelPartition.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"


namespace OCC
{
    FITKOCCAdaptorDec(FITKOCCAdaptorModelBox, Interface::FITKAdaptorGeoModelBox)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelBox, FITKOCCAdaptorModelBox)
   
    FITKOCCAdaptorDec(FITKOCCAdaptorModelCylinder, Interface::FITKAdaptorGeoModelCylinder)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelCylinder, FITKOCCAdaptorModelCylinder)

    FITKOCCAdaptorDec(FITKOCCAdaptorModelSphere, Interface::FITKAdaptorGeoModelSphere)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelSphere, FITKOCCAdaptorModelSphere)

    FITKOCCAdaptorDec(FITKOCCAdaptorModelBool, Interface::FITKAdaptorGeoModelBool)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoOperBool, FITKOCCAdaptorModelBool)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoPoint, Interface::FITKAdaptorGeoPoint)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPoint, FITKOCCAdaptorGeoPoint)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoCurveEnd, Interface::FITKAdaptorGeoCurveEnd)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelCurveEnd, FITKOCCAdaptorGeoCurveEnd)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoCurveEndDistance, Interface::FITKAdaptorGeoCurveEndDistance)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelCurveEndDistance, FITKOCCAdaptorGeoCurveEndDistance)
       
    FITKOCCAdaptorDec(FITKOCCAdaptorGeoCurveRadio, Interface::FITKAdaptorGeoCurveRadio)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelCurveRadio, FITKOCCAdaptorGeoCurveRadio)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoPointProjectionOnCurve, Interface::FITKAdaptorGeoPointProjectionOnCurve)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPointProjectionOnCurve, FITKOCCAdaptorGeoPointProjectionOnCurve)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoTwoCurveIntersection, Interface::FITKAdaptorGeoTwoCurveIntersection)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelTwoCurveIntersection, FITKOCCAdaptorGeoTwoCurveIntersection)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoPointProjectionOnSurface, Interface::FITKAdaptorGeoPointProjectionOnSurface)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPointProjectionOnSurface, FITKOCCAdaptorGeoPointProjectionOnSurface)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoCurveIntersectionSurface, Interface::FITKAdaptorGeoCurveIntersectionSurface)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelCurveIntersectionSurface, FITKOCCAdaptorGeoCurveIntersectionSurface)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelDelete, Interface::FITKAdaptorGeoModelDelete)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoDelete, FITKOCCAdaptorGeoModelDelete)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelMultiDelete, Interface::FITKAdaptorGeoModelMultiDelete)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoMultiDelete, FITKOCCAdaptorGeoModelMultiDelete)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoImprintSolid, Interface::FITKAdaptorGeoImprintSolid)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelOperImprintSolid, FITKOCCAdaptorGeoImprintSolid)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelImport, Interface::FITKAdaptorGeoModelImport)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelImport, FITKOCCAdaptorGeoModelImport)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelExport, Interface::FITKAdaptorGeoModelExport)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelExport, FITKOCCAdaptorGeoModelExport)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelCompound, Interface::FITKAdaptorGeoModelCompound)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelOperCompound, FITKOCCAdaptorGeoModelCompound)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelTrans, Interface::FITKAdaptorGeoModelTrans)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelTransform, FITKOCCAdaptorGeoModelTrans)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelTransformByTwoPoints, Interface::FITKAdaptorGeoModelTransformByTwoPoints)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelTransformByTwoPoints, FITKOCCAdaptorGeoModelTransformByTwoPoints)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelTransformByDirAndDis, Interface::FITKAdaptorGeoModelTransformByDirAndDis)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelTransformByDirAndDis, FITKOCCAdaptorGeoModelTransformByDirAndDis)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelRotate, Interface::FITKAdaptorGeoModelRotate)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelRotate, FITKOCCAdaptorGeoModelRotate)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelScale, Interface::FITKAdaptorGeoModelScale)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelScale, FITKOCCAdaptorGeoModelScale)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelLine, Interface::FITKAdaptorGeoModelLine)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelLine, FITKOCCAdaptorGeoModelLine)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelSegment, Interface::FITKAdaptorGeoModelSegment)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelSegment, FITKOCCAdaptorGeoModelSegment)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelWire, Interface::FITKAdaptorGeoModelWire)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelWire, FITKOCCAdaptorGeoModelWire)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelCircle, Interface::FITKAdaptorGeoModelCircle)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelCircle, FITKOCCAdaptorGeoModelCircle)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelCircleArc, Interface::FITKAdaptorGeoModelCircleArc)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelCircleArc, FITKOCCAdaptorGeoModelCircleArc)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelThreePointsCircle, Interface::FITKAdaptorGeoModelThreePointsCircle)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelThreePointsCircle, FITKOCCAdaptorGeoModelThreePointsCircle)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelEllipse, Interface::FITKAdaptorGeoModelEllipse)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelEllipse, FITKOCCAdaptorGeoModelEllipse)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelEllipseArc, Interface::FITKAdaptorGeoModelEllipseArc)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelEllipseArc, FITKOCCAdaptorGeoModelEllipseArc)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelHyperbola, Interface::FITKAdaptorGeoModelHyperbola)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelHyperbola, FITKOCCAdaptorGeoModelHyperbola)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelHyperbolaArc, Interface::FITKAdaptorGeoModelHyperbolaArc)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelHyperbolaArc, FITKOCCAdaptorGeoModelHyperbolaArc)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelParabola, Interface::FITKAdaptorGeoModelParabola)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelParabola, FITKOCCAdaptorGeoModelParabola)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelParabolaArc, Interface::FITKAdaptorGeoModelParabolaArc)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelParabolaArc, FITKOCCAdaptorGeoModelParabolaArc)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelOffsetCurve, Interface::FITKAdaptorGeoModelOffsetCurve)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelOffsetCurve, FITKOCCAdaptorGeoModelOffsetCurve)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelBeizeByControlPoints, Interface::FITKAdaptorGeoModelBeizeByControlPoints)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelBeizeByControlPoints, FITKOCCAdaptorGeoModelBeizeByControlPoints)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelBsplineByThroughPoints, Interface::FITKAdaptorGeoModelBsplineByThroughPoints)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelBsplineByThroughPoints, FITKOCCAdaptorGeoModelBsplineByThroughPoints)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelThreePointsEllipse, Interface::FITKAdaptorGeoModelThreePointsEllipse)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelThreePointsEllipse, FITKOCCAdaptorGeoModelThreePointsEllipse)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelThreePointsHyperbolae, Interface::FITKAdaptorGeoModelThreePointsHyperbolae)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelThreePointsHyperbola, FITKOCCAdaptorGeoModelThreePointsHyperbolae)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelThreePointsParabola, Interface::FITKAdaptorGeoModelThreePointsParabola)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelThreePointsParabola, FITKOCCAdaptorGeoModelThreePointsParabola)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelCurveProjectionOnSurface, Interface::FITKAdaptorGeoModelCurveProjectionOnSurface)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelCurveProjectionOnSurface, FITKOCCAdaptorGeoModelCurveProjectionOnSurface)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelTrimmedCurve, Interface::FITKAdaptorGeoModelTrimmedCurve)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelTrimmedCurve, FITKOCCAdaptorGeoModelTrimmedCurve)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelSurfaceIntersectionSurface, Interface::FITKAdaptorGeoModelSurfaceIntersectionSurface)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelSurfaceIntersectionSurface, FITKOCCAdaptorGeoModelSurfaceIntersectionSurface)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelBridgeCurve, Interface::FITKAdaptorGeoModelBridgeCurve)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelBridgeCurve, FITKOCCAdaptorGeoModelBridgeCurve)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelSurfaceEdge, Interface::FITKAdaptorGeoModelSurfaceEdge)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelSurfaceEdge, FITKOCCAdaptorGeoModelSurfaceEdge)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelOperLine, Interface::FITKAdaptorGeoModelOperLine)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelOperLine, FITKOCCAdaptorGeoModelOperLine)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelBridgeSurface, Interface::FITKAdaptorGeoModelBridgeSurface)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelBridgeSurface, FITKOCCAdaptorGeoModelBridgeSurface)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelSolidSurface, Interface::FITKAdaptorGeoModelSolidSurface)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelSolidSurface, FITKOCCAdaptorGeoModelSolidSurface)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelMultiSectionSurface, Interface::FITKAdaptorGeoModelMultiSectionSurface)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelMultiSectionSurface, FITKOCCAdaptorGeoModelMultiSectionSurface)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelSweepSurface, Interface::FITKAdaptorGeoModelSweepSurface)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelSweepSurface, FITKOCCAdaptorGeoModelSweepSurface)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelRevolSurface, Interface::FITKAdaptorGeoModelRevolSurface)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelRevolSurface, FITKOCCAdaptorGeoModelRevolSurface)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelExtrudeSurface, Interface::FITKAdaptorGeoModelExtrudeSurface)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelExtrudeSurface, FITKOCCAdaptorGeoModelExtrudeSurface)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelOffsetSurface, Interface::FITKAdaptorGeoModelOffsetSurface)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelOffsetSurface, FITKOCCAdaptorGeoModelOffsetSurface)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelClosedWireSurface, Interface::FITKAdaptorGeoModelClosedWireSurface)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelClosedWireSurface, FITKOCCAdaptorGeoModelClosedWireSurface)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelOperFaceFillHoles, Interface::FITKAdaptorGeoModelOperFaceFillHoles)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelOperFaceFillHoles, FITKOCCAdaptorGeoModelOperFaceFillHoles)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelOperFaceFillGaps, Interface::FITKAdaptorGeoModelOperFaceFillGaps)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelOperFaceFillGaps, FITKOCCAdaptorGeoModelOperFaceFillGaps)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelOperFaceDeleteFloatingEdge, Interface::FITKAdaptorGeoModelOperFaceDeleteFloatingEdge)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge, FITKOCCAdaptorGeoModelOperFaceDeleteFloatingEdge)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelChamferSolid, Interface::FITKAdaptorGeoModelChamferSolid)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelChamferSolid, FITKOCCAdaptorGeoModelChamferSolid)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelRemoveChamferSolid, Interface::FITKAdaptorGeoModelRemoveChamferSolid)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelRemoveChamferSolid, FITKOCCAdaptorGeoModelRemoveChamferSolid)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelFilletSolid, Interface::FITKAdaptorGeoModelFilletSolid)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelFilletSolid, FITKOCCAdaptorGeoModelFilletSolid)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelRemoveFilletSolid, Interface::FITKAdaptorGeoModelRemoveFilletSolid)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelRemoveFilletSolid, FITKOCCAdaptorGeoModelRemoveFilletSolid)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelDefeatureSolid, Interface::FITKAdaptorGeoModelDefeatureSolid)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelDefeatureSolid, FITKOCCAdaptorGeoModelDefeatureSolid)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelFillHoleSolid, Interface::FITKAdaptorGeoModelFillHoleSolid)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelFillHoleSolid, FITKOCCAdaptorGeoModelFillHoleSolid)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelClosedSurfaceSolid, Interface::FITKAdaptorGeoModelClosedSurfaceSolid)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelClosedSurfaceSolid, FITKOCCAdaptorGeoModelClosedSurfaceSolid)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelExtrudeSolid, Interface::FITKAdaptorGeoModelExtrudeSolid)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelExtrudeSolid, FITKOCCAdaptorGeoModelExtrudeSolid)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelRevolSolid, Interface::FITKAdaptorGeoModelRevolSolid)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelRevolSolid, FITKOCCAdaptorGeoModelRevolSolid)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelSweepSolid, Interface::FITKAdaptorGeoModelSweepSolid)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelSweepSolid, FITKOCCAdaptorGeoModelSweepSolid)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelMultiSectionSolid, Interface::FITKAdaptorGeoModelMultiSectionSolid)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelMultiSectionSolid, FITKOCCAdaptorGeoModelMultiSectionSolid)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelStiffener, Interface::FITKAdaptorGeoModelStiffener)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoStiffener, FITKOCCAdaptorGeoModelStiffener)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelNearestPointLine, Interface::FITKAdaptorGeoModelNearestPointLine)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelNearestPointLine, FITKOCCAdaptorGeoModelNearestPointLine)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelSampleCurve, Interface::FITKAdaptorGeoModelSampleCurve)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoSampleCurve, FITKOCCAdaptorGeoModelSampleCurve)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelSketch, Interface::FITKAdaptorGeoModelSketch)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoSketch2D, FITKOCCAdaptorGeoModelSketch)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoCurveSplitter, Interface::FITKAdaptorGeoModelCurveSplitter)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoCurveSplitter, FITKOCCAdaptorGeoCurveSplitter)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoSurfaceSplitter, Interface::FITKAdaptorGeoModelSurfaceSplitter)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoSurfaceSplitter, FITKOCCAdaptorGeoSurfaceSplitter)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoSolidSplitter, Interface::FITKAdaptorGeoModelSolidSplitter)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoSolidSplitter, FITKOCCAdaptorGeoSolidSplitter)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoInteractionPoint, Interface::FITKAdaptorGeoInteractionPoint)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelInteractionPoint, FITKOCCAdaptorGeoInteractionPoint)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelImportFlattenAssembly, Interface::FITKAdaptorGeoModelImportFlattenAssembly)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelImportFlattenAssembly, FITKOCCAdaptorGeoModelImportFlattenAssembly)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelMirror, Interface::FITKAdaptorGeoModelMirror)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelMirror, FITKOCCAdaptorGeoModelMirror)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelRectangularPattern, Interface::FITKAdaptorGeoModelRectangularPattern)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelRectangularPattern, FITKOCCAdaptorGeoModelRectangularPattern)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelCircularPattern, Interface::FITKAdaptorGeoModelCircularPattern)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelCircularPattern, FITKOCCAdaptorGeoModelCircularPattern)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoDatumPoint, Interface::FITKAdaptorGeoDatumPoint)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoDatumPoint, FITKOCCAdaptorGeoDatumPoint)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoDatumLine, Interface::FITKAdaptorGeoDatumLine)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoDatumLine, FITKOCCAdaptorGeoDatumLine)

    FITKOCCAdaptorDec(FITKOCCAdaptorModelReferenceLineByIntersectPlanes, Interface::FITKAdaptorModelReferenceLineByIntersectPlanes)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoReferenceLineByIntersectPlanes, FITKOCCAdaptorModelReferenceLineByIntersectPlanes)

    FITKOCCAdaptorDec(FITKOCCAdaptorModelReferenceLineByByRotateLine, Interface::FITKAdaptorModelReferenceLineByRotateLine)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoReferenceLineByRotateLine, FITKOCCAdaptorModelReferenceLineByByRotateLine)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoDatumPlane, Interface::FITKAdaptorGeoDatumPlane)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoDatumPlane, FITKOCCAdaptorGeoDatumPlane)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoReferenceOffsetPlane, Interface::FITKAdaptorGeoReferenceOffsetPlane)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoReferenceOffsetPlane, FITKOCCAdaptorGeoReferenceOffsetPlane)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoReferenceThreePointsPlane, Interface::FITKAdaptorGeoReferenceThreePointsPlane)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoReferenceThreePointsPlane, FITKOCCAdaptorGeoReferenceThreePointsPlane)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoReferenceEquationPlane, Interface::FITKAdaptorGeoReferenceEquationPlane)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoReferenceEquationPlane, FITKOCCAdaptorGeoReferenceEquationPlane)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoReferencePointAndDirectionPlane, Interface::FITKAdaptorGeoReferencePointAndDirectionPlane)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoReferencePointAndDirectionPlane, FITKOCCAdaptorGeoReferencePointAndDirectionPlane)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoReferenceRotateFromPlanePlane, Interface::FITKAdaptorModelReferenceRotateFromPlanePlane)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoReferenceRotateFromPlanePlane, FITKOCCAdaptorGeoReferenceRotateFromPlanePlane)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoPart, Interface::FITKAdaptorGeoPart)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoPart, FITKOCCAdaptorGeoPart)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoExtractCenter, Interface::FITKAdaptorGeoExtractCenter)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelExtractCenter, FITKOCCAdaptorGeoExtractCenter)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelFromMesh, Interface::FITKAdaptorGeoModelFromMesh)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelFromMesh, FITKOCCAdaptorGeoModelFromMesh)

    FITKOCCAdaptorDec(FITKOCCAdaptorGeoOperNBodyUnite, Interface::FITKAdaptorGeoOperNBodyUnite)
    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoOperNBodyUnite, FITKOCCAdaptorGeoOperNBodyUnite)

        FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelConstruction, Interface::FITKAdaptorGeoModelConstruction)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsConstruction, FITKOCCAdaptorGeoModelConstruction)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionEdgeWithParameter, Interface::FITKAdaptorGeoModelPartitionEdgeWithParameter)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionEdgeWithParameter, FITKOCCAdaptorModelPartitionEdgeWithParameter)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionEdgeWithPoint, Interface::FITKAdaptorGeoModelPartitionEdgeWithPoint)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionEdgeWithPoint, FITKOCCAdaptorModelPartitionEdgeWithPoint)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionEdgeWithDatumPlane, Interface::FITKAdaptorGeoModelPartitionEdgeWithDatumPlane)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionEdgeWithDatumPlane, FITKOCCAdaptorModelPartitionEdgeWithDatumPlane)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionFaceWithSketch, Interface::FITKAdaptorGeoModelPartitionFaceWithSketch)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionFaceWithSketch, FITKOCCAdaptorModelPartitionFaceWithSketch)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionFaceWithTwoPoints, Interface::FITKAdaptorGeoModelPartitionFaceWithTwoPoints)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionFaceWithTwoPoints, FITKOCCAdaptorModelPartitionFaceWithTwoPoints)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionFaceWithDatumPlane, Interface::FITKAdaptorGeoModelPartitionFaceWithDatumPlane)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionFaceWithDatumPlane, FITKOCCAdaptorModelPartitionFaceWithDatumPlane)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionFaceWithCurvedPath, Interface::FITKAdaptorGeoModelPartitionFaceWithCurvedPath)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionFaceWithCurvedPath, FITKOCCAdaptorModelPartitionFaceWithCurvedPath)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionFaceWithExtendFace, Interface::FITKAdaptorGeoModelPartitionFaceWithExtendFace)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionFaceWithExtendFace, FITKOCCAdaptorModelPartitionFaceWithExtendFace)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionFaceWithIntersectFace, Interface::FITKAdaptorGeoModelPartitionFaceWithIntersectFace)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionFaceWithIntersectFace, FITKOCCAdaptorModelPartitionFaceWithIntersectFace)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionFaceWithProjectEdges, Interface::FITKAdaptorGeoModelPartitionFaceWithProjectEdges)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionFaceWithProjectEdges, FITKOCCAdaptorModelPartitionFaceWithProjectEdges)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionSolidWithPlane, Interface::FITKAdaptorGeoModelPartitionSolidWithPlane)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionSolidWithPlane, FITKOCCAdaptorModelPartitionSolidWithPlane)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionSolidWithExtendFace, Interface::FITKAdaptorGeoModelPartitionSolidWithExtendFace)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionSolidWithExtendFace, FITKOCCAdaptorModelPartitionSolidWithExtendFace)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionSolidWithSweepEdge, Interface::FITKAdaptorGeoModelPartitionSolidWithSweepEdge)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionSolidWithSweepEdge, FITKOCCAdaptorModelPartitionSolidWithSweepEdge)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelPartitionSolidWithSketchPlanar, Interface::FITKAdaptorGeoModelPartitionSolidWithSketchPlanar)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelPartitionSolidWithSketchPlanar, FITKOCCAdaptorModelPartitionSolidWithSketchPlanar)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelSketchDeleter, Interface::FITKAdaptorModelSketchDeleter)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoSketchDeleter, FITKOCCAdaptorModelSketchDeleter)

        FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelCoverEdges, Interface::FITKAdaptorGeoModelCoverEdges)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoCoverEdges, FITKOCCAdaptorGeoModelCoverEdges)

        FITKOCCAdaptorDec(FITKOCCAdaptorGeoStitch, Interface::FITKAdaptorGeoStitch)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoStitch, FITKOCCAdaptorGeoStitch)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelReplaceFaces, Interface::FITKAdaptorModelReplaceFaces)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoReplaceFaces, FITKOCCAdaptorModelReplaceFaces)

        FITKOCCAdaptorDec(FITKOCCAdaptorModelRemoveFace, Interface::FITKAdaptorModelRemoveFace)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelOperFaceRemoveFace, FITKOCCAdaptorModelRemoveFace)

        FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelCircularHole, Interface::FITKAdaptorGeoModelCircularHole)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelCircularHoleSolid, FITKOCCAdaptorGeoModelCircularHole)

        FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelRepairSmallEdges, Interface::FITKAdaptorGeoModelModelRepairSmallEdges)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoRepairSmallEdges, FITKOCCAdaptorGeoModelRepairSmallEdges)

        FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelRepairSmallFaces, Interface::FITKAdaptorGeoModelModelRepairSmallFaces)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoRepairSmallFaces, FITKOCCAdaptorGeoModelRepairSmallFaces)

        FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelExtrudeCut, Interface::FITKAdaptorGeoModelModelExtrudeCut)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelExtrudeCut, FITKOCCAdaptorGeoModelExtrudeCut)

        FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelRevolCut, Interface::FITKAdaptorGeoModelModelRevolCut)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelRevolCut, FITKOCCAdaptorGeoModelRevolCut)

        FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelSweepCut, Interface::FITKAdaptorGeoModelModelSweepCut)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelSweepCut, FITKOCCAdaptorGeoModelSweepCut)

        FITKOCCAdaptorDec(FITKOCCAdaptorGeoModelExportTopos, Interface::FITKAdaptorGeoModelModelExportTopos)
        Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbsGeoModelExportTopos, FITKOCCAdaptorGeoModelExportTopos)
}

#endif