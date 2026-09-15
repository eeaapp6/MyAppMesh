/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKProbeSection.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"

namespace Radioss
{
    FITKProbeSection::~FITKProbeSection()
    {}

    FITKAbstractProbe::ProbeType FITKProbeSection::getType() const
    {
        //获取探针类型
        return ProbeType::Probe_Section;
    }

    void FITKProbeSection::setSetID1(int id)
    {
        m_set_ID1 = id;
    }

    int FITKProbeSection::getNodeID1()
    {
        Interface::FITKModelSet*  set = FITKDATAREPO->getTDataByID< Interface::FITKModelSet>(m_set_ID1);
        if (!set)return -1;
        QList<int> nodeIDs = set->getMember();
        if (nodeIDs.isEmpty())return -1;
        return nodeIDs[0];
    }

    int FITKProbeSection::getSetID1()
    {
        return m_set_ID1;
    }

    void FITKProbeSection::setSetID2(int id)
    {
        m_set_ID2 = id;
    }

    int FITKProbeSection::getNodeID2()
    {
        Interface::FITKModelSet*  set = FITKDATAREPO->getTDataByID< Interface::FITKModelSet>(m_set_ID2);
        if (!set)return -1;
        QList<int> nodeIDs = set->getMember();
        if (nodeIDs.isEmpty())return -1;
        return nodeIDs[0];
    }

    int FITKProbeSection::getSetID2()
    {
        return m_set_ID2;
    }

    void FITKProbeSection::setSetID3(int id)
    {
        m_set_ID3 = id;
    }

    int FITKProbeSection::getNodeID3()
    {
        Interface::FITKModelSet*  set = FITKDATAREPO->getTDataByID< Interface::FITKModelSet>(m_set_ID3);
        if (!set)return -1;
        QList<int> nodeIDs = set->getMember();
        if (nodeIDs.isEmpty())return -1;
        return nodeIDs[0];
    }

    int FITKProbeSection::getSetID3()
    {
        return m_set_ID3;
    }

    void FITKProbeSection::setGrndID(int id)
    {
        //设置节点组 ID
        m_grnd_ID = id;
    }

    int FITKProbeSection::getGrndID()
    {
        //获取节点组 ID
        return m_grnd_ID;
    }

    void FITKProbeSection::setIsave(int flag)
    {
        //Set Saving or reading the section data from file_name
        m_Isave = flag;
    }

    int FITKProbeSection::getIsave()
    {
        //Get Saving or reading the section data from file_name
        return m_Isave;
    }

    void FITKProbeSection::setFrameID(int id)
    {
        //Set Moving frame ID
        m_frame_ID = id;
    }

    int FITKProbeSection::getFrameID()
    {
        //Get Moving frame ID
        return m_frame_ID;
    }

    void FITKProbeSection::setDt(double value)
    {
        //Set Time step for saving the section data
        m_Dt = value;
    }

    double FITKProbeSection::getDt()
    {
        //Get Time step for saving the section data
        return m_Dt;
    }

    void FITKProbeSection::setAlpha(double value)
    {
        //Set Exponential moving average filtering constant
        m_alpha = value;
    }

    double FITKProbeSection::getAlpha()
    {
        //Get Exponential moving average filtering constant
        return m_alpha;
    }

    void FITKProbeSection::setFileName(QString file)
    {
        //Set Root name of the file
        m_file_name = file;
    }

    QString FITKProbeSection::getFileName()
    {
        //Get Root name of the file
        return m_file_name;
    }

    void FITKProbeSection::setGrbricID(int id)
    {
        //Set Brick group id
        m_grbric_ID = id;
    }

    int FITKProbeSection::getGrbricID()
    {
        //Get Brick group id
        return m_grbric_ID;
    }

    void FITKProbeSection::setGrshelID(int id)
    {
        //Set Shell group id
        m_grshel_ID = id;
    }

    int FITKProbeSection::getGrshelID()
    {
        //Get Shell group id
        return m_grshel_ID;
    }

    void FITKProbeSection::setGrtrusID(int id)
    {
        //Set Truss group id
        m_grtrus_ID = id;
    }

    int FITKProbeSection::getGrtrusID()
    {
        //Get Truss group id
        return m_grtrus_ID;
    }

    void FITKProbeSection::setGrbeamID(int id)
    {
        //Set Beam group id
        m_grbeam_ID = id;
    }

    int FITKProbeSection::getGrbeamID()
    {
        //Get Beam group id
        return m_grbeam_ID;
    }

    void FITKProbeSection::setGrsprgID(int id)
    {
        //Set Spring group id
        m_grsprg_ID = id;
    }

    int FITKProbeSection::getGrsprgID()
    {
        //Get Spring group id
        return m_grsprg_ID;
    }

    void FITKProbeSection::setGrtriaID(int id)
    {
        //Set Triangle group id
        m_grtria_ID = id;
    }

    int FITKProbeSection::getGrtriaID()
    {
        //Get Triangle group id
        return m_grtria_ID;
    }

    int FITKProbeSection::getNinter()
    {
        //Get Interface Num
        return m_interfacesID.size();
    }

    void FITKProbeSection::setIframe(int flag)
    {
        //Set Flag defining the center of the local system used in the calculation of the section forces and moments.
        m_Iframe = flag;
    }

    int FITKProbeSection::getIframe()
    {
        //Get Flag defining the center of the local system used in the calculation of the section forces and moments.
        return m_Iframe;
    }

    void FITKProbeSection::setInterIDs(QList<int> ids)
    {
        //Set Optional interface identifiers
        m_interfacesID = ids;
    }

    QList<int> FITKProbeSection::getInterIDs()
    {
        //Get Optional interface identifiers
        return m_interfacesID;
    }
    FITKRadiossElementGroup::ElementGroupType FITKProbeSection::getElementGroupType()
    {
        return _eleType;
    }
    void FITKProbeSection::setElementGroupType(FITKRadiossElementGroup::ElementGroupType type)
    {
        _eleType = type;
    }
    int FITKProbeSection::getEleGroupID(FITKRadiossElementGroup::ElementGroupType type)
    {
        switch (type)
        {
        case FITKRadiossElementGroup::ElementGroupType::GRBRIC: 
            return m_grbric_ID;
        case FITKRadiossElementGroup::ElementGroupType::GRTRUS: 
            return m_grtrus_ID;
        case FITKRadiossElementGroup::ElementGroupType::GRBEAM: 
            return m_grbeam_ID;
        case FITKRadiossElementGroup::ElementGroupType::GRSHEL: 
            return m_grshel_ID;
        case FITKRadiossElementGroup::ElementGroupType::GRSPRI: 
            return m_grsprg_ID;
        case FITKRadiossElementGroup::ElementGroupType::GRTRIA: 
            return m_grtria_ID;
        default:
            return -1;
        }
    }
}