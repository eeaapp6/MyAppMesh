/**
 * 
 * @file FITKSectionAssignGroup.h
 * @brief 截面指派组及其管理类声明
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2025-01-01
 * 
 */
#ifndef _FITK_SECTIONASSIGN_GROUP_H___
#define _FITK_SECTIONASSIGN_GROUP_H___
 
#include "FITKSectionAssign.h"
#include "FITKInterfacePhysicsAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"



namespace Interface
{
    /**
     * @brief 截面指派组管理器
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2025-01-01
     */
    class FITKInterfacePhysicsAPI FITKSectionAssignGroup : public FITKSectionAssignManager
    {
    public:
        /**
         * @brief Construct a new FITKSectionAssignGroup object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-01-01
         */
        explicit FITKSectionAssignGroup() = default;
        /**
         * @brief Destroy the FITKSectionAssignGroup object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-01-01
         */
        virtual ~FITKSectionAssignGroup() = default;
        /**
         * @brief 设置截面,组里面全部的截面都设置为sectionID
         * @param sectionID 截面ID
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-01-01
         */ 
        void setSection(const int  sectionID);
        /**
         * @brief 获取截面ID 默认返回第一个截面ID，如果组内Section不一致则返回-1
         * @return int 截面ID
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-01-01
         */
        int getSectionID();
 
    };

    
    /**
     * @brief 截面指派组管理器
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2025-01-01
     */ 
    class FITKInterfacePhysicsAPI FITKSectionAssignGroupManager : public Core::FITKAbstractNDataObject,
        public Core::FITKAbstractDataManager<FITKSectionAssignGroup>
    {
    public:
        /**
         * @brief Construct a new FITKSectionAssignGroupManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-01-01
         */
        explicit FITKSectionAssignGroupManager() = default;
        /**
         * @brief Destroy the FITKSectionAssignGroupManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-01-01
         */
        virtual ~FITKSectionAssignGroupManager() = default;
    };


}


#endif
