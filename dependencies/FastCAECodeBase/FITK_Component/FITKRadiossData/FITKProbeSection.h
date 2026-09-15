/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKProbeSection.h
 * @brief  探针 截面
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-11-05
 *********************************************************************/
#ifndef __FITKPROBESECTION_H__
#define __FITKPROBESECTION_H__

#include "FITKAbstractProbe.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Component/FITKRadiossData/FITKRadiossElementGroup.h"

namespace Radioss
{
     /**
     * @class  FITKProbeSection
     * @brief  截面对象
     * @author liuzhonghua
     * @date   2025-08-20
     */
    class FITKRadiossDataAPI FITKProbeSection : public FITKAbstractProbe
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKProbeSection)
        RadiossKeyWord(FITKProbeSection, /SECT)
    public:
        explicit FITKProbeSection() =default;
        virtual ~FITKProbeSection();
 
        virtual ProbeType getType() const override;
        /**
         * @brief    设置单点集合1 ID
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setSetID1(int id);
        /**
         * @brief    获取节点1 ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getNodeID1();
        /**
        * @brief    获取集合1ID
        * @return   int
        * @author   liuzhonghua (liuzhonghuaszch@163.com)
        * @date     2025-11-05
        */
        int getSetID1();
        /**
         * @brief    设置单点集合2 ID
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setSetID2(int id);
        /**
         * @brief    获取节点2 ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getNodeID2();
        /**
         * @brief    获取集合2ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getSetID2();
        /**
         * @brief    设置单点集合3 ID
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setSetID3(int id);
        /**
         * @brief    获取节点3 ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getNodeID3();
        /**
         * @brief    获取集合3ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getSetID3();
        /**
         * @brief    设置节点组 ID
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setGrndID(int id);
        /**
         * @brief    获取节点组 ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getGrndID();
        /**
         * @brief    Set Saving or reading the section data from file_name
         * @param[i] flag 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setIsave(int flag);
        /**
         * @brief    Get Saving or reading the section data from file_name
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getIsave();
        /**
         * @brief    Set Moving frame ID
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setFrameID(int id);
        /**
         * @brief    Get Moving frame ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getFrameID();
        /**
         * @brief    Set Time step for saving the section data
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setDt(double value);
        /**
         * @brief    Get Time step for saving the section data
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        double getDt();
        /**
         * @brief    Set Exponential moving average filtering constant
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setAlpha(double value);
        /**
         * @brief    Get Exponential moving average filtering constant
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        double getAlpha();
        /**
         * @brief    Set Root name of the file
         * @param[i] file 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setFileName(QString file);
        /**
         * @brief    Get Root name of the file
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        QString getFileName();
        /**
         * @brief    Set Brick group id
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setGrbricID(int id);
        /**
         * @brief    Get Brick group id
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getGrbricID();
        /**
         * @brief    Set Shell group id
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setGrshelID(int id);
        /**
         * @brief    Get Shell group id
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getGrshelID();
        /**
         * @brief    Set Truss group id
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setGrtrusID(int id);
        /**
         * @brief    Get Truss group id
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getGrtrusID();
        /**
         * @brief    Set Beam group id
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setGrbeamID(int id);
        /**
         * @brief    Get Beam group id
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getGrbeamID();
        /**
         * @brief    Set Spring group id
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setGrsprgID(int id);
        /**
         * @brief    Get Spring group id
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getGrsprgID();
        /**
         * @brief    Set Triangle group id
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setGrtriaID(int id);
        /**
         * @brief    Get Triangle group id
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getGrtriaID();
        /**
         * @brief    Get Interface Num
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getNinter();
        /**
         * @brief    Set Flag defining the center of the local system used in the calculation of the section forces and moments.
         * @param[i] flag 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setIframe(int flag);
        /**
         * @brief    Get Flag defining the center of the local system used in the calculation of the section forces and moments.
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        int getIframe();
        /**
         * @brief    Set Optional interface identifiers
         * @param[i] ids 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        void setInterIDs(QList<int> ids);
        /**
         * @brief    Get Optional interface identifiers
         * @return   QList<int>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-05
         */
        QList<int> getInterIDs();

        //获取单元组类型
        FITKRadiossElementGroup::ElementGroupType getElementGroupType();
        //设置单元组类型
        void setElementGroupType(FITKRadiossElementGroup::ElementGroupType type);
        //根据单元组类型获取对应单元组ID
        int getEleGroupID(FITKRadiossElementGroup::ElementGroupType type);

    private:
        /**
         * @brief  单点集合1 ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        int m_set_ID1{ 0 };
        /**
         * @brief  单点集合2 ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        int m_set_ID2{ 0 };
        /**
         * @brief  单点集合3 ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        int m_set_ID3{ 0 };
        /**
         * @brief  节点组 ID
         * @author LiBaojun 
         */
        int m_grnd_ID{-1}; 
        /**
         * @brief  Saving or reading the section data from file_name
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        int m_Isave{ 0 };
        /**
         * @brief  Moving frame ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        int m_frame_ID{ 0 };
        /**
         * @brief  Time step for saving the section data used with ISAVE =1 or 2
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        double m_Dt{ 0.1 };
        /**
         * @brief  Exponential moving average filtering constant
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        double m_alpha{ 0 };
        /**
         * @brief  Root name of the file
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        QString m_file_name{};
        /**
         * @brief  Brick group id
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        int m_grbric_ID{ 0 };
        /**
         * @brief  Shell group id
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        int m_grshel_ID{ 0 };
        /**
         * @brief  Truss group id
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        int m_grtrus_ID{ 0 };
        /**
         * @brief  Beam group id
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        int m_grbeam_ID{ 0 };
        /**
         * @brief  Spring group id
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        int m_grsprg_ID{ 0 };
        /**
         * @brief  Triangle group id
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        int m_grtria_ID{ 0 };
        /**
         * @brief  Flag defining the center of the local system used in the calculation of the section forces and moments.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        int m_Iframe{ 0 };
        /**
         * @brief  Optional interface identifiers
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-05
         */
        QList<int> m_interfacesID{};

        //当前单元组类型
        FITKRadiossElementGroup::ElementGroupType _eleType =
            FITKRadiossElementGroup::ElementGroupType::GRNone;
    };

}


#endif