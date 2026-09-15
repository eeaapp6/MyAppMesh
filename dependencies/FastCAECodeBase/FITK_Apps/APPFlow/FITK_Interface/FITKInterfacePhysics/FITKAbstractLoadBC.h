/**
 *
 * @file FITKAbstractLoadConstraint.h
 * @brief 约束与载荷基类与管理类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-30
 *
 */
#ifndef _FITKLOADCONSTRAINTS_H____
#define _FITKLOADCONSTRAINTS_H____

#include "FITKInterfacePhysicsAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKBoundaryLocation.h"

namespace Interface
{
    /**
     * @brief 载荷抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-30
     */
    class FITKInterfacePhysicsAPI FITKAbstractLoad
        : public FITKBoundaryLocation
    {
    public:
        /**
         * @brief Construct a new FITKAbstractLoad object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        explicit FITKAbstractLoad() = default;
        /**
         * @brief Destroy the FITKAbstractLoad object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        virtual ~FITKAbstractLoad() = 0;

    };
    /**
     * @brief 约束抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-30
     */
    class FITKInterfacePhysicsAPI FITKAbstractBC
        : public FITKBoundaryLocation
    {
    public:
        /**
         * @brief Construct a new FITKAbstractConstraint object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        explicit FITKAbstractBC() = default;
        /**
         * @brief Destroy the FITKAbstractConstraint object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        virtual ~FITKAbstractBC() = 0 ;

    };
    /**
     * @brief 载荷管理类声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-30
     */
    class FITKInterfacePhysicsAPI FITKLoadManager :public Core::FITKAbstractNDataObject,
        public Core::FITKAbstractDataManager<FITKAbstractLoad>
    {
    public:
        /**
         * @brief Construct a new FITKLoadManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        explicit FITKLoadManager() = default;
        /**
         * @brief Destroy the FITKLoadManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        virtual ~FITKLoadManager() = default;

    };
    /**
     * @brief 约束管理类声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-30
     */
    class FITKInterfacePhysicsAPI FITKBCManager : public Core::FITKAbstractNDataObject,
        public Core::FITKAbstractDataManager<FITKAbstractBC>
    {
    public:
        /**
         * @brief Construct a new FITKConstraintManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        explicit FITKBCManager() = default;
        /**
         * @brief Destroy the FITKConstraintManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        virtual ~FITKBCManager() = default;

    };

}


#endif // !_FITKLOADCONSTRAINTS_H___
