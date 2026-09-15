/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKPolyDataTool.h
 * @brief 面网格处理工具
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2023-07-29
 *
 */

#ifndef FITKPOLYDATATOOL_H
#define FITKPOLYDATATOOL_H

#include "FITKVTKAlgorithmAPI.h"

#include <QList> 
#include <QPair>
#include <vtkLine.h>
#include <QVector3D>
#include <QMap>

#define CELLPAIR QPair<int, int>

class vtkPolyData;
class vtkDataArray;
class vtkIntArray;

namespace Common
{
    /**
     * @brief 面网格处理工具
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2023-07-29
     */
    class FITKVTKALGORITHMAPI FITKPolyDataTool
    {
    public:
        FITKPolyDataTool(vtkPolyData* poly, vtkDataArray* normals);
        ~FITKPolyDataTool();
        /**
         * @brief 获取共面节点索引，需要构造函数指定法向
         * @param  seedID           种子点
         * @param  angleHold        角度阈值
         * @return QList<int>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2023-07-29
         */
        QList<int> getCoPlanePointIDs(int seedID, double angleHold);
        /**
         * @brief 获取共面单元索引，需要构造函数指定法向
         * @param  seedID           种子点
         * @param  angleHold        角度阈值
         * @return QList<int>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2023-07-29
         */
        QList<int> getCoPlaneCellIDs(int seedID, double angleHold);

        ///**
        // * @brief 获取单元法向奇异的单元编号
        // * @param  seedID           种子点
        // * @param  eles             寻找单元范围
        // * @return QList<int>
        // * @author LiBaojun (libaojunqd@foxmail.com)
        // * @date 2023-07-29
        // */
        //QList<int>  getNormalSingularID(const int seedID, const QList<int> &eles);

        /**
        * @brief 在由线型单元构成的网格上，基于节点ID，查找到第一个具有该节点的线单元，并给出节点ID是该线单元的首点还是尾点
        * @param  int pointID 线单元节点ID
        * @param  bool & isFirst 该节点是否为查找到单元的首节点，输入参数
        * @return  int 查找到的线单元ID
        * @time  2023/10/28
        * @author maguiwa(maguiwa520@163.com)
        */
        int getCellIdFromEdgePoint(int pointID, bool& isFirst);

        /**
        * @brief 在由线型单元构成的网格上通过用户选取的点、查找所有符合条件点集
        * @param  int pointID 用户点选节点ID
        * @param  double angle 节点所在单元查找所需后续单元满足的最小角度
        * @return  QList<int> 所有符合条件节点集合
        * @time  2023/10/28
        * @author maguiwa(maguiwa520@163.com)
        */
        QList<int> getEdgePointsByAngle(int pointID, double angle);

        /**
        * @brief 在由线型单元构成的网格上通过一个线线单元ID、查找相邻夹角符合要求线单元
        * @param  int cellID 选取线单元ID
        * @param  double angle 单元查找所需后续单元满足的最小角度
        * @return  QList<int> 特征边线单元列表
        * @time  2023/10/28
        * @author maguiwa(maguiwa520@163.com)
        */
        QList<int> getEdgeLinesByAngle(int cellID, double angle);

        /**
        * @brief
        * @param  int pt1ID
        * @param  int pt2ID
        * @return  int
        * @time  2023/10/29
        * @author maguiwa(maguiwa520@163.com)
        */
        int getCellIDbyPtID(int pt1ID, int pt2ID);

        /**
        * @brief 在由线型单元构成的网格上基于单元ID及给出的推导方向pointIndex，角度要求angle，推导后续符合要求线单元
        * @param  int cellID 推导起始线单元ID
        * @param  int pointIndex 推导后续线单元方向，0代表线单元0号节点方向，1代表线单元1号节点方向
        * @param  double angle 后续推导需满足的线单元间夹角要求
        * @param  bool autoReverse 遇见反向单元是否不考虑方向，只考虑最小夹角
        * @return  int 推导出的下一线单元ID
        * @time  2023/10/28
        * @author maguiwa(maguiwa520@163.com)
        */
        int getNextEdgeLineByAngle(int cellID, int pointIndex, double angle);

        /// 获取与该单元同向的特征边，正反向查找
        QList<int> getEdgeLinesByDirectionAndAngle(int cellID, double angle, bool endWithCross = false);

        /// 获取与该特征边单元同向的符合条件的特征边
        QList<int> getNextEdgeLineByDirectionAndAngle(int cellID, bool down, double angle, bool endWithCross = false);

        int getPointIDByPointPosition(double pos[3]);

        QVector<int> reuduceSharePointLine(QVector<int> cellIDs);

    private:


        /**
         * @brief 获取单元一阶邻域的单元编号
         * @param  cellID           单元号
         * @param  scanedCellIDs    已经扫描过的单元ID
         * @return QList<CELLPAIR>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2023-07-29
         */
        QList<CELLPAIR> getNeighborCell(int cellID/*, const QList<int>& scanedCellIDs*/);

        bool calculateCellNormal(const int cellID, double* normal);

        /**
        * @brief 获取线单元空间向量
        * @param  int cellID 线单元ID
        * @param  double & vec [ 3 ] 线单元空间向量
        * @return  void
        * @time  2023/10/31
        * @author maguiwa(maguiwa520@163.com)
        */
        void getLineItemVector(int cellID, double(&vec)[3]);

    private:
        vtkPolyData*  _poly{};     //面网格
        vtkDataArray* _normals{};  //面网格法向
        QMap<int, QVector3D> _points;
        
        // 是否已被检索过。   
        vtkIntArray* m_scanCellFlag{ nullptr };

        // 是否为共面（夹角）单元的节点。
        vtkIntArray* m_scanPointCoPlaneFlag{ nullptr };

        // 是否为共面（夹角）单元。
        vtkIntArray* m_scanCellCoPlaneFlag{ nullptr };
    };
}  // namespace Common

#endif // FITKPOLYDATATOOL_H