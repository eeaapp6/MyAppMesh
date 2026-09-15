/**
 *
 * @file FITKBoundaryLocation.h
 * @brief 边界位置声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-30
 *
 */
#ifndef _FITKBOUNDARY_LOCATION_H__
#define _FITKBOUNDARY_LOCATION_H__

#include "FITKInterfacePhysicsAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
    class FITKAbstractModel;
    class FITKAbstractModelComponent;
    /**
     * @brief 定义边界位置类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-13
     */
    class FITKInterfacePhysicsAPI FITKBoundaryLocation
        : public Core::FITKAbstractNDataObject
    {
    public:
        /**
         * @brief Construct a new FITKBoundaryLocation object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        explicit FITKBoundaryLocation() = default;
        /**
         * @brief Destroy the FITKBoundaryLocation object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        virtual ~FITKBoundaryLocation() = default;
        /**
         * @brief 设置模型ID，模型、装配、装配实例
         * @param[i]  id            模型ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        void setModel(int id);
        /**
         * @brief 设置模型，模型、装配、装配实例
         * @param[i]  model            模型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        void setModel(FITKAbstractModel* model);
        /**
         * @brief 设置组件ID，集合、表面等
         * @param[i]  id            组件ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        void setComponent(int id);
        /**
         * @brief 设置组件，集合、表面等
         * @param[i]  comp            组件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        void setComponent(FITKAbstractModelComponent* comp);
        /**
         * @brief 获取模型id
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        int getModelID() const;
        /**
         * @brief 获取部件ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        int getComponentID() const;
        /**
         * @brief 获取模型对象
         * @return FITKAbstractModel*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        FITKAbstractModel* getModelObj();
        /**
         * @brief  获取组件对象
         * @return FITKAbstractModelComponent*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        FITKAbstractModelComponent* getComponentObj();
        /**
         * @brief  是否有效
         * @return bool
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-22
         */
        bool isValid();

        template <class T>
        /**
         * @brief 获取模型对象转换类型
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        T* getTModelObj()
        {
            return dynamic_cast<T*> (this->getModelObj());
        }
        template <class T>
        /**
         * @brief 获取组件对象，并转换类型
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        T* getTComponentObj()
        {
            return dynamic_cast<T*> (this->getComponentObj());
        }
        /**
         * @brief 获取位置字符名称
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-17
         */
        QString getLocationTextLabel();

        /**
         * @brief 运算符重载
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-03
         */
        void operator = (const FITKBoundaryLocation& loc);

    protected:
        /**
         * @brief 模型ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        int _modelID{ -1 };
        /**
         * @brief 组件ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        int _componentID{ -1 };

    };

}
#endif
