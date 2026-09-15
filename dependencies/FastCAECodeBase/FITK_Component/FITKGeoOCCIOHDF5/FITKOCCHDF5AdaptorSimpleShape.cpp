/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCHDF5AdaptorSimpleShape.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoWireEdgeFromEdge.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

#include <H5Cpp.h>
#include "FITK_Component/FITKGeoCompOCC/FITKAbstractOCCModel.h"

namespace OCC
{
    FITKOCCAdaptorDef(FITKOCCAdaptorModelBox)
    FITKOCCAdaptorDef(FITKOCCAdaptorModelCylinder)
    FITKOCCAdaptorDef(FITKOCCAdaptorModelSphere)
    FITKOCCAdaptorDef(FITKOCCAdaptorModelBool)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelCone)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelFrustum)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelFilledEllipse)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelFilledRectangle)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelFilledParaboloid)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelFilledPolygon)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelMergePart)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelSplitPart)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoPoint)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoCurveEnd)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoCurveEndDistance)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoCurveRadio)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoPointProjectionOnCurve)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoTwoCurveIntersection)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoPointProjectionOnSurface)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoCurveIntersectionSurface)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelDelete)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelMultiDelete)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoImprintSolid)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelImport)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelExport)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelCompound)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelTrans)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelTransformByTwoPoints)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelTransformByDirAndDis)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelRotate)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelScale)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelLine)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelSegment)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelWire)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelCircle)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelCircleArc)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelThreePointsCircle)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelEllipse)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelEllipseArc)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelHyperbola)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelHyperbolaArc)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelParabola)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelParabolaArc)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelOffsetCurve)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelBeizeByControlPoints)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelBsplineByThroughPoints)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelThreePointsEllipse)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelThreePointsHyperbolae)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelThreePointsParabola)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelCurveProjectionOnSurface)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelTrimmedCurve)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelSurfaceIntersectionSurface)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelBridgeCurve)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelSurfaceEdge)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelOperLine)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelBridgeSurface)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelSolidSurface)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelMultiSectionSurface)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelSweepSurface)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelRevolSurface)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelExtrudeSurface)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelOffsetSurface)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelClosedWireSurface)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelOperFaceFillHoles)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelOperFaceFillGaps)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelOperFaceDeleteFloatingEdge)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelChamferSolid)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelRemoveChamferSolid)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelFilletSolid)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelRemoveFilletSolid)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelDefeatureSolid)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelFillHoleSolid)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelClosedSurfaceSolid)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelExtrudeSolid)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelRevolSolid)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelSweepSolid)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelMultiSectionSolid)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelStiffener)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelNearestPointLine)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelSampleCurve)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelSketch)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoCurveSplitter)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoSurfaceSplitter)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoSolidSplitter)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoInteractionPoint)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelImportFlattenAssembly)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelMirror)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelRectangularPattern)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelCircularPattern)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelTranslatePart)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelRotatePart)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelScalePart)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelMirrorPart)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelRectangularPatternPart)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelCircularPatternPart)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoDatumPoint)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoDatumLine)
    FITKOCCAdaptorDef(FITKOCCAdaptorModelReferenceLineByIntersectPlanes)
    FITKOCCAdaptorDef(FITKOCCAdaptorModelReferenceLineByByRotateLine)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoDatumPlane)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoReferenceOffsetPlane)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoReferenceThreePointsPlane)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoReferenceEquationPlane)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoReferencePointAndDirectionPlane)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoReferenceRotateFromPlanePlane)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoPart)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoExtractCenter)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelFromMesh)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelFromSketch)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoOperNBodyUnite)
    FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelConstruction)

        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionEdgeWithParameter)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionEdgeWithPoint)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionEdgeWithDatumPlane)

        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionFaceWithSketch)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionFaceWithTwoPoints)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionFaceWithDatumPlane)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionFaceWithCurvedPath)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionFaceWithExtendFace)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionFaceWithIntersectFace)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionFaceWithProjectEdges)

        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionSolidWithPlane)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionSolidWithExtendFace)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionSolidWithSweepEdge)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelPartitionSolidWithSketchPlanar)

        FITKOCCAdaptorDef(FITKOCCAdaptorModelSketchDeleter)

        FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelCoverEdges)
        FITKOCCAdaptorDef(FITKOCCAdaptorGeoStitch)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelReplaceFaces)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelRemoveFace)

        FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelCircularHole)

        FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelRepairSmallEdges)
        FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelRepairSmallFaces)
        FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelExtrudeCut)
        FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelRevolCut)
        FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelSweepCut)
        FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelExportTopos)

        FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelMergeEdges)
        FITKOCCAdaptorDef(FITKOCCAdaptorModelRemoveWireEdges)

        bool FITKOCCAdaptorGeoWireEdgeFromEdge::writeShape(H5::Group & g)
        {
            Interface::FITKAbsGeoCommand* command = this->getDataObjectAs<Interface::FITKAbsGeoCommand>();
            if (command == nullptr) return false;
            OCC::FITKAbstractOCCModel* model = dynamic_cast<OCC::FITKAbstractOCCModel*>(command->getShapeAgent());
            if (model == nullptr && _writer == nullptr) return false;
            return OCC::FITKOCCShapeHDF5IOHelper::writeShape(model, _writer, g);
        }

        bool FITKOCCAdaptorGeoWireEdgeFromEdge::readShape()
        {
            Interface::FITKAbsGeoCommand* command = this->getDataObjectAs<Interface::FITKAbsGeoCommand>();
            if (command == nullptr) return false;
            return OCC::FITKOCCShapeHDF5IOHelper::readShape(command, _reader, *_h5Group);
        }

        bool FITKOCCAdaptorGeoWireEdgeFromEdge::readParameters()
        {
            Interface::FITKAbsGeoWireEdgeFromEdge* cmd = this->getDataObjectAs<Interface::FITKAbsGeoWireEdgeFromEdge>();
            if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
            IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
            if (h5tools == nullptr) return false;

            std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
            if (!std::get<0>(paramGroup)) return false;

            int count = 0;
            QList<Interface::VirtualShape> edges = {};
            if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
            for (int i = 0; i < count; i++)
            {
                int edgeArray[3] = {};
                if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edgeArray, 1, 3)) return false;
                edges.append(Interface::VirtualShape(edgeArray[0], edgeArray[1], edgeArray[2]));
            }

            cmd->setEdges(edges);
            return true;
        }

        bool FITKOCCAdaptorGeoWireEdgeFromEdge::writeParameters(H5::Group & g)
        {
            Interface::FITKAbsGeoWireEdgeFromEdge* cmd = this->getDataObjectAs<Interface::FITKAbsGeoWireEdgeFromEdge>();
            if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
            IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
            if (h5tools == nullptr) return false;

            std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
            if (!std::get<0>(paramGroup)) return false;

            QList<Interface::VirtualShape> edges = cmd->edges();
            int count = edges.size();
            h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
            for (int i = 0; i < count; i++)
            {
                Interface::VirtualShape edge = edges[i];
                int edgeArray[3]{ edge.CmdId, edge.VirtualTopoId, edge.VirtualTopoIndex };
                h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edgeArray, 1, 3);
            }

            return true;
        }

        FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelOffsetFace)
        FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelShellFromSolid)
        FITKOCCAdaptorDef(FITKOCCAdaptorGeoModelSolidFromShell)
}