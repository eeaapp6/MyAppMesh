/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef OPERATORSSKETCHMANAGER_H
#define OPERATORSSKETCHMANAGER_H

#include "OperatorsModelAPI.h"
#include "OperManagerBase.h"
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCC.h"

namespace ModelOper
{
    class OperatorsModelAPI OperSketchManager : public OperManagerBase
    {
        Q_OBJECT
    public:
        /**
         * @brief 构造函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-19
         */
        explicit OperSketchManager() = default;
        /**
         * @brief 析构函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-19
         */
        ~OperSketchManager() override = default;

        bool setSketchMode(QString actionName);
    private slots:
        /**
         * @brief  开始草绘槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-24
         */
        void startSketchSlot();
        /**
         * @brief  结束当前草绘.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-26
         */
        void endSketchMode();

    private:
        /**
         * @brief 界面逻辑，生成UI交互
         * @return 执行成功返回true，否则返回false
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-15
         */
        bool execGUI() override;
        /**
         * @brief 业务处理逻辑，在execGUI后执行
         * @return 执行成功返回true，否则返回false
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-15
         */
        bool execProfession() override;

    private:
        /**
         * @brief  草绘类型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-26
         */
        CommonOCC::SketchShapeType m_SketchModeType{ CommonOCC::SketchShapeType::None };

    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(OperSketchManager, OperSketchManager);
    Register2FITKOperatorRepo(actionSketch2DCreate, OperSketchManager);
    Register2FITKOperatorRepo(actionSketch2DEdit, OperSketchManager);
    Register2FITKOperatorRepo(actionSketch2DDelete, OperSketchManager);
    Register2FITKOperatorRepo(actionSketch2DShow, OperSketchManager);
    Register2FITKOperatorRepo(actionSketch2DHide, OperSketchManager);

    Register2FITKOperatorRepo(actionSketchLine, OperSketchManager);
    Register2FITKOperatorRepo(actionSketchWire, OperSketchManager);
    Register2FITKOperatorRepo(actionSketchCircle, OperSketchManager);
    Register2FITKOperatorRepo(actionSketchCircleArc, OperSketchManager);
    Register2FITKOperatorRepo(actionSketchEllipse, OperSketchManager);
    Register2FITKOperatorRepo(actionSketchHyperbolaArc, OperSketchManager);
    Register2FITKOperatorRepo(actionSketchParabolaArc, OperSketchManager);
    Register2FITKOperatorRepo(actionSketchRectangle, OperSketchManager);
    Register2FITKOperatorRepo(actionSketchBezier, OperSketchManager);
    Register2FITKOperatorRepo(actionSketchBspline, OperSketchManager);

} // namespace ModelOper
#endif // !OPERATORSSKETCHMANAGER_H
