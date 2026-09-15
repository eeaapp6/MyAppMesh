/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PyFITKGeoBehavior.h
 * @brief 几何操作 为python装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2025-06-25
 */
#ifndef _PY_FITKGeoBehavior_H___
#define _PY_FITKGeoBehavior_H___

#include <QString>
#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"

namespace Interface
{
    /**
     * @brief 几何操作（倒角，）
     * @author wangning (2185896382@qq.com)
     * @date 2025-06-25
     */
    class FITKInterfaceGeometryAPI PyFITKGeoBehavior : public QObject
    {
        Q_OBJECT
    public:
        explicit PyFITKGeoBehavior() = default;
        explicit PyFITKGeoBehavior(int geoCmdID);
        explicit PyFITKGeoBehavior(Interface::FITKAbsGeoCommand* geoCmd);
        virtual ~PyFITKGeoBehavior() = default;

        /**
        * @brief  倒角
        * @param[i] cmdID几何命令ID index倒角索引 length倒角长度
        * @author wangning (2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand * chamfer( QList<int> index, double length = 1.0);
        Interface::FITKAbsGeoCommand * fillet( QList<int> index, double length = 1.0);
        /**
         * @brief  拉伸切削/切削到面
         * @param[i]  sketchId草绘ID direction方向  depth深度 index拉伸到面索引
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* extrudeCutByDepth(int sketchId, const QVector<double>& direction, double depth);
        Interface::FITKAbsGeoCommand* extrudeCutByFace(int sketchId, const QVector<double>& direction, int index);
        /**
        * @brief  拉伸实体/拉伸实体到面
        * @param[i]  sketchId草绘ID direction方向  length长度 index拉伸到面索引
        * @author wangning (2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* extrudeSolidByLength(int sketchId, const QVector<double>& direction, double depth);
        Interface::FITKAbsGeoCommand* extrudeSolidByFace(int sketchId, const QVector<double>& direction, int index);
        /**
         * @brief  拉伸壳/拉伸壳到面
         * @param[i]  sketchId草绘ID direction方向  length长度 index拉伸到面索引
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* extrudeShellByLength(int sketchId, const QVector<double>& direction, double depth);
        Interface::FITKAbsGeoCommand* extrudeShellByFace(int sketchId, const QVector<double>& direction, int index);
        /**
         * @brief  旋转切削/旋转拉伸实体/旋转拉伸壳
         * @param[i]  sketchId草绘ID  points轴端点  angle角度
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* revolveCut(int sketchId, const QVector<double>& points, double angle);
        Interface::FITKAbsGeoCommand* revolveSolid(int sketchId, const QVector<double>& points, double angle);
        Interface::FITKAbsGeoCommand* revolveShell(int sketchId, const QVector<double>& points, double angle);
        /**
         * @brief  扫掠实体/面/切削
         * @param[i] 第一个参数是脊线  第二个参数是截面  第三个参数是类型1实体 2面 3切削
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* sweepByEdgeFace(const QList<int>& indexEdge,int indexFace,int type);
        Interface::FITKAbsGeoCommand* sweepBySketchFace(const QString& sketchNameEdge, int indexFace, int type);
        Interface::FITKAbsGeoCommand* sweepByEdgeSketch(const QList<int>& indexEdge, const QString& sketchNameFace, int type);
        Interface::FITKAbsGeoCommand* sweepBySketchSketch(const QString& sketchNameEdge, const QString& sketchNameFace, int type);
        /*
         * @brief  打贯通孔
         * @param[i] 面索引，第一条边索引，第二条边索引，第一条边距离，第二条边距离，打孔直径，是否翻转
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* circularHoleThrough(int faceIndex,int edgeIndex1,int edgeIndex2,double distance1,double distance2,double diameter,bool dir=false);
        /*
         * @brief  移除在面上的特征
         * @param[i] indexs 面索引
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* removeFeaturesOnFaces(const QList<int>& indexs);
        /*
         * @brief  分割面（草图方式）
         * @param[i] indexs 面索引
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* partitionFaceSketch(int sketchId, const QList<int>& indexs);
        /*
        * @brief  分割面（两点方式）
        * @param[i] twoPoints 两点坐标 indexs面索引
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* partitionFaceTwoPoints(const QVector<double>& twoPoints, const QList<int>& indexs);
        /*
         * @brief  分割面（两边取点划曲线方式）
         * @param[i] twoPoints 两点坐标 indexs面索引
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* partitionFaceCurvedPath(int faceIndex,int edgeIndex1,int edgeIndex2,const QVector<double>& twoPoints);
        /*
        * @brief  分割面（面切面方式）
        * @param[i] twoPoints 两点坐标 indexs面索引
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* partitionFaceExtendFace(int extendfaceIndex, const QList<int>& indexs);
        /*
        * @brief  分割面（基准面切面方式）
        * @param[i] indexs面索引 planeName基准面名称
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* partitionFaceWithDatumPlane(const QList<int>& indexs, const QString& planeName);
        /*
         * @brief  分割面（边投影方式）
         * @param[i] twoPoints 两点坐标 indexs面索引
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* partitionFaceProjectEdges(const QList<int>& edgeIndexs, const QList<int>& faceIndexs);
        /*
         * @brief  分割面（工具面方式）
         * @param[i] partitionIndexs 待分割面索引 toolIndexs工具面索引
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* partitionFaceWithIntersectFace(const QList<int>& partitionIndexs, const QList<int>& toolIndexs);
        /*
        * @brief  通过主基准面偏移创建基准面
        * @param[i] type 偏移Plane 1xy 2yz 3xz  offset偏移距离  name名字
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* datumPlaneOffsetPrincipal(int type , double offset,const QString& name=QString());
        /*
        * @brief  通过指定面偏移创建基准面
        * @param[i] pos_nor_up pos：面中心点坐标 nor：面法向 up：面向上方向  offset偏移距离  name名字
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* datumPlaneOffsetPlane(const QVector<double>& pos_nor_up, double offset, const QString& name = QString());
        /*
         * @brief  通过三点创建基准面
         * @param[i] points三点坐标 name名字
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* datumPlaneThreePoints(const QVector<double>& points, const QString& name = QString());
        /*
        * @brief  通过点和法向创建基准面
        * @param[i] point点坐标 dir方向 name名字
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* datumPlanePointAndNormal(const QVector<double>& point, const QVector<double>& dir, const QString& name = QString());
        /*
         * @brief  两点之间创建平面
         * @param[i] points两点坐标 name名字
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* datumPlaneMidwaybetween(const QVector<double>& points, const QString& name = QString());
        /*
        * @brief  旋转选中面来创建平面
        * @param[i] pos_nor_up:pos面中心点坐标nor面法向 up面向上方向  axis两点确定的轴 angle角度 name名字
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* datumPlaneRotate(const QVector<double>& pos_nor_up, const QVector<double>& axis, double angle, const QString& name = QString());
        /*
        * @brief  创建点
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* datumPoint(double x,double y,double z, const QString& name = QString());
        /*
        * @brief  修复小边
        * @edgeIndexs edgeIndexs 边索引
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* repairSmallEdges(const QList<int>& edgeIndexs);
        /*
        * @brief  修复小面
        * @edgeIndexs faceIndexs 边索面
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* repairSmallFaces(const QList<int>& faceIndexs);
        /*
        * @brief  移除面
        * @edgeIndexs faceIndexs 面索引
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* removeFaces(const QList<int>& faceIndexs);
        /*
        * @brief  边生成面
        * @edgeIndexs edgeIndexs 边索引
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* coverEdges(const QList<int>& edgeIndexs);
        /*
         * @brief  缝合面
         * @edgeIndexs tolerance 容差  edgeIndexs边索引
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* stitch(double tolerance, const QList<int>& edgeIndexs = QList<int>{});
        /*
         * @brief  替换面
         * @edgeIndexs  faceIndexs面索引 extendNeighboringFaces是否扩充邻面
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* replaceFaces(const QList<int>& faceIndexs, bool extendNeighboringFaces);
        /*
         * @brief  延伸面
         * @edgeIndexs  edgeIndexs边索引 distance延伸长度
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* extendFaces(const QList<int>& edgeIndexs, double distance);
        /*
        * @brief  创建基准轴
        * @edgeIndexs  type 1x轴2y轴3z轴 name名字
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* datumAxisPrincipal(int type, const QString& name = QString());
        /*
         * @brief  通过面相交创建轴
         * @edgeIndexs  pos_nor_up pos面中心点坐标 nor面法向 up面向上方向 name名字
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* datumAxisIntersectPlanes(const QVector<double>& pos_nor_up_1, const QVector<double>& pos_nor_up_2, const QString& name = QString());
        /*
         * @brief  通过直线边创建轴
         * @edgeIndexs  index直线边索引
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* datumAxisStraightEdge(int index,const QString& name = QString());
        /*
        * @brief 通过两点创建轴
        * @edgeIndexs  points两点坐标
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* datumAxisTwoPoints(const QVector<double>& points, const QString& name = QString());
        /*
        * @brief 通过点和平面法向创建轴
        * @edgeIndexs pos_nor_up pos面中心点坐标 nor面法向 up面向上方向 point点坐标  name名字
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* datumAxisNormalToPlane(const QVector<double>& pos_nor_up_1, QVector<double> point, const QString& name = QString());
        /*
         * @brief 通过圆上三点创建轴
         * @edgeIndexs points 三点坐标  name名字
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* datumAxisPointsOnCircle(const QVector<double>& points, const QString& name = QString());
        /*
         * @brief 通过边旋转创建轴
         * @edgeIndexs linePoints旋转边两端点 rotatePoints旋转轴两端点 angle角度 name名字
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* datumAxisRotateFromLine(const QVector<double>& linePoints, const QVector<double>& rotatePoints, double angle, const QString& name = QString());
        /*
         * @brief 用面切割体(通过点和法向来确定用于切割的平面)
         * @edgeIndexs indexSolid体索引 point点坐标 normal法向
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* partitionCellWithPlane(QList<int> indexSolid, const QVector<double>& point, const QVector<double>& normal);
        /*
         * @brief 用延伸面切割体
         * @edgeIndexs indexSolid体索引 faceIndex面索引
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* partitionCellWithExtendFace(QList<int> indexSolid, int faceIndex);
        /*
         * @brief 用边创建延伸面切割体
         * @edgeIndexs indexSolid体索引 edgeIndex边索引 dir延伸方向 sweepIndex扫掠边索引
         * @author wangning(2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* partitionCellWithSweepEdge(QList<int> indexSolid, QList<int> edgeIndex, const QVector<double>& dir);
        Interface::FITKAbsGeoCommand* partitionCellWithSweepEdge(QList<int> indexSolid, QList<int> edgeIndex, int sweepIndex);
        /*
        * @brief 用草图切割体
        * @edgeIndexs indexSolid体索引 sketchId草图ID
        * @author wangning(2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* partitionCellWithSketch( QList<int> indexSolid, int sketchId);
        /*
        * @brief 用线上一点切割线
        * @edgeIndexs indexEdge边索引 point点坐标
        * @author wangning(2185896382@qq.com)
        * @date   2025-08-11
        */
        Interface::FITKAbsGeoCommand* partitionEdgeWithLocation(QList<int> indexEdge, const QVector<double>& point);
        /*
        * @brief 用比例参数切割线
        * @edgeIndexs indexEdge边索引 parameter参数
        * @author wangning(2185896382@qq.com)
        * @date   2025-08-11
        */
        Interface::FITKAbsGeoCommand* partitionEdgeWithParameter(QList<int> indexEdge, double parameter);
        /*
        * @brief 用基准面切割线
        * @edgeIndexs indexEdge边索引 planeName基准面名称
        * @author wangning(2185896382@qq.com)
        * @date   2025-08-11
        */
        Interface::FITKAbsGeoCommand* partitionEdgeWithDatumPlane(QList<int> indexEdge, const QString& planeName);
        /*
        * @brief （mesh）合并面
        * @edgeIndexs indexs面索引 skipSharpAngle标志 combineBounding标志
        * @author wangning(2185896382@qq.com)
        * @date   2025-08-11
        */
        Interface::FITKAbsGeoCommand* virtualTopologyCombineFaces(QList<int> indexs, bool skipSharpAngle,bool combineBounding);
        /*
        * @brief （mesh）合并边
        * @edgeIndexs indexs边索引 skipSharpAngle标志
        * @author wangning(2185896382@qq.com)
        * @date   2025-08-11
        */
        Interface::FITKAbsGeoCommand* virtualTopologyCombineEdges(QList<int> indexs, bool skipSharpAngle);
        /**
         * @brief  删除几何命令
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        bool removeFeature(const QString& name = QString());
        /**
         * @brief  设置当前模型命令
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        void setGeoCmd(int geoCmdID);

    signals:
        /**
         * @brief 删除特征信号
         * @author wangning (2185896382@qq.com)
         * @date 2025-06-25
         */
        void deleteSig(int id);

    private:
        /**
        * @brief  通过索引获取虚拓扑链表 
        * @param[i]cmdID几何命令ID index特征索引 type类型
        * @author wangning (2185896382@qq.com)
        * @date   2025-06-25
        */
        QList<Interface::VirtualShape> getShapes(const QList<int>& indexs, Interface::FITKGeoEnum::VTopoShapeType type);
        Interface::VirtualShape        getShapes(int indexs, Interface::FITKGeoEnum::VTopoShapeType type);

    private:
        /**
        * @brief  模型命令
        * @author wangning (2185896382@qq.com)
        * @date   2025-06-25
        */
        Interface::FITKAbsGeoCommand* _geoCmd{};
        /**
        * @brief  草图关联特征链表
        * @param  int特征ID ,int草图ID
        * @author wangning (2185896382@qq.com)
        * @date   2025-06-25
        */
        QMap<int, int> _FeaturesUseSketches{};
    };
}

#endif
