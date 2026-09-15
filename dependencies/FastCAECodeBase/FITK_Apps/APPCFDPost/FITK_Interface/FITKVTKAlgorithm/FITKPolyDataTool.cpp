/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPolyDataTool.h"

#include "qmath.h"

#include <QVector3D>
#include <vtkCellData.h>
#include <vtkIdList.h>
#include <vtkLine.h>
#include <vtkMath.h>
#include <vtkPolyData.h>
#include <vtkIntArray.h>

#include <vtkPolygon.h>

#include "FITK_Kernel/FITKCore/FITKLibInfo.h"
FITKLIBINFOREGISTER(FITKVTKAlgorithm, FITKVTKAlgorithmVersion);

namespace Common
{
    FITKPolyDataTool::FITKPolyDataTool(vtkPolyData* poly, vtkDataArray* normal)
        : _poly(poly), _normals(normal)
    {
        m_scanPointCoPlaneFlag = vtkIntArray::New();
        m_scanPointCoPlaneFlag->SetNumberOfComponents(1);

        int ptNum = _poly->GetNumberOfPoints();
        for (int iPt = 0; iPt < ptNum; ++iPt)
        {
            QVector3D pt;
            double    pos[3] = { .0 };
            _poly->GetPoint(iPt, pos);
            pt.setX(pos[0]);
            pt.setY(pos[1]);
            pt.setZ(pos[2]);
            _points.insert(iPt, pt);

            m_scanPointCoPlaneFlag->InsertNextValue(0);
        }

        // 初始化数组，全部置空标识。
        int nCell = _poly->GetNumberOfCells();

        m_scanCellFlag = vtkIntArray::New();
        m_scanCellFlag->SetNumberOfComponents(1);
        m_scanCellFlag->SetNumberOfValues(nCell);
        m_scanCellFlag->FillComponent(0, 0);

        m_scanCellCoPlaneFlag = vtkIntArray::New();
        m_scanCellCoPlaneFlag->SetNumberOfComponents(1);
        m_scanCellCoPlaneFlag->SetNumberOfValues(nCell);
        m_scanCellCoPlaneFlag->FillComponent(0, 0);
    }

    FITKPolyDataTool::~FITKPolyDataTool()
    {
        // 析构。
        if (m_scanCellFlag)
        {
            m_scanCellFlag->Delete();
            m_scanCellFlag = nullptr;
        }

        if (m_scanCellCoPlaneFlag)
        {
            m_scanCellCoPlaneFlag->Delete();
            m_scanCellCoPlaneFlag = nullptr;
        }

        if (m_scanPointCoPlaneFlag)
        {
            m_scanPointCoPlaneFlag->Delete();
            m_scanPointCoPlaneFlag = nullptr;
        }
    }

    QList< int > FITKPolyDataTool::getCoPlanePointIDs(int seedID, double angleHold)
    {
        //错误输入判断
        if (_poly == nullptr || angleHold < 0 || angleHold >= 180 || !m_scanPointCoPlaneFlag) return QList< int >();
        vtkSmartPointer<vtkIdList> cellids = vtkSmartPointer<vtkIdList>::New();
        _poly->GetPointCells(seedID, cellids);
        //选择一个节点所在的单元，作为起始种子点
        if (cellids->GetNumberOfIds() == 0) return QList< int >();
        const int acell = cellids->GetId(0);
        //获取在同一个平面的单元
        QList< int > neighborCells = this->getCoPlaneCellIDs(acell, angleHold);
        for (int cell : neighborCells)
        {
            vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
            _poly->GetCellPoints(cell, cellPointIds);
            for (vtkIdType i = 0; i < cellPointIds->GetNumberOfIds(); i++)
            {
                int pt = cellPointIds->GetId(i);

                // Modified by CHT
                //@{
                m_scanPointCoPlaneFlag->SetValue(pt, 1);
                // if (!cellPoints.contains(pt)) cellPoints.append(pt);
                //@}              
            }
        }

        // Added by CHT
        //@{
        QList< int > cellPoints;

        int nPt = m_scanPointCoPlaneFlag->GetNumberOfValues();
        for (int i = 0; i < nPt; i++)
        {
            if (m_scanPointCoPlaneFlag->GetValue(i))
            {
                cellPoints.push_back(i);
            }
        }
        //@}

        qSort(cellPoints);
        return cellPoints;
    }

    QList< int > FITKPolyDataTool::getCoPlaneCellIDs(int seedID, double angleHold)
    {
        if (_poly == nullptr || _normals == nullptr) return QList< int >();

        // Modified by CHT 修改记录单元方式。
        //@{
        m_scanCellCoPlaneFlag->SetValue(seedID, 1);
        QList< int > afWave = { seedID };

        //QList< int > coPlaneIDs{ seedID };
        //QList< int > scanedCell = coPlaneIDs;
        //QList< int > afWave = coPlaneIDs;
        //@}

        while (!afWave.isEmpty())
        {
            QList< int > tempAF = afWave;
            afWave.clear();
            QList< CELLPAIR > afRound;
            //计算所有最外层单元的邻接单元
            for (int cellID : tempAF)
            {
                QList< CELLPAIR > pa = getNeighborCell(cellID/*, scanedCell*/);
                afRound.append(pa);
            }
            //计算法向夹角，产生新的最外层单元
            for (CELLPAIR p : afRound)
            {
                const int cell0 = p.second, cell1 = p.first;
                double    normal0[3] = { 0, 0, 0 }, normal1[3] = { 0, 0, 0 };
                // vtkPolygon::ComputeNormal(_poly->GetCell(cell0)->GetPoints(), normal0);
                _normals->GetTuple(cell0, normal0);
                double norm0 = vtkMath::Norm(normal0);
                // vtkPolygon::ComputeNormal(_poly->GetCell(cell1)->GetPoints(), normal1);
                _normals->GetTuple(cell1, normal1);
                double norm1 = vtkMath::Norm(normal1);
                double numerator = normal0[0] * normal1[0] + normal0[1] * normal1[1] + normal0[2] * normal1[2];
                double value = numerator / (norm0 * norm1);
                value = value > 0.99999999999999 ? 0.99999999999999 : value;

                double angle1 = acos(value) * 180.0 / 3.141592654;
                if (angle1 <= angleHold)
                {
                    // Modified by CHT 修改记录单元方式。
                    //@{
                    if (!m_scanCellCoPlaneFlag->GetValue(cell1)) afWave.append(cell1);
                    m_scanCellCoPlaneFlag->SetValue(cell1, 1);
                    //@}

                    // if (!coPlaneIDs.contains(cell1)) coPlaneIDs.append(cell1);
                }

                // Modified by CHT 将表示设置为true，后续跳过检索。
                //@{
                m_scanCellFlag->SetValue(cell1, 1);
                //@}
                // if (!scanedCell.contains(cell1)) scanedCell.append(cell1);
            }

            afRound.clear();
        }

        // Added by CHT 通过标识数组获取共面单元。
        //@{
        QList<int> coPlaneIDs;
        int nCell = m_scanCellCoPlaneFlag->GetNumberOfValues();
        for (int i = 0; i < nCell; i++)
        {
            if (m_scanCellCoPlaneFlag->GetValue(i))
            {
                coPlaneIDs.push_back(i);
            }
        }
        //@}

        return coPlaneIDs;
    }

    QList< int > FITKPolyDataTool::getNormalSingularID(const int seedID, const QList< int >& eles)
    {
        if (_poly == nullptr || _normals == nullptr || !eles.contains(seedID)) return QList< int >();

        QList< int > singualIDs;

        QList< int > coPlaneIDs{ seedID };
        QList< int > scanedCell = coPlaneIDs;
        QList< int > afWave = coPlaneIDs;

        while (!afWave.isEmpty())
        {
            QList< int > tempAF = afWave;
            afWave.clear();
            QList< CELLPAIR > afRound;
            //计算所有最外层单元的邻接单元
            for (int cellID : tempAF)
            {
                QList< CELLPAIR > pa = getNeighborCell(cellID/*, scanedCell*/);
                afRound.append(pa);
            }
            //计算法向夹角，产生新的最外层单元
            for (CELLPAIR p : afRound)
            {
                const int cell0 = p.second, cell1 = p.first;
                if (!eles.contains(cell0) || !eles.contains(cell1))
                    continue;

                double normal0[3] = { 0, 0, 0 }, normal1[3] = { 0, 0, 0 };
                this->calculateCellNormal(cell0, normal0);
                double norm0 = vtkMath::Norm(normal0);
                this->calculateCellNormal(cell1, normal1);
                double norm1 = vtkMath::Norm(normal1);
                if (singualIDs.contains(cell0))
                    vtkMath::MultiplyScalar(normal0, -1);
                //                if (singualIDs.contains(cell1))
                //                    vtkMath::MultiplyScalar(normal1, -1);

                double numerator = normal0[0] * normal1[0] + normal0[1] * normal1[1] + normal0[2] * normal1[2];
                double angle1 = acos(numerator / (norm0 * norm1)) * 180.0 / 3.141592654;

                if (angle1 <= 180)
                {
                    if (!afWave.contains(cell1)) afWave.append(cell1);
                    if (!coPlaneIDs.contains(cell1)) coPlaneIDs.append(cell1);
                    if (angle1 > 90 && !scanedCell.contains(cell1))
                        singualIDs.append(cell1);
                }
                if (!scanedCell.contains(cell1)) scanedCell.append(cell1);
            }
            afRound.clear();
        }
        return singualIDs;
    }

    int FITKPolyDataTool::getCellIdFromEdgePoint(int pointID, bool& isFirst)
    {
        if (_poly == nullptr ||
            pointID < 0 ||
            pointID > _poly->GetNumberOfPoints())
        {
            return -1;
        }

        auto cellNum = _poly->GetNumberOfCells();
        for (int iCell = 0; iCell < cellNum; ++iCell)
        {
            auto cell = _poly->GetCell(iCell);
            if (!cell)
            {
                return -1;
            }

            auto idList = cell->GetPointIds();
            if (idList->GetNumberOfIds() != 2)
            {
                return -1;
            }

            auto id0 = idList->GetId(0);
            auto id1 = idList->GetId(1);

            if (id0 == pointID)
            {
                isFirst = true;
                return iCell;
            }
            else if (id1 == pointID)
            {
                isFirst = false;
                return iCell;
            }
        }
        return -1;
    }

    QList< int > FITKPolyDataTool::getEdgePointsByAngle(int pointID, double angle)
    {
        /// 对传入参数进行合法性筛选
        QSet< int > result;
        if (!_poly ||
            pointID < 0 ||
            pointID >= _poly->GetNumberOfPoints() ||
            angle < 0 ||
            angle > 90)
        {
            return result.toList();
        }

        /// 获取所有该选取点所在单元
        vtkSmartPointer<vtkIdList> cellIDs = vtkSmartPointer<vtkIdList>::New();
        _poly->GetPointCells(pointID, cellIDs);
        int iCellNum = cellIDs->GetNumberOfIds();

        /// 当前在此处对正反向单元只保留一个，提高效率，后续根据业务可能需要添加逻辑
        QList< QPair< int, int > > linePointRelation;
        for (int iCell = 0; iCell < iCellNum; ++iCell)
        {
            /// 基于线单元查找线单元所在的所有单元
            int               cellID = cellIDs->GetId(iCell);
            vtkCell*          cell = _poly->GetCell(cellID);
            int               findCellPt0 = cell->GetPointId(0);
            int               findCellPt1 = cell->GetPointId(1);
            QPair< int, int > direction1(findCellPt0, findCellPt1), direction2(findCellPt1, findCellPt0);
            if (linePointRelation.contains(direction1) ||
                linePointRelation.contains(direction2))
            {
                continue;
            }
            linePointRelation.append(direction1);

            /// 基于获取的单元进行逐单元双向查找
            QList< int > lines = getEdgeLinesByAngle(cellID, angle);
            for (int findCellID : lines)
            {
                vtkCell* findCell = _poly->GetCell(findCellID);
                if (!cell) { return result.toList(); }

                result.insert(findCell->GetPointId(0));
                result.insert(findCell->GetPointId(1));
            }
        }

        return result.toList();
    }

    QList< int > FITKPolyDataTool::getEdgeLinesByAngle(int cellID, double angle)
    {
        /// 对输入参数进行合法性检查
        QSet< int > lines;
        if (!_poly ||
            cellID < 0 ||
            cellID >= _poly->GetNumberOfCells() ||
            angle < 0 ||
            angle > 90 ||
            _poly->GetCell(cellID)->GetCellType() != VTK_LINE)
        {
            return lines.toList();
        }
        lines.insert(cellID);

        /// 获取线单元首点方向、尾点方向向下查找的所有单元
        for (int pointIndex = 0; pointIndex <= 1; ++pointIndex)
        {
            /// 首次进入，或单方向完成、确认查找的起始单元及查找方向
            int nextPointIndex = pointIndex;
            int nextCellID = cellID;

            /// 循环直至该方向不存在符合条件的后续线单元
            while (true)
            {
                int p0 = _poly->GetCell(nextCellID)->GetPointId(0);
                int p1 = _poly->GetCell(nextCellID)->GetPointId(1);

                nextCellID = getNextEdgeLineByAngle(nextCellID, nextPointIndex, angle);
                if (nextCellID < 0) { break; }
                if (lines.contains(nextCellID)) { break; }

                /// 准备下一个节点向下推导
                lines.insert(nextCellID);

                int p2 = _poly->GetCell(nextCellID)->GetPointId(0);
                //int p3 = _poly->GetCell(nextCellID)->GetPointId(1);
                //vtkNotUsed(p3);

                /// 查找到单元中不与初始单元共用的节点所在方向为下次查找方向
                if (nextPointIndex == 0)
                {
                    if (p2 == p0)
                    {
                        nextPointIndex = 1;
                    }
                    else
                        nextPointIndex = 0;
                }
                else if (nextPointIndex == 1)
                {
                    if (p1 == p2)
                    {
                        nextPointIndex = 1;
                    }
                    else
                        nextPointIndex = 0;
                }
            }
        }
        return lines.toList();
    }

    int FITKPolyDataTool::getCellIDbyPtID(int pt1ID, int pt2ID)
    {
        vtkSmartPointer<vtkIdList> cellIDs = vtkSmartPointer<vtkIdList>::New();
        _poly->GetPointCells(pt1ID, cellIDs);
        int cellNum = cellIDs->GetNumberOfIds();
        for (int iCell = 0; iCell < cellNum; ++iCell)
        {
            int      cellID = cellIDs->GetId(iCell);
            vtkCell* cell = _poly->GetCell(cellID);
            int      cellPt0ID = cell->GetPointId(0);
            int      cellPt1ID = cell->GetPointId(1);

            if ((pt1ID == cellPt0ID && pt2ID == cellPt1ID) ||
                (pt2ID == cellPt0ID && pt1ID == cellPt1ID))
            {
                return cellID;
            }
        }

        return -1;
    }

    int FITKPolyDataTool::getNextEdgeLineByAngle(int cellID, int pointIndex, double angle)
    {
        /// 对输入变量进行有效性判定，由于是私有内部接口，上层函数中进行了校验，本函数无需校验，若需暴露外部使用，需放开
        if (!_poly ||
            cellID < 0 ||
            cellID >= _poly->GetNumberOfCells() ||
            (pointIndex != 0 && pointIndex != 1) ||
            angle > 90 ||
            angle < 0)
        {
            return -1;
        }

        /// 构造输入单元向量
        double   p0[3] = { 0 }, p1[3] = { 0 };
        vtkCell* cell = _poly->GetCell(cellID);
        int      pt1ID = cell->GetPointId(0);
        int      pt2ID = cell->GetPointId(1);
        _poly->GetPoint(pt1ID, p0);
        _poly->GetPoint(pt2ID, p1);
        double vec1[3] = { p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2] };

        /// 查找推导方向上，节点所有单元
        int                          ptID = cell->GetPointId(pointIndex);
        vtkSmartPointer< vtkIdList > cellIDs = vtkSmartPointer< vtkIdList >::New();
        _poly->GetPointCells(ptID, cellIDs);
        int cellsNum = cellIDs->GetNumberOfIds();

        QMap< double, int > nextLineDegrees;
        for (int iCell = 0; iCell < cellsNum; ++iCell)
        {
            int findCellID = cellIDs->GetId(iCell);

            /// 该单元为当前查找单元，跳过查找下一个
            if (findCellID == cellID) { continue; }

            /// 存在当前查找到单元使用节点相同，顺序相反情况，这种情况不能作为最优解，该单元应因正反面情况后生成的，后续是否可考虑优化掉
            vtkCell* cell = _poly->GetCell(findCellID);
            int      findPt1ID = cell->GetPointId(0);
            int      findPt2ID = cell->GetPointId(1);

            if ((findPt1ID == pt2ID && findPt2ID == pt1ID) ||
                (findPt1ID == pt1ID && findPt2ID == pt2ID))
            {
                continue;
            }

            /// 构造查找到的线单元向量
            double p2[3] = { 0 }, p3[3] = { 0 };
            _poly->GetPoint(findPt1ID, p2);
            _poly->GetPoint(findPt2ID, p3);
            double vec2[3] = { p3[0] - p2[0], p3[1] - p2[1], p3[2] - p2[2] };

            /// 计算两个线单元夹角，不考虑矢量方向，按照最小夹角进行数据记录
            double currentAngle = vtkMath::Dot(vec1, vec2) / (vtkMath::Norm(vec1) * vtkMath::Norm(vec2));
            currentAngle = acos(currentAngle) * 180 / M_PI;
            if (currentAngle >= 90)
            {
                currentAngle = 180 - currentAngle;
            }

            nextLineDegrees.insert(currentAngle, findCellID);
        }

        /// 返回符合角度要求的最小夹角线单元
        if (nextLineDegrees.isEmpty()) { return -1; }
        double minAngle = nextLineDegrees.firstKey();
        if (minAngle < angle)
        {
            return nextLineDegrees.first();
        }
        return -1;
    }

    QList< int > FITKPolyDataTool::getEdgeLinesByDirectionAndAngle(int cellID, double angle, bool endWithCross /*= false */)
    {
        /// 对输入参数进行合法性检查
        QList< int > lookupCellIDs, finishedCellIDs;
        if (!_poly || cellID < 0 || cellID >= _poly->GetNumberOfCells() || angle < 0 || angle > 90 || _poly->GetCell(cellID)->GetCellType() != VTK_LINE) { return finishedCellIDs; }
        lookupCellIDs.append(cellID);
        finishedCellIDs.append(cellID);

        while (!lookupCellIDs.isEmpty())
        {
            int          current = lookupCellIDs.takeFirst();
            QList< int > ids = getNextEdgeLineByDirectionAndAngle(current, true, angle, endWithCross);

            for (int id : ids)
            {
                if (finishedCellIDs.contains(id) || id < 0) { continue; }
                lookupCellIDs.append(id);
                finishedCellIDs.append(id);
            }
        }

        /// 向上查找
        lookupCellIDs.append(cellID);
        while (!lookupCellIDs.isEmpty())
        {
            int          current = lookupCellIDs.takeFirst();
            QList< int > ids = getNextEdgeLineByDirectionAndAngle(current, false, angle, endWithCross);

            for (int id : ids)
            {
                if (finishedCellIDs.contains(id) || id < 0) { continue; }
                lookupCellIDs.append(id);
                finishedCellIDs.append(id);
            }
        }
        return finishedCellIDs;
    }


    QList< int > FITKPolyDataTool::getNextEdgeLineByDirectionAndAngle(int cellID, bool down, double angle, bool endWithCross /*= false */)
    {
        /// 对输入变量进行有效性判定，由于是私有内部接口，上层函数中进行了校验，本函数无需校验，若需暴露外部使用，需放开
        QMap< int, double > idAngles;
        if (!_poly || cellID < 0 || cellID >= _poly->GetNumberOfCells() || angle > 90 || angle < 0) { return idAngles.keys(); }

        /// 构造输入单元向量
        double vec1[3] = { 0 };
        getLineItemVector(cellID, vec1);

        /// 查找推导方向上，节点所有单元
        vtkCell*            cell = _poly->GetCell(cellID);
        int                 ptID = cell->GetPointId(down == true ? 1 : 0);
        vtkSmartPointer<vtkIdList> cellIDs = vtkSmartPointer<vtkIdList>::New();
        _poly->GetPointCells(ptID, cellIDs);
        int cellsNum = cellIDs->GetNumberOfIds();

        /// 相交即返回情况判定
        if (endWithCross && cellsNum > 2) { return idAngles.keys(); }

        for (int iCell = 0; iCell < cellsNum; ++iCell)
        {
            int findCellID = cellIDs->GetId(iCell);

            /// 该单元为当前查找单元，跳过查找下一个
            if (findCellID == cellID) { continue; }

            vtkCell* cell = _poly->GetCell(findCellID);
            int      findPt0ID = cell->GetPointId(0);
            int      findPt1ID = cell->GetPointId(1);

            /// 去除反向单元
            if ((findPt1ID == ptID && down) || (findPt0ID == ptID && !down)) { continue; }

            /// 构造查找到的线单元向量
            double vec2[3] = { .0 };
            getLineItemVector(findCellID, vec2);

            /// 计算两个线单元夹角，不考虑矢量方向，按照最小夹角进行数据记录
            double currentAngle = vtkMath::Dot(vec1, vec2) / (vtkMath::Norm(vec1) * vtkMath::Norm(vec2));
            currentAngle = acos(currentAngle) * 180 / M_PI;
            if (currentAngle <= angle)
            {
                idAngles.insert(findCellID, currentAngle);
            }
        }
        return idAngles.keys();
    }

    int FITKPolyDataTool::getPointIDByPointPosition(double pos[3])
    {
        QVector3D pt(pos[0], pos[1], pos[2]);
        return _points.key(pt);
    }


    QVector< int > FITKPolyDataTool::reuduceSharePointLine(QVector< int > cellIDs)
    {
        QVector< int >             results;
        QList< QPair< int, int > > pairs1;
        QList< int >               pairs2;

        for (int current : cellIDs)
        {
            /// 已与前序线单元构成配对
            if (pairs2.contains(current)) { continue; }

            int p1Index = _poly->GetCell(current)->GetPointId(0);
            int p2Index = _poly->GetCell(current)->GetPointId(1);
            bool getCurrentPair = false;

            for (int other : cellIDs)
            {
                if (current == other) { continue; }
                int p3Index = _poly->GetCell(other)->GetPointId(0);
                int p4Index = _poly->GetCell(other)->GetPointId(1);

                /// 两条边共用节点，节点序号相反，其中一个要被去掉
                if (p1Index == p4Index && p2Index == p3Index)
                {
                    QPair< int, int > pair(current, other);
                    pairs1.append(pair);
                    pairs2.append(current);
                    pairs2.append(other);
                    getCurrentPair = true;
                    results.append(current);
                    break;
                }
            }
            if (!getCurrentPair)
            {
                results.append(current);
            }
        }
        return results;
    }

    QList< CELLPAIR > FITKPolyDataTool::getNeighborCell(int cellID/*, const QList< int >& scanedCellIDs*/)
    {
        QList< CELLPAIR > neighcells;
        vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
        _poly->GetCellPoints(cellID, cellPointIds);

        QList< int > sigNeighbor{};
        for (vtkIdType i = 0; i < cellPointIds->GetNumberOfIds(); i++)
        {
            vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
            idList->InsertNextId(cellPointIds->GetId(i));
            // get the neighbors of the cell
            vtkSmartPointer<vtkIdList> neighborCellIds = vtkSmartPointer<vtkIdList>::New();
            _poly->GetCellNeighbors(cellID, idList, neighborCellIds);

            for (vtkIdType j = 0; j < neighborCellIds->GetNumberOfIds(); j++)
            {
                const int roundID = neighborCellIds->GetId(j);

                // Modified by CHT 不通过数据包含关系判断，改用数组直接获取标识。
                //@{
                int flag = m_scanCellCoPlaneFlag->GetValue(roundID);
                if (flag || sigNeighbor.contains(roundID)) continue;
                //@}

                // if (scanedCellIDs.contains(roundID) || sigNeighbor.contains(roundID)) continue;
                neighcells.append(CELLPAIR(roundID, cellID));
                sigNeighbor.append(roundID);
            }
        }
        return neighcells;
    }

    /*bool FITKPolyDataTool::calculateCellNormal(const int cellID, double* normal)
    {
        if (_poly == nullptr) return false;
        vtkCell* cell = _poly->GetCell(cellID);
        if (cell == nullptr) return false;

        const int nedge = cell->GetNumberOfEdges();
        if (nedge <= 2) return false;

        auto calEdgeDirection = [&](int p1, int p2, double* dir) -> bool {
            if (dir == nullptr) return false;

            double pcoor1[3] = { 0 }, pcoor2[3] = { 0 };
            _poly->GetPoint(p1, pcoor1);
            _poly->GetPoint(p2, pcoor1);
            vtkMath::Subtract(pcoor2, pcoor1, dir);

            return vtkMath::Norm(dir) > 1e-7;
        };

        const int np = cell->GetNumberOfPoints();
        if (np < 3) return false;
        const int p1 = cell->GetPointId(0);
        const int p2 = cell->GetPointId(1);
        const int p3 = cell->GetPointId(np - 1);

        double dir0[3] = { 0 }, dir1[3] = { 0 };
        bool   ok = calEdgeDirection(p1, p2, dir0);
        if (!ok) return false;
        ok = calEdgeDirection(p2, p3, dir1);
        if (!ok) return false;

        vtkMath::Cross(dir0, dir1, normal);
        return true;
    }*/

    bool FITKPolyDataTool::calculateCellNormal(const int cellID, double* normal)
    {
        vtkCell* cell = _poly->GetCell(cellID);
        if (cell == nullptr) return false;
        vtkPolygon::ComputeNormal(cell->GetPoints(), normal); 
        return true;
    }

    void FITKPolyDataTool::getLineItemVector(int cellID, double(&vec)[3])
    {
        vtkCell* cell = _poly->GetCell(cellID);
        if (!cell || cell->GetCellType() != VTK_LINE) { return; }

        double p1[3] = { 0 }, p2[3] = { 0 };
        int    pt1ID = cell->GetPointId(0);
        int    pt2ID = cell->GetPointId(1);
        _poly->GetPoint(pt1ID, p1);
        _poly->GetPoint(pt2ID, p2);

        for (int iCnt = 0; iCnt < 3; ++iCnt)
        {
            vec[iCnt] = p2[iCnt] - p1[iCnt];
        }
    }

}  // namespace Common
