/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorProbeSection.h"
#include "FITK_Component/FITKRadiossData/FITKProbeSection.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodeGroup.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"
#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"

namespace Radioss
{
    QString FITKRadiossAdaptorProbeSection::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorProbeSection";
    }

    bool FITKRadiossAdaptorProbeSection::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        // 创建数据对象
        FITKProbeSection* section = new FITKProbeSection();
        //读取关键字
        RadKeyLineSpliter* keyLineSpliter = _reader->getKeyLineSpliter();
        RadReadInformationMapper* infoMapper = _reader->getReadRadInformationMapper();
        if (!keyLineSpliter || !infoMapper) return false;

        //获取id和关键字
        int pid = keyLineSpliter->getId(0);
        //int uid = keyLineSpliter->getId(1);
        QString keyLine = keyLineSpliter->getCurrentLine();

        bool ok = false;
        while (_reader && !_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            //读取材料数据
            ok = this->readSection(section);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Section Error : %1 %2").arg(keyLine).arg(section->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Section : %1 %2").arg(keyLine).arg(section->getDataObjectName()));
        _caseData->getCurrentSolution()->getProbeManager()->appendDataObj(section);
        infoMapper->insertProbeSectionIDMap(pid, section->getDataObjectID());
        return true;
    }

    bool FITKRadiossAdaptorProbeSection::adaptW()
    { 
        //写出数据
        FITKProbeSection* data = dynamic_cast<FITKProbeSection*>(_dataObj);
        if (data == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadSectionID() + 1;
        //建立映射关系
        infoMapper->insertSectionIDMap(data->getDataObjectID(), radID);
        *stream << data->GetFITKProbeSectionRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << data->getDataObjectName() << endl;

        //node_ID1  node_ID2  node_ID3  grnd_ID  ISAVE  frame_ID  Dt  alpha
        *stream << QString("#%1%2%3%4%5%6%7%8").arg("node_ID1", 9).arg("node_ID2", 10).arg("node_ID3", 10)
            .arg("grnd_ID", 10).arg("ISAVE", 10).arg("frame_ID", 10).arg("Dt", 20).arg("alpha", 20) << endl;
        *stream << gapStr(data->getNodeID1()) << gapStr(data->getNodeID2()) << gapStr(data->getNodeID3())
            << gapStr(data->getGrndID()) << gapStr(data->getIsave()) << gapStr(data->getFrameID())
            << gapStr(data->getDt()) << gapStr(data->getAlpha()) << endl;

        //file_name
        *stream << QString("#%1").arg("file_name", 9) << endl;
        *stream << data->getFileName().left(100).trimmed() << endl;

        //grbric_ID	 	grshel_ID	grtrus_ID	grbeam_ID	grsprg_ID	grtria_ID	Ninter	 	Iframe
        *stream << QString("#%1%2%3%4%5%6%7%8").arg("grbric_ID", 9).arg("grshel_ID", 20).arg("grtrus_ID", 10)
            .arg("grbeam_ID", 10).arg("grsprg_ID", 10).arg("grtria_ID", 10).arg("Ninter", 10).arg("Iframe", 20) << endl;
        *stream << gapStr(data->getGrbricID()) << gapStr(data->getGrshelID(),20) << gapStr(data->getGrtrusID())
            << gapStr(data->getGrbeamID()) << gapStr(data->getGrsprgID()) << gapStr(data->getGrtriaID())
            << gapStr(data->getNinter()) << gapStr(data->getIframe(),20) << endl;
        //interface ID
        if (data->getNinter() > 0)
        {
            const QList<int>& interfaceIDs = data->getInterIDs();
            int count = interfaceIDs.size();
            int rows = (count - 1) / 10 + 1;
            for (int row = 0; row < rows; ++row)
            {
                for (int i = 0; i < 10; ++i)
                {
                    int index = row * 10 + i;
                    if (index < count)
                    {
                        *stream << gapStr(interfaceIDs[index]);
                    }
                }
                *stream << endl;
            }
        }
        return true;
    }

    bool FITKRadiossAdaptorProbeSection::readSection(FITKProbeSection* data)
    {
        if (data == nullptr || _reader == nullptr) return false;

        QString line = _reader->previousLine().trimmed();
        //名称
        data->setDataObjectName(line);
        //参数存储
        QStringList lines;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("#")) continue;
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            lines.append(line);
        }
        //与手册说明不符
        if (lines.size() < 3)
        {
            _reader->consoleMessage(3, "Invalid Section : /SECT");
            return false;
        }
        //读取参数、赋值
        this->readParam(lines, data);
        this->setSectionData(data);
        return true;
    }

    void FITKRadiossAdaptorProbeSection::readParam(const QStringList& lines, FITKProbeSection* data)
    {
        if (lines.size() < 3 || _reader == nullptr||!data) return;

        //node_ID1  node_ID2  node_ID3  grnd_ID  ISAVE  frame_ID  Dt  alpha
        QList<QPair<QString, int>> matPar = { {"node_ID1", 10}, {"node_ID2", 10}, {"node_ID3", 10},
            {"grnd_ID", 10}, {"ISAVE", 10}, {"frame_ID", 10},{"Dt", 20}, {"alpha", 20} };
        splitParam(lines[0], matPar);

        //file_name 直接设置
        QString file_name = lines[1].mid(0, 100).trimmed();
        data->setFileName(file_name);

        //grbric_ID	 	grshel_ID	grtrus_ID	grbeam_ID	grsprg_ID	grtria_ID	Ninter	 	Iframe
        matPar = { {"grbric_ID", 10}, {"grshel_ID", 20}, {"grtrus_ID", 10},
            {"grbeam_ID", 10}, {"grsprg_ID", 10}, {"grtria_ID", 10},{"Ninter", 10}, {"Iframe", 20} };
        splitParam(lines[2], matPar);

        if (_sectionParam.value("Ninter", 0) > 0)
        {
            //读取inter的int_ID
            for (int row = 3; row < lines.size(); ++row)
            {
                for (int i = 0; i < 10; ++i)
                {
                    QString key = QString("int_ID%1").arg((row-3)*10 + i + 1);
                    double val = lines[row].mid(i*10, 10).simplified().toDouble();
                    _sectionParam.insert(key, val);
                }
            }
        }
    }

    void FITKRadiossAdaptorProbeSection::splitParam(const QString& line, const QList<QPair<QString, int>>& par)
    {
        //字符串分割位置
        int split = 0;
        for (auto it = par.begin(); it != par.end(); ++it)
        {
            bool ok = false;
            const QString& parKey = it->first;
            int width = it->second;
            double parVal = line.mid(split, width).simplified().toDouble(&ok);
            //如果转double为空值，则跳过
            if (ok)
                _sectionParam.insert(parKey, parVal);
            split += width;
        }
    }

    void FITKRadiossAdaptorProbeSection::setSectionData(FITKProbeSection* data)
    {
        if (!data) return;
        RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        if (!mapper)return;
        Radioss::FITKRadiossMeshModel* radiossMeshModel = _caseData->getMeshModel();
        if (!radiossMeshModel)return;
        Interface::FITKComponentManager* componentManager = radiossMeshModel->getComponentManager();
        if (!componentManager)return;

        auto createSet = [&](int id)->int
        {
            if (id < 0)return -1;
            Interface::FITKModelSet* modelSet = new FITKRadiossNodeGroup();
            modelSet->setDataObjectName(componentManager->checkName("Single_point_Set-1"));
            modelSet->setModelSetType(Interface::FITKModelEnum::FITKModelSetType::FMSNode);
            modelSet->appendMember(id);
            modelSet->setModel(radiossMeshModel);
            componentManager->appendDataObj(modelSet);
            return modelSet->getDataObjectID();
        };
        int NodeID = mapper->getNodeIDByRadID(_sectionParam.value("node_ID1", -1));
        data->setSetID1(createSet(NodeID));
        NodeID = mapper->getNodeIDByRadID(_sectionParam.value("node_ID2", -1));
        data->setSetID2(createSet(NodeID));
        NodeID = mapper->getNodeIDByRadID(_sectionParam.value("node_ID3", -1));
        data->setSetID3(createSet(NodeID));
        data->setGrndID(static_cast<int>(_sectionParam.value("grnd_ID", data->getGrndID())));
        data->setIsave(static_cast<int>(_sectionParam.value("ISAVE", data->getIsave())));
        data->setFrameID(static_cast<int>(_sectionParam.value("frame_ID", data->getFrameID())));
        data->setDt(_sectionParam.value("Dt", data->getDt()));
        data->setAlpha(_sectionParam.value("alpha", data->getAlpha()));

        int grbric_ID = _sectionParam.value("grbric_ID", data->getGrbricID());
        int grshel_ID = _sectionParam.value("grshel_ID", data->getGrshelID());
        int grtrus_ID = _sectionParam.value("grtrus_ID", data->getGrtrusID());
        int grbeam_ID = _sectionParam.value("grbeam_ID", data->getGrbeamID());
        int grsprg_ID = _sectionParam.value("grsprg_ID", data->getGrsprgID());
        int grtria_ID = _sectionParam.value("grtria_ID", data->getGrtriaID());

        if (grbric_ID > 0)
            data->setElementGroupType(FITKRadiossElementGroup::ElementGroupType::GRBRIC);
        else if (grshel_ID > 0)
            data->setElementGroupType(FITKRadiossElementGroup::ElementGroupType::GRSHEL);
        else if (grtrus_ID > 0)
            data->setElementGroupType(FITKRadiossElementGroup::ElementGroupType::GRTRUS);
        else if (grbeam_ID > 0)
            data->setElementGroupType(FITKRadiossElementGroup::ElementGroupType::GRBEAM);
        else if (grsprg_ID > 0)
            data->setElementGroupType(FITKRadiossElementGroup::ElementGroupType::GRSPRI);
        else if (grtria_ID > 0)
            data->setElementGroupType(FITKRadiossElementGroup::ElementGroupType::GRTRIA);

        data->setGrbricID(static_cast<int>(grbric_ID));
        data->setGrshelID(static_cast<int>(grshel_ID));
        data->setGrtrusID(static_cast<int>(grtrus_ID));
        data->setGrbeamID(static_cast<int>(grbeam_ID));
        data->setGrsprgID(static_cast<int>(grsprg_ID));
        data->setGrtriaID(static_cast<int>(grtria_ID));

        int N_inter = static_cast<int>(_sectionParam.value("Ninter", 0));
        QList<int> interIDs;
        for (int i = 0; i < N_inter; ++i)
        {
            QString key = QString("int_ID%1").arg(i + 1);
            int interID = static_cast<int>(_sectionParam.value(key, -1));
            if (interID != -1)
                interIDs.append(interID);
        }
        data->setInterIDs(interIDs);

        data->setIframe(static_cast<int>(_sectionParam.value("Iframe", data->getIframe())));
    }
}