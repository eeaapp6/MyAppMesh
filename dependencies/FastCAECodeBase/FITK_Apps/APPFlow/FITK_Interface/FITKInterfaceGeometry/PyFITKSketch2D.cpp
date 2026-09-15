/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PyFITKSketch2D.h"
#include "FITKGeoInterfaceFactory.h"
#include "FITKGeoCommandList.h"
#include "FITKAbsGeoModelPoint.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITKAbsGeoSketchDeleter.h"

#include <math.h>
#include <iostream>

namespace Interface
{
    PyFITKSketch2D::PyFITKSketch2D(int type)
    {
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        _sketch2D = fac->createCommandT<Interface::FITKAbsGeoSketch2D>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D);
        _sketch2DId = _sketch2D->getDataObjectID();
        if (_sketch2D)
            _sketch2D->setUserFor(static_cast<FITKAbsGeoSketch2D::SketchUseFor>(type));
    }

    PyFITKSketch2D::~PyFITKSketch2D()
    {
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return;
        auto cmd = cmdList->getDataByID(getSketchID());
        /*调用析构时，如果草绘没添加到命令列表中，代表草绘未使用，可以删除草绘对象*/
        if (!cmd) {
            if (_sketch2D) {
                delete _sketch2D;
                _sketch2D = nullptr;
            }
        }
    }

    bool PyFITKSketch2D::setPlane(const QVector<double>& pos_nor_up)
    {
        if (!_sketch2D || !isUsable())return false;
        if (pos_nor_up.size() != 9) return false;
        double pos[3] = { pos_nor_up[0], pos_nor_up[1], pos_nor_up[2] };
        double nor[3] = { pos_nor_up[3], pos_nor_up[4], pos_nor_up[5] };
        double up[3] = { pos_nor_up[6], pos_nor_up[7], pos_nor_up[8] };
        _sketch2D->setPlane(pos, nor, up);
        return true;
    }

    bool PyFITKSketch2D::addDraw(int type, const QVector<double>& points)
    {
        if (!_sketch2D )return false;

        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!fac) return false;
        switch (type)
        {
        case SS_Vertex:
        {
            auto coordPointCmd = fac->createCommandT<Interface::FITKAbsGeoModelPoint>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPoint);
            coordPointCmd->setCoord(points.at(0), points.at(1), points.at(2));
            if (!_sketch2D->addAndUpdate(coordPointCmd)) return false;
            break;
        }
        case SS_Wire:
        {
            // 顺序点
            if (points.size() == 6) {
                auto segmentCmd = fac->createCommandT<Interface::FITKAbsGeoModelSegment>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSegment);
                segmentCmd->setStartPoint(points.at(0), points.at(1), points.at(2));
                segmentCmd->setEndPoint(points.at(3), points.at(4), points.at(5));
                if (!_sketch2D->addAndUpdate(segmentCmd)) return false;
                _tempLastPoint = { points.at(3), points.at(4), points.at(5) };
            }
            else if (_tempLastPoint.size() == 3 && points.size() == 3) {
                auto segmentCmd = fac->createCommandT<Interface::FITKAbsGeoModelSegment>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSegment);
                segmentCmd->setStartPoint(_tempLastPoint.at(0), _tempLastPoint.at(1), _tempLastPoint.at(2));
                segmentCmd->setEndPoint(points.at(0), points.at(1), points.at(2));
                if (!_sketch2D->addAndUpdate(segmentCmd)) return false;
                _tempLastPoint = { points.at(0), points.at(1), points.at(2) };
            }
            break;
        }
        case SS_Rect:
            // 四个顶点
            if (points.size() == 12) {
                auto rectWireCmd = fac->createCommandT<Interface::FITKAbsGeoModelWire>(Interface::FITKGeoEnum::FITKGeometryComType::FGTWire);
                rectWireCmd->addPoint(points.at(0), points.at(1), points.at(2));
                rectWireCmd->addPoint(points.at(3), points.at(4), points.at(5));
                rectWireCmd->addPoint(points.at(6), points.at(7), points.at(8));
                rectWireCmd->addPoint(points.at(9), points.at(10), points.at(11));
                rectWireCmd->addPoint(points.at(0), points.at(1), points.at(2));
                if (!_sketch2D->addAndUpdate(rectWireCmd)) return false;
            }
            break;
        case SS_Circle:
            // 圆心、圆上任意一点
            if (points.size() == 6) {
                auto circleCmd = fac->createCommandT<Interface::FITKAbsGeoModelCircle>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCircle);
                double pos[3], nor[3], up[3];
                _sketch2D->getPlane(pos, nor, up);
                circleCmd->setYDirection(up[0], up[1], up[2]);
                circleCmd->setZDirection(nor[0], nor[1], nor[2]);
                circleCmd->setOrigin(points.at(0), points.at(1), points.at(2));
                circleCmd->setRadius(std::sqrt(std::pow(points.at(3) - points.at(0), 2) + std::pow(points.at(4) - points.at(1), 2) + std::pow(points.at(5) - points.at(2), 2)));
                if (!_sketch2D->addAndUpdate(circleCmd)) return false;
            }
            break;
        case SS_Ellipse:
            // 中心，长短轴
            if (points.size() == 9) {
                auto ellipseCmd = fac->createCommandT<Interface::FITKAbsGeoModelEllipse>(Interface::FITKGeoEnum::FITKGeometryComType::FGTEllipse);
                double pos[3], nor[3], up[3];
                _sketch2D->getPlane(pos, nor, up);
                ellipseCmd->setZDirection(nor[0], nor[1], nor[2]);
                ellipseCmd->setOrigin(points.at(0), points.at(1), points.at(2));
                auto radius1 = std::sqrt(std::pow(points.at(3) - points.at(0), 2) + std::pow(points.at(4) - points.at(1), 2) + std::pow(points.at(5) - points.at(2), 2));
                auto radius2 = std::sqrt(std::pow(points.at(6) - points.at(0), 2) + std::pow(points.at(7) - points.at(1), 2) + std::pow(points.at(8) - points.at(2), 2));
                if (radius1 > radius2) {
                    ellipseCmd->setMajorRadius(radius1);
                    ellipseCmd->setMinorRadius(radius2);
                    ellipseCmd->setYDirection(points.at(6) - points.at(0), points.at(7) - points.at(1), points.at(8) - points.at(2));
                }
                else {
                    ellipseCmd->setMajorRadius(radius2);
                    ellipseCmd->setMinorRadius(radius1);
                    ellipseCmd->setYDirection(points.at(3) - points.at(0), points.at(4) - points.at(1), points.at(5) - points.at(2));
                }
                if (!_sketch2D->addAndUpdate(ellipseCmd)) return false;
            }
            break;
        case SS_ArcOfCircle:
        case SS_ArcOfTan:
        case SS_ArcOfP3:
            // 圆心、圆弧上的起始点、终止点
            if (points.size() == 9) {
                auto arcCmd = fac->createCommandT<Interface::FITKAbsGeoModelCircleArc>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCircleArc);
                double pos[3], nor[3], up[3];
                _sketch2D->getPlane(pos, nor, up);
                arcCmd->setYDirection(up[0], up[1], up[2]);
                arcCmd->setZDirection(nor[0], nor[1], nor[2]);
                arcCmd->setOrigin(points.at(0), points.at(1), points.at(2));
                arcCmd->setRadius(std::sqrt(std::pow(points.at(3) - points.at(0), 2) + std::pow(points.at(4) - points.at(1), 2) + std::pow(points.at(5) - points.at(2), 2)));
                arcCmd->setStartPoint(points.at(3), points.at(4), points.at(5));
                arcCmd->setEndPoint(points.at(6), points.at(7), points.at(8));
                if (!_sketch2D->addAndUpdate(arcCmd)) return false;
            }
            break;
        case SS_Spline:
            // 圆心、圆弧上的起始点、终止点
            if (points.size() >= 9) {
                auto splineCmd = fac->createCommandT<Interface::FITKAbsGeoModelBsplineByThroughPoints>(Interface::FITKGeoEnum::FITKGeometryComType::FGTBSplineByThroughPoints);
                double pos[3], nor[3], up[3];
                _sketch2D->getPlane(pos, nor, up);
                splineCmd->setYDirection(up[0], up[1], up[2]);
                splineCmd->setZDirection(nor[0], nor[1], nor[2]);
                splineCmd->setOrigin(points.at(0), points.at(1), points.at(2));
                auto nbPoints = points.size() / 3;
                for (int i = 0; i < nbPoints; ++i) {
                    splineCmd->addThroughPoint(points.at(i * 3), points.at(i * 3 + 1), points.at(i * 3 + 2));
                }
                if (!_sketch2D->addAndUpdate(splineCmd)) return false;
            }
            break;
        case SS_ConLine:
        case SS_ConHLine:
        case SS_ConVLine:
        case SS_ConAngleLine:
            // 顺序点
            if (points.size() == 6)
            {
                // 获取构造线管理器。
                Interface::FITKConstructionManager* consMgr = _sketch2D->getConstructionManager();
                if (!consMgr)return false;

                // 创建构造线。
                Interface::FITKAbsConstruction* consLine = new Interface::FITKAbsConstruction(Interface::FITKGeoEnum::FCTLine);
                consLine->setPosition1(points.at(0), points.at(1), points.at(2));
                consLine->setPosition2(points.at(3), points.at(4), points.at(5));
                consMgr->appendDataObj(consLine);
            }
            else return false;
            break;
        default:
            break;
        }
        return true;
    }

    int PyFITKSketch2D::getSketchID()
    {
        if(!_sketch2D)return -1;
        return _sketch2D->getDataObjectID();
    }

    bool PyFITKSketch2D::setCurrentSketch(const QString& name)
    {
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return false;
        auto cmd = cmdList->getDataByID(getSketchID());
        /*如果草绘没添加到命令列表中，代表草绘未使用，删除原始草绘对象*/
        if (!cmd) {
            if (_sketch2D) 
                delete _sketch2D;
        }
        //查找待设置草图是否存在
        cmd = cmdList->getDataByName(name);
        if (!cmd)return false;
        Interface::FITKAbsGeoSketch2D* sketch2D = dynamic_cast<Interface::FITKAbsGeoSketch2D*>(cmd);
        if (!sketch2D)return false;
        _sketch2D = sketch2D;
        return true;
    }

    Interface::FITKAbsGeoCommand * PyFITKSketch2D::getSketch()
    {
        return _sketch2D;
    }

    bool PyFITKSketch2D::checkSketch()
    {
        if (!_sketch2D)return false;
        bool ok1 = !_sketch2D->isSelfIntersection();
        bool ok2 = _sketch2D->isClosed();
        return ok1 && ok2;
    }

    bool PyFITKSketch2D::eraserEdge(int num, const QList<int>& indexs)
    {
        if (!_sketch2D|| !isUsable())return false;
        QList<Interface::VirtualShape> shapes = getShapes(num,indexs, Interface::FITKGeoEnum::VSEdge);
        if(shapes.isEmpty())return false;
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto sketchDeleterCmd = fac->createCommandT<Interface::FITKAbsGeoSketchDeleter>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSketchDeleter);
        sketchDeleterCmd->setToBeRemovedEntities(shapes);
        if (!_sketch2D->addAndUpdate(sketchDeleterCmd)) return false;
        return true;
    }

    QList<Interface::VirtualShape> PyFITKSketch2D::getShapes(int num, const QList<int>& indexs, Interface::FITKGeoEnum::VTopoShapeType type)
    {
        QList<Interface::VirtualShape> virtualshapes{};
        if (!_sketch2D) return virtualshapes;
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return virtualshapes;
        Interface::FITKAbsGeoCommand* shapeCmd = _sketch2D->getDataByIndex(num);
        if (!shapeCmd)return virtualshapes;
        cmdList->appendDataObj(shapeCmd);
        auto vmgr = shapeCmd->getShapeVirtualTopoManager(type);
        if (!vmgr) return virtualshapes;

        for (auto index : indexs) {
            int shapeid = vmgr->getIDByIndex(index);
            Interface::VirtualShape shape = Interface::VirtualShape(shapeCmd->getDataObjectID(), shapeid, index);
            virtualshapes.append(shape);
        }
        return virtualshapes;
    }

    bool PyFITKSketch2D::eraserConstructions(const QList<int>& indexs)
    {
        if (!_sketch2D || !isUsable())return false;
        Interface::FITKConstructionManager* consMgr = _sketch2D->getConstructionManager();
        if (!consMgr) return false;

        for (const int & index : indexs)
        {
            consMgr->removeDataByIndex(index);
        }
        return true;
    }

    bool PyFITKSketch2D::isUsable()
    {
        if(!_sketch2D) return false;
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return false;
        auto cmd = cmdList->getDataByID(_sketch2D->getDataObjectID());
        if(cmd)
            return false;
        return true;
    }

    void PyFITKSketch2D::sketchToNull(int id) {
        if(_sketch2DId == id)
            _sketch2D = nullptr;
    }
}