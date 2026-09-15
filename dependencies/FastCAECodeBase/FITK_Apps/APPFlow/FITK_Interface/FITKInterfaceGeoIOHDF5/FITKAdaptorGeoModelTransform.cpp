/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelTransform.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoTransformation.h"
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
}



