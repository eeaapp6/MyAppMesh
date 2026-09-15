/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelTransform.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoTransformation.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelTransformPart.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

namespace Interface
{
    bool  FITKAdaptorGeoModelTrans::readParameters()
    {
        Interface::FITKAbsGeoModelTransform* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelTransform>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int sShape[3] = {};
        bool isCopy = false;
        double v3List[3] = {};
        //读取数据
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3))return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_IsCopy, &isCopy))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Vector, v3List, 1, 3)) return false;
        Interface::VirtualShape vShape(sShape[0], sShape[1], sShape[2]);
        cmd->setSourceShape(vShape);
        cmd->setCopy(isCopy);
        cmd->setVector(v3List[0], v3List[1], v3List[2]);
        return true;
    }
    bool FITKAdaptorGeoModelTrans::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelTransform* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelTransform>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool,H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if(!std::get<0>(paramGroup)) return false;
        int sShape[3] = {};
        sShape[0] = cmd->sourceShape().CmdId;
        sShape[1] = cmd->sourceShape().VirtualTopoId;
        sShape[2] = cmd->sourceShape().VirtualTopoIndex;
        bool isCopy = cmd->isCopy();

        double v3List[3] = {};
        auto v3 = cmd->vector();
        v3List[0] = v3[0];
        v3List[1] = v3[1];
        v3List[2] = v3[2];
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_IsCopy, isCopy);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Vector, v3List, 1, 3);

        return true;
    }

    bool  FITKAdaptorGeoModelTransformByTwoPoints::readParameters()
    {
        Interface::FITKAbsGeoModelTransformByTwoPoints* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelTransformByTwoPoints>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int sShape[3] = {};
        bool isCopy = false;
        double sPoint[3] = {};
        double ePoint[3] = {};
        //读取数据
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3))return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_IsCopy, &isCopy))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_SPoint, sPoint, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EPoint, ePoint, 1, 3)) return false;
        Interface::VirtualShape vShape(sShape[0], sShape[1], sShape[2]);
        cmd->setSourceShape(vShape);
        cmd->setCopy(isCopy);
        cmd->setStartPoint(sPoint[0], sPoint[1], sPoint[2]);
        cmd->setEndPoint(ePoint[0], ePoint[1], ePoint[2]);
        return true;
    }
    bool FITKAdaptorGeoModelTransformByTwoPoints::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelTransformByTwoPoints* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelTransformByTwoPoints>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        int sShape[3] = {};
        sShape[0] = cmd->sourceShape().CmdId;
        sShape[1] = cmd->sourceShape().VirtualTopoId;
        sShape[2] = cmd->sourceShape().VirtualTopoIndex;
        bool isCopy = cmd->isCopy();

        double sPoint[3] = {};
        double ePoint[3] = {};
        auto sPointArray = cmd->startPoint();
        sPoint[0] = sPointArray[0];
        sPoint[1] = sPointArray[1];
        sPoint[2] = sPointArray[2];
        auto ePointArray = cmd->endPoint();
        ePoint[0] = ePointArray[0];
        ePoint[1] = ePointArray[1];
        ePoint[2] = ePointArray[2];
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_IsCopy, isCopy);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_SPoint, sPoint, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EPoint, ePoint, 1, 3);
        return true;
    }

    bool  FITKAdaptorGeoModelTransformByDirAndDis::readParameters()
    {
        Interface::FITKAbsGeoModelTransformByDirAndDis* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelTransformByDirAndDis>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int sShape[3] = {};
        bool isCopy = false;
        double dirList[3] = {};
        double dis = 0;
        //读取数据
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3))return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_IsCopy, &isCopy))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Direction, dirList, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Distance, &dis)) return false;
        Interface::VirtualShape vShape(sShape[0], sShape[1], sShape[2]);
        cmd->setSourceShape(vShape);
        cmd->setCopy(isCopy);
        cmd->setDirection(dirList[0], dirList[1], dirList[2]);
        cmd->setDistance(dis);
        return true;
    }
    bool FITKAdaptorGeoModelTransformByDirAndDis::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelTransformByDirAndDis* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelTransformByDirAndDis>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        int sShape[3] = {};
        sShape[0] = cmd->sourceShape().CmdId;
        sShape[1] = cmd->sourceShape().VirtualTopoId;
        sShape[2] = cmd->sourceShape().VirtualTopoIndex;
        bool isCopy = cmd->isCopy();

        double dirList[3];
        auto dir = cmd->direction();
        dirList[0] = dir[0];
        dirList[1] = dir[1];
        dirList[2] = dir[2];
        double dis = cmd->distance();
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_IsCopy, isCopy);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Direction, dirList, 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Distance, dis);
        return true;
    }

    bool  FITKAdaptorGeoModelRotate::readParameters()
    {
        Interface::FITKAbsGeoModelRotate* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRotate>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int sShape[3] = {};
        bool isCopy = false;
        double sPoint[3] = {};
        double ePoint[3] = {};
        double degree = 0;
        double radian = 0;
        //读取数据
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3))return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_IsCopy, &isCopy))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_SPoint, sPoint, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EPoint, ePoint, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Degree, &degree)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Radian, &radian)) return false;
        Interface::VirtualShape vShape(sShape[0], sShape[1], sShape[2]);
        cmd->setSourceShape(vShape);
        cmd->setCopy(isCopy);
        cmd->setAxisStartPoint(sPoint[0], sPoint[1], sPoint[2]);
        cmd->setAxisEndPoint(ePoint[0], ePoint[1], ePoint[2]);
        cmd->setDegree(degree);
        cmd->setRadian(radian);
        return true;
    }
    bool FITKAdaptorGeoModelRotate::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelRotate* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRotate>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        int sShape[3] = {};
        sShape[0] = cmd->sourceShape().CmdId;
        sShape[1] = cmd->sourceShape().VirtualTopoId;
        sShape[2] = cmd->sourceShape().VirtualTopoIndex;
        bool isCopy = cmd->isCopy();

        double sPoint[3] = {};
        double ePoint[3] = {};
        auto sPointArray = cmd->axisStartPoint();
        sPoint[0] = sPointArray[0];
        sPoint[1] = sPointArray[1];
        sPoint[2] = sPointArray[2];
        auto ePointArray = cmd->axisEndPoint();
        ePoint[0] = ePointArray[0];
        ePoint[1] = ePointArray[1];
        ePoint[2] = ePointArray[2];
        double degree = cmd->degree();
        double radian = cmd->radian();
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_IsCopy, isCopy);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_SPoint, sPoint, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EPoint, ePoint, 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Degree, degree);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Radian, radian);
        return true;
    }

    bool  FITKAdaptorGeoModelScale::readParameters()
    {
        Interface::FITKAbsGeoModelScale* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelScale>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int sShape[3] = {};
        bool isCopy = false;
        double factors[3] = {};
        double basePoint[3] = {};

        //读取数据
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3))return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_IsCopy, &isCopy))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Factors, factors, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_BasePoint, basePoint, 1, 3)) return false;

        Interface::VirtualShape vShape(sShape[0], sShape[1], sShape[2]);
        cmd->setSourceShape(vShape);
        cmd->setCopy(isCopy);
        cmd->setFactors(factors[0], factors[1], factors[2]);
        cmd->setBasePoint(basePoint[0], basePoint[1], basePoint[2]);
        return true;
    }
    bool FITKAdaptorGeoModelScale::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelScale* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelScale>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        int sShape[3] = {};
        sShape[0] = cmd->sourceShape().CmdId;
        sShape[1] = cmd->sourceShape().VirtualTopoId;
        sShape[2] = cmd->sourceShape().VirtualTopoIndex;
        bool isCopy = cmd->isCopy();

        double factors[3] = {};
        double basePoint[3] = {};
        auto factorsArray = cmd->factors();
        factors[0] = factorsArray[0];
        factors[1] = factorsArray[1];
        factors[2] = factorsArray[2];
        auto basePointArray = cmd->basePoint();
        basePoint[0] = basePointArray[0];
        basePoint[1] = basePointArray[1];
        basePoint[2] = basePointArray[2];

        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_IsCopy, isCopy);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Factors, factors, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_BasePoint, basePoint, 1, 3);

        return true;
    }

    bool  FITKAdaptorGeoModelMirror::readParameters()
    {
        Interface::FITKAbsGeoModelMirror* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelMirror>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int sShape[3] = {};
        bool isCopy = false;
        int referenceShape[3] = {};

        //读取数据
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3))return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_IsCopy, &isCopy))return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ReferenceShape, referenceShape, 1, 3))return false;

        Interface::VirtualShape vShape(sShape[0], sShape[1], sShape[2]);
        Interface::VirtualShape rShape(referenceShape[0], referenceShape[1], referenceShape[2]);
        cmd->setSourceShape(vShape);
        cmd->setCopy(isCopy);
        cmd->setReference(rShape);
        return true;
    }
    bool FITKAdaptorGeoModelMirror::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelMirror* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelMirror>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        int sShape[3] = {};
        sShape[0] = cmd->sourceShape().CmdId;
        sShape[1] = cmd->sourceShape().VirtualTopoId;
        sShape[2] = cmd->sourceShape().VirtualTopoIndex;
        bool isCopy = cmd->isCopy();
        int referenceShape[3] = {};
        referenceShape[0] = cmd->reference().CmdId;
        referenceShape[1] = cmd->reference().VirtualTopoId;
        referenceShape[2] = cmd->reference().VirtualTopoIndex;
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_IsCopy, isCopy);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ReferenceShape, referenceShape, 1, 3);

        return true;
    }

    bool  FITKAdaptorGeoModelRectangularPattern::readParameters()
    {
        Interface::FITKAbsGeoModelRectangularPattern* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRectangularPattern>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int sShape[3] = {};
        bool isCopy = false;
        double firstDirection[3] = {};
        int firstCount = {};
        double firstSpacing = {};
        double firstTotal = {};
        double secondDirection[3] = {};
        int secondCount = {};
        double secondSpacing = {};
        double secondTotal = {};
        std::string firstType = {};
        std::string secondType = {};
        //读取数据
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3))return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_IsCopy, &isCopy))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_FirstDirection, firstDirection,1,3))return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_FirstCount, &firstCount))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_FirstSpacing, &firstSpacing))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_FirstTotal, &firstTotal))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_SecondDirection, secondDirection, 1, 3))return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_SecondCount, &secondCount))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_SecondSpacing, &secondSpacing))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_SecondTotal, &secondTotal))return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_FirstDefinition, firstType)) return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_SecondDefinition, secondType)) return false;
        Interface::VirtualShape vShape(sShape[0], sShape[1], sShape[2]);
        cmd->setSourceShape(vShape);
        cmd->setCopy(isCopy);
        cmd->setFirstDirection(firstDirection[0], firstDirection[1], firstDirection[2]);
        cmd->setFirstCount(firstCount);
        cmd->setFirstSpacing(firstSpacing);
        cmd->setFirstTotal(firstTotal);
        cmd->setSecondDirection(secondDirection[0], secondDirection[1], secondDirection[2]);
        cmd->setSecondCount(secondCount);
        cmd->setSecondSpacing(secondSpacing);
        cmd->setSecondTotal(secondTotal);
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKAbsGeoModelPattern::Definition>();
        Interface::FITKAbsGeoModelPattern::Definition cmdFirstType = (Interface::FITKAbsGeoModelPattern::Definition)metaEnum.keyToValue(firstType.c_str());
        Interface::FITKAbsGeoModelPattern::Definition cmdSecondType = (Interface::FITKAbsGeoModelPattern::Definition)metaEnum.keyToValue(secondType.c_str());
        cmd->setFirstDefinition(Interface::FITKAbsGeoModelPattern::Definition(cmdFirstType));
        cmd->setSecondDefinition(Interface::FITKAbsGeoModelPattern::Definition(cmdSecondType));
        return true;
    }
    bool FITKAdaptorGeoModelRectangularPattern::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelRectangularPattern* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRectangularPattern>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        int sShape[3] = {};
        sShape[0] = cmd->sourceShape().CmdId;
        sShape[1] = cmd->sourceShape().VirtualTopoId;
        sShape[2] = cmd->sourceShape().VirtualTopoIndex;
        bool isCopy = cmd->isCopy();
        std::array<double, 3> firstDirection = cmd->firstDirection();
        int firstCount = cmd->firstCount();
        double firstSpacing = cmd->firstSpacing();
        double firstTotal = cmd->firstTotal();
        std::array<double, 3> secondDirection = cmd->secondDirection();
        int secondCount = cmd->secondCount();
        double secondSpacing = cmd->secondSpacing();
        double secondTotal = cmd->secondTotal();
        FITKAbsGeoModelPattern::Definition firsttype = cmd->firstDefinition();
        FITKAbsGeoModelPattern::Definition secondtype = cmd->secondDefinition();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoModelPattern::Definition> enumFirst;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoModelPattern::Definition> enumSecond;
        QString cmdFirstTypeStr = enumFirst.toString(firsttype, ok);
        QString cmdSecondTypeStr = enumSecond.toString(secondtype, ok);
        if (!ok) return false;
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_IsCopy, isCopy);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_FirstDirection, firstDirection.data(), 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_FirstCount, firstCount);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_FirstSpacing, firstSpacing);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_FirstTotal, firstTotal);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_SecondDirection, secondDirection.data(), 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_SecondCount, secondCount);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_SecondSpacing, secondSpacing);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_SecondTotal, secondTotal);
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_FirstDefinition, cmdFirstTypeStr.toStdString());
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_SecondDefinition, cmdSecondTypeStr.toStdString());
        return true;
    }

    bool  FITKAdaptorGeoModelCircularPattern::readParameters()
    {
        Interface::FITKAbsGeoModelCircularPattern* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCircularPattern>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int sShape[3] = {};
        bool isCopy = false;
        int axisShape[3] = {};
        bool isReverse = false;
        int axisCount = {};
        double axisSpacingDegree = {};
        double axisTotalDegree = {};
        int crownCount = {};
        double crownSpacing = {};
        double crownTotal = {};
        std::string firstType = {};
        std::string secondType = {};
        //读取数据
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3))return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_IsCopy, &isCopy))return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_AxisShape, axisShape, 1, 3))return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_isReverse, &isReverse))return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_AxisCount, &axisCount))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_AxisSpacing, &axisSpacingDegree))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_AxisTotal, &axisTotalDegree))return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_CrownCount, &crownCount))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_CrownSpacing, &crownSpacing))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_CrownTotal, &crownTotal))return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_FirstDefinition, firstType)) return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_SecondDefinition, secondType)) return false;
        Interface::VirtualShape vShape(sShape[0], sShape[1], sShape[2]);
        Interface::VirtualShape aShape(axisShape[0], axisShape[1], axisShape[2]);
        cmd->setSourceShape(vShape);
        cmd->setCopy(isCopy);
        cmd->setAxis(aShape);
        cmd->reverse(isReverse);
        cmd->setAxisCount(axisCount);
        cmd->setAxisSpacing(axisSpacingDegree);
        cmd->setAxisTotal(axisTotalDegree);
        cmd->setCrownCount(crownCount);
        cmd->setCrownSpacing(crownSpacing);
        cmd->setCrownTotal(crownTotal);
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKAbsGeoModelPattern::Definition>();
        Interface::FITKAbsGeoModelPattern::Definition cmdFirstType = (Interface::FITKAbsGeoModelPattern::Definition)metaEnum.keyToValue(firstType.c_str());
        Interface::FITKAbsGeoModelPattern::Definition cmdSecondType = (Interface::FITKAbsGeoModelPattern::Definition)metaEnum.keyToValue(secondType.c_str());
        cmd->setFirstDefinition(Interface::FITKAbsGeoModelPattern::Definition(cmdFirstType));
        cmd->setSecondDefinition(Interface::FITKAbsGeoModelPattern::Definition(cmdSecondType));
        return true;
    }
    bool FITKAdaptorGeoModelCircularPattern::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelCircularPattern* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCircularPattern>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        int sShape[3] = {};
        sShape[0] = cmd->sourceShape().CmdId;
        sShape[1] = cmd->sourceShape().VirtualTopoId;
        sShape[2] = cmd->sourceShape().VirtualTopoIndex;
        bool isCopy = cmd->isCopy();
        int axisShape[3] = {};
        axisShape[0] = cmd->axis().CmdId;
        axisShape[1] = cmd->axis().VirtualTopoId;
        axisShape[2] = cmd->axis().VirtualTopoIndex;
        bool isReverse = cmd->isReverse();
        int axisCount = cmd->axisCount();
        double axisSpacingDegree = cmd->axisSpacing();
        double axisTotalDegree = cmd->axisTotal();
        int crownCount = cmd->crownCount();
        double crownSpacing = cmd->crownSpacing();
        double crownTotal = cmd->crownTotal();
        FITKAbsGeoModelPattern::Definition firsttype = cmd->firstDefinition();
        FITKAbsGeoModelPattern::Definition secondtype = cmd->secondDefinition();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoModelPattern::Definition> enumFirst;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoModelPattern::Definition> enumSecond;
        QString cmdFirstTypeStr = enumFirst.toString(firsttype, ok);
        QString cmdSecondTypeStr = enumSecond.toString(secondtype, ok);
        if (!ok) return false;
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_IsCopy, isCopy);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_AxisShape, axisShape, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_isReverse, isReverse);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_AxisCount, axisCount);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_AxisSpacing, axisSpacingDegree);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_AxisTotal, axisTotalDegree);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_CrownCount, crownCount);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_CrownSpacing, crownSpacing);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_CrownTotal, crownTotal);
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_FirstDefinition, cmdFirstTypeStr.toStdString());
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_SecondDefinition, cmdSecondTypeStr.toStdString());
        return true;
    }

    bool FITKAdaptorGeoModelTranslatePart::readParameters()
    {
        Interface::FITKAbsGeoModelTranslatePart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelTranslatePart>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double from[3] = {};
        double to[3] = {};
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_SPoint, from, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EPoint, to, 1, 3)) return false;

        cmd->setTranslateFrom(from[0], from[1], from[2]);
        cmd->setTranslateTo(to[0], to[1], to[2]);
        return true;
    }

    bool FITKAdaptorGeoModelTranslatePart::writeParameters(H5::Group& g)
    {
        Interface::FITKAbsGeoModelTranslatePart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelTranslatePart>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double from[3] = {};
        double to[3] = {};
        cmd->getTranslateFrom(from[0], from[1], from[2]);
        cmd->getTranslateTo(to[0], to[1], to[2]);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_SPoint, from, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EPoint, to, 1, 3);
        return true;
    }

    bool FITKAdaptorGeoModelRotatePart::readParameters()
    {
        Interface::FITKAbsGeoModelRotatePart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRotatePart>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double origin[3] = {};
        double direction[3] = {};
        double angle = 0.0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Direction, direction, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Angle, &angle)) return false;

        cmd->setRotateOrigin(origin[0], origin[1], origin[2]);
        cmd->setRotateDirection(direction[0], direction[1], direction[2]);
        cmd->setRotateAngle(angle);
        return true;
    }

    bool FITKAdaptorGeoModelRotatePart::writeParameters(H5::Group& g)
    {
        Interface::FITKAbsGeoModelRotatePart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRotatePart>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double origin[3] = {};
        double direction[3] = {};
        cmd->getRotateOrigin(origin[0], origin[1], origin[2]);
        cmd->getRotateDirection(direction[0], direction[1], direction[2]);

        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Direction, direction, 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Angle, cmd->getRotateAngle());
        return true;
    }

    bool FITKAdaptorGeoModelScalePart::readParameters()
    {
        Interface::FITKAbsGeoModelScalePart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelScalePart>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double origin[3] = {};
        double scaleFactor = 1.0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_ScaleFactor, &scaleFactor)) return false;

        cmd->setScaleOrigin(origin[0], origin[1], origin[2]);
        cmd->setScaleFactor(scaleFactor);
        return true;
    }

    bool FITKAdaptorGeoModelScalePart::writeParameters(H5::Group& g)
    {
        Interface::FITKAbsGeoModelScalePart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelScalePart>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double origin[3] = {};
        cmd->getScaleOrigin(origin[0], origin[1], origin[2]);

        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_ScaleFactor, cmd->getScaleFactor());
        return true;
    }

    bool FITKAdaptorGeoModelMirrorPart::readParameters()
    {
        Interface::FITKAbsGeoModelMirrorPart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelMirrorPart>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double origin[3] = {};
        int planeType = static_cast<int>(Interface::FITKGeoEnum::PT_XY);
        double rotateX = 0.0;
        double rotateY = 0.0;
        double rotateZ = 0.0;
        bool keepOrigin = false;
        double customPlaneNormal[3]{ 0.0, 0.0, 1.0 };

        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_PlaneType, &planeType))
        {
            std::string planeTypeStr = {};
            if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_PlaneType, planeTypeStr)) return false;
            QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::PlaneType>();
            planeType = metaEnum.keyToValue(planeTypeStr.c_str());
        }
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_RotateX, &rotateX)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_RotateY, &rotateY)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_RotateZ, &rotateZ)) return false;
        if (std::get<1>(paramGroup).attrExists(FGKW_KeepOrigin))
            if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_KeepOrigin, &keepOrigin)) return false;
        if (std::get<1>(paramGroup).attrExists(FGKW_CustomPlaneNormal))
            if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_CustomPlaneNormal, customPlaneNormal, 1, 3)) return false;

        cmd->setMirrorOrigin(origin[0], origin[1], origin[2]);
        cmd->setMirrorPlaneType(static_cast<Interface::FITKGeoEnum::PlaneType>(planeType));
        cmd->setRotateX(rotateX);
        cmd->setRotateY(rotateY);
        cmd->setRotateZ(rotateZ);
        cmd->setKeepOrigin(keepOrigin);
        cmd->setCustomPlaneNormal(customPlaneNormal[0], customPlaneNormal[1], customPlaneNormal[2]);
        return true;
    }

    bool FITKAdaptorGeoModelMirrorPart::writeParameters(H5::Group& g)
    {
        Interface::FITKAbsGeoModelMirrorPart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelMirrorPart>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double origin[3] = {};
        double customPlaneNormal[3] = {};
        cmd->getMirrorOrigin(origin[0], origin[1], origin[2]);
        cmd->getCustomPlaneNormal(customPlaneNormal[0], customPlaneNormal[1], customPlaneNormal[2]);

        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_PlaneType, static_cast<int>(cmd->getMirrorPlaneType()));
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_RotateX, cmd->getRotateX());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_RotateY, cmd->getRotateY());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_RotateZ, cmd->getRotateZ());
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_KeepOrigin, cmd->getKeepOrigin());
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_CustomPlaneNormal, customPlaneNormal, 1, 3);
        return true;
    }

    bool FITKAdaptorGeoModelRectangularPatternPart::readParameters()
    {
        Interface::FITKAbsGeoModelRectangularPatternPart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRectangularPatternPart>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double firstDirection[3] = {};
        int firstCount = 1;
        double firstSpacing = 0.0;
        double firstTotal = 0.0;
        int firstDefinition = static_cast<int>(Interface::FITKAbsGeoModelRectangularPatternPart::Spacing);
        double secondDirection[3] = {};
        int secondCount = 1;
        double secondSpacing = 0.0;
        double secondTotal = 0.0;
        int secondDefinition = static_cast<int>(Interface::FITKAbsGeoModelRectangularPatternPart::Spacing);

        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_FirstDirection, firstDirection, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_FirstCount, &firstCount)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_FirstSpacing, &firstSpacing)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_FirstTotal, &firstTotal)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_FirstDefinition, &firstDefinition)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_SecondDirection, secondDirection, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_SecondCount, &secondCount)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_SecondSpacing, &secondSpacing)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_SecondTotal, &secondTotal)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_SecondDefinition, &secondDefinition)) return false;

        cmd->setFirstDirection(firstDirection[0], firstDirection[1], firstDirection[2]);
        cmd->setFirstCount(firstCount);
        cmd->setFirstSpacing(firstSpacing);
        cmd->setFirstTotal(firstTotal);
        cmd->setFirstDefinition(static_cast<Interface::FITKAbsGeoModelRectangularPatternPart::Definition>(firstDefinition));
        cmd->setSecondDirection(secondDirection[0], secondDirection[1], secondDirection[2]);
        cmd->setSecondCount(secondCount);
        cmd->setSecondSpacing(secondSpacing);
        cmd->setSecondTotal(secondTotal);
        cmd->setSecondDefinition(static_cast<Interface::FITKAbsGeoModelRectangularPatternPart::Definition>(secondDefinition));
        return true;
    }

    bool FITKAdaptorGeoModelRectangularPatternPart::writeParameters(H5::Group& g)
    {
        Interface::FITKAbsGeoModelRectangularPatternPart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRectangularPatternPart>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> firstDirection = cmd->firstDirection();
        std::array<double, 3> secondDirection = cmd->secondDirection();

        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_FirstDirection, firstDirection.data(), 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_FirstCount, cmd->firstCount());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_FirstSpacing, cmd->firstSpacing());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_FirstTotal, cmd->firstTotal());
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_FirstDefinition, static_cast<int>(cmd->firstDefinition()));
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_SecondDirection, secondDirection.data(), 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_SecondCount, cmd->secondCount());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_SecondSpacing, cmd->secondSpacing());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_SecondTotal, cmd->secondTotal());
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_SecondDefinition, static_cast<int>(cmd->secondDefinition()));
        return true;
    }

    bool FITKAdaptorGeoModelCircularPatternPart::readParameters()
    {
        Interface::FITKAbsGeoModelCircularPatternPart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCircularPatternPart>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        int axisShape[3] = {};
        bool isReverse = false;
        int axisCount = 1;
        double axisSpacing = 0.0;
        double axisTotal = 0.0;
        int crownCount = 1;
        double crownSpacing = 0.0;
        double crownTotal = 0.0;
        int firstDefinition = static_cast<int>(Interface::FITKAbsGeoModelCircularPatternPart::Spacing);
        int secondDefinition = static_cast<int>(Interface::FITKAbsGeoModelCircularPatternPart::Spacing);

        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_AxisShape, axisShape, 1, 3)) return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_isReverse, &isReverse)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_AxisCount, &axisCount)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_AxisSpacing, &axisSpacing)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_AxisTotal, &axisTotal)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_CrownCount, &crownCount)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_CrownSpacing, &crownSpacing)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_CrownTotal, &crownTotal)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_FirstDefinition, &firstDefinition)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_SecondDefinition, &secondDefinition)) return false;

        cmd->setAxis(Interface::VirtualShape(axisShape[0], axisShape[1], axisShape[2]));
        cmd->reverse(isReverse);
        cmd->setAxisCount(axisCount);
        cmd->setAxisSpacing(axisSpacing);
        cmd->setAxisTotal(axisTotal);
        cmd->setCrownCount(crownCount);
        cmd->setCrownSpacing(crownSpacing);
        cmd->setCrownTotal(crownTotal);
        cmd->setFirstDefinition(static_cast<Interface::FITKAbsGeoModelCircularPatternPart::Definition>(firstDefinition));
        cmd->setSecondDefinition(static_cast<Interface::FITKAbsGeoModelCircularPatternPart::Definition>(secondDefinition));
        return true;
    }

    bool FITKAdaptorGeoModelCircularPatternPart::writeParameters(H5::Group& g)
    {
        Interface::FITKAbsGeoModelCircularPatternPart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCircularPatternPart>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        int axisShape[3] = { cmd->axis().CmdId, cmd->axis().VirtualTopoId, cmd->axis().VirtualTopoIndex };
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_AxisShape, axisShape, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_isReverse, cmd->isReverse());
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_AxisCount, cmd->axisCount());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_AxisSpacing, cmd->axisSpacing());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_AxisTotal, cmd->axisTotal());
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_CrownCount, cmd->crownCount());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_CrownSpacing, cmd->crownSpacing());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_CrownTotal, cmd->crownTotal());
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_FirstDefinition, static_cast<int>(cmd->firstDefinition()));
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_SecondDefinition, static_cast<int>(cmd->secondDefinition()));
        return true;
    }
}



