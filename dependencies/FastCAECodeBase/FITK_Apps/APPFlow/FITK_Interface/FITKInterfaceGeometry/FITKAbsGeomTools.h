/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbsGeomTools.h
 * @brief 抽象查询类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-08-18
 *
 */
#ifndef _FITK_ABSTRACT_GEOM_TOOLS_H___
#define _FITK_ABSTRACT_GEOM_TOOLS_H___ 

#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp"
#include "FITKGeoEnum.h"
#include "FITKInterfaceGeometryAPI.h"

 // 前置声明
namespace Interface
{
    class VirtualShape;
}

namespace Interface
{
    class FITKAbsVirtualTopo;

    /**
     * @brief 点查询类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-08-18
     */
    class FITKInterfaceGeometryAPI FITKAbstractGeomPointTool :
        public Core::FITKAbstractObject
    {
    public:
        /**
         * @brief Construct a new FITKAbstractGeomPointTool object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         */
        explicit FITKAbstractGeomPointTool() = default;
        /**
         * @brief Destroy the FITKAbstractGeomPointTool object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         */
        virtual ~FITKAbstractGeomPointTool() = default;
        /**
         * @brief 创建点查询类
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         * @return FITKAbstractGeomPointTool*
         */
        static FITKAbstractGeomPointTool* createTool();
        /**
         * @brief 获取点坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         * @param id 虚拓扑的id
         * @param oXyz 点的坐标
         * @return true 成功
         * @return false 失败
         */
        virtual bool getXYZByID(const int & id, double* oXyz, int modelId = -1);
        /**
         * @brief 获取点坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         * @param vtp 虚拓扑对象
         * @param oXyz 点的坐标
         * @return true 成功
         * @return false 失败
         */
        virtual bool getXYZ(FITKAbsVirtualTopo* vtp, double* oXyz, int modelId = -1)
        {
            Q_UNUSED(modelId);
            Q_UNUSED(vtp);
            Q_UNUSED(oXyz);
            return false;
        };

    };

    /**
     * @brief       线查询工具。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-12-03
     */
    class FITKInterfaceGeometryAPI FITKAbstractGeomEdgeTool :
        public Core::FITKAbstractObject
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-03
         */
        explicit FITKAbstractGeomEdgeTool() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-03
         */
        virtual ~FITKAbstractGeomEdgeTool() = default;

        /**
         * @brief       创建线信息查询工具。[静态]
         * @return      线信息查询工具
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-03
         */
        static FITKAbstractGeomEdgeTool* createTool();

        /**
         * @brief       获取方向。[虚函数]
         * @param[in]   id：虚拓扑ID
         * @param[out]  oPos：返回坐标信息
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-03
         */
        virtual bool getStartPointDirectionByID(const int & id, double* oPos, double* oDir, int iModelId = -1);

        /**
         * @brief       获取方向。[虚函数]
         * @param[in]   vtp：虚拓扑对象
         * @param[out]  oPos：返回坐标信息
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-11
         */
        virtual bool getStartPointDirection(FITKAbsVirtualTopo* vtp, double* oPos, double* oDir, int iModelId = -1)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oPos);
            Q_UNUSED(oDir);
            Q_UNUSED(iModelId);
            return false;
        };

        /**
         * @brief       获取方向。[虚函数]
         * @param[in]   id：虚拓扑ID
         * @paramp[out] oPos：返回坐标信息
         * @paramp[out] oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-03
         */
        virtual bool getEndPointDirectionByID(const int & id, double* oPos, double* oDir, int iModelId = -1);

        /**
         * @brief       获取方向。[虚函数]
         * @param[in]   vtp：虚拓扑对象
         * @param[out]  oPos：返回坐标信息
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-11
         */
        virtual bool getEndPointDirection(FITKAbsVirtualTopo* vtp, double* oPos, double* oDir, int iModelId = -1)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oPos);
            Q_UNUSED(oDir);
            Q_UNUSED(iModelId);
            return false;
        };

        /**
         * @brief       获取给定坐标切向。[虚函数]
         * @param[in]   id：虚拓扑ID
         * @param[in]   iPos：输入坐标
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-03
         */
        virtual bool getDirectionByID(const int & id, double* iPos, double* oDir, int iModelId = -1);

        /**
         * @brief       获取给定坐标切向。[虚函数]
         * @param[in]   vtp：虚拓扑对象
         * @param[in]   iPos：输入坐标
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-03
         */
        virtual bool getDirection(FITKAbsVirtualTopo* vtp, double* iPos, double* oDir, int iModelId = -1)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(iPos);
            Q_UNUSED(oDir);
            Q_UNUSED(iModelId);
            return false;
        };

        /**
         * @brief       获取面上半边的给定坐标坐标切向。[虚函数]
         * @param[in]   faceId：面虚拓扑ID
         * @param[in]   edgeId：面上边虚拓扑ID
         * @param[in]   iPos：输入坐标
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-08
         */
        virtual bool getCoedgeDirectionByID(const int & faceId, const int & edgeId, double* iPos, double* oDir, int iModelId = -1);

        /**
         * @brief       获取面上半边的给定坐标坐标切向。[虚函数]
         * @param[in]   vtpFace：面虚拓扑对象
         * @param[in]   vtpEdge：面上边虚拓扑对象
         * @param[in]   iPos：输入坐标
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-08
         */
        virtual bool getCoedgeDirection(FITKAbsVirtualTopo* vtpFace, FITKAbsVirtualTopo* vtpEdge, double* iPos, double* oDir, int iModelId = -1)
        {
            Q_UNUSED(vtpFace);
            Q_UNUSED(vtpEdge);
            Q_UNUSED(iPos);
            Q_UNUSED(oDir);
            Q_UNUSED(iModelId);
            return false;
        };

        /**
         * @brief       获取面上边界线法向。[虚函数]
         * @param[in]   faceId：面虚拓扑ID
         * @param[in]   edgeId：面上边虚拓扑ID
         * @param[in]   iPos：输入坐标信息
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-20
         */
        virtual bool getBoundaryNormalByID(const int & faceId, const int & edgeId, double* iPos, double* oDir, int iModelId = -1);

        /**
         * @brief       获取面上边界线法向。[虚函数]
         * @param[in]   vtpFace：面虚拓扑对象
         * @param[in]   vtpEdge：面上边虚拓扑对象
         * @param[in]   iPos：输入坐标信息
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-20
         */
        virtual bool getBoundaryNormal(FITKAbsVirtualTopo* vtpFace, FITKAbsVirtualTopo* vtpEdge, double* iPos, double* oDir, int iModelId = -1)
        {
            Q_UNUSED(vtpFace);
            Q_UNUSED(vtpEdge);
            Q_UNUSED(iPos);
            Q_UNUSED(oDir);
            Q_UNUSED(iModelId);
            return false;
        };

        /**
         * @brief       判断给定坐标是否在线上。[虚函数]
         * @param[in]   id：虚拓扑ID
         * @param[in]   iPos：输入坐标
         * @return      坐标是否在线上
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-07
         */
        virtual bool isPosOnEdgeByID(const int & id, double* iPos);

        /**
         * @brief       判断给定坐标是否在线上。[虚函数]
         * @param[in]   vtp：虚拓扑对象
         * @param[in]   iPos：输入坐标
         * @return      坐标是否在线上
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-07
         */
        virtual bool isPosOnEdge(FITKAbsVirtualTopo* vtp, double* iPos)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(iPos);
            return false;
        };

        /**
         * @brief       判断给定坐标是否在线上。[虚函数]
         * @param[in]   start：起点
         * @param[in]   dir：方向
         * @param[in]   pos：坐标
         * @return      坐标是否在线上
         * @author      YanZhiHui (chanyuantiandao@126.com)
         * @date        2025-06-30
         */
        virtual bool isPosOnEdge(double* start, double* dir, double* pos)
        {
            Q_UNUSED(start);
            Q_UNUSED(dir);
            Q_UNUSED(pos);
            return false;
        };

        /**
         * @brief       查询当前对象是否为直线。[虚函数]
         * @param[in]   id：虚拓扑ID[引用]
         * @return      是否为直线
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-10
         */
        virtual bool isLineByID(const int & id);

        /**
         * @brief       查询当前对象是否为直线。[虚函数]
         * @param[in]   vtp：线虚拓扑
         * @return      是否为直线
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-10
         */
        virtual bool isLine(FITKAbsVirtualTopo* vtp)
        {
            Q_UNUSED(vtp);
            return false;
        };

        /**
         * @brief       查询当前对象长度。[虚函数]
         * @param[in]   id：虚拓扑ID[引用]
         * @param[out]  oLen：线长[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-16
         */
        virtual bool getLengthByID(const int & id, double & oLen);

        /**
         * @brief       查询当前对象长度。[虚函数]
         * @param[in]   vtp：线虚拓扑
         * @param[out]  oLen：线长[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-16
         */
        virtual bool getLength(FITKAbsVirtualTopo* vtp, double & oLen)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oLen);
            return false;
        };

        /**
         * @brief       获取边中点坐标。[虚函数]
         * @param[in]   id：边虚拓扑ID
         * @param[out]  oPos：返回坐标信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-01
         */
        virtual bool getMidPointByID(const int & id, double* oPos, int iModelId = -1);

        /**
         * @brief       获取边中点坐标。[虚函数]
         * @param[in]   vtp：边虚拓扑对象
         * @param[out]  oPos：返回坐标信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-01
         */
        virtual bool getMidPoint(FITKAbsVirtualTopo* vtp, double* oPos, int iModelId = -1)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oPos);
            Q_UNUSED(iModelId);
            return false;
        };

        /**
         * @brief       获取方向。[虚函数]
         * @param[in]   id：虚拓扑ID
         * @paramp[out] oPos：返回坐标信息
         * @paramp[out] oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2025-07-25
         */
        virtual bool getMidPointDirectionByID(const int & id, double* oPos, double* oDir, int iModelId = -1);

        /**
         * @brief       获取方向。[虚函数]
         * @param[in]   vtp：虚拓扑对象
         * @param[out]  oPos：返回坐标信息
         * @param[out]  oDir：返回方向信息
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2025-07-25
         */
        virtual bool getMidPointDirection(FITKAbsVirtualTopo* vtp, double* oPos, double* oDir, int iModelId = -1)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oPos);
            Q_UNUSED(oDir);
            Q_UNUSED(iModelId);
            return false;
        };

        /**
         * @brief       获取边上特征点坐标。[虚函数]
         * @param[in]   id：边虚拓扑ID
         * @param[out]  oPoses：返回坐标列表[引用]
         * @param[in]   skipMidPt：是否跳过中点[缺省]
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-02
         */
        virtual bool getFeaturePointsByID(const int & id, QList<QList<double>> & oPoses, bool skipMidPt = false, int iModelId = -1);

        /**
         * @brief       获取边上特征点坐标。[虚函数]
         * @param[in]   vtp：虚拓扑对象
         * @param[out]  oPoses：返回坐标列表[引用]
         * @param[in]   skipMidPt：是否跳过中点[缺省]
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-02
         */
        virtual bool getFeaturePoints(FITKAbsVirtualTopo* vtp, QList<QList<double>> & oPoses, bool skipMidPt = false, int iModelId = -1)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oPoses);
            Q_UNUSED(skipMidPt);
            Q_UNUSED(iModelId);
            return false;
        };

        /**
         * @brief  根据参数获取边上对应点的坐标
         * @param  id 虚拓扑ID
         * @param  param 参数
         * @param  pos 点坐标
         * @param  iModelId 模型ID[缺省]
         * @return 查询结果是否有效
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-05-08
         */
        virtual bool evalPointByID(const int& id, double param, double* pos, bool normalized = false, int iModelId = -1);

        /**
         * @brief  根据参数获取边上对应点的坐标
         * @param  id 虚拓扑对象
         * @param  param 参数
         * @param  pos 点坐标
         * @param  iModelId 模型ID[缺省]
         * @return 查询结果是否有效
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-05-08
         */
        virtual bool evalPoint(FITKAbsVirtualTopo* vtp, double param, double* pos, bool normalized = false, int iModelId = -1)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(param);
            Q_UNUSED(pos);
            Q_UNUSED(normalized);
            Q_UNUSED(iModelId);
            return false;
        };
        /**
         * @brief   获取当前对象的所有自由边ID。[虚函数]
         * @param   commandId：命令ID
         * @return  自由边ID列表
         * @author  libaojun
         * @date    2025-05-13
         */
        virtual QList<int> getFreeEdgeIDs(int commandId)
        {
            Q_UNUSED(commandId);
            return QList<int>();
        };

        /**
         * @brief       获取几何中在给定平面的拓扑边。[虚函数]
         * @param[in]   cmdId：命令ID
         * @param[in]   iOrg：平面中心
         * @param[in]   iNor：平面法向
         * @return      拓扑列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-19
         */
        virtual QList<int> getEdgesOnPlane(int cmdId, double* iOrg, double* iNor)
        {
            Q_UNUSED(cmdId);
            Q_UNUSED(iOrg);
            Q_UNUSED(iNor);
            return QList<int>();
        }


        /**
         * @brief  判断一组面是否存在共点（且不被其它边共享的点）
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-07-14
         */
        virtual bool hasCommonVertexNotSharedByOther(QList<int> ids) { return false; }
        /**
         * @brief  判断一组边的夹角是否存在尖角
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-07-14
         */
        virtual bool hasSharpAngles(QList<int> ids) { return false; };
    };

    /**
     * @brief       面信息查询工具。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-05
     */
    class FITKInterfaceGeometryAPI FITKAbstractGeomFaceTool :
        public Core::FITKAbstractObject
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        explicit FITKAbstractGeomFaceTool() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        virtual ~FITKAbstractGeomFaceTool() = default;

        /**
         * @brief       创建（实例化）工具方法。[静态]
         * @return      查询工具实例
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        static FITKAbstractGeomFaceTool* createTool();

        /**
         * @brief       获取面的闭合线框。[虚函数]
         * @param[in]   cmdId：几何命令ID
         * @param[in]   id：面虚拓扑ID[引用]
         * @param[out]  oWires：线框组成的边列表[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-09
         */
        virtual bool getWiresByID(const int cmdId, const int & id, QList<QList<int>> & oWires);

        /**
         * @brief       获取面的闭合线框。[虚函数]
         * @param[in]   cmdId：几何命令ID
         * @param[in]   vtp：面虚拓扑
         * @param[out]  oWires：线框组成的边列表[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-09
         */
        virtual bool getWires(const int cmdId, FITKAbsVirtualTopo* vtp, QList<QList<int>> & oWires)
        {
            Q_UNUSED(cmdId);
            Q_UNUSED(vtp);
            Q_UNUSED(oWires);
            return false;
        }

        /**
         * @brief       获取面积。[虚函数]
         * @param[in]   id：面虚拓扑ID[引用]
         * @param[out]  oArea：面积[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-26
         */
        virtual bool getAreaByID(const int & id, double & oArea);

        /**
         * @brief       获取面积。[虚函数]
         * @param[in]   vtp：面虚拓扑
         * @param[out]  oArea：面积[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-05-26
         */
        virtual bool getArea(FITKAbsVirtualTopo* vtp, double & oArea)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oArea);
            return false;
        }

        /**
         * @brief       获取面包围盒尺寸。[虚函数]
         * @param[in]   id：面虚拓扑ID[引用]
         * @param[out]  oLen：最长边长度[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-04
         */
        virtual bool getFaceBoundSizeByID(const int & id, double & oLen);

        /**
         * @brief       获取面包围盒尺寸。[虚函数]
         * @param[in]   vtp：面虚拓扑
         * @param[out]  oLen：最长边长度[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-04
         */
        virtual bool getFaceBoundSize(FITKAbsVirtualTopo* vtp, double & oLen)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oLen);
            return false;
        }

        /**
         * @brief       获取曲面上任意位置平均曲率。[虚函数]
         * @param[in]   id：面虚拓扑ID[引用]
         * @param[in]   iPos：面虚拓扑ID
         * @param[out]  oCur：平均曲率[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-04
         */
        virtual bool getMeanCurvatureAtPosByID(const int & id, double* iPos, double & oCur);

        /**
         * @brief       获取曲面上任意位置平均曲率。[虚函数]
         * @param[in]   vtp：面虚拓扑
         * @param[in]   iPos：面虚拓扑ID
         * @param[out]  oCur：平均曲率[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-04
         */
        virtual bool getMeanCurvatureAtPos(FITKAbsVirtualTopo* vtp, double* iPos, double & oCur)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(iPos);
            Q_UNUSED(oCur);
            return false;
        }

        /**
         * @brief       获取曲面上任意位置平均曲率。[虚函数]
         * @param[in]   id：面虚拓扑ID[引用]
         * @param[in]   iPos：面虚拓扑ID
         * @param[out]  oCur1：曲率1[引用]
         * @param[out]  oCur2：曲率2[引用]
         * @param[out]  oDir1：曲率1方向[引用]
         * @param[out]  oDir2：曲率2方向[引用]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-04
         */
        virtual bool getCurvatureAtPosByID(const int & id, double* iPos,
            double & oCur1, double & oCur2, double* oDir1, double* oDir2);

        /**
         * @brief       获取曲面上任意位置平均曲率。[虚函数]
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
        virtual bool getCurvatureAtPos(FITKAbsVirtualTopo* vtp, double* iPos,
            double & oCur1, double & oCur2, double* oDir1, double* oDir2)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(iPos);
            Q_UNUSED(oCur1);
            Q_UNUSED(oCur2);
            Q_UNUSED(oDir1);
            Q_UNUSED(oDir2);
            return false;
        }

        /**
         * @brief       获取平面信息。[虚函数]
         * @param[in]   id：面虚拓扑ID（必须为平面）[引用]
         * @param[out]  oPos：面中心点坐标
         * @param[out]  oNormal：面法向
         * @param[out]  oUp：面向上方向
         * @param[in]   iModelId：模型ID[缺省]
         * @return      是否为有效平面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        virtual bool getPlaneByID(const int & id, double* oPos, double* oNormal, double* oUp, int iModelId = -1);

        /**
         * @brief       获取平面信息。[虚函数]
         * @param[in]   vtp：面虚拓扑（必须为平面）
         * @param[out]  oPos：面中心点坐标
         * @param[out]  oNormal：面法向
         * @param[out]  oUp：面向上方向
         * @param[in]   iModelId：模型ID[缺省]
         * @return      是否为有效平面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        virtual bool getPlane(FITKAbsVirtualTopo* vtp, double* oPos, double* oNormal, double* oUp, int iModelId = -1)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oPos);
            Q_UNUSED(oNormal);
            Q_UNUSED(oUp);
            Q_UNUSED(iModelId);
            return false;
        };

        /**
         * @brief       获取面上中心位置及中心位置法线。[虚函数]
         * @param[in]   id：面虚拓扑ID[引用]
         * @param[out]  oPos：返回面中心
         * @param[out]  oNormal：法向
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-16
         */
        virtual bool getNormalByID(const int & id, double* oPos, double* oNormal, int iModelId = -1);

        /**
         * @brief       获取面上中心位置及中心位置法线。[虚函数]
         * @param[in]   vtp：面虚拓扑
         * @param[out]  oPos：返回面中心
         * @param[out]  oNormal：法向
         * @param[in]   iModelId：模型ID[缺省]
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-16
         */
        virtual bool getNormal(FITKAbsVirtualTopo* vtp, double* oPos, double* oNormal, int iModelId = -1)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oPos);
            Q_UNUSED(oNormal);
            Q_UNUSED(iModelId);
            return false;
        };


        /**
         * @brief   获取面上指定位置法向。[虚函数]
         * @param   id：面虚拓扑ID[引用]
         * @param   iPos：面上的任意位置
         * @param   oNormal：法向
         * @param   iModelId：模型ID[缺省]
         * @return  查询结果是否有效
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-05-08
         */
        virtual bool getNormalAtPosByID(const int & id, double* iPos, double* oNormal, int iModelId = -1);

        /**
         * @brief   获取面上指定位置法向。[虚函数]
         * @param   vtp：面虚拓扑
         * @param   iPos：面上的任意位置
         * @param   oNormal：法向
         * @param   iModelId：模型ID[缺省]
         * @return  查询结果是否有效
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-05-08
         */
        virtual bool getNormalAtPos(FITKAbsVirtualTopo* vtp, double* iPos, double* oNormal, int iModelId = -1)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(iPos);
            Q_UNUSED(oNormal);
            Q_UNUSED(iModelId);
            return false;
        };

        /**
         * @brief       查询当前对象是否为平面。[虚函数]
         * @param[in]   id：虚拓扑ID[引用]
         * @return      是否为平面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-10
         */
        virtual bool isPlaneByID(const int & id);

        /**
         * @brief       查询当前对象是否为平面。[虚函数]
         * @param[in]   vtp：面虚拓扑
         * @return      是否为平面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-10
         */
        virtual bool isPlane(FITKAbsVirtualTopo* vtp)
        {
            Q_UNUSED(vtp);
            return false;
        };

        /**
         * @brief       查询当前对象是否为圆柱面。[虚函数]
         * @param[in]   id：虚拓扑ID[引用]
         * @return      是否为圆柱面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual bool isCylindByID(const int & id);

        /**
         * @brief       查询当前对象是否为圆柱面。[虚函数]
         * @param[in]   vtp：面虚拓扑
         * @return      是否为圆柱面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual bool isCylind(FITKAbsVirtualTopo* vtp)
        {
            Q_UNUSED(vtp);
            return false;
        };

        /**
         * @brief       查询当前对象是否为圆柱面。[虚函数]
         * @param[in]   id：虚拓扑ID[引用]
         * @param[out]  oCt：圆柱面中心
         * @param[out]  oDir：圆柱面轴线方向
         * @return      是否为圆柱面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual bool getCylindSurfByID(const int & id, double* oCt, double* oDir);

        /**
         * @brief       查询当前对象是否为圆柱面。[虚函数]
         * @param[in]   vtp：面虚拓扑
         * @param[out]  oCt：圆柱面中心
         * @param[out]  oDir：圆柱面轴线方向
         * @return      是否为圆柱面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual bool getCylindSurf(FITKAbsVirtualTopo* vtp, double* oCt, double* oDir)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oCt);
            Q_UNUSED(oDir);
            return false;
        };

        /**
         * @brief       查询圆柱面半径。（部分几何引擎不支持）[虚函数]
         * @param[in]   id：虚拓扑ID[引用]
         * @param[out]  oRadius：圆柱面半径
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-22
         */
        virtual bool getCylindSurfRadiusByID(const int & id, double & oRadius);

        /**
         * @brief       查询圆柱面半径。（部分几何引擎不支持）[虚函数]
         * @param[in]   vtp：面虚拓扑
         * @param[out]  oRadius：圆柱面半径
         * @return      查询结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-22
         */
        virtual bool getCylindSurfRadius(FITKAbsVirtualTopo* vtp, double & oRadius)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oRadius);
            return false;
        };

        /**
         * @brief       查询当前对象是否为圆锥面。[虚函数]
         * @param[in]   id：虚拓扑ID[引用]
         * @return      是否为圆锥面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual bool isConeByID(const int & id);

        /**
         * @brief       查询当前对象是否为圆锥面。[虚函数]
         * @param[in]   vtp：面虚拓扑
         * @return      是否为圆锥面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual bool isCone(FITKAbsVirtualTopo* vtp)
        {
            Q_UNUSED(vtp);
            return false;
        };

        /**
         * @brief       查询当前对象是否为圆锥面。[虚函数]
         * @param[in]   id：虚拓扑ID[引用]
         * @param[out]  oCt：圆锥面中心
         * @param[out]  oDir：圆锥面轴线方向
         * @return      是否为圆锥面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual bool getConeSurfByID(const int & id, double* oCt, double* oDir);

        /**
         * @brief       查询当前对象是否为圆锥面。[虚函数]
         * @param[in]   vtp：面虚拓扑
         * @param[out]  oCt：圆锥面中心
         * @param[out]  oDir：圆锥面轴线方向
         * @return      是否为圆锥面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual bool getConeSurf(FITKAbsVirtualTopo* vtp, double* oCt, double* oDir)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oCt);
            Q_UNUSED(oDir);
            return false;
        };

        /**
         * @brief       查询指定列表拓扑是否均为平面且共面（不包含平行）。[虚函数]
         * @param[in]   ids：虚拓扑ID列表
         * @return      拓扑是否均为平面且共面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-02
         */
        virtual bool isCoplanar(QList<int> ids)
        {
            Q_UNUSED(ids);
            return false;
        }

        /**
         * @brief       查询指定列表拓扑是否均为面且在同一个几何面上。[虚函数]
         * @param[in]   ids：虚拓扑ID列表
         * @return      拓扑是否均为面且在同一个几何面上
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-22
         */
        virtual bool isSameSurface(QList<int> ids)
        {
            Q_UNUSED(ids);
            return false;
        }

        /**
         * @brief  判断一组面是否存在共边（且不被其它面共享的边）
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-07-14
         */
        virtual bool hasCommonEdgeNotSharedByOther(QList<int> ids) { return false; }
        /**
         * @brief  判断一组面的夹角是否存在尖角
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-07-14
         */
        virtual bool hasSharpAngles(QList<int> ids) { return false; };
    };

    /**
     * @brief       通用信息查询工具。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-24
     */
    class FITKInterfaceGeometryAPI FITKAbstractGeomCommonTool :
        public Core::FITKAbstractObject
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-24
         */
        explicit FITKAbstractGeomCommonTool() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-24
         */
        virtual ~FITKAbstractGeomCommonTool() = default;

        /**
         * @brief       创建（实例化）工具方法。[静态]
         * @return      查询工具实例
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-24
         */
        static FITKAbstractGeomCommonTool* createTool();

        /**
         * @brief       根据拓扑获取二者距离。[虚函数]
         * @param[in]   vTopoId_1：第一个虚拓扑ID
         * @param[in]   vTopoId_2：第二个虚拓扑ID
         * @param[out]  oDist：距离[引用]
         * @param[out]  oPos_1：拓扑1位置
         * @param[out]  oPos_2：拓扑2位置
         * @param[in]   iModelId_1：拓扑1模型ID[缺省]
         * @param[in]   iModelId_2：拓扑2模型ID[缺省]
         * @return      是否存在有效距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-24
         */
        virtual bool getDistanceByTopoIDs(const int vTopoId_1, const int vTopoId_2,
            double & oDist, double oPos_1[3], double oPos_2[3], int iModelId_1 = -1, int iModelId_2 = -1);

        /**
         * @brief       根据拓扑获取二者距离。[虚函数]
         * @param[in]   vtp_1：第一个虚拓扑对象
         * @param[in]   vtp_2：第二个虚拓扑对象
         * @param[out]  oDist：距离[引用]
         * @param[out]  oPos_1：拓扑1位置
         * @param[out]  oPos_2：拓扑2位置
         * @param[in]   iModelId_1：拓扑1模型ID[缺省]
         * @param[in]   iModelId_2：拓扑2模型ID[缺省]
         * @return      是否存在有效距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-24
         */
        virtual bool getDistanceByTopos(FITKAbsVirtualTopo* vtp_1, FITKAbsVirtualTopo* vtp_2
            , double & oDist, double oPos_1[3], double oPos_2[3], int iModelId_1 = -1, int iModelId_2 = -1)
        {
            Q_UNUSED(vtp_1);
            Q_UNUSED(vtp_2);
            Q_UNUSED(oDist);
            Q_UNUSED(oPos_1);
            Q_UNUSED(oPos_2);
            Q_UNUSED(iModelId_1);
            Q_UNUSED(iModelId_2);
            return false;
        }

        /**
         * @brief       获取拓扑与三维坐标点距离。[虚函数]
         * @param[in]   vTopoId：虚拓扑ID
         * @param[in]   iPos3：坐标
         * @param[out]  oDist：距离[引用]
         * @param[out]  oPos：拓扑位置
         * @param[in]   iModelId：拓扑模型ID[缺省]
         * @return      是否存在有效距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-24
         */
        virtual bool getDistanceByTopoIDAndPos(const int vTopoId, double iPos3[3], 
            double & oDist, double oPos[3], int iModelId = -1);

        /**
         * @brief       获取拓扑与三维坐标点距离。[虚函数]
         * @param[in]   vtp：虚拓扑对象
         * @param[in]   iPos3：坐标
         * @param[out]  oDist：距离[引用]
         * @param[out]  oPos：拓扑位置
         * @param[in]   iModelId：拓扑模型ID[缺省]
         * @return      是否存在有效距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-24
         */
        virtual bool getDistanceByTopoAndPos(FITKAbsVirtualTopo* vtp, double iPos3[3], 
            double & oDist, double oPos[3], int iModelId = -1)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(iPos3);
            Q_UNUSED(oDist);
            Q_UNUSED(oPos);
            Q_UNUSED(iModelId);
            return false;
        }

        /**
         * @brief       获取拓扑边与给定直线距离。[虚函数]
         * @param[in]   vEdgeId：虚拓扑边ID
         * @param[in]   iPos1：直线坐标1
         * @param[in]   iPos2：直线坐标2
         * @param[out]  oPos：交点坐标
         * @return      是否存在有效距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-06
         */
        virtual bool getCrossByEdgeIDAndLine(const int vEdgeId, double iPos1[3], double iPos2[3],
            double oPos[3]);

        /**
         * @brief       获取拓扑边与给定直线距离。[虚函数]
         * @param[in]   vtp：虚拓扑边对象
         * @param[in]   iPos1：直线坐标1
         * @param[in]   iPos2：直线坐标2
         * @param[out]  oDist：距离[引用]
         * @param[out]  oPos：交点坐标
         * @return      是否存在有效距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-06
         */
        virtual bool getCrossByEdgeAndLine(FITKAbsVirtualTopo* vtp, double iPos1[3], double iPos2[3],
            double oPos[3])
        {
            Q_UNUSED(vtp);
            Q_UNUSED(iPos1);
            Q_UNUSED(iPos2);
            Q_UNUSED(oPos);
            return false;
        }

        /**
         * @brief       获取几何引擎是否支持查询无效拓扑与不精确拓扑。[虚函数]
         * @param[out]  oInvalid：是否可查询无效拓扑[引用]
         * @param[out]  oImprecise：是否可查询不精确拓扑[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        virtual void canCheckTopos(bool & oInvalid, bool & oImprecise)
        {
            oInvalid = false;
            oImprecise = false;
        }

        /**
         * @brief       获取几何命令中的无效实体拓扑。[虚函数]
         * @param[in]   cmdId：几何命令ID
         * @param[out]  oTopos：无效拓扑信息[引用]
         * @param[in]   warnings：是否查询警告信息[缺省]
         * @return      返回结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-17
         */
        virtual bool getInvalidTopos(int cmdId, QList<VirtualShape> & oTopos, bool warnings = false)
        {
            Q_UNUSED(cmdId);
            Q_UNUSED(oTopos);
            Q_UNUSED(warnings);
            return false;
        }

        /**
         * @brief       获取几何命令中的不精确拓扑。[虚函数]
         * @param[in]   cmdId：几何命令ID
         * @param[out]  oTopos：不精确拓扑信息[引用]
         * @return      返回结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-17
         */
        virtual bool getImpreciseTopos(int cmdId, QList<VirtualShape> & oTopos)
        {
            Q_UNUSED(cmdId);
            Q_UNUSED(oTopos);
            return false;
        }

        /**
         * @brief  计算过三点圆的信息
         * @param[in]  pt1 第一个点
         * @param[in]  pt2 第二个点
         * @param[in]  pt3 第三个点
         * @param[out]  center 圆心位置
         * @param[out]  normal 圆所在平面法线
         * @param[out]  radius 圆半径
         * @return 是否计算成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-07-10
         */
        virtual bool calcCircleInfoBy3Points(QVector<double> pt1, QVector<double> pt2, QVector<double> pt3, QVector<double>& center, QVector<double>& normal, double& radius) 
        { 
            Q_UNUSED(pt1);
            Q_UNUSED(pt2);
            Q_UNUSED(pt3);
            Q_UNUSED(center);
            Q_UNUSED(normal);
            Q_UNUSED(radius);
            return false;
        }

        /**
         * @brief       获取给定虚拓扑面的采样点。[虚函数]
         * @param[in]   vTopoId：虚拓扑ID
         * @param[out]  oPoses：采样点坐标[引用]
         * @param[in]   iSpacing：预估采样点间隔
         * @param[in]   iModelId：模型ID[缺省]
         * @return      是否存在有效采样点
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-23
         */
        virtual bool getFaceSamplePointsByID(const int vTopoId, QList<QList<double>> & oPoses, int iSpacing = 5, int iModelId = -1);

        /**
         * @brief       获取给定虚拓扑面的采样点。[虚函数]
         * @param[in]   vtp：虚拓扑对象
         * @param[out]  oPoses：采样点坐标[引用]
         * @param[in]   spacing：预估采样点间隔
         * @param[in]   iModelId：模型ID[缺省]
         * @return      是否存在有效采样点
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-23
         */
        virtual bool getFaceSamplePoints(FITKAbsVirtualTopo* vtp, QList<QList<double>> & oPoses, int iSpacing = 5, int iModelId = -1)
        {
            Q_UNUSED(vtp);
            Q_UNUSED(oPoses);
            Q_UNUSED(iSpacing);
            Q_UNUSED(iModelId);
            return false;
        }

    };

    /**
     * @brief   特征信息查询工具。
     * @author  fulipeng (flipengqd@yeah.net)
     * @date    2025-01-09
     */
    class FITKInterfaceGeometryAPI FITKAbstractGeomFeatureTool : public Core::FITKAbstractObject
    {
    public:
        /**
         * @brief   构造函数。
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-09
         */
        explicit FITKAbstractGeomFeatureTool() = default;

        /**
         * @brief   析构函数。
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-09
         */
        virtual ~FITKAbstractGeomFeatureTool() = default;

        /**
         * @brief   创建（实例化）工具方法。[静态]
         * @return  查询工具实例
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-09
         */
        static FITKAbstractGeomFeatureTool* createTool();

        /**
         * @brief   通过给定的虚拟拓扑数据对象获取特征信息.[虚函数]
         * @param   comId：命令ID 传递part对象ID
         * @param   featureType：特征类型
         * @param   oFeatures: 返回所有特征的链表 每个特征可能有多个面QList<QList<face>>
         * @return  查询结果是否有效
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-09
         */
        virtual bool getFeatures(int comId, FITKGeoEnum::FITKFeatureType featureType, QList<QList<Interface::FITKAbsVirtualTopo*>>& oFeatures)
        {
            Q_UNUSED(comId);
            Q_UNUSED(featureType);
            Q_UNUSED(oFeatures);
            return false;
        }

        /**
         * @brief       获取几何命令中的孔洞面拓扑。[虚函数]
         * @param[in]   cmdId：几何命令ID
         * @param[out]  oHoles：孔洞面组[引用]
         * @return      返回结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-22
         */
        virtual bool getHoles(const int cmdId, QList<QList<Interface::FITKAbsVirtualTopo*>> & oHoles)
        {
            Q_UNUSED(cmdId);
            Q_UNUSED(oHoles);
            return false;
        }

        /**
         * @brief       获取几何命令中的倒圆面拓扑。[虚函数]
         * @param[in]   cmdId：几何命令ID
         * @param[out]  oHoles：倒圆面组[引用]
         * @return      返回结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        virtual bool getBlends(const int cmdId, QList<QList<Interface::FITKAbsVirtualTopo*>> & oHoles)
        {
            Q_UNUSED(cmdId);
            Q_UNUSED(oHoles);
            return false;
        }

        /**
         * @brief       获取倒圆面半径。[虚函数]
         * @param[in]   faceId：几何面ID
         * @param[out]  oRadii：倒圆半径[引用]
         * @return      返回结果是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        virtual bool getBlendFaceRadii(int faceId, double & oRadii)
        {
            Q_UNUSED(faceId);
            Q_UNUSED(oRadii);
            return false;
        }
    };
    /**
     * @brief 抽象查询类生成器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-08-18
     */
    class FITKInterfaceGeometryAPI FITKAbstractGeomToolsCreator
    {
    public:
        /**
         * @brief Construct a new FITKAbstractGeomToolsCreator object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         */
        explicit FITKAbstractGeomToolsCreator() = default;
        /**
         * @brief Destroy the FITKAbstractGeomToolsCreator object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         */
        virtual ~FITKAbstractGeomToolsCreator() = 0;
        /**
         * @brief 创建点查询类
         * @return FITKAbstractGeomPointTool*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-18
         */
        virtual FITKAbstractGeomPointTool* createPointTool();

        /**
         * @brief       创建线信息查询工具。[虚函数]
         * @return      线信息查询工具
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-03
         */
        virtual Interface::FITKAbstractGeomEdgeTool* createEdgeTool();

        /**
         * @brief       创建面信息查询工具。[虚函数]
         * @return      平面信息查询工具
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        virtual Interface::FITKAbstractGeomFaceTool* createFaceTool();

        /**
         * @brief       创建通用信息查询工具。[虚函数]
         * @return      通用信息查询工具
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-24
         */
        virtual Interface::FITKAbstractGeomCommonTool* createCommonTool();

        /**
         * @brief   创建特征信息查询工具。[虚函数]
         * @return  特征信息查询工具
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-09
         */
        virtual Interface::FITKAbstractGeomFeatureTool* createFeatureTool();

    };
}

#endif
