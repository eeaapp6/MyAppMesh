/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperGraphPreprocess.h
 * @brief  前处理可视化事件操作器
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-05-22
 *********************************************************************/

#ifndef _OPERGRAPHPREPROCESS_H__
#define _OPERGRAPHPREPROCESS_H__

#include "OperGraphEvent3D.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUIOper
{
    /**
     * @brief  前处理可视化事件操作器
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-05-22
     */
    class OperGraphPreprocess : public OperGraphEvent3D
    {
        Q_OBJECT
    public:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        explicit OperGraphPreprocess() = default;
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        ~OperGraphPreprocess() = default;
        /**
         * @brief  刷新渲染对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        virtual void updateGraph(int dataObjId, bool forceUpdate = false, bool isFitView = false) override;
        /**
         * @brief    更新全部可视化对象。（没有则创建）[虚函数]
         * @param[i] forceUpdate                针对已存在模型是否强制刷新
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-04
         */
        virtual void updateAll(bool forceUpdate = false) override;
        /**
         * @brief    更新部分颜色
         * @param[i] dataObjId                对象id -1表示全部几何对象 -2表示全部网格对象
         * @param[i] color                    颜色
         * @param[i] otherPar                 其他参数 0：点颜色 1：线颜色
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-28
         */
        virtual void updateColor(int dataObjId, QColor color, QVariant otherPar = QVariant()) override;
        /**
         * @brief    更新整体颜色
         * @param[i] dataObjId                对象id
         * @param[i] color                    颜色
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-07-18
         */
        void updateAllColor(int dataObjId, QColor color) override;
        /**
         * @brief    更新全部模型可视化对象。（没有则创建）[虚函数]
         * @param[i] forceUpdate                针对已存在模型是否强制刷新
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-04
         */
        virtual void updateAllModel(bool forceUpdate) override;
        /**
         * @brief    隐藏或显示模型。
         * @param    dataObjId：               数据对象ID
         * @param    visibility：              是否可见
         * @param    isFitView：               是否自适应
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-07-18
         */
        virtual void setModelVisible(int dataObjId, bool visibility, bool isFitView = true) override;
        /**
         * @brief    隐藏或显示点演员。
         * @param    visibility：              是否可见
         * @param    isPart：                  是否是Part
         * @param    isFitView：               是否自适应
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void setPointVisible(bool visibility,bool isPart = true, bool isFitView = true) override;
        /**
         * @brief    隐藏或显示线演员。
         * @param[i] visibility                 是否可见
         * @param[i] isPart                     是否是Part
         * @param    isFitView                  是否自适应
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-19
         */
        void setLineVisible(bool visibility, bool isPart = true, bool isFitView = true) override;
        /**
         * @brief    设置全部部件显示隐藏状态
         * @param[i] visibility                 是否可见
         * @param[i] isPart                     是否是Part
         * @param    isFitView                  是否自适应
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        virtual void setAllModelVisible(bool visibility, bool isPart = true, bool isFitView = true) override;
        /**
         * @brief    高亮对象
         * @param[i] dataObjId                  对象id
         * @param[i] color                      高亮颜色
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-21
         */
        virtual void hightObject(int dataObjId, QColor color = QColor(255,0,0)) override;
        /**
         * @brief    清除所有高亮接口
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-21
         */
        virtual void clearAllHight() override;
        /**
         * @brief    更新点大小
         * @param[i] dataObjId                对象id 对象id -1表示全部几何对象 -2表示全部网格对象
         * @param[i] pointSize                点大小
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updatePointSize(int dataObjId, int pointSize) override;
        /**
         * @brief    更新线宽
         * @param[i] dataObjId                对象id 对象id -1表示全部几何对象 -2表示全部网格对象
         * @param[i] lineWidth                线宽
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updateLineWidth(int dataObjId, int lineWidth) override;
        /**
         * @brief    更新SPH粒子大小
         * @param[i] dataObjId 
         * @param[i] size 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-01-16
         */
        virtual void updateSHPSize(int dataObjId, double size) override;
        /**
         * @brief    清空全部窗口可视化对象
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-03-18
         */
        void finalize() override;
    };
    Register2FITKOperatorRepo(GraphPreprocess, OperGraphPreprocess);
}  // namespace GUIOper
#endif

