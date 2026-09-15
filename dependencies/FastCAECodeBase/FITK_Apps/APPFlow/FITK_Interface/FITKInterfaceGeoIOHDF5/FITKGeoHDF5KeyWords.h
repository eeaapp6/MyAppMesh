/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKGeoHDF5KeyWords.h
 * @brief HDF5几何数据关键字定义
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2025-04-16
 */
#ifndef __FITKGEO_HDF5_KEYWORDS_H__
#define __FITKGEO_HDF5_KEYWORDS_H__


#define FGKW_DataObjectID          "DataObjectID" // 数据对象ID
#define FGKW_DataObjectName        "DataObjectName" // 数据对象名称
#define FGKW_DataObjectEnable      "IsEnable" // 数据对象是否启用
#define FGKW_DataObjectParentID    "ParentDataObjectID" // 数据对象父ID
#define FGKW_Geometry              "Geometry" // 几何

#define FGKW_CommandType           "GeometoryCommandType" // 几何命令类型
#define FGKW_CommandStatus         "CommandStatus" // 命令状态
#define FGKW_EnableRefList         "EnableReferenceList" // 是否启用引用列表
#define FGKW_RefCommandCount       "ReferenceCmdCount" // 引用命令数量
#define FGKW_VirtualTopoMgrID      "VirtualTopoManagerRootID" // 虚拟拓扑管理器ID
#define FGKW_ModifiedCmdID         "ModifiedCmdID" // 修改命令ID

#define FGKW_RefCommandID          "ReferenceCmdID" // 引用命令ID
#define FGKW_RefCommandType        "ReferenceCmdType" // 引用命令类型

#define FGKW_Parameter             "Parameter" // 参数组
#define FGKW_Point                 "Point"    // 参数点
#define FGKW_Length                "Length"    // 参数长度、
#define FGKW_Location              "Location"    // 圆心坐标
#define FGKW_Radius                "Radius"    // 半径
#define FGKW_Direction             "Direction"    // 轴线方向

#define FGKW_ReferenceCmdID        "ReferenceCmdID"    // 引用命令ID
#define FGKW_ReferenceCmdType      "ReferenceCmdType"    // 引用命令类型

#define FGKW_vTopoShapeManager     "vTopoShapeManager"    // 引用命令类型
#define FGKW_Count                 "Count"    // 数量
#define FGKW_DataObject            "_DataObject"    // 数据对象
#define FGKW_ChildVTopo            "_ChildVTopo"    // 虚拓扑

#define FGKW_Shape                 "Shape"    //  参数形状
#define FGKW_ShapeType             "ShapeType"    //  参数形状类型
#define FGKW_ShapeDataSize         "Len" // 形状数据大小

#define FGKW_CommandProp           "CommandProp" // 几何命令属性
#define FGKW_Number                "Number" // 数字属性
#define FGKW_Color                 "Color" // 颜色属性

#define FGKW_Type                  "Type" // 类型
#define FGKW_Target                "Target" // 目标模型
#define FGKW_ToolArray             "ToolArray" // 工具模型

#define FGKW_Point                 "Point" // 点
#define FGKW_SourceCurve           "SourceCurve" // 源曲线
#define FGKW_SourceCurve1          "SourceCurve1" // 源曲线1
#define FGKW_SourceCurve2          "SourceCurve2" // 源曲线2
#define FGKW_SourcePoint           "SourcePoint" // 源点
#define FGKW_SourceSurface         "SourceSurface" // 源曲面
#define FGKW_SourceSurface1        "SourceSurface1" // 源曲面1
#define FGKW_SourceSurface2        "SourceSurface2" // 源曲面2
#define FGKW_SourceSolid           "SourceSolid" // 源曲面2
#define FGKW_UseStartEnd           "UseStartEnd" // 
#define FGKW_Distance              "Distance" // 距离
#define FGKW_Radio                 "Radio" // 距离

#define FGKW_SourceShape           "SourceShape" //源形状
#define FGKW_IsCopy                "IsCopy" //是否拷贝
#define FGKW_Vector                "Vector" //向量
#define FGKW_SPoint                "SPoint" //起始点
#define FGKW_EPoint                "EPoint" //起始点
#define FGKW_Direction             "Direction" //方向
#define FGKW_Distance              "Distance" //距离
#define FGKW_Degree                "Degree" //角度
#define FGKW_Radian                "Radian" //弧度
#define FGKW_Factors               "Factors" //缩放因子
#define FGKW_BasePoint             "BasePoint" //缩放基准点
#define FGKW_ReferenceShape        "ReferenceShape" //参考图形
#define FGKW_FirstDirection        "FirstDirection" //第一方向
#define FGKW_FirstCount            "FirstCount" //第一方向上的数量
#define FGKW_FirstSpacing          "FirstSpacing" //第一方向上的间距
#define FGKW_FirstTotal            "FirstTotal" //第一方向上的总距离
#define FGKW_SecondDirection       "SecondDirection" //第二方向
#define FGKW_SecondCount           "SecondCount" //第二方向上的数量
#define FGKW_SecondSpacing         "SecondSpacing" //第二方向上的间距
#define FGKW_SecondTotal           "SecondTotal" //第二方向上的总距离
#define FGKW_AxisShape             "AxisShape" //镜像轴
#define FGKW_isReverse             "isReverse" //是否反转
#define FGKW_AxisCount             "AxisCount" //环向上的数量
#define FGKW_AxisSpacing           "AxisSpacing" //环向上的间距
#define FGKW_AxisTotal             "AxisTotal" //环向上的总距离
#define FGKW_CrownCount            "CrownCount" //径向上的数量
#define FGKW_CrownSpacing          "CrownSpacing" //径向上的间距
#define FGKW_CrownTotal            "CrownTotal" //径向上的总距离
#define FGKW_FirstDefinition       "FirstDefinition" //第一方向定义
#define FGKW_SecondDefinition      "SecondDefinition" //第二方向定义

#define FGKW_DeleteCommandID       "DeleteCommandID" //移除命令ID
#define FGKW_DeleteCommandIDs      "DeleteCommandIDs" //移除命令ID列表
#define FGKW_Tolerance             "Tolerance" //公差
#define FGKW_FileName              "FileName" //文件名字
#define FGKW_InputCmd              "InputCmd" //复合数据
#define FGKW_InputCmdID            "InputCmdID" //复合数据ID

#define FGKW_Origin                "Origin" //源点
#define FGKW_XDirection            "XDirection" //X轴方向
#define FGKW_YDirection            "YDirection" //X轴方向
#define FGKW_ZDirection            "ZDirection" //X轴方向
#define FGKW_MajorPoint            "MajorPoint" //长轴点
#define FGKW_MinorPoint            "MinorPoint" //短轴点
#define FGKW_MajorRadius           "MajorRadius" //短轴半径
#define FGKW_MinorRadius           "MinorRadius" //短轴半径
#define FGKW_FocalLength           "FocalLength" //焦距
#define FGKW_EdgeVirtualTopoId     "EdgeVirtualTopoId" //边线ID
#define FGKW_EdgeVirtualTopoId1    "EdgeVirtualTopoId1" //边线ID1
#define FGKW_EdgeVirtualTopoId2    "EdgeVirtualTopoId2" //边线ID2
#define FGKW_SourceCurveId         "SourceCurveId" //曲线ID
#define FGKW_FaceVirtualTopoId     "FaceVirtualTopoId" //曲线ID
#define FGKW_StartTrimmed          "StartTrimmed" //开始裁剪
#define FGKW_EndTrimmed            "EndTrimmed" //结束裁剪
#define FGKW_UseStartEnd1          "UseStartEnd1" //开始结束1
#define FGKW_UseStartEnd2          "UseStartEnd2" //开始结束2
#define FGKW_Parameter1            "Parameter1" //参数1
#define FGKW_Parameter2            "Parameter2" //参数2
#define FGKW_Angle                 "Angle" //角度
#define FGKW_Dir                   "Dir" //角度
#define FGKW_StartPoint            "StartPoint" //开始点
#define FGKW_EndPoint              "EndPoint" //结束点
#define FGKW_Offset                "Offset" //偏移
#define FGKW_cmdID                 "CmdID" //命令ID
#define FGKW_vTopoID               "VirtualTopoID" //虚拓扑ID
#define FGKW_vTopoIndex            "VirtualTopoIndex" //虚拓扑索引
#define FGKW_ThroughPointShapesCount            "ThroughPointShapesCount" //形状（点）数量
#define FGKW_ThroughPointShapes    "ThroughPointShapes" //形状（点）
#define FGKW_ExtrudeSurface        "ExtrudeSurface" //拉伸面
#define FGKW_RotateAxisPoint1      "RotateAxisPoint1" //旋转轴点1
#define FGKW_RotateAxisPoint2      "RotateAxisPoint2" //旋转轴点2
#define FGKW_Profile               "Profile" //截面曲线
#define FGKW_ProfileShape          "ProfileShape" //截面曲线形状
#define FGKW_Curve                 "Curve" //曲线
#define FGKW_Thickness1            "Thickness1" //方向1的厚度
#define FGKW_Thickness2            "Thickness2" //方向2的厚度
#define FGKW_NearestPoint          "NearestPoint" //最近点
#define FGKW_SampleMethod          "SampleMethod" //采样方法
#define FGKW_SampleCurve           "SampleCurve" //采样曲线
#define FGKW_SampleCount           "SampleCount" //采样数量
#define FGKW_vFace                 "vFace" //虚形状（面）
#define FGKW_Childs                "Childs" //子节点
#define FGKW_Center                "Center" //子节点
#define FGKW_Position              "Position" //基准元素位置
#define FGKW_Normal                "Normal" //基准面法向
#define FGKW_Up                    "Up" //基准面向上方向
#define FGKW_ToolShape             "ToolShape" //工具形状
#define FGKW_ToolPoint             "ToolPoint" //工具点
#define FGKW_Position              "Position" //位置点
#define FGKW_Position1             "Position1" //位置点1
#define FGKW_Position2             "Position2" //位置点2
#define FGKW_A                     "A" //方程A
#define FGKW_B                     "B" //方程B
#define FGKW_C                     "C" //方程C
#define FGKW_D                     "D" //方程D

#define FGKW_NodeCount             "NodeCount" //节点数量
#define FGKW_TriangleCount         "TriangleCount" //三角形网格数量
#define FGKW_TriangleNodes         "TriangleNodes" //三角形网格节点
#define FGKW_NodeIDs               "NodeIDs" //节点ID
#define FGKW_NodeCoordinates       "NodeCoordinates"//节点坐标
#define FGKW_TriangleNormals       "TriangleNormals"//三角形法向量

#endif // __FITKGEO_HDF5_KEYWORDS_H__
