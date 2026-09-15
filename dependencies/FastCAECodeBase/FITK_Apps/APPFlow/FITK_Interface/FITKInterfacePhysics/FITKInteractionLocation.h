/**
 *
 * @file FITKInteractionLocation.h
 * @brief 相互作用位置声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-30
 *
 */
#ifndef __FITKINTERACTION_LOCATION_H__
#define __FITKINTERACTION_LOCATION_H__

#include "FITKInterfacePhysicsAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

#include <QList>

namespace Interface
{
    class FITKBoundaryLocation;
    /**
     * @brief 相互作用声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-30
     */
    class FITKInterfacePhysicsAPI FITKInteractionLocation : public Core::FITKAbstractNDataObject
    {
    public:
        /**
         * @brief Construct a new FITKInteractionLocation object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        explicit FITKInteractionLocation();
        /**
         * @brief Destroy the FITKInteractionLocation object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        virtual ~FITKInteractionLocation();
        /**
         * @brief 获取第一个位置
         * @return FITKBoundaryLocation*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        FITKBoundaryLocation* getFirstLocation();
        /**
         * @brief 获取第二个位置
         * @return FITKBoundaryLocation*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        FITKBoundaryLocation* getSecondLocation();
        /**
         * @brief 交换第一位置与第二位置
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        void swap();
        /**
         * @brief 获取第index个对象
         * @param[i]  index          索引
         * @return FITKBoundaryLocation*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-03
         */
        FITKBoundaryLocation* getLocationAt(const int index);
        /**
         * @brief 获取数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-03
         */
        int getLocationCount() const;
        /**
         * @brief 从obj复制对象
         * @param[i]  obj          复制对象
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-03
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief 移除第index个对象
         * @param[i]  index         索引
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-05-16
         */
        void removeLocationAt(int index);
        /**
         * @brief 批量移除对象
         * @param[i]  indexs         对象索引列表
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-05-16
         */
        void removeLocationAt(QList<int> indexs);
        /**
         * @brief 移除对象，可用于移除接触对
         * @param[i]  index1         第一位置索引
         * @param[i]  index2         第二位置索引
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-05-16
         */
        void removeLocationAt(int index1, int index2);
        /**
         * @brief 移除对象
         * @param[i]  loc             位置对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-05-16
         */
        void removeLocation(FITKBoundaryLocation* loc);
        /**
         * @brief 移除最后一个
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-05-16
         */
        void removeLastLocation();
        /**
         * @brief 将位置设置为无效
         * @param[i]  indexs         对象索引
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-05-22
         */
        void setLocationInvalid(const int index);

    private:
        /**
         * @brief 重新设置大小
         * @param[i]  nize          数量
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-03
         */
        void resize(const int nize);

    protected:
        /**
         * @brief 位置链表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-03
         */
        QList<FITKBoundaryLocation*> _locations{};
    };
}


#endif // ifndef