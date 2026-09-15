/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorsGraphPreprocess.h
 * @brief  渲染操作器
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _OperatorsGraphPreprocess_H__
#define _OperatorsGraphPreprocess_H__

#include "OperatorsGUIAPI.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace GUI {
    class MainWindow;
}

namespace Interface {
    class PostGraphObjectManager;
    class FITKCFDPost3DManager;
    class FITKCFDPostLightManager;
};

namespace Comp {
    class FITKGraph3DWindowVTK;
}

namespace OperGUI
{
    /**
     * @brief  渲染操作器
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class OperatorsGUIAPI OperatorsGraphPreprocess : public EventOper::GraphEventOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        explicit OperatorsGraphPreprocess();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        ~OperatorsGraphPreprocess() = default;
        /**
         * @brief    更新全部
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual void updateAll() override;
        /**
         * @brief    更新单个
         * @param[i] postDataID 后处理数据id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual void updateSingle(int postDataID) override;
        /**
         * @brief    更新是否显示
         * @param[i] postDataID 后处理数据id
         * @param[i] isShow     是否显示
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        virtual void updateShow(int postDataID, bool isShow) override;
        /**
         * @brief    更新灯光
         * @param[i] dataID     灯光ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        virtual void updateLight(int dataID) override;
        /**
         * @brief    更新是否全部旋转可见性
         * @param[i] isVis                      是否全部可见
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-19
         */
        virtual void updateAllRotateVis(bool isVis) override;
        /**
         * @brief    强制刷新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        virtual void reRender() override;
    private slots:
        ;
        /**
         * @brief    主窗口关闭事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-12
         */
        void slotMainWindowClose();
    private:
        /**
         * @brief    获取渲染窗口对象
         * @return   FITKGraph3DWindowVTK*  渲染窗口
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        Comp::FITKGraph3DWindowVTK* getGraphWidget();
    private:
        /**
         * @brief  主窗口
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        GUI::MainWindow* _mainWin = nullptr;
        /**
         * @brief  渲染对象管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        Interface::PostGraphObjectManager* _postGraphManager = nullptr;
        /**
         * @brief  渲染数据管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        Interface::FITKCFDPost3DManager* _postDataManager = nullptr;
        /**
         * @brief  灯光数据管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-16
         */
        Interface::FITKCFDPostLightManager* _postListManager = nullptr;
    };

    Register2FITKOperatorRepo(OperGraphPreprocess, OperatorsGraphPreprocess);

}  // namespace GUIOper

#endif