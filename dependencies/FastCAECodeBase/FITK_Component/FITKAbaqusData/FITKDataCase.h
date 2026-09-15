/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file DataCase.h
 * @brief 算例数据 ，相当于Abaqus中的model
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2023-04-14
 *
 */
#ifndef __DATACASE_H__
#define __DATACASE_H__

#include "FITKAbaqusDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

 //前置声明
ForwardDeclarNS(Interface, FITKMaterialManager)
ForwardDeclarNS(Interface, FITKAbaProfileManager)
ForwardDeclarNS(Interface, FITKAbstractSectionManager)
ForwardDeclarNS(Interface, FITKMaterialAssignManager)
//ForwardDeclarNS(Interface, FITKSectionAssignGroupManager)
ForwardDeclarNS(Interface, FITKAbaAssembly)
ForwardDeclarNS(Interface, FITKAbaStepManager)
ForwardDeclarNS(Interface, FITKAbaLoadManager)
ForwardDeclarNS(Interface, FITKAbaBoundaryConditionManager)
ForwardDeclarNS(Interface, FITKAbaAmplitudeManager)
ForwardDeclarNS(Interface, FITKAbaFieldOutputManager)
ForwardDeclarNS(Interface, FITKAbaHistoryOutputManager)
ForwardDeclarNS(Interface, FITKAbaTimePointsManager)
ForwardDeclarNS(Interface, FITKAbaInteractionManager)
ForwardDeclarNS(Interface, FITKAbaConstraintManager)
ForwardDeclarNS(Interface, FITKAbaInteractionPropertyManager)
ForwardDeclarNS(Interface, FITKAbaConnectorSectionManager)
ForwardDeclarNS(Interface, FITKAbaPredefinedFieldManager)
ForwardDeclarNS(Interface, FITKFieldsManager)

namespace AbaqusData
{
    class FITKAbaqusPartManager;
    class FITKAbaqusAssembly;
    /**
     * @brief 算例基本信息
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2025-02-13
     */
    struct  CaseInfo
    {
        QString _author{};     ///< 作者
        QString _describe{};   ///< 算例描述
        QString _createTime{}; ///< 创建时间

        bool _useAbsZeroTemp{ false }; ///< 是否使用绝对零度温度
        double _absZeroTemp{ -273.15 };    ///< 绝对零度温度
        bool _useStefanBoltzmannConst{ false }; ///< 是否使用斯特藩-玻尔兹曼常数
        double _stefanBoltzmannConst{ 5.670367e-8 }; ///< 斯特藩-玻尔兹曼常数
    };


    /**
     * @brief 算例数据
     * @author fulipeng (flipengqd@yeah.net)
     * @date 2024-03-06
     */
    class FITKAbaqusDataAPI FITKDataCase : public Core::FITKAbstractNDataObject
    {
        FITKCLASS(AbaqusData, FITKDataCase);
        //FITKSafeDownCastA(FITKDataCase, Core::FITKAbstractDataObject);
    public:
        explicit FITKDataCase();
        virtual ~FITKDataCase();

        /**
         * @brief 获取算例分析类型
         * @return 算例分析类型
         * @author fulipeng (flipengqd@yeah.net)
         * @date 2024-03-06
         */
 //       CaseAnalysisType getAnsysType() const;

        /**
         * @brief 获取部件（Part）管理器
         * @return 部件（Part）管理器
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-15
         */
        FITKAbaqusPartManager* getPartManager();

        /**
         * @brief       移除当前算例所有部件几何数据。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-03
         */
        void removePartsGeom();

        /**
         * @brief       移除当前算例装配内基准元素。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-22
         */
        void removeAssemblyDatums();

        /**
         * @brief 获取材料（Material）管理器
         * @return 材料（Material）管理器
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-20
         */
        Interface::FITKMaterialManager* getMaterialManager();
        /**
         * @brief 获取梁截面管理器
         * @return  Interface::FITKAbaProfileManager*
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-04-09
         */
        Interface::FITKAbaProfileManager* getProfileManager();
        /**
         * @brief 获取截面（section）管理器
         * @return 截面（section）管理器
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-03-22
         */
        Interface::FITKAbstractSectionManager* getSectionManager();
        /**
         * @brief 获取材料指派管理器（模型/部件/集合 → 材料）
         * @return 材料指派管理器
         */
        Interface::FITKMaterialAssignManager* getMaterialAssignManager();
        /**
         * @brief 获取截面指派组（section assign）管理器
         * @return  Interface::FITKAbstractSectionManager*
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2025-01-01
         */
 //       Interface::FITKSectionAssignGroupManager* getSectionAssignGroupManager();


        /**
         * @brief 获取装配
         * @return  FITKAbaAssembly
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-03-22
         */
        AbaqusData::FITKAbaqusAssembly* getAssembly();

        /**
         * @brief 获取分析步管理器
         * @return  FITKAbaStepManager
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-03-28
         */
        Interface::FITKAbaStepManager* getStepManager();
        /**
         * @brief 获取载荷管理器
         * @return  FITKAbaLoadManager
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-03-28
         */
        Interface::FITKAbaLoadManager* getLoadManager();
        /**
         * @brief 获取边界管理器
         * @return  FITKAbaBoundaryConditionManager
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-03-28
         */
        Interface::FITKAbaBoundaryConditionManager* getBCManager();

        /**
         * @brief 获取约束管理器
         * @return  FITKAbaConstraintManager
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-04-18
         */
        Interface::FITKAbaConstraintManager* getConstraintManager();

        /**
         * @brief 获取相互作用属性管理器
         * @return  FITKAbaInteractionPropertyManager
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-04-22
         */
        Interface::FITKAbaInteractionPropertyManager* getInteractionPropertyManager();
        /**
        * @brief 幅值管理器
        * @return Interface::FITKAbaAmplitudeManager*
        * @author libaojun (libaojunqd@foxmail.conm)
        * @date 2024-04-03
        */
        Interface::FITKAbaAmplitudeManager*  getAmplitudeManager();

        /**
         * @brief 场输出管理器
         * @return Interface::FITKAbaFieldOutputManager*
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-04-17
         */
        Interface::FITKAbaFieldOutputManager* getFieldOutputManager();

        /**
        * @brief 输出时间点管理器
        * @return Interface::FITKAbaTimePointsManager*
        * @author libaojun (libaojunqd@foxmail.conm)
        * @date 2024-04-03
        */
        Interface::FITKAbaTimePointsManager*  getTimePointsManager();

        /**
         * @brief 历程输出管理器
         * @return Interface::FITKAbaHistoryOutputManager*
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-04-22
         */
        Interface::FITKAbaHistoryOutputManager* getHistoryOutputManager();

        /**
         * @brief 相互作用-接触 管理器
         * @return Interface::FITKAbaInteractionManager*
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-04-22
         */
        Interface::FITKAbaInteractionManager* getInteractionManager();
        /**
         * @brief 连接器截面管理器
         * @return Interface::FITKAbaConnectorSectionManager*
         * @authorLiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        Interface::FITKAbaConnectorSectionManager* getConnectorSectionManager();
        /**
         * @brief 获取预定义场管理器
         * @return  FITKAbaPredefinedFieldManager
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-04
         */
        Interface::FITKAbaPredefinedFieldManager* getPredefinedFieldManager();
        /**
         * @brief 获取场管理器
         * @return Interface::FITKFieldsManager*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        Interface::FITKFieldsManager* getFieldsManager();
        /**
         * @brief 获取案例信息
         * @return CaseInfo
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-02-13
         */
        const CaseInfo& getCaseInfo() const;
        /**
         * @brief 设置案例信息
         * @param info 案例信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-02-13
         */
        void setCaseInfo(const CaseInfo& info );

        /**
         * @brief 清空数据
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-20
         */
        void clear();

        /**
         * @brief       部件模型组件校验：检查并移除部件与该部件产生的装配实例内的集合与表面不可用元素。
         * @param[in]   partId：部件ID
         * @param[in]   checkGeom：是否检查几何数据[缺省]
         * @param[in]   checkMesh：是否检查网格数据[缺省]
         * @return      发生变化的集合与表面ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-03
         */
        QList<int> validatePartComponent(int partId, bool checkGeom = true, bool checkMesh = true);

        /**
         * @brief       部件模型组件校验：尝试替换部件单元集合数据，first为原始数据ID，second为替换后数据ID。
         * @param[in]   partId：部件ID
         * @param[in]   pairs：单元替换信息
         * @return      发生变化的集合与表面ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-03
         */
        QList<int> replacePartEleComponent(int partId, QList<QPair<QList<int>, QList<int>>> & pairs);

    private:
        /***
        * @brief 设置数据对象ID，需谨慎使用
        * @param ID 数据对象id
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-06-18
        */
        void setAbsDataID(const int id) override;

    private:
        /**
         * @brief 算例信息
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-13
         */
        CaseInfo _caseInfo{};
        /**
         * @brief 部件（Part）管理器
         * @author fulipeng (flipengqd@yeah.net)
         * @date 2024-03-06
         */
        FITKAbaqusPartManager* _partManager{};
        /**
         * @brief 材料（Material）管理器
         * @author fulipeng (flipengqd@yeah.net)
         * @date 2024-03-06
         */
        Interface::FITKMaterialManager* _materialManager{};
        /**
        * @brief 梁截面（section）管理器
        * @author libaojun (libaojunqd@foxmail.conm)
        * @date 2024-04-09
        */
        Interface::FITKAbaProfileManager* _profileManager{};
        /**
         * @brief 截面（section）管理器
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-03-22
         */
        Interface::FITKAbstractSectionManager* _sectionManager{};
        /**
         * @brief 材料指派管理器
         */
        Interface::FITKMaterialAssignManager* _materialAssignManager{};
        /**
        * @brief 截面组（section）管理器
        * @author libaojun (libaojunqd@foxmail.conm)
        * @date 2025-01-01
        */
//        Interface::FITKSectionAssignGroupManager* _sectionAssGroupManager{};
        /**
         * @brief 装配体
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-03-22
         */
        AbaqusData::FITKAbaqusAssembly* _assembly{};
        /**
         * @brief 分析步管理器
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-03-28
         */
        Interface::FITKAbaStepManager* _stepManager{};
        /**
        * @brief 载荷管理器
        * @author libaojun (libaojunqd@foxmail.conm)
        * @date 2024-03-28
        */
        Interface::FITKAbaLoadManager* _loadManager{};
        /**
        * @brief 边界条件管理器
        * @author libaojun (libaojunqd@foxmail.conm)
        * @date 2024-03-28
        */
        Interface::FITKAbaBoundaryConditionManager* _bcManager{};
        /**
        * @brief 幅值管理器
        * @author libaojun (libaojunqd@foxmail.conm)
        * @date 2024-04-03
        */
        Interface::FITKAbaAmplitudeManager* _ampManager{};
        /**
        * @brief 时间点管理器
        * @author libaojun (libaojunqd@foxmail.conm)
        * @date 2024-04-03
        */
        Interface::FITKAbaTimePointsManager* _timepointsManager{};

        /**
         * @brief  相互作用管理器
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-04-18
         */
        Interface::FITKAbaInteractionManager* _interactionManager{};

        /**
         * @brief 相互作用属性管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-04-22
         */
        Interface::FITKAbaInteractionPropertyManager* _interactionPropertyManager{};
        /**
         * @brief 约束管理器
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-04-18
         */
        Interface::FITKAbaConstraintManager* _contrainsManager{};

        /**
         * @brief 场输出管理器
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-04-17
         */
        Interface::FITKAbaFieldOutputManager* _fieldOutputManager{};
        /**
         * @brief 历程输出管理器
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-04-22
         */
        Interface::FITKAbaHistoryOutputManager* _historyOutputManager{};
        /**
         * @brief 连接器截面管理器
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        Interface::FITKAbaConnectorSectionManager* _connectorSecManager{};
        /**
         * @brief 预定义场管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-04
         */
        Interface::FITKAbaPredefinedFieldManager* _predefineFieldManager{};
        /**
         * @brief 场管理器
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        Interface::FITKFieldsManager* _fieldsManager{};

        /**
         * @brief Preprint下的属性信息
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-15
         */
        bool _echo = false;
        /**
         * @brief Preprint下的属性信息
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-15
         */
        bool _model = false;
        /**
         * @brief Preprint下的属性信息
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-15
         */
        bool _history = false;
        /**
         * @brief Preprint下的属性信息
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-15
         */
        bool _contact = false;
    };
} // namespace AbaqusData
#endif

