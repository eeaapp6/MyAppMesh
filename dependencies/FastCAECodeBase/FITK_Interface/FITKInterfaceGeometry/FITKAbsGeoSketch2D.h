/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoSketch2D.h
 * @brief       2D平面草绘数据抽象类。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-03
 *********************************************************************/

#ifndef  FITKABSGEOSKETCH2D_H
#define  FITKABSGEOSKETCH2D_H

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

#include "FITKGeoEnum.h"
#include "FITKAbsGeoCommand.h"
#include "FITKAbsGeoModelCurve.h"
#include "FITKAbsConstruction.h"

#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    ///**
    // * @brief   2D平面草绘构造形状类。
    // * @author  Yanzhihui (chanyuantiandao@126.com)
    // * @date    2024-09-03
    // */
    //struct FITKInterfaceGeometryAPI FITKAbsGeoSketch2DConstruction {
    //    std::array<double, 3> point1{};
    //    std::array<double, 3> point2{};
    //    /// 类型：1->构造线, 2->构造圆
    //    int type = 1;
    //};
    /**
     * @brief       2D平面草绘数据抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-03
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoSketch2D : public Interface::FITKAbsGeoCommand, public FITKGeoCommandManager
    {
        FITKCLASS(Interface, FITKAbsGeoSketch2D);
        Q_OBJECT

    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-03
         */
        FITKAbsGeoSketch2D();

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-03
         */
        virtual ~FITKAbsGeoSketch2D();

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       获取数据类型。[重写]
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        bool update() override;

        /**
         * @brief       重建草图。[虚函数]
         * @param[in]   fromDataId：从指定ID的命令开始更新，为-1则从第一条命令开始更新[缺省]
         * @return      执行中断命令ID，-1则表示重构模型正常退出
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-22
         */
        virtual int reGenerate(int fromDataId = -1) override;

        /**
         * @brief       设置草图平面信息。
         * @param[in]   pos：面中心点坐标
         * @param[in]   normal：面法向
         * @param[in]   up：面向上方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-20
         */
        void setPlane(double* pos, double* normal, double* up);

        /**
         * @brief       获取草图平面信息。
         * @param[out]  pos：面中心点坐标
         * @param[out]  normal：面法向
         * @param[out]  up：面向上方向
         * @return      是否为有效平面
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-20
         */
        void getPlane(double* pos, double* normal, double* up);

        /**
         * @brief       设置基准元素位置。
         * @param[in]   pos：坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        void setPosition(double* pos);

        /**
         * @brief       获取基准元素位置。
         * @param[out]  pos：坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        void getPosition(double* pos);

        /**
         * @brief       设置基准面法向。
         * @param[in]   nor：法向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        void setNormal(double* nor);

        /**
         * @brief       获取基准面法向。
         * @param[out]  nor：法向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        void getNormal(double* nor);

        /**
         * @brief       设置基准面向上方向。
         * @param[in]   up：向上方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        void setUp(double* up);

        /**
         * @brief       获取基准面向上方向。
         * @param[out]  up：向上方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        void getUp(double* up);

        /**
         * @brief       设置草图初始默认尺寸。
         * @param[in]   size：尺寸
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-05
         */
        void setDefaultSize(double size);

        /**
         * @brief       获取草图初始默认尺寸。
         * @return      尺寸
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-05
         */
        double getDefaultSize();

        /**
         * @brief       撤销。[虚函数][重写]
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-20
         */
        virtual bool undo() override;

        /**
         * @brief       重做。[虚函数][重写]
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-20
         */
        virtual bool redo() override;

        /**
         * @brief       重写添加数据对象功能。（添加命令并清空RedoList）[虚函数][重写]
         * @param[in]   obj：数据对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-20
         */
        virtual void appendDataObj(Core::FITKAbstractDataObject* obj) override;

        /**
         * @brief       获取是否可以执行撤销操作。[虚函数][重写]
         * @return      是否可以撤销
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-20
         */
        virtual bool canUndo() override;

        /**
         * @brief       获取是否可以执行重做操作。[虚函数][重写]
         * @return      是否可以重做
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-20
         */
        virtual bool canRedo() override;

        /**
         * @brief       获取撤销列表。
         * @return      撤销列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-20
         */
        FITKGeoCommandManager* getUndoList();

        /**
         * @brief       获取重做列表。
         * @return      重做列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-20
         */
        FITKGeoCommandManager* getRedoList();

        /**
         * @brief       获取构造线管理器。
         * @return      构造线管理器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        FITKConstructionManager* getConstructionManager();

        /**
         * @brief       获取当前激活构造线。（2025/01/02当前版本默认最后一条构造线为激活状态）
         * @return      构造线
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-01-02
         */
        FITKAbsConstruction* getCurrentConstruction();

        /**
         * @brief  草图是否为空.
         * @param  construction 是否把构造形状计算在内
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2024-12-17
         */
        virtual bool isEmpty(bool construction = false);
        /**
         * @brief  判断草绘是否为封闭的
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2024-12-25
         */
        virtual bool isClosed();
        /**
         * @brief  判断草绘是否为自相交
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2024-12-25
         */
        virtual bool isSelfIntersection();        

        /**
         * @brief       判断草图是否连贯。[虚函数]
         * @return      是否连贯
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-16
         */
        virtual bool isConnected();

        /**
         * @brief  检查草绘状态
         * @param  isClosed 是否闭合
         * @param  isSelfIntersection 是否自相交
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2024-12-25
         */
        virtual void checkState(bool& isClosed, bool& isSelfIntersection);
        /**
         * @brief  获取草图里面的第一个点以及该点的方向
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2024-12-25
         */
        virtual bool getFirstPointAndDir(double* point, double* dir);
        /**
         * @brief  获取草图里面的最后一个点以及该点的方向
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2024-12-25
         */
        virtual bool getLastPointAndDir(double* point, double* dir);
        ///**
        //  * @brief  判断草绘是否包含构造线（用于旋转轴）
        //  * @author Yanzhihui (chanyuantiandao@126.com)
        //  * @date   2024-12-25
        //  */
        //virtual bool hasConstructionLine();
        ///**
        //  * @brief  获取草绘中最后一条构造线
        //  * @author Yanzhihui (chanyuantiandao@126.com)
        //  * @date   2024-12-25
        //  */
        //virtual bool getLastConstructionLine(FITKAbsGeoSketch2DConstruction& line);
        ///**
        // * @brief  添加构造形状.
        // * @author Yanzhihui (chanyuantiandao@126.com)
        // * @date   2024-12-30
        // */
        //void addConstruction(FITKAbsGeoSketch2DConstruction construct);

        ///**
        // * @brief     获取全部构造线。
        // * @return    构造线
        // * @author    ChengHaotian (yeguangbaozi@foxmail.com)
        // * @date      2024-12-31
        // */
        //const QVector<FITKAbsGeoSketch2DConstruction> & getConstructions();
        /**
         * @brief  草图的用途（用去区分不同的提示以及草图检查）。
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-04-24
         */
        enum SketchUseFor {
            Undefined,
            // PartCreateWire,
            PartCreatePlanar,
            PartExtrudeSolid,
            PartRevolveSolid,
            PartSweepSolidProfile,
            PartSweepSolidPath,
            PartPlanarShell,
            PartExtrudeShell,
            PartRevolveShell,
            PartSweepShellProfile,
            PartSweepShellPath,
            PartFromSketch,
            PartExtrudeCut,
            PartRevolveCut,
            PartSweepCutProfile,
            PartSweepCutPath,
            PartitionFace,
            PartitionCell,        
        };
        Q_ENUM(SketchUseFor)
            SketchUseFor userFor() const { return m_UserFor; };
        void setUserFor(SketchUseFor usefor) { m_UserFor = usefor; };

    protected:
        /**
         * @brief       归一化。[静态]
         * @param[in]   db3：长度为3的数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-19
         */
        static void Normalize(double* db3);

    protected:
        /**
         * @brief       位置坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        double m_pos[3]{ 0., 0., 0. };

        /**
         * @brief       基准面法向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        double m_nor[3]{ 0., 0., 1. };

        /**
         * @brief       基准面向上方向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        double m_up[3]{ 0., 1., 0. };

        /**
         * @brief       子命令重做列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-20
         */
        FITKGeoCommandManager* m_redoList{ nullptr };

        /**
         * @brief       草图初始默认尺寸。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-05
         */
        double m_defaultSize = 200.;

        ///**
        // * @brief  构造形状列表
        // * @author Yanzhihui (chanyuantiandao@126.com)
        // * @date   2024-12-25
        // */
        //QVector<FITKAbsGeoSketch2DConstruction> m_ConstrctionList{};

        /**
         * @brief       构造线管理器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-31
         */
        FITKConstructionManager* m_constructionMgr{ nullptr };
        /**
         * @brief  草图的用途（用去区分不同的提示以及草图检查）。
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-04-24
         */
        SketchUseFor m_UserFor{};

    };
}

#endif // !FITKABSGEOSKETCH2D_H
