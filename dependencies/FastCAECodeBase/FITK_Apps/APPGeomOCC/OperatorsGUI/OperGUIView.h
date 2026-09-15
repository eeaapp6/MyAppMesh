/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef OPERGUIVIEW_H
#define OPERGUIVIEW_H

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include <QAction>

namespace GUI {
    class MultiViewport;
    class Viewport;
}

namespace GUIOper
{
    /**
     * @brief  视口菜单对应的操作器类
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-05-09
     */
    class OperGUIView : public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        /**
         * @brief 构造函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-05-09
         */
        explicit OperGUIView() = default;
        /**
         * @brief 析构函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-05-09
         */
        ~OperGUIView() = default;

    private:
        /**
         * @brief 界面逻辑，生成UI交互
         * @return 执行成功返回true，否则返回false
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2024-05-09
         */
        bool execGUI() override;
        /**
         * @brief 业务处理逻辑，在execGUI后执行
         * @return 执行成功返回true，否则返回false
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2024-05-09
         */
        bool execProfession() override;

    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(OperGUIView, OperGUIView);
    Register2FITKOperatorRepo(actionGUIViewFront, OperGUIView);
    Register2FITKOperatorRepo(actionGUIViewBack, OperGUIView);
    Register2FITKOperatorRepo(actionGUIViewTop, OperGUIView);
    Register2FITKOperatorRepo(actionGUIViewBottom, OperGUIView);
    Register2FITKOperatorRepo(actionGUIViewLeft, OperGUIView);
    Register2FITKOperatorRepo(actionGUIViewRight, OperGUIView);
    Register2FITKOperatorRepo(actionGUIViewISO, OperGUIView);
    Register2FITKOperatorRepo(actionGUIViewAutoFit, OperGUIView);
    Register2FITKOperatorRepo(actionGUIViewOrthogonal, OperGUIView);
    Register2FITKOperatorRepo(actionGUIViewPerspective, OperGUIView);
    Register2FITKOperatorRepo(actionGUIViewSaveImage, OperGUIView);
    Register2FITKOperatorRepo(actionGUIViewShowGrid, OperGUIView);
    Register2FITKOperatorRepo(actionGUIShowDatums, OperGUIView);


}

#endif // !OPERGUIVIEW_H
