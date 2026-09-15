/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFBlockMeshDictWriter.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeBox.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeSphere.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeCylinder.h"
#include <QFileInfo>
#include <QtMath>
//命名空间
namespace IO
{
    FITKOFBlockMeshDictWriter::FITKOFBlockMeshDictWriter()
    {
    }
    FITKOFBlockMeshDictWriter::~FITKOFBlockMeshDictWriter()
    {
    }

    void FITKOFBlockMeshDictWriter::setFilePath(const QString& filePath)
    {
        FITKOFAbstractDictWriter::setFilePath(filePath);
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        m_fileName = QString("%1/system/blockMeshDict").arg(filePath);
    }

    bool FITKOFBlockMeshDictWriter::run()
    {
        //判断文件是否存在
        if (m_fileName.isEmpty())return false;
        //获取网格生成单例
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        //获取区域网格尺寸管理器
        Interface::FITKRegionMeshSizeManager* meshRegionManager = meshGen->getRegionMeshSizeMgr();
        if (!meshRegionManager)return false;
        //{
        //    //插入数据
        //    Interface::FITKRegionMeshSizeBox* meshSizeData = new Interface::FITKRegionMeshSizeBox;
        //    meshRegionManager->appendDataObj(meshSizeData);
        //}
        if (meshRegionManager->getDataCount() == 0)return false;
        //获取区域网格尺寸
        m_regionMeshSize = meshRegionManager->getDataByIndex(0);
        //获取字典工程
        this->openDictObject();
        if (!m_objectDict)return false;
        //写出FoamFile
        this->dictObjectFoamFile();
        //设置写出的网格区域数据
        bool Ok = this->blockMeshRegionMeshW();
        //写出字典文件
        if (Ok)
            Ok = this->writeDictFile();

        //关闭字典工程
        this->closeDictObject();
        return Ok;
    }

    QString FITKOFBlockMeshDictWriter::getFileName()
    {
        return m_fileName;
    }

    void FITKOFBlockMeshDictWriter::dictObjectFoamFile()
    {
        if (!m_objectDict)return;
        //FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "dictionary");
        group->append("location", "\"system\"");
        group->append("object", "blockMeshDict");
        m_objectDict->append("FoamFile", group);
    }

    bool FITKOFBlockMeshDictWriter::blockMeshRegionMeshW()
    {
        if (!m_regionMeshSize)return false;
        bool ok = false;
        //获取区域类型
        Interface::FITKAbstractRegionMeshSize::RegionType type = m_regionMeshSize->getRegionType();
        if (type == Interface::FITKAbstractRegionMeshSize::RegionType::RegionBox)
        {
            ok = this->blockMeshRegionMeshBox();
        }
        else if (type == Interface::FITKAbstractRegionMeshSize::RegionType::RegionCylinder)
        {
            ok = this->blockMeshRegionMeshCylinder();
        }
        else if (type == Interface::FITKAbstractRegionMeshSize::RegionType::RegionSphere)
        {
            ok = this->blockMeshRegionMeshSphere();
        }

        return ok;
    }

    bool FITKOFBlockMeshDictWriter::blockMeshRegionMeshBox()
    {
        Interface::FITKRegionMeshSizeBox* box = dynamic_cast<Interface::FITKRegionMeshSizeBox*>(m_regionMeshSize);
        if (box == nullptr)return false;
        //vertices
        if (!this->vertexBox())return false;
        //获取离散数据
        int division[3]{ box->getDivision(0), box->getDivision(1), box->getDivision(2) };
        double grading[3]{ box->getGrading(0), box->getGrading(1), box->getGrading(2) };
        QString divisionStr = QString("%1 %2 %3").arg(division[0]).arg(division[1]).arg(division[2]);
        if (fabsl(grading[0]) < 1e-16 || fabsl(grading[1]) < 1e-16 || fabsl(grading[2]) < 1e-16)return false;
        QString gradingStr = QString("%1 %2 %3").arg(grading[0]).arg(grading[1]).arg(grading[2]);
        //blocks
        DICT::FITKDictArray* blocks = new DICT::FITKDictArray;
        blocks->append(QStringList() << "(0 1 2 3 4 5 6 7)" << QString("(%1)").arg(divisionStr) << "simpleGrading" << QString("(%1)").arg(gradingStr), "hex");
        m_objectDict->append("blocks", blocks);
        //edges
        DICT::FITKDictArray* edges = new DICT::FITKDictArray;
        m_objectDict->append("edges", edges);
        //boundary
        if (!this->boundaryBox())return false;
        return true;
    }

    bool FITKOFBlockMeshDictWriter::vertexBox()
    {
        Interface::FITKRegionMeshSizeBox* box = dynamic_cast<Interface::FITKRegionMeshSizeBox*>(m_regionMeshSize);
        if (box == nullptr)return false;
        double mixPoint[3], length[3];
        box->getPoint1(mixPoint);
        box->getLength(length);
        double maxPoint[3]{mixPoint[0] + length[0] , mixPoint[1] + length[1] , mixPoint[2] + length[2] };
        //区域点数据vertices
        DICT::FITKDictArray* vertex = new DICT::FITKDictArray;
        //8个点数据
        DICT::FITKDictVector* point1 = new DICT::FITKDictVector;
        point1->append(mixPoint[0]);
        point1->append(mixPoint[1]);
        point1->append(mixPoint[2]);
        DICT::FITKDictVector* point2 = new DICT::FITKDictVector;
        point2->append(maxPoint[0]);
        point2->append(mixPoint[1]);
        point2->append(mixPoint[2]);
        DICT::FITKDictVector* point3 = new DICT::FITKDictVector;
        point3->append(maxPoint[0]);
        point3->append(maxPoint[1]);
        point3->append(mixPoint[2]);
        DICT::FITKDictVector* point4 = new DICT::FITKDictVector;
        point4->append(mixPoint[0]);
        point4->append(maxPoint[1]);
        point4->append(mixPoint[2]);
        DICT::FITKDictVector* point5 = new DICT::FITKDictVector;
        point5->append(mixPoint[0]);
        point5->append(mixPoint[1]);
        point5->append(maxPoint[2]);
        DICT::FITKDictVector* point6 = new DICT::FITKDictVector;
        point6->append(maxPoint[0]);
        point6->append(mixPoint[1]);
        point6->append(maxPoint[2]);
        DICT::FITKDictVector* point7 = new DICT::FITKDictVector;
        point7->append(maxPoint[0]);
        point7->append(maxPoint[1]);
        point7->append(maxPoint[2]);
        DICT::FITKDictVector* point8 = new DICT::FITKDictVector;
        point8->append(mixPoint[0]);
        point8->append(maxPoint[1]);
        point8->append(maxPoint[2]);
        //插入8个点数据
        vertex->append(point1);
        vertex->append(point2);
        vertex->append(point3);
        vertex->append(point4);
        vertex->append(point5);
        vertex->append(point6);
        vertex->append(point7);
        vertex->append(point8);
        //插入vertices
        m_objectDict->append("vertices", vertex);
        return true;
    }

    bool FITKOFBlockMeshDictWriter::boundaryBox()
    {
        Interface::FITKRegionMeshSizeBox* box = dynamic_cast<Interface::FITKRegionMeshSizeBox*>(m_regionMeshSize);
        if (box == nullptr)return false;
        //类型
        QHash<Interface::FITKAbstractRegionMeshSize::BoundaryType, QString> strBoundaryType;
        strBoundaryType.insert(Interface::FITKAbstractRegionMeshSize::BoundaryType::BTWall, "wall");
        strBoundaryType.insert(Interface::FITKAbstractRegionMeshSize::BoundaryType::BTPatch, "patch");
        strBoundaryType.insert(Interface::FITKAbstractRegionMeshSize::BoundaryType::BTSymmetry, "symmetry");
        strBoundaryType.insert(Interface::FITKAbstractRegionMeshSize::BoundaryType::BTEmpty, "empty");
        //区域边界
        DICT::FITKDictArray* boundary = new DICT::FITKDictArray;
        //插入boundary
        m_objectDict->append("boundary", boundary);

        QHash<QString, DICT::FITKDictGroup*> groupHash;
        for (int i = 0; i < 6; ++i)
        {
            int index = i;
            //获取类型
            Interface::FITKAbstractRegionMeshSize::BoundaryType type = box->getBoundary(index);
            //获取类型名称
            QString boundaryStr = box->getFaceName(i);
            DICT::FITKDictGroup* group = nullptr;
            if (groupHash.contains(boundaryStr))
                group = groupHash.value(boundaryStr);
            else
            {
                group = new DICT::FITKDictGroup;
                group->append("type", strBoundaryType.value(type));
                group->append("faces", new DICT::FITKDictArray);
                boundary->append(group, boundaryStr);
                groupHash.insert(boundaryStr, group);
            }
            if (!group)return false;
            DICT::FITKDictValue* value = group->value("faces");
            if (!value)return false;
            bool ok = false;
            DICT::FITKDictArray* face = value->toArray(ok);
            if (!face)return false;
            DICT::FITKDictVector* vector = new DICT::FITKDictVector;
            this->vectorDataBoxByFaceIndex(vector, index);
            face->append(vector);
        }
        return true;
    }

    bool FITKOFBlockMeshDictWriter::vectorDataBoxByFaceIndex(DICT::FITKDictVector* face, int index)
    {
        if (!face) return false;
        if (index == 0)
        {
            face->append(0);
            face->append(4);
            face->append(7);
            face->append(3);
        }
        else if (index == 1)
        {
            face->append(2);
            face->append(6);
            face->append(5);
            face->append(1);
        }
        else if (index == 2)
        {
            face->append(1);
            face->append(5);
            face->append(4);
            face->append(0);
        }
        else if (index == 3)
        {
            face->append(3);
            face->append(7);
            face->append(6);
            face->append(2);
        }
        else if (index == 4)
        {
            face->append(0);
            face->append(3);
            face->append(2);
            face->append(1);
        }
        else if (index == 5)
        {
            face->append(4);
            face->append(5);
            face->append(6);
            face->append(7);
        }
        return true;
    }

    bool FITKOFBlockMeshDictWriter::blockMeshRegionMeshCylinder()
    {
        Interface::FITKRegionMeshSizeCylinder* cylinder = dynamic_cast<Interface::FITKRegionMeshSizeCylinder*>(m_regionMeshSize);
        if (cylinder == nullptr)return false;
        //vertices
        if (!this->vertexCylinder())return false;
        //获取离散数据
        int division[3]{ cylinder->getDivision(0), cylinder->getDivision(1), cylinder->getDivision(2) };
        double grading[3]{ cylinder->getGrading(0), cylinder->getGrading(1), 1 };
        if (fabsl(grading[0]) < 1e-16 || fabsl(grading[1]) < 1e-16)return false;
        double axialGrad = qPow(grading[1], 4);
        //blocks
        DICT::FITKDictArray* blocks = new DICT::FITKDictArray;
        blocks->append(QStringList() << QString("(0 1 2 3 4 5 6 7) (%1 %2 %3) simpleGrading (1 %4 1)").arg(division[2]).arg(division[1]).arg(division[2]).arg(axialGrad), "hex");
        blocks->append(QStringList() << QString("(1 0 3 2 9 8 11 10) (%1 %2 %3) simpleGrading (1 %4 %5)").arg(division[2]).arg(division[1]).arg(division[0]).arg(axialGrad).arg(grading[0]), "hex");
        blocks->append(QStringList() << QString("(5 1 2 6 13 9 10 14) (%1 %2 %3) simpleGrading (1 %4 %5)").arg(division[2]).arg(division[1]).arg(division[0]).arg(axialGrad).arg(grading[0]), "hex");
        blocks->append(QStringList() << QString("(4 5 6 7 12 13 14 15) (%1 %2 %3) simpleGrading (1 %4 %5)").arg(division[2]).arg(division[1]).arg(division[0]).arg(axialGrad).arg(grading[0]), "hex");
        blocks->append(QStringList() << QString("(0 4 7 3 8 12 15 11) (%1 %2 %3) simpleGrading (1 %4 %5)").arg(division[2]).arg(division[1]).arg(division[0]).arg(axialGrad).arg(grading[0]), "hex");
        m_objectDict->append("blocks", blocks);

        //edges
        if (!this->edgesCylinder())return false;
        
        //boundary
        if (!this->boundaryCylinder())return false;
        return true;
    }

    void FITKOFBlockMeshDictWriter::conversionglobalPoint(double point[], double global[])
    {
        Interface::FITKRegionMeshSizeCylinder* cylinder = dynamic_cast<Interface::FITKRegionMeshSizeCylinder*>(m_regionMeshSize);
        if (cylinder == nullptr)return;
        //获取原点坐标
        double originCoor[3];
        cylinder->getLocation(originCoor);
        //获取方向
        double direction[3];
        cylinder->getDirection(direction);
        double axis[6];
        cylinder->getPlaneAxis(axis);
        global[0] = originCoor[0] + axis[0] * point[0] + axis[3] * point[1] + direction[0] * point[2];
        global[1] = originCoor[1] + axis[1] * point[0] + axis[4] * point[1] + direction[1] * point[2];
        global[2] = originCoor[2] + axis[2] * point[0] + axis[5] * point[1] + direction[2] * point[2];
    }

    bool FITKOFBlockMeshDictWriter::vertexCylinder()
    {
        Interface::FITKRegionMeshSizeCylinder* cylinder = dynamic_cast<Interface::FITKRegionMeshSizeCylinder*>(m_regionMeshSize);
        if (cylinder == nullptr)return false;

        
        //获取长度
        double length = cylinder->getLength();
        //获取半径
        double r = cylinder->getRadius();
        //获取半径比例
        double fraction = cylinder->getRadialFraction();

        double bigLength = qSqrt(r * r / 2.0);
        double smallLength = bigLength * (1 - fraction);
        //获取局部坐标系
        double point1[3]{ (-smallLength), (-smallLength), 0 };
        double point2[3]{ (-smallLength), smallLength, 0 };
        double point3[3]{ (-smallLength), smallLength, length };
        double point4[3]{ (-smallLength), (-smallLength), length };
        double point5[3]{ smallLength, (-smallLength), 0 };
        double point6[3]{ smallLength, smallLength, 0 };
        double point7[3]{ smallLength, smallLength, length };
        double point8[3]{ smallLength, (-smallLength), length };
        double point9[3]{ (-bigLength), (-bigLength), 0 };
        double point10[3]{ (-bigLength), bigLength, 0 };
        double point11[3]{ (-bigLength), bigLength, length };
        double point12[3]{ (-bigLength), (-bigLength), length };
        double point13[3]{ bigLength, (-bigLength), 0 };
        double point14[3]{ bigLength, bigLength, 0 };
        double point15[3]{ bigLength, bigLength, length };
        double point16[3]{ bigLength, (-bigLength), length };
        //转化全局坐标系
        double global1[3]{ 0 }, global2[3]{ 0 }, global3[3]{ 0 }, global4[3]{ 0 }, global5[3]{ 0 }, global6[3]{ 0 }, global7[3]{ 0 }, global8[3]{ 0 };
        this->conversionglobalPoint(point1, global1);
        this->conversionglobalPoint(point2, global2);
        this->conversionglobalPoint(point3, global3);
        this->conversionglobalPoint(point4, global4);
        this->conversionglobalPoint(point5, global5);
        this->conversionglobalPoint(point6, global6);
        this->conversionglobalPoint(point7, global7);
        this->conversionglobalPoint(point8, global8);
        double global9[3]{ 0 }, global10[3]{ 0 }, global11[3]{ 0 }, global12[3]{ 0 }, global13[3]{ 0 }, global14[3]{ 0 }, global15[3]{ 0 }, global16[3]{ 0 };
        this->conversionglobalPoint(point9, global9);
        this->conversionglobalPoint(point10, global10);
        this->conversionglobalPoint(point11, global11);
        this->conversionglobalPoint(point12, global12);
        this->conversionglobalPoint(point13, global13);
        this->conversionglobalPoint(point14, global14);
        this->conversionglobalPoint(point15, global15);
        this->conversionglobalPoint(point16, global16);
        //区域点
        DICT::FITKDictArray* vertices = new DICT::FITKDictArray;
        //插入vertices
        m_objectDict->append("vertices", vertices);
        insertPoint(vertices, global1);
        insertPoint(vertices, global2);
        insertPoint(vertices, global3);
        insertPoint(vertices, global4);
        insertPoint(vertices, global5);
        insertPoint(vertices, global6);
        insertPoint(vertices, global7);
        insertPoint(vertices, global8);
        insertPoint(vertices, global9);
        insertPoint(vertices, global10);
        insertPoint(vertices, global11);
        insertPoint(vertices, global12);
        insertPoint(vertices, global13);
        insertPoint(vertices, global14);
        insertPoint(vertices, global15);
        insertPoint(vertices, global16);
        return true;
    }

    bool FITKOFBlockMeshDictWriter::edgesCylinder()
    {
        Interface::FITKRegionMeshSizeCylinder* cylinder = dynamic_cast<Interface::FITKRegionMeshSizeCylinder*>(m_regionMeshSize);
        if (cylinder == nullptr)return false;
        //获取长度
        double length = cylinder->getLength();
        //获取半径
        double r = cylinder->getRadius();
        //获取半径比例
        double fraction = cylinder->getRadialFraction();
        //局部坐标系上弧线的中点
        double bigLength = qSqrt(r * r / 2.0);
        double smallLength = bigLength * (1 - fraction);
        double coor = r + smallLength - qSqrt(r * r - smallLength * smallLength);

        double point1[3]{ (-coor), 0, 0 };//0 1
        double point2[3]{ 0, coor, 0 };//1 5
        double point3[3]{ coor, 0, 0 };//4 5
        double point4[3]{ 0, (-coor), 0 };//0 4
        double point5[3]{ (-coor), 0, length };//3 2
        double point6[3]{ 0, coor, length };//2 6
        double point7[3]{ coor, 0, length };//7 6
        double point8[3]{ 0, (-coor), length };//3 7

        double point9[3]{ (-r), 0, 0 };//8 9
        double point10[3]{ 0, r, 0 };//9 13
        double point11[3]{ r, 0, 0 };//12 13
        double point12[3]{ 0, (-r), 0 };// 8 12
        double point13[3]{ (-r), 0, length };//11 10
        double point14[3]{ 0, r, length };//10 14
        double point15[3]{ r, 0, length };//15 14
        double point16[3]{ 0, (-r), length };//11 15
        //转化全局坐标系
        double global1[3]{ 0 }, global2[3]{ 0 }, global3[3]{ 0 }, global4[3]{ 0 }, global5[3]{ 0 }, global6[3]{ 0 }, global7[3]{ 0 }, global8[3]{ 0 };
        this->conversionglobalPoint(point1, global1);
        this->conversionglobalPoint(point2, global2);
        this->conversionglobalPoint(point3, global3);
        this->conversionglobalPoint(point4, global4);
        this->conversionglobalPoint(point5, global5);
        this->conversionglobalPoint(point6, global6);
        this->conversionglobalPoint(point7, global7);
        this->conversionglobalPoint(point8, global8);
        double global9[3]{ 0 }, global10[3]{ 0 }, global11[3]{ 0 }, global12[3]{ 0 }, global13[3]{ 0 }, global14[3]{ 0 }, global15[3]{ 0 }, global16[3]{ 0 };
        this->conversionglobalPoint(point9, global9);
        this->conversionglobalPoint(point10, global10);
        this->conversionglobalPoint(point11, global11);
        this->conversionglobalPoint(point12, global12);
        this->conversionglobalPoint(point13, global13);
        this->conversionglobalPoint(point14, global14);
        this->conversionglobalPoint(point15, global15);
        this->conversionglobalPoint(point16, global16);
        //区域弧线
        DICT::FITKDictArray* edges = new DICT::FITKDictArray;
        m_objectDict->append("edges", edges);
        edges->append(QStringList() << QString("0 1 (%1 %2 %3)").arg(global1[0]).arg(global1[1]).arg(global1[2]), "arc");
        edges->append(QStringList() << QString("1 5 (%1 %2 %3)").arg(global2[0]).arg(global2[1]).arg(global2[2]), "arc");
        edges->append(QStringList() << QString("4 5 (%1 %2 %3)").arg(global3[0]).arg(global3[1]).arg(global3[2]), "arc");
        edges->append(QStringList() << QString("0 4 (%1 %2 %3)").arg(global4[0]).arg(global4[1]).arg(global4[2]), "arc");
        edges->append(QStringList() << QString("3 2 (%1 %2 %3)").arg(global5[0]).arg(global5[1]).arg(global5[2]), "arc");
        edges->append(QStringList() << QString("2 6 (%1 %2 %3)").arg(global6[0]).arg(global6[1]).arg(global6[2]), "arc");
        edges->append(QStringList() << QString("7 6 (%1 %2 %3)").arg(global7[0]).arg(global7[1]).arg(global7[2]), "arc");
        edges->append(QStringList() << QString("3 7 (%1 %2 %3)").arg(global8[0]).arg(global8[1]).arg(global8[2]), "arc");

        edges->append(QStringList() << QString("8 9 (%1 %2 %3)").arg(global9[0]).arg(global9[1]).arg(global9[2]), "arc");
        edges->append(QStringList() << QString("9 13 (%1 %2 %3)").arg(global10[0]).arg(global10[1]).arg(global10[2]), "arc");
        edges->append(QStringList() << QString("12 13 (%1 %2 %3)").arg(global11[0]).arg(global11[1]).arg(global11[2]), "arc");
        edges->append(QStringList() << QString("8 12 (%1 %2 %3)").arg(global12[0]).arg(global12[1]).arg(global12[2]), "arc");
        edges->append(QStringList() << QString("11 10 (%1 %2 %3)").arg(global13[0]).arg(global13[1]).arg(global13[2]), "arc");
        edges->append(QStringList() << QString("10 14 (%1 %2 %3)").arg(global14[0]).arg(global14[1]).arg(global14[2]), "arc");
        edges->append(QStringList() << QString("15 14 (%1 %2 %3)").arg(global15[0]).arg(global15[1]).arg(global15[2]), "arc");
        edges->append(QStringList() << QString("11 15 (%1 %2 %3)").arg(global16[0]).arg(global16[1]).arg(global16[2]), "arc");

        return true;
    }

    bool FITKOFBlockMeshDictWriter::boundaryCylinder()
    {
        Interface::FITKRegionMeshSizeCylinder* cylinder = dynamic_cast<Interface::FITKRegionMeshSizeCylinder*>(m_regionMeshSize);
        if (cylinder == nullptr)return false;
        //类型
        QHash<Interface::FITKAbstractRegionMeshSize::BoundaryType, QString> strBoundaryType;
        strBoundaryType.insert(Interface::FITKAbstractRegionMeshSize::BoundaryType::BTWall, "wall");
        strBoundaryType.insert(Interface::FITKAbstractRegionMeshSize::BoundaryType::BTPatch, "patch");
        strBoundaryType.insert(Interface::FITKAbstractRegionMeshSize::BoundaryType::BTSymmetry, "symmetry");
        strBoundaryType.insert(Interface::FITKAbstractRegionMeshSize::BoundaryType::BTEmpty, "empty");
        //区域边界
        DICT::FITKDictArray* boundary = new DICT::FITKDictArray;
        //插入boundary
        m_objectDict->append("boundary", boundary);
        //顺序链表
        QList<int> orderList;
        orderList << 2 << 0 << 1;

        QHash<QString, DICT::FITKDictGroup*> groupHash;
        for (int i = 0; i < 3; ++i)
        {
            int index = orderList.at(i);
            //获取类型
            Interface::FITKAbstractRegionMeshSize::BoundaryType type = cylinder->getBoundary(index);
            //获取类型名称
            QString boundaryStr = cylinder->getFaceName(i);
            DICT::FITKDictGroup* group = nullptr;
            if (groupHash.contains(boundaryStr))
                group = groupHash.value(boundaryStr);
            else
            {
                group = new DICT::FITKDictGroup;
                group->append("type", strBoundaryType.value(type));
                group->append("faces", new DICT::FITKDictArray);
                boundary->append(group, boundaryStr);
                groupHash.insert(boundaryStr, group);
            }
            if (!group)return false;
            DICT::FITKDictValue* value = group->value("faces");
            if (!value)return false;
            bool ok = false;
            DICT::FITKDictArray* face = value->toArray(ok);
            if (!face)return false;
            this->vectorDataCylinderByFaceIndex(face, index);
        }
        return true;
    }

    bool FITKOFBlockMeshDictWriter::vectorDataCylinderByFaceIndex(DICT::FITKDictArray* face, int index)
    {
        if (!face) return false;
        if (index == 0)
        {
            DICT::FITKDictVector* vector1 = new DICT::FITKDictVector;
            vector1->append(1);
            vector1->append(5);
            vector1->append(4);
            vector1->append(0);
            face->append(vector1);
            DICT::FITKDictVector* vector2 = new DICT::FITKDictVector;
            vector2->append(5);
            vector2->append(13);
            vector2->append(12);
            vector2->append(4);
            face->append(vector2);
            DICT::FITKDictVector* vector3 = new DICT::FITKDictVector;
            vector3->append(4);
            vector3->append(12);
            vector3->append(8);
            vector3->append(0);
            face->append(vector3);
            DICT::FITKDictVector* vector4 = new DICT::FITKDictVector;
            vector4->append(0);
            vector4->append(8);
            vector4->append(9);
            vector4->append(1);
            face->append(vector4);
            DICT::FITKDictVector* vector5 = new DICT::FITKDictVector;
            vector5->append(1);
            vector5->append(9);
            vector5->append(13);
            vector5->append(5);
            face->append(vector5);
        }
        else  if (index == 1)
        {
            DICT::FITKDictVector* vector1 = new DICT::FITKDictVector;
            vector1->append(3);
            vector1->append(7);
            vector1->append(6);
            vector1->append(2);
            face->append(vector1);
            DICT::FITKDictVector* vector2 = new DICT::FITKDictVector;
            vector2->append(6);
            vector2->append(14);
            vector2->append(10);
            vector2->append(2);
            face->append(vector2);
            DICT::FITKDictVector* vector3 = new DICT::FITKDictVector;
            vector3->append(2);
            vector3->append(10);
            vector3->append(11);
            vector3->append(3);
            face->append(vector3);
            DICT::FITKDictVector* vector4 = new DICT::FITKDictVector;
            vector4->append(3);
            vector4->append(11);
            vector4->append(15);
            vector4->append(7);
            face->append(vector4);
            DICT::FITKDictVector* vector5 = new DICT::FITKDictVector;
            vector5->append(7);
            vector5->append(15);
            vector5->append(14);
            vector5->append(6);
            face->append(vector5);
        }
        else  if (index == 2)
        {
            DICT::FITKDictVector* vector1 = new DICT::FITKDictVector;
            vector1->append(13);
            vector1->append(14);
            vector1->append(15);
            vector1->append(12);
            face->append(vector1);
            DICT::FITKDictVector* vector2 = new DICT::FITKDictVector;
            vector2->append(12);
            vector2->append(15);
            vector2->append(11);
            vector2->append(8);
            face->append(vector2);
            DICT::FITKDictVector* vector3 = new DICT::FITKDictVector;
            vector3->append(8);
            vector3->append(11);
            vector3->append(10);
            vector3->append(9);
            face->append(vector3);
            DICT::FITKDictVector* vector4 = new DICT::FITKDictVector;
            vector4->append(9);
            vector4->append(10);
            vector4->append(14);
            vector4->append(13);
            face->append(vector4);
        }
        return true;
    }

    bool FITKOFBlockMeshDictWriter::blockMeshRegionMeshSphere()
    {
        Interface::FITKRegionMeshSizeSphere* sphere = dynamic_cast<Interface::FITKRegionMeshSizeSphere*>(m_regionMeshSize);
        if (sphere == nullptr)return false;
        return true;
    }

    void FITKOFBlockMeshDictWriter::insertPoint(DICT::FITKDictArray* array, double point[])
    {
        //插入点数据
        DICT::FITKDictVector* vector = new DICT::FITKDictVector;
        array->append(vector);
        vector->append(point[0]);
        vector->append(point[1]);
        vector->append(point[2]);
    }
}



