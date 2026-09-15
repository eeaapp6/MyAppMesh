/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKOCCGeomTools.h
 * @brief OCC 几何工具类实现
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-08-18
 * 
 */

#ifndef _FITKOCC_GEOM_TOOLS_H___
#define _FITKOCC_GEOM_TOOLS_H___
 
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"

class TopoDS_Shape;
class gp_Ax1;
class gp_Ax3;

namespace OCC
{
    /**
     * @brief OCC工具生成器，注册到Interface层geomToolsCreator中
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-08-18
     */
    class FITKOCCToolCreator : public Interface::FITKAbstractGeomToolsCreator
    {
    public:
        /**
         * @brief       Get the OCC TopoDS_Shape of the virtual topo object.[static]
         * @param[in]   vtp: The virtual topo object
         * @param[in]   sType: The needly shape type
         * @return      The OCC TopoDS_Shape
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-12
         */
        static const TopoDS_Shape GetVirtualTopoShape(Interface::FITKAbsVirtualTopo* vtp, Interface::FITKGeoEnum::VTopoShapeType sType);

        /**
         * @brief       Get the OCC TopoDS_Shape of the virtual topo object.[static]
         * @param[in]   vtp: The virtual topo object
         * @return      The OCC TopoDS_Shape
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-12
         */
        static const TopoDS_Shape GetVirtualTopoShape(Interface::FITKAbsVirtualTopo* vtp);

        /**
         * @brief       比较是否相同。[静态]
         * @param[in]   first：第一个对象[引用]
         * @param[in]   second：第二个对象[引用]
         * @return      是否相同
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-12
         */
        static bool IsEqual(const gp_Ax1 & first, const gp_Ax1 & second);

        /**
         * @brief       比较是否相同。[静态]
         * @param[in]   first：第一个对象[引用]
         * @param[in]   second：第二个对象[引用]
         * @return      是否相同
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-12
         */
        static bool IsEqual(const gp_Ax3 & first, const gp_Ax3 & second);

        /**
         * @brief Construct a new FITKOCCToolCreator object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         */
        explicit FITKOCCToolCreator() = default;
        /**
         * @brief Destroy the FITKOCCToolCreator object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         */
        virtual ~FITKOCCToolCreator() = default;
        /**
         * @brief 创建点工具类
         * @return Interface::FITKAbstractGeomPointTool* 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         */
        Interface::FITKAbstractGeomPointTool* createPointTool() override;

        /**
         * @brief       创建边查询工具[重写]
         * @return      边查询工具
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        Interface::FITKAbstractGeomEdgeTool* createEdgeTool() override;

        /**
         * @brief       创建平面查询工具。[重写]
         * @return      平面查询工具
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        Interface::FITKAbstractGeomFaceTool* createFaceTool() override;

        /**
         * @brief       创建通用查询工具。[重写]
         * @return      通用查询工具
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-15
         */
        Interface::FITKAbstractGeomCommonTool* createCommonTool() override;

    };

    /**
     * @brief 点工具类实现
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-08-18
     */
    class FITKOCCPointTool : public Interface::FITKAbstractGeomPointTool
    {
    public:
        /**
         * @brief Construct a new FITKOCCPointTool object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         */
        explicit FITKOCCPointTool() = default;
        /**
         * @brief Destroy the FITKOCCPointTool object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         */
        virtual ~FITKOCCPointTool() = default;

        /**
         * @brief 获取点坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         * @param vtp 虚拓扑对象
         * @param xyz 点的坐标
         * @return true 成功
         * @return false 失败
         */
        bool getXYZ(Interface::FITKAbsVirtualTopo* vtp, double* xyz, int iModelId = -1) override;
    };

    /**
     * @brief       OCC边查询工具。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-27
     */
    class FITKOCCEdgeTool : public Interface::FITKAbstractGeomEdgeTool
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        explicit FITKOCCEdgeTool() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        virtual ~FITKOCCEdgeTool() = default;

        /**
         * @brief       获取线起始点方向。[重写]
         * @param[in]   vtp：虚拓扑对象
         * @param[out]  oPos：返回坐标信息
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        bool getStartPointDirection(Interface::FITKAbsVirtualTopo* vtp, double* oPos, double* oDir, int iModelId = -1) override;

        /**
         * @brief       获取线终止点方向。[重写]
         * @param[in]   vtp：虚拓扑对象
         * @param[out]  oPos：返回坐标信息
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        bool getEndPointDirection(Interface::FITKAbsVirtualTopo* vtp, double* oPos, double* oDir, int iModelId = -1) override;

        /**
         * @brief       获取线上某一点方向。[重写]
         * @param[in]   vtp：虚拓扑对象
         * @param[in]   iPos：输入坐标
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        bool getDirection(Interface::FITKAbsVirtualTopo* vtp, double* iPos, double* oDir, int iModelId = -1) override;

        /**
         * @brief       获取半边上某位置方向。[重写]
         * @param[in]   vtpFace：面虚拓扑对象
         * @param[in]   vtpEdge：边虚拓扑对象
         * @param[in]   iPos：边上位置
         * @param[out]  oDir：返回方向
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-12
         */
        bool getCoedgeDirection(Interface::FITKAbsVirtualTopo* vtpFace, Interface::FITKAbsVirtualTopo* vtpEdge,
            double* iPos, double* oDir, int iModelId = -1) override;

        /**
         * @brief       获取面上边界线法向。[重写]
         * @param[in]   vtpFace：面虚拓扑对象
         * @param[in]   vtpEdge：面上边虚拓扑对象
         * @param[in]   iPos：输入坐标信息
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-20
         */
        bool getBoundaryNormal(Interface::FITKAbsVirtualTopo* vtpFace, Interface::FITKAbsVirtualTopo* vtpEdge,
            double* iPos, double* oDir, int iModelId = -1) override;

        /**
         * @brief       判断给定坐标是否在线上。[重写]
         * @param[in]   vtp：虚拓扑对象
         * @param[in]   iPos：输入坐标
         * @return      坐标是否在线上
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-07
         */
        bool isPosOnEdge(Interface::FITKAbsVirtualTopo* vtp, double* iPos) override;

        /**
         * @brief       查询当前对象是否为直线。[重写]
         * @param[in]   vtp：线虚拓扑
         * @return      是否为直线
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        bool isLine(Interface::FITKAbsVirtualTopo* vtp) override;

        /**
         * @brief       查询当前对象长度。[重写]
         * @param[in]   vtp：线虚拓扑
         * @param[in]   oLen：线长[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        bool getLength(Interface::FITKAbsVirtualTopo* vtp, double & oLen) override;

        /**
         * @brief       获取中点坐标。[重写]
         * @param[in]   vtp：虚拓扑对象
         * @param[out]  oPos：返回坐标信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-02
         */
        bool getMidPoint(Interface::FITKAbsVirtualTopo* vtp, double* oPos, int iModelId = -1) override;

        /**
         * @brief       获取边中点坐标与方向。[重写]
         * @param[in]   vtp：虚拓扑对象
         * @param[in]   oPos：返回坐标信息
         * @param[in]   oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        bool getMidPointDirection(Interface::FITKAbsVirtualTopo* vtp, double* oPos, double* oDir, int iModelId = -1) override;

        /**
         * @brief       获取边上特征点坐标。[重写]
         * @param[in]   vtp：虚拓扑对象
         * @param[out]  oPoses：返回坐标列表[引用]
         * @param[in]   skipMidPt：是否跳过中点[缺省]
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-02
         */
        bool getFeaturePoints(Interface::FITKAbsVirtualTopo* vtp, QList<QList<double>> & oPoses, bool skipMidPt = false, int iModelId = -1) override;

        /**
         * @brief       根据参数获取边上对应点的坐标。[重写]
         * @param[in]   vtp：虚拓扑对象
         * @param[in]   iParam：输入参数值
         * @param[out]  oPos：返回点坐标
         * @param[in]   iNormalized：参数值是否归一化（0. ~ 1.）
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @data        2025-05-12
         */
        bool evalPoint(Interface::FITKAbsVirtualTopo* vtp, double iParam, double* oPos, bool iNormalized = false, int iModelId = -1) override;
        /**
         * @brief   获取当前对象的所有自由边ID。[重写]
         * @param   commandId：命令ID
         * @return  自由边ID列表
         * @author  libaojun
         * @date    2025-05-13
         */
        virtual QList<int> getFreeEdgeIDs(int commandId) override;

        /**
         * @brief       获取几何中在给定平面的拓扑边。[重写]
         * @param[in]   cmdId：命令ID
         * @param[in]   iOrg：平面中心
         * @param[in]   iNor：平面法向
         * @return      拓扑列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-23
         */
        QList<int> getEdgesOnPlane(int cmdId, double* iOrg, double* iNor) override;

    };

    /**
     * @brief       OCC面查询工具。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-05
     */
    class FITKOCCFaceTool : public Interface::FITKAbstractGeomFaceTool
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        explicit FITKOCCFaceTool() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        virtual ~FITKOCCFaceTool() = default;

        /**
         * @brief       获取面积。[重写]
         * @param[in]   vtp：面虚拓扑
         * @param[out]  oArea：面积[引用]
         * @return      是否为有效平面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-26
         */
        bool getArea(Interface::FITKAbsVirtualTopo* vtp, double & oArea) override;

        /**
         * @brief       获取曲面上任意位置平均曲率。[重写]
         * @param[in]   vtp：面虚拓扑
         * @param[in]   iPos：面虚拓扑ID
         * @param[out]  oCur：平均曲率[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-04
         */
        bool getMeanCurvatureAtPos(Interface::FITKAbsVirtualTopo* vtp, double* iPos, double & oCur) override;

        /**
         * @brief       获取曲面上任意位置平均曲率。[重写]
         * @param[in]   vtp：面虚拓扑
         * @param[in]   iPos：面虚拓扑ID
         * @param[out]  oCur1：曲率1[引用]
         * @param[out]  oCur2：曲率2[引用]
         * @param[out]  oDir1：曲率1方向[引用]
         * @param[out]  oDir2：曲率2方向[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-04
         */
        bool getCurvatureAtPos(Interface::FITKAbsVirtualTopo* vtp, double* iPos,
            double & oCur1, double & oCur2, double* oDir1, double* oDir2) override;

        /**
         * @brief       获取查询信息。[重写]
         * @param[in]   vtp：面虚拓扑（必须为平面）
         * @param[out]  oPos：面中心点坐标
         * @param[out]  oNor：面法向
         * @param[out]  oUp：面向上方向
         * @param[in]   iModelId：模型ID[缺省]
         * @return      是否为有效平面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        bool getPlane(Interface::FITKAbsVirtualTopo* vtp, double* oPos, double* oNor, double* oUp, int iModelId = -1) override;

        /**
         * @brief       获取面上中点位置与法向。[重写]
         * @param[in]   id：面虚拓扑ID[引用]
         * @param[out]  oPos：面中点位置
         * @param[out]  oNormal：法向
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        bool getNormal(Interface::FITKAbsVirtualTopo* vtp, double* oPos, double* oNormal, int iModelId = -1) override;

        /**
         * @brief       获取面上指定位置法向。[重写]
         * @param[in]   id：面虚拓扑ID[引用]
         * @param[in]   iPos：面上任意坐标
         * @param[out]  oNormal：法向
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        bool getNormalAtPos(Interface::FITKAbsVirtualTopo* vtp, double* iPos, double* oNormal, int iModelId = -1) override;

        /**
         * @brief       查询当前对象是否为平面。[重写]
         * @param[in]   vtp：面虚拓扑
         * @return      是否为平面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        bool isPlane(Interface::FITKAbsVirtualTopo* vtp) override;

        /**
         * @brief       查询当前对象是否为圆柱面。[重写]
         * @param[in]   vtp：面虚拓扑
         * @return      是否为圆柱面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        bool isCylind(Interface::FITKAbsVirtualTopo* vtp) override;

        /**
         * @brief       查询当前对象是否为圆柱面。[重写]
         * @param[in]   vtp：面虚拓扑
         * @param[out]  oCt：圆柱面中心
         * @param[out]  oDir：圆柱面轴线方向
         * @return      是否为圆柱面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        bool getCylindSurf(Interface::FITKAbsVirtualTopo* vtp, double* oCt, double* oDir) override;

        /**
         * @brief       查询当前对象是否为圆锥面。[重写]
         * @param[in]   vtp：面虚拓扑
         * @return      是否为圆锥面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-10
         */
        bool isCone(Interface::FITKAbsVirtualTopo* vtp) override;

        /**
         * @brief       查询当前对象是否为圆锥面。[重写]
         * @param[in]   vtp：面虚拓扑
         * @param[out]  oCt：圆锥面中心
         * @param[out]  oDir：圆锥面轴线方向
         * @return      是否为圆锥面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-10
         */
        bool getConeSurf(Interface::FITKAbsVirtualTopo* vtp, double* oCt, double* oDir) override;

        /**
         * @brief       Get wether the given topos are all plane and coplanar.[override]
         * @param[in]   ids: The topos id
         * @return      Are the topos all plane and coplanar
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-12
         */
        bool isCoplanar(QList<int> ids) override;

        /**
         * @brief       Get wether the given topos are all faces and on the same geometry surface.[override]
         * @param[in]   ids: The topos id
         * @return      Are the topos all faces and on the same geometry surface
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-12
         */
        bool isSameSurface(QList<int> ids) override;

    };     

    /**
     * @brief       OCC通用几何查询工具.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-05-12
     */
    class FITKOCCCommonTool : public Interface::FITKAbstractGeomCommonTool
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-12
         */
        explicit FITKOCCCommonTool() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-12
         */
        virtual ~FITKOCCCommonTool() = default;

        /**
         * @brief       获取两个形状间距离与位置。[重写]
         * @param[in]   vtp_1：形状1虚拓扑对象
         * @param[in]   vtp_2：形状2虚拓扑对象
         * @param[out]  oDist：形状距离[引用]
         * @param[out]  oPos_1：形状1位置
         * @param[out]  oPos_2：形状2位置
         * @param[in]   iModelId_1：形状1模型ID[缺省]
         * @param[in]   iModelId_2：形状2模型ID[缺省]
         * @return      是否可计算出距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-12
         */
        bool getDistanceByTopos(Interface::FITKAbsVirtualTopo* vtp_1, Interface::FITKAbsVirtualTopo* vtp_2,
            double & oDist, double oPos_1[3], double oPos_2[3], int iModelId_1 = 1, int iModelId_2 = 1) override;

        /**
         * @brief       获取形状与到某一点距离与极值坐标。[重写]
         * @param[in]   vtp：形状虚拓扑对象
         * @param[in]   iPos3：给定坐标
         * @param[out]  oDist：形状距离[引用]
         * @param[out]  oPos：形状位置
         * @param[in]   iModelId：形状模型ID[缺省]
         * @return      是否可计算出距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-12
         */
        bool getDistanceByTopoAndPos(Interface::FITKAbsVirtualTopo* vtp, double iPos3[3],
            double & oDist, double oPos[3], int iModelId = 1) override;

        /**
         * @brief       获取拓扑边与给定直线距离。[重写]
         * @param[in]   vtp：虚拓扑边对象
         * @param[in]   iPos1：直线坐标1
         * @param[in]   iPos2：直线坐标2
         * @param[out]  oDist：距离[引用]
         * @param[out]  oPos：交点坐标
         * @return      是否存在有效距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-06
         */
        bool getCrossByEdgeAndLine(Interface::FITKAbsVirtualTopo* vtp, double iPos1[3], double iPos2[3],
            double oPos[3]) override;

        /**
         * @brief       获取几何引擎是否支持查询无效拓扑与不精确拓扑。[重写]
         * @param[out]  oInvalid：是否可查询无效拓扑[引用]
         * @param[out]  oImprecise：是否可查询不精确拓扑[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-09
         */
        void canCheckTopos(bool & oInvalid, bool & oImprecise) override;

        /**
         * @brief       获取几何命令中的无效实体拓扑。[重写]
         * @param[in]   cmdId：几何命令ID
         * @param[out]  oTopos：无效拓扑信息[引用]
         * @param[in]   warnings：是否查询警告信息[缺省]
         * @return      返回结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-17
         */
        bool getInvalidTopos(int cmdId, QList<Interface::VirtualShape> & oTopos, bool warnings = false) override;

        /**
         * @brief       获取几何命令中的不精确拓扑。[重写]
         * @param[in]   cmdId：几何命令ID
         * @param[out]  oTopos：不精确拓扑信息[引用]
         * @return      返回结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-17
         */
        bool getImpreciseTopos(int cmdId, QList<Interface::VirtualShape> & oTopos) override;

        /**
         * @brief       三点创建圆。
         * @param[in]   pt1：第一个点
         * @param[in]   pt2：第二个点
         * @param[in]   pt3：第三个点
         * @param[out]  center：圆心位置
         * @param[out]  normal：圆所在平面法线
         * @param[out]  radius：圆半径
         * @return      是否可创建圆
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-03
         */
        bool calcCircleInfoBy3Points(QVector<double> pt1, QVector<double> pt2, QVector<double> pt3,
            QVector<double> & center, QVector<double> & normal, double & radius) override;

        /**
         * @brief       获取给定虚拓扑面的采样点。[重写]
         * @param[in]   vtp：虚拓扑对象
         * @param[out]  oPoses：采样点坐标[引用]
         * @param[in]   spacing：预估采样点间隔
         * @param[in]   iModelId：模型ID[缺省]
         * @return      是否存在有效采样点
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-23
         */
        bool getFaceSamplePoints(Interface::FITKAbsVirtualTopo* vtp, QList<QList<double>> & oPoses, int iSpacing = 5, int iModelId = -1) override;

    };
}

#endif
