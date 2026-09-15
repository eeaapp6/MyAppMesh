/**
*
* @file FITKMaterialAssign.h
* @brief 截面指派类声明
* @author LiBaojun (libaojunqd@foxmail.com)
* @date 2024-03-07
*
*/
#ifndef __FITKSECTION_ASSIGN_H__
#define __FITKSECTION_ASSIGN_H__

#include "FITKInterfacePhysicsAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    /**
     * @brief 截面指派
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-07
     */
    class FITKInterfacePhysicsAPI FITKSectionAssign : public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief Construct a new FITKSectionAssign object
         * @param[i]  modelID        模型ID，可以是集合也可以是整个模型
         * @param[i]  sectionID      截面ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        explicit FITKSectionAssign(int modelID, int sectionID);
        explicit FITKSectionAssign() = default;
        /**
         * @brief Destroy the FITKMaterialAssign object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual ~FITKSectionAssign() = default;
        /**
         * @brief 获取材料ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual int getSectionID();
        /**
         * @brief 获取模型ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual int getModelID();
        /**
         * @brief 建立截面与模型的关联关系
         * @param[i]  modelID        模型ID，可以是集合也可以是整个模型
         * @param[i]  sectionID      section ID
         * @param[i]  checkValid     检查有效性，true时，强制检查，数据错误不进行记录
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual void assign(int modelID, int sectionID, bool checkValid = true);
        /**
         * @brief 设置section
         * @param[i]  sectionID      section ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-01-01
         */
        virtual void setSection(const int sectionID);
        /**
         * @brief 是否为有效关联，能够找到截面与模型就认为有效
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
        T* getTSectionObj()
        {
            return dynamic_cast<T*>(this->getSectionObj());
        }

        template<class T>
        /**
         * @brief  获取模型并进行类型转换
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        T* getTModelObj()
        {
            return dynamic_cast<T*>(this->getModelObj());
        }
        /**
         * @brief 获取Section名称
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-27
         */
        QString getSectionName();
        /**
         * @brief 获取Section类型字符串
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-27
         */
        virtual QString getSectionType();
        bool copy(FITKAbstractDataObject* obj) override;
        /**
         * @brief 获取集合/模型的名称
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-27
         */
        QString getModelName();

        /**
         * @brief 数据是否被使用（关联）
         * @param[i]  data           被检查的数据ID
         * @return true              管理器中使用了Data中包含的数据
         * @return false             管理器中没有使用Data中包含的数据
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-12
         */
        virtual bool isUsedDataObject(const QList<int> & dataIDs) override;
    protected:
        /**
         * @brief 获取截面
         * @return FITKAbstractDataObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual FITKAbstractDataObject* getSectionObj();

        /**
         * @brief 获取模型
         * @return FITKAbstractDataObject*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        FITKAbstractDataObject* getModelObj();



    protected:
        /**
         * @brief 模型ID，集合ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        int _modelID{ -1 };
        /**
         * @brief 截面ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        int _sectionID{ -1 };
    };
    /**
     * @brief 截面指派管理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-07
     */
    class FITKInterfacePhysicsAPI FITKSectionAssignManager : public Core::FITKAbstractNDataObject
        , public Core::FITKAbstractDataManager<FITKSectionAssign>
    {
    public:
        /**
         * @brief Construct a new FITKSectionAssignManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        explicit FITKSectionAssignManager() = default;
        /**
         * @brief Destroy the FITKSectionAssignManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        virtual ~FITKSectionAssignManager() = default;
        /**
         * @brief 根据集合获取截面指派
         * @param[i]  modelsetID     集合ID
         * @return FITKSectionAssign* 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        FITKSectionAssign* getMaterialAssignByModelSetID(const int modelsetID);


    };

}


#endif
