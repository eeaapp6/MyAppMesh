/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaSectionBeam.h
 * @brief  BeamSection类型
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-20
 * 
 */
#ifndef FITKABASECTIONBEAM_H
#define FITKABASECTIONBEAM_H
#include "FITKAbaAbstractSection.h"
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"


namespace Interface {
    class FITKAbaProfile;
    /**
     * @brief  梁截面属性BeamSection类型
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-20
     */
    class FITKInterfaceStructuralAPI FITKAbaSectionBeam: public FITKAbaAbastractSection
    {
        FITKCLASS(Interface, FITKAbaSectionBeam);
    public:
        /**
         * @brief  获取截面类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaSectionType getSectionType() override;
        bool copy(FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取梁截面名称
         * @param[in]  void
         * @return 梁截面名称
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        QString getProfileName() const;
        /**
         * @brief  设置梁截面形状Id
         * @param[in]  profileId 要设置的梁截面形状Id
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setProfileId(int profileId);
        /**
         * @brief  设置梁截面形状
         * @param[in]  profile 梁截面形状指针
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setProfile(FITKAbaProfile* profile);
        /**
         * @brief  获取梁截面形状Id
         * @param[in]  void
         * @return 梁截面形状Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        int getProfileId() const;
        /**
         * @brief  获取梁截面形状
         * @param[in]  void
         * @return 梁截面形状指针
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        FITKAbaProfile* getProfile() const;

        /**
          * @brief  获取截面类型字符串，用于UI显式
          * @return QString
          * @author libaojun (libaojunqd@foxmail.com)
          * @date   2024-03-27
          */
        virtual QString getTypeString() override;

        FITKAbaAbastractSection::TemperatureVariation getTemperatureVariation() const;
        void setTemperatureVariation(FITKAbaAbastractSection::TemperatureVariation tv);
    private:
        /**
         * @brief  梁截面id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        int m_profileId{-1};
        FITKAbaAbastractSection::TemperatureVariation m_temperatureVariation{ FITKAbaAbastractSection::TemperatureVariation::LinearByGradients };
       
    };
}


#endif // FITKABASECTIONBEAM_H
