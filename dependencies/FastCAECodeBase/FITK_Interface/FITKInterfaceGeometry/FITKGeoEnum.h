/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_GEO_ENUM_H___
#define _FITK_GEO_ENUM_H___

#include <QObject>
#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
    * @brief 几何命令相关枚举
    * @author LiBaojun (libaojunqd@foxmail.com)
    * @date 2024-07-12
    */
    class FITKInterfaceGeometryAPI FITKGeoEnum : public QObject
    {
        Q_OBJECT
    public:
        /*
        * @brief 几何引擎枚举
        */
        enum FITKGeoEngine
        {
            FGENone = -1,   ///< None
            FGEOCC = 0,         ///< OpenCASCADE
            FGEOGG,         ///< OpenGeometry
            FGEPS,          ///< Parasolid
            FGEACIS,        ///< ACIS
            FGECGM,         ///< CGM
        };
        Q_ENUM(FITKGeoEngine);
        /**
         * @brief 虚拟拓扑类型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        enum VTopoShapeType
        {
            VSNone = 0,   ///< 无
            VSPoint,       ///< 点
            VSEdge,        ///< 线
            VSWire,        ///< 线环 多条线
            VSFace,        ///< 面
            VSShell,       ///< 壳 多个面组成
            VSSolid,       ///< 体
            VSLumpShell,   ///< 组成块的壳组
            VSLump,        ///< 块 多个区域
            VSAssembly,    ///< 组合

        };
        Q_ENUM(VTopoShapeType);

        /*
         * @brief   基准元素类型枚举。
         */
        enum FITKDatumType
        {
            FDTNone = -1,
            FDTPoint = 0,
            FDTCurveRadio = 1,
            FDTLine = 1000,
            FDTSegment = 1001,
            FDTLineByIntersectPlanes = 1002,
            FDTLineByRotateLine = 1003,
            FDTPlane = 2000,
            FDTOffsetPlane = 2001,
            FDTThreePointsPlane = 2002,
            FDTEquationPlane = 2003,
            FDTPointAndDirectionPlane = 2004,
            FDTRotateFromPlanePlane = 2005
        };
        Q_ENUM(FITKDatumType);

        /*
         * @brief   几何命令状态。
         */
        enum FITKGeoStatus
        {
            FGSNone = -1,   ///< None
            FGSNormal = 0,      ///< 常规
            FGSModified,    ///< 被修改
            FGSDeleted,     ///< 被移除
        };
        Q_ENUM(FITKGeoStatus);

        /*
        * @brief 几何命令枚举
        */
        enum FITKGeometryComType
        {
            FGTNone = -1,     ///< None
            FGTDelete = 0,        ///< 模型删除
            FGTMultiDelete,   ///< 多选删除
            FGTDatum,         ///< 基准元素（不可用于命令创建工厂）
            FGTSketch2D,      ///< 2D草图
            FGTConstruction,  ///< 构造线
            FGTImport,        ///< 导入文件
            FGTImportFlattenAssembly,   ///< 导入文件-平铺装配
            FGTExport,                  ///< 导出文件
            FGTExportTopos,             ///< 导出拓扑
            FGTImportedPart,              ///< 导入文件的几何数据
            FGTBox,           ///< 创建长方体
            FGTCylinder,      ///< 创建圆柱体
            FGTCone,          ///< 创建圆锥体
            FGTFrustum,       ///< 创建棱台
            FGTFilledRectangle,     ///< 创建填充矩形
            FGTFilledEllipse,       ///< 创建填充椭圆
            FGTFilledPolygon,       ///< 创建填充多边形
            FGTFilledParaboloid,       ///< 创建填充抛物面
            FGTSphere,        ///< 创建球体
            FGTBool,          ///< bool操作
            FGTNBodyUnite,      ///< bool操作-多实体合并
            FGTPoint,       ///< 点
            FGTCurveEnd,///< 曲线的端点
            FGTCurveEndDistance,///< 距曲线的端点指定距离
            FGTCurveRadio,///< 曲线指定比例
            FGTPointProjectionOnCurve,///< 点在线上的投影
            FGTTwoCurveIntersectPoint,///< 两条曲线交点
            FGTPointProjectionOnSurface,///< 点在面上的投影
            FGTCurveIntersectSurface,///< 曲线和面的交点
            FGTInteractionPoint, ///< 交互生成点
            FGTLine, ///< 无限直线
            FGTSegment,       ///< 有限线段
            FGTWire,        ///< 多段线
            FGTCircle,       ///< 圆
            FGTCircleArc,       ///< 圆弧
            FGTThreePointsCircle, ///< 三点构建圆
            FGTThreePointsEllipse, ///< 三点构建椭圆
            FGTThreePointsHyperbola, ///< 三点构建双曲线
            FGTThreePointsParabola, ///< 三点构建抛物线
            FGTCurveProjectionOnSurface,///< 线在面上的投影
            FGTSurfaceIntersectSurface,///< 面和面的交点
            FGTEllipse,       ///< 椭圆
            FGTEllipseArc,       ///< 椭圆弧
            FGTHyperbola,       ///< 双曲线
            FGTHyperbolaArc,       ///< 双曲线弧
            FGTParabola,       ///< 抛物线
            FGTParabolaArc,       ///< 抛物线弧
            FGTOffsetCurve, ///< 偏移线
            FGTBezierByControlPoints, ///< 通过控制点创建贝塞尔曲线
            FGTBezierByThroughPoints, ///< 通过曲线上的点点创建贝塞尔曲线
            FGTBSplineByControlPoints, ///< 通过控制点创建B样条曲线
            FGTBSplineByThroughPoints, ///< 通过曲线上的点创建B样条曲线
            FGTTrimmed,       ///< 裁剪曲线
            FGTBridgeCurve,     ///< 桥接曲线
            FGTCurveFromSurface, ///< 曲面的边线
            FGTClosedWireSurface,      ///< 封闭曲线形成的面
            FGTOffsetSurface,      ///< 偏移面
            FGTExtrudeSurface,      ///< 拉伸面
            FGTRevolSurface,      ///< 旋转面
            FGTSweepSurface,///< 扫略曲面
            FGTClosedSurfaceSolid,      ///< 封闭曲面形成的体
            FGTBridgeSurface, ///< 桥接曲面
            FGTSurfaceFromSolid, ///< 从体上提取面
            FGTMultiSectionSurface, ///< 多截面扫略面
            FGTExtrudeSolid,      ///< 拉伸体
            FGTRevolSolid,      ///< 旋转体
            FGTSweepSolid,///< 扫略体
            FGTMultiSectionSolid, ///< 多截面扫略体
            FGTExtrudeCut,      ///< 拉伸切削
            FGTRevolCut,      ///< 旋转切削
            FGTSweepCut,///< 扫略切削
            FGTMultiSectionCut, ///< 多截面扫略切削
            FGTChamferSolid,///< 倒角（体）
            FGTFilletSolid,///< 倒圆（体）
            FGTDefeature, ///< 移除特征（体）
            FGTRemoveChamfer,///< 移除倒角（体）
            FGTRemoveFillet,///< 移除倒圆（体）
            FGTFillHole,///< 填补孔洞（体）
            FGTMergeLine, ///合并线
            FGTSplitByMiddlePositionLine, ///线分割 基于中点分割
            FGTSplitByAngleLine, ///线分割 基于角度分割
            FGTDeleteFloatingEdge, ///删除悬浮边
            FGTRemoveWireEdges, ///移除线边
            FGTFillHolesFace,    ///填补孔洞（面）
            FGTFillGapsFace,     ///填补缝隙（面）
            FGTImprintMapPointToFace, ///点映射到面
            FGTImprintMapLineToFace,  ///线映射到面
            FGTImprintSolid,          ///压印体
            FGTCompoundAppendShape,      ///复合对象添加对象
            FGTCompoundRemoveShape,   ///复合对象移除对象
            FGTTransform,   ///通过向量平移
            FGTTransformByTwoPoints, /// 通过两点平移
            FGTTransformByDirAndDis, /// 通过方向和距离平移
            FGTRotate,   ///旋转
            FGTScale,   ///缩放
            FGTMirror,   ///镜像
            FGTPattern,   ///阵列
            FGTRectangularPattern,   ///矩阵缩放
            FGTCircularPattern,   ///环形缩放
            FGTCurveSplitter,   /// 曲线分割
            FGTSurfaceSplitter,   /// 曲面分割
            FGTSolidSplitter,   /// 体分割
            FGTStiffener,       /// 肋特征
            FGTCircularHole,    ///圆孔
            FGTPart,        /// 部件
            FGTBody,        /// 部件下实体
            FGTInstance,    /// 装配实例
            FGTRemoveFace,      ///移除面
            FGTExtendFace,      ///延伸面
            FGTRepairFace,     ///修复面
            FGTStitch, /// 缝合面
            FGTSampleCurve, ///< 曲线采样
            FGTNearestPointLine, ///< 最近点线
            FGTExtractCenter, ///< 提取中心
            FGTModelFromMesh, ///< 从网格模型创建
            FGTCoverEdges, ///覆盖面
            FGTRepairSmallEdges, ///< 修复小边
            FGTRepairSmallFaces, ///< 修复小面
            FGTMergeEdges,  ///< 合并边
            FGTWireEdgeFromEdge, ///< 根据边创建线边
            FGTPartitionEdgeWithParameter, ///< 参数分割边
            FGTPartitionEdgeWithPoint, ///< 点分割边
            FGTPartitionEdgeWithDatumPlane, ///< 基准面分割边
            FGTPartitionFaceWithSketch,///< 草图分割面
            FGTPartitionFaceWithTwoPoints,///< 两点分割面
            FGTPartitionFaceWithDatumPlane, ///< 基准面分割面
            FGTPartitionFaceWithCurvedPath, ///< 曲线路径分割面
            FGTPartitionFaceWithExtendFace, ///< 扩展面分割面
            FGTPartitionFaceWithIntersectFace, ///< 相交面分割面
            FGTPartitionFaceWithProjectEdges, ///< 投影边面分割面
            FGTPartitionSolidWithPlane,///< 平面分割体
            FGTPartitionSolidWithExtendFace,///< 扩展面分割体
            FGTPartitionSolidWithSweepEdge,///< 扫掠边分割体
            FGTPartitionSolidWithNSidedPatch,///< 补丁分割体
            FGTPartitionSolidWithSketchPlanar,///< 草绘分割体
            FGTSketchDeleter, ///< 草图删除命令
            FGTSketchFillet, ///< 草图倒角
            FGTReplaceFaces, ///< 替换面
            FGTVirtualTopoCombineFaces, ///< 虚拓扑合并面
            FGTVirtualTopoCombineEdges, ///< 虚拓扑合并边
            FGTModifyCylinder,        ///< 编辑圆柱面
            FGTTransformFace,           ///< 平移面
            FGTRotateFace,              ///< 旋转面
            FGTModifyBlend,             ///< 编辑倒圆面
            FGTMergePart,                ///< 合并Part（布尔运算）
            FGTTranslatePart,            ///< 平移Part
            FGTRotatePart,              ///< 旋转Part
            FGTScalePart,               ///< 缩放Part
            FGTSplitPart,               ///< 分割Part
            FGTMirrorPart,             ///< 镜像Part
            FGTRectangularPatternPart, ///< 线性阵列Part
            FGTCircularPatternPart,    ///< 环形阵列Part
            FGTFromSketch,             ///< 从草图创建模型
            FGTPlanarShell,             ///< 从草图创建平面壳模型
            FGTOffsetFace,                ///< 偏移面
            FGTSolidFromShell,            ///< 封闭壳面形成实体
            FGTShellFromSolid,            ///< 实体抽壳形成壳面

            // 用户定义
            FGTUserDefine_1 = 1001,
            FGTUserDefine_2 = 1002,
            FGTUserDefine_3 = 1003,
            FGTUserDefine_4 = 1004,
            FGTUserDefine_5 = 1005,
            FGTUserDefine_6 = 1006,
            FGTUserDefine_7 = 1007,
            FGTUserDefine_8 = 1008,
        };
        Q_ENUM(FITKGeometryComType);

        /*
         * @brief   构造线类型。
         */
        enum FITKConstructionType
        {
            FCTNone = -1,   ///< None
            FCTLine,        ///< 直线
            FCTCircle       ///< 圆形
        };
        Q_ENUM(FITKConstructionType);

        enum FITKFeatureType
        {
            None = 0,                           ///< None
            FeatureTypeChamfer = 1,        ///< 倒角
            FeatureTypeCutOut = 2,        ///< 裁剪
            FeatureTypeFillet = 4,        ///< 圆角
            FeatureTypeHole = 8,        ///< 孔洞
            FeatureTypeLogo = 16,       ///< logo
            FeatureTypeNotch = 32,       ///< 缺口
            FeatureTypePad = 64,       ///< 
            FeatureTypePocket = 128,
            FeatureTypeSlot = 256,
            FeatureTypeWallAndBend = 512,
            FeatureTypeAll = 1024,     ///< 所有
        };
        Q_ENUM(FITKFeatureType);

        /**
         * @brief       基准面类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-23
         */
        enum DatumPlnType
        {
            DPT_Principal_XOY = 0,
            DPT_Principal_YOZ,
            DPT_Principal_ZOX,
            DPT_UserDefine
        };

        /**
         * @brief       基准轴类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-23
         */
        enum DatumAxisType
        {
            DAT_Principal_X = 0,
            DAT_Principal_Y,
            DAT_Principal_Z,
            DAT_UserDefine
        };

        /**
         * @brief       平面位置类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        enum PlaneType
        {
            PT_XY = 0,      ///< XY平面
            PT_XZ,          ///< XZ平面
            PT_YZ,          ///< YZ平面
            PT_UserDefine   ///< 用户自定义平面
        };
        Q_ENUM(PlaneType);

    private:
        /**
         * @brief 私有构造函数
         */
        FITKGeoEnum() = default;
    };

}


#endif
