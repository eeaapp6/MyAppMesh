/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaBeamOrientation.h
 * @brief  梁方向及其管理
 * @author LiJin (1003680421@qq.com)
 * @date   2024-07-04
 * 
 */
#ifndef FITKABABEAMORIENTATION_H
#define FITKABABEAMORIENTATION_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include <array>


namespace Interface {

    class FITKAbstractModel;
    /**
     * @brief  梁方向类，通过setId关联模型
     * @author LiJin (1003680421@qq.com)
     * @date   2024-07-04
     */
    class FITKInterfaceStructuralAPI FITKAbaBeamOrientation: public Core::FITKAbstractDataObject
    {
        FITKCLASS(Interface, FITKAbaBeamOrientation)
    public:
        FITKAbaBeamOrientation() = default;
        ~FITKAbaBeamOrientation() = default;
        /**
         * @brief  设置集合Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-04
         */
        void setModelSetID(const int id);
        /**
         * @brief  获取集合Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-04
         */
        int getModelSetID() const;
        /**
         * @brief  获取梁方向
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-04
         */
        std::array<double, 3> getN1Direction() const;
        /**
         * @brief  设置梁方向
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-04
         */
        void setN1Direction(const std::array<double,3>& dir);
        /**
         * @brief  获取关联的部件
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-04
         */
        FITKAbstractModel* getPart();
        /**
         * @brief  拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-04
         */
        bool copy(FITKAbstractDataObject* obj) override;
    private:
        int m_setId{-1};
        std::array<double,3> m_n1Direction{0,0,-1};

    };
    /**
     * @brief  梁方向管理器
     * @author LiJin (1003680421@qq.com)
     * @date   2024-07-04
     */
    class FITKInterfaceStructuralAPI FITKAbaBeamOrientationManager: public Core::FITKAbstractDataManager<FITKAbaBeamOrientation>
    {
    public:
        /**
         * @brief  根据集合Id，获取梁方向
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-04
         */
        FITKAbaBeamOrientation* getBeamOrientation(int setID );
    };

}


#endif // FITKABABEAMORIENTATION_H
