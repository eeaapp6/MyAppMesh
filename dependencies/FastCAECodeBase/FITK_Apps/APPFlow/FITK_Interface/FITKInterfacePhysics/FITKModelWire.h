/**
 *
 * @file FITKModelWire.h
 * @brief  线特征与管理器声明
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-06-20
 *
 */
#ifndef __FITK_MODEL_LOCATION_H___
#define __FITK_MODEL_LOCATION_H___

#include <QList>
#include <QPair>
#include "FITKInterfacePhysicsAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"

namespace Interface
{
    class FITKAbstractModel;

    /**
     * @brief 线特征声明，存储结构为模型ID+节点/单元ID列表
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-06-20
     */
    class FITKInterfacePhysicsAPI FITKModelWire
        : public Core::FITKAbstractNDataObject
    {
    public:
        /**
         * @brief Construct a new FITKModelWire object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        explicit FITKModelWire() = default;
        /**
         * @brief Destroy the FITKModelWire object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        virtual ~FITKModelWire() = default;
        /**
         * @brief 复制对象
         * @param[i]  obj         待复制的对象
         * @return true
         * @return false
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        virtual bool copy(FITKAbstractDataObject* obj) override;
        /**
         * @brief 指定成员对象类型
         * @param[i]  type           成员对象类型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        void setLocationType(FITKModelEnum::FITKModelSetType type);
        /**
         * @brief 获取成员对象类型
         * @return FITKModelEnum::FITKModelSetType
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        FITKModelEnum::FITKModelSetType getLocationType();
        /**
         * @brief 获取成员对象数量
         * @return int
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        int getMemberCount();
        /**
         * @brief 获取模型ID
         * @param[i]  index          成员对象索引
         * @return int
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        int getModelID(const int index);
        /**
         * @brief 获取成员对象ID
         * @param[i]  index          成员对象索引
         * @return int
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        int getMemberID(const int index);
        /**
         * @brief 添加成员对象
         * @param[i]  modelID        模型ID
         * @param[i]  memberID       成员对象ID
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        void appendMember(const int modelID, const int memberID);
        /**
         * @brief 获取成员对象
         * @param[i]  index          成员对象索引
         * @return FITKAbstractModel*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        FITKAbstractModel* getModel(const int index);
        /**
         * @brief 设置成员对象
         * @param[i]  index          成员对象索引
         * @param[i]  model          模型ID
         * @param[i]  member         成员对象ID
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        void setModelMember(const int index, const int model, const int member);
        /**
         * @brief 获取成员对象名称
         * @param[i]  index          成员对象索引
         * @return QString
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        QString getModelName(const int index);
        /**
         * @brief 移除成员对象
         * @param[i]  index          成员对象索引
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        void removeMember(const int index);
        /**
         * @brief 移除成员对象
         * @param[i]  modelID        模型ID
         * @param[i]  memberID       成员对象ID
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        void removeMember(const int modelID, const int member);
        /**
         * @brief 交换成员对象，只交换第一个和第二个
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        void swap();
        /**
         * @brief 获取成员对象文本标签， 模型名称 +类型[节点/单元ID]
         * @param[i]  index          成员对象索引
         * @return QString
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        QString getTextLabel(const int index);
        /**
         * @brief 获取成员对象坐标
         * @param[i]  index          成员对象索引
         * @param[o]  coor           坐标
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        void getPointCoor(const int index, double* coor);

    private:
        /**
         * @brief 成员对象类型，默认是节点类型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        FITKModelEnum::FITKModelSetType _locType{ FITKModelEnum::FMSNode };
        /**
         * @brief 组成成员，模型ID + 成员对象ID
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        QList<QPair<int, int>> _memberLoc{};

    };

    /**
     * @brief 线对象管理器
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-06-20
     */
    class FITKInterfacePhysicsAPI FITKModelWireManager
        : public Core::FITKAbstractDataManager<FITKModelWire>
    {
    public:
        /**
         * @brief Construct a new FITKModelWireManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        explicit FITKModelWireManager() = default;
        /**
         * @brief Destroy the FITKModelWireManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        virtual ~FITKModelWireManager() = default;
    };
}



#endif
