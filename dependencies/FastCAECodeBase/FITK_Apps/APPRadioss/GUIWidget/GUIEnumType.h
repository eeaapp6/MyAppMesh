/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __GUI_ENUM_TYPE_H___
#define __GUI_ENUM_TYPE_H___

namespace GUI
{
    enum PartTreeItemType
    {
        MITNone = 0,
        MITPartRoot = 1,  //PartRoot节点
        MITPart = 2,
        
    };

    enum PostTreeItemType
    {
        PITNone = 0,
        PITPost3DRoot = 1,     //Post3DRoot节点
          PITPost3D = 2,       //后处理3D数据节点
            PITStepRoot = 3,   //分析步根节点
              PITStep = 4,     //分析步节点
        PITPost2DRoot = 5,     //Post2DRoot节点
          PITPost2D = 6,       //后处理2D数据节点
            PITXYPlot = 7,     //XY图
    };

    enum TreeAdditionalInfo
    {
        TAIDataObjID = 256,  //当前数据id
        TAICaseID,           //dataCase的id
        TAIPartID,           //Part的id
        TAIModelID,          //AssemblyMgr的Model的id  AssemblyMgr->getDataByIndex(iIns);
        TAIMatrialID,        //Matrial的ID
        TAIPropertyID,       //Property的ID
        TAICurveID,
        TAIRigidWallID,
        TAISetID,
        TAISurfID,
        TAIPostID,           //Post的ID
        TAIStepIndex,        //PostStep的Index
    };

    enum TreeWidgetAssemblyType
    {
        TWAssemblyNone = 0,
        TWAssemblyGeometry = 1,
        TWAssemblyGeometryChild = 11,
        TWAssemblyGeometryChildBool = 111,
        TWAssemblyPart = 2,
        TWAssemblyPartChild = 21,
    };

    enum TreeWidgetGroupType
    {
        TWGroupNone = 0,
        TWGroupSet = 1,
        TWGroupSetChild = 11,
        TWGroupSurface = 2,
        TWGroupSurfaceChild = 21,
    };

    enum TreeWidgetPropertyType
    {
        TWPropertyNone = 0,
        TWPropertyMaterial = 1,
        TWPropertyMaterialChild = 11,
        TWPropertyProperty = 2,
        TWPropertyPropertyChild = 21,
        TWPropertyCurve = 3,
        TWPropertyCurveChild = 31,
        TWPropertyEOS = 4,
        TWPropertyEOSChild = 41,
        TWPropertyFailureModel = 5,
        TWPropertyFailureModelChild = 51,
    };

    enum TreeWidgetSolutionType
    {
        TWSolutionNone = 0,
        TWSolutionRoot = 1,
        TWSolutionChild = 2,
        TWSolutionChildSetting = 21,
        TWSolutionChildSettingSolverSetting = 211,
        TWSolutionChildSettingResultRequest = 212,
        TWSolutionChildInteraction = 22,
        TWSolutionChildInteractionKinematicTied = 221,
        TWSolutionChildInteractionMultiUsageImpact = 222,
        TWSolutionChildInitialCondition = 23,
        TWSolutionChildInitialConditionInitialField = 231,
        TWSolutionChildLoad = 24,
        TWSolutionChildConstraint = 25,
        TWSolutionChildConstraintRigidWall = 251,
        TWSolutionChildConstraintGravity = 252,
        TWSolutionChildBoundary = 26,
        TWSolutionChildBoundaryBCS = 261,
        TWSolutionChildConnection = 27,
        TWSolutionChildConnectionRBody = 271,
        TWSolutionChildProbeAccelRoot = 28,
        TWSolutionChildProbeAccel = 281,
        TWSolutionChildProbeSectionRoot = 29,
        TWSolutionChildProbeSection = 291,
    };

    enum ModelElementType
    {
        METNone = 0,
        METPoint = 1,
        METLine = 2,
        METSurface = 3,
        METMeshVert = 4,
        METMeshEdge = 5,
        METMeshFace = 6,
    };
}
#endif  // __GUI_ENUM_TYPE_H___