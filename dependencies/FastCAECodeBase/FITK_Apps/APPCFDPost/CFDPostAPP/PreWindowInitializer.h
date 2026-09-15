/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PreWindowInitializer.h
 * @brief  前处理窗口操作器初始化
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-11-04
 *********************************************************************/

#ifndef __PRE_WIDOWINIINTIALIZER_H___
#define __PRE_WIDOWINIINTIALIZER_H___

#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowInterface.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphInteractionStyle.h"

#include <QTimer>

namespace EventOper{
    class GraphInteractionOperator;
    class GraphEventOperator;
}

namespace Comp{
    class FITKGraph3DWindowVTK;
}

class vtkRenderer;
class vtkRenderWindow;
/**
 * @brief  前处理窗口初始化
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-11-04
 */
class PreWindowInitializer : public Comp::Graph3DWindowInitializer
{
public:
    /**
     * @brief    构造函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    explicit PreWindowInitializer();
    /**
     * @brief    析构函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    virtual ~PreWindowInitializer() = default;
    /**
     * @brief    获取交互方式
     * @return   Comp::FITKGraphInteractionStyle*  交互方式
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    virtual Comp::FITKGraphInteractionStyle* getStyle() override;
};


class PreWindowInteractionStyle : public  Comp::FITKGraphInteractionStyle
{
    Q_OBJECT
public:
    /**
     * @brief  创建实例[静态]
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-11-04
     */
    static PreWindowInteractionStyle* New();
    vtkTypeMacro(PreWindowInteractionStyle,FITKGraphInteractionStyle);
private:
    /**
     * @brief    鼠标左键按下事件
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    virtual void OnLeftButtonDown() override;
    /**
     * @brief    鼠标左键抬起事件
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    virtual void OnLeftButtonUp() override;
    /**
     * @brief    鼠标中键按下事件
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    virtual void OnMiddleButtonDown() override;
    /**
     * @brief    鼠标中键抬起事件
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    virtual void OnMiddleButtonUp() override;
    /**
     * @brief    鼠标移动事件
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    virtual void OnMouseMove() override;
    /**
     * @brief    鼠标滚轮向前事件
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    void OnMouseWheelForward() override;
    /**
     * @brief    鼠标滚轮向后事件
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    void OnMouseWheelBackward() override;
    /**
     * @brief    鼠标右键按下事件
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    void OnRightButtonDown() override;
    /**
     * @brief    鼠标右键抬起事件
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    void OnRightButtonUp() override;
    /**
     * @brief    框选
     * @param[i] startPos  鼠标起始坐标 
     * @param[i] endPos    鼠标结束坐标
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    void areaPick(int* startPos, int* endPos) override;
private:
    /**
     * @brief    构造函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    PreWindowInteractionStyle();
    /**
     * @brief    析构函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    ~PreWindowInteractionStyle() = default;
    /**
     * @brief    获取当前渲染窗口
     * @return   Comp::FITKGraph3DWindowVTK*  当前渲染窗口
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-04
     */
    Comp::FITKGraph3DWindowVTK* getCurrentGraphWindow();
    /**
     * @brief   在当前鼠标位置执行一次点击拾取操作。
     * @param   isPreview：是否为预选[缺省]
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-11-04
     */
    void pick(bool isPreview = false);
    /**
     * @brief   在当前鼠标位置执行一次点击拾取节点操作。（预留）
     * @param   graphWindow：可视化窗口
     * @param   pos：拾取位置
     * @param   isPreview：是否为预选
     * @param   tol：拾取精度容差[缺省]
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-11-04
     */
    void pickPoint(Comp::FITKGraph3DWindowVTK* graphWindow, int* pos, bool isPreview, double tol = 0.005);
    /**
     * @brief   在当前鼠标位置执行一次点击拾取单元操作。（点、线、面、体拾取均使用此方法）
     * @param   graphWindow：可视化窗口
     * @param   pos：拾取位置
     * @param   isPreview：是否为预选
     * @param   tol：拾取精度容差[缺省]
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-11-04
     */
    void pickCell(Comp::FITKGraph3DWindowVTK* graphWindow, int* pos, bool isPreview, double tol = 0.003);
private slots:
    /**
     * @brief  预选高亮定时器槽函数，执行预选高亮
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-11-04
     */
    void slot_prePick();
private:
    /**
     * @brief  预选高亮定时器，悬浮200ms后显示预选拾取对象
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-11-04
     */
    QTimer m_timerForPrePick;
    /**
     * @brief  预选高亮可视化操作器
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-11-04
     */
    EventOper::GraphInteractionOperator* m_operPreview{ nullptr };
    /**
     * @brief  高亮可视化操作器
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-11-04
     */
    EventOper::GraphInteractionOperator* m_operPick{ nullptr };
    /**
     * @brief  前处理可视化操作器
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-11-04
     */
    EventOper::GraphEventOperator* m_operGraph{ nullptr }; 
};
#endif