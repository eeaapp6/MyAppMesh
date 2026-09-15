/**
 *
 * @file FITKMaterialAssign.h
 * @brief 材料指派类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-07
 *
 */
#ifndef __FITKMATERIAL_ASSIGN_H__
#define __FITKMATERIAL_ASSIGN_H__

#include "FITKInterfacePhysicsAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    /**
     * @brief 材料指派
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-07
     */
    class FITKInterfacePhysicsAPI FITKMaterialAssign : public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief Construct a new FITKMaterialAssign object
         * @param[i]  modelID        模型ID，可以是集合也可以是整个模型
         * @param[i]  materialID     材料ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        explicit FITKMaterialAssign(int modelID, int materialID);
        explicit FITKMaterialAssign() = default;
        /**
         * @brief Destroy the FITKMaterialAssign object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual ~FITKMaterialAssign() = default;
        /**
         * @brief 获取材料ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual int getMaterialID();
        /**
         * @brief 获取模型ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual int getModelID();
        /**
         * @brief 建立材料与模型的关联关系
         * @param[]  modelID        My Param doc
         * @param[]  materialID     My Param doc
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual void assign(int modelID, int materialID);
        /**
         * @brief 是否为有效关联，能够找到材料与模型就认为有效
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual bool isValid();
        /**
         * @brief 材料是否与模型部件建立的关联关系
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual bool isAssignComponent();

        template<class T>
        /**
         * @brief  获取材料并进行类型转换
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        T* getMaterial()
        {
            return dynamic_cast<T*>(this->getMaterialObj());
        }

    private:
        /**
         * @brief 获取材料
         * @return FITKAbstractDataObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        FITKAbstractDataObject* getMaterialObj();

    protected:
        /**
         * @brief 模型ID，集合ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        int _modelID{ -1 };
        /**
         * @brief 材料ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        int _materialID{ -1 };
    };
    /**
     * @brief 材料指派管理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-07
     */
    class FITKInterfacePhysicsAPI FITKMaterialAssignManager : public Core::FITKAbstractDataObject
        , public Core::FITKAbstractDataManager<FITKMaterialAssign>
    {
    public:
        explicit FITKMaterialAssignManager() = default;
        virtual ~FITKMaterialAssignManager() = default;
    };

}


#endif
