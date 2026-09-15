/**
 *
 * @file FITKAbstractSection.h
 * @brief 截面抽象类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-30
 *
 */
#ifndef __FITKABSTRACTSEDTION_H__
#define __FITKABSTRACTSEDTION_H__

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKInterfacePhysicsAPI.h"
#include <QList>


namespace Interface
{
    class FITKAbstractMaterial;
    /**
     * @brief section附加信息，例如惯性矩等
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2025-02-27
     */
    class FITKInterfacePhysicsAPI FITKAbstractSectionInfo : public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
    public:
        /**
         * @brief section 类型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        enum SectionType
        {
            STNone,      ///<无类型
            STNasBDF,    ///<Nastran  bdf 格式中的section
        };
        Q_ENUM(SectionType);
    public:
        /**
         * @brief Construct a new FITKAbstractSectionInfo object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        explicit FITKAbstractSectionInfo() = default;
        /**
         * @brief Destroy the FITKAbstractSectionInfo object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        virtual ~FITKAbstractSectionInfo() = 0;
        /**
         * @brief 获取类型
         * @return SectionType 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        virtual SectionType getSectionType() = 0;
    };


    /**
     * @brief 抽象截面类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-30
     */
    class FITKInterfacePhysicsAPI FITKAbstractSection : public Core::FITKAbstractNDataObject
    {
    public:
    /**
     * @brief Construct a new FITKAbstractSection object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-30
     */
        explicit FITKAbstractSection();
        /**
         * @brief Destroy the FITKAbstractSection object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        virtual ~FITKAbstractSection() = 0;

        /**
         * @brief 设置材料的数据ID
         * @param[i]  id            材料id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        void setMaterial(int id, int index = 0);
        /**
         * @brief 设置材料
         * @param[i]  mat            材料指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        void setMaterial(Core::FITKAbstractDataObject* mat, int index = 0);
        /**
         * @brief 获取材料ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        int getMaterialID( int index = 0) const;
        /**
         * @brief 获取材料对象
         * @return FITKAbaMaterial*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        FITKAbstractMaterial* getMaterial(int index = 0) const;
        /**
         * @brief 获取材料名称
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        QString getMaterialName( int index = 0) const;
        /**
         * @brief 获取材料数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        int getMaterialCount();
        /**
         * @brief 移除第index个材料
         * @param[i] index 索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        void removeMaterial(int index);

        /**
          * @brief  获取截面类型字符串，用于UI显式
          * @return QString
          * @author libaojun (libaojunqd@foxmail.com)
          * @date   2024-03-27
          */
        virtual QString getTypeString();
        /**
         * @brief 获取截面id，注意：该值与管理器的中使用的id无关，与其他对象的关联也无关
         * @return int
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        int getSectionID() const;
        /**
          * @brief  获取截面信息
          * @return FITKAbstractSectionInfo*
          * @author libaojun (libaojunqd@foxmail.com)
          * @date   2025-02-27
          */
        FITKAbstractSectionInfo* getSectionInfo();
        /**
         * @brief 设置截面信息
         * @param[i]  info         截面信息
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        void setSectionInfo(FITKAbstractSectionInfo* info);

    private:
        /**
         * @brief 设置材料数组长度，不足的补充-1
         * @param[i]  count        最大长度
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-27
         */
        void setSize(int count);

    protected:
        /**
         * @brief 材料列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-27
         */
        QList<int> _materialIDs{};

        FITKAbstractSectionInfo* _sectionInfo{};

    private:
        /**
         * @brief 截面id，与管理器无关，不能被使用
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        int _sectionID = -1;
    };

    /**
     * @brief 截面管理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-30
     */
    class FITKInterfacePhysicsAPI FITKAbstractSectionManager : public Core::FITKAbstractObject,
        public Core::FITKAbstractDataManager<FITKAbstractSection>
    {
    public:
        explicit FITKAbstractSectionManager() = default;
        virtual ~FITKAbstractSectionManager() = default;

    };
}



#endif
