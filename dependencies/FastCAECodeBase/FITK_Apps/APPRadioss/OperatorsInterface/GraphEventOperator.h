/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphEventOperator.h
 * @brief  三维可视对象数据操作器接口类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-12-18
 *********************************************************************/
#ifndef __GRAPHEVENTOPERATOR_H__
#define __GRAPHEVENTOPERATOR_H__

#include "OperatorsInterfaceAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractOperator.h"

#include <QColor>

namespace Exchange {
    class GraphVTKObject3D;
}
namespace Comp
{
    class FITKGraph3DWindowVTK;
}
namespace EventOper
{
    /**
     * @brief  三维可视对象数据操作器接口类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-12-18
     */
    class OperatorsInterfaceAPI GraphEventOperator : public Core::FITKAbstractOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        explicit GraphEventOperator() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        virtual ~GraphEventOperator() = default;
        /**
         * @brief    更新全部可视化对象。（没有则创建）[虚函数]
         * @param[i] forceUpdate                针对已存在模型是否强制刷新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-02-08
         */
        virtual void updateAll(bool forceUpdate = false);
        /**
         * @brief    根据提供的数据对象ID更新对应可视化对象。（没有则创建）[虚函数]
         * @param    dataObjId：数据对象ID
         * @param    forceUpdate：是否强制刷新[缺省]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        virtual void updateGraph(int dataObjId, bool forceUpdate = false, bool isFitView = false);
        /**
         * @brief    更新全部模型可视化对象。（没有则创建）[虚函数]
         * @param[i] forceUpdate                针对已存在模型是否强制刷新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-02-08
         */
        virtual void updateAllModel(bool forceUpdate);
        /**
         * @brief    更新全部实例可视化对象。（没有则创建）[虚函数]
         * @param[i] forceUpate                 针对已存在模型是否强制刷新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-02-08
         */
        virtual void updateAllInstance(bool forceUpdate);
        /**
         * @brief    根据数据对象ID获取模型可视化对象。（没有不会创建）[虚函数]
         * @param[i] dataObjId                            数据id
         * @return   Exchange::GraphVTKObject3D*          渲染对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual Exchange::GraphVTKObject3D* getModelGraphObjectByDataId(int dataObjId);
        /**
         * @brief    根据提供的数据对象ID递归更新对应可视化对象。（没有则不创建）
         * @param[i] dataObjId                                 数据对象ID
         * @param[i] refLayer                                  更新引用数据的层级（-1则递归至根数据）[缺省]
         * @param[i] recDown                                   向下递归（被引用方向查询）[缺省]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-02
         */
        virtual void updateGraphRecursively(int dataObjId, int refLayer = 2, bool recDown = true);
        /**
         * @brief    隐藏或显示模型。[虚函数]
         * @param    dataObjId：               数据对象ID
         * @param    visibility：              是否可见
         * @param    isFitView：               是否自适应
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        virtual void setModelVisible(int dataObjId, bool visibility,bool isFitView = true);
        /**
         * @brief    隐藏或显示点演员。[虚函数]
         * @param    visibility：              是否可见
         * @param    isPart：                  是否是Part
         * @param    isFitView：               是否自适应
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void setPointVisible(bool visibility, bool isPart = true, bool isFitView = true);
        /**
         * @brief    隐藏或显示线演员。[虚函数]
         * @param    visibility：              是否可见
         * @param    isPart：                  是否是Part
         * @param    isFitView：               是否自适应
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        virtual void setLineVisible(bool visibility, bool isPart = true, bool isFitView = true);
        /**
         * @brief    设置全部部件显示隐藏状态
         * @param[i] visibility                 是否可见
         * @param    isFitView                  是否自适应
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        virtual void setAllModelVisible(bool visibility,bool isPart = true, bool isFitView = true);
        /**
         * @brief    设置全部实例隐藏状态
         * @param[i] visibility                 是否可见
         * @param    isFitView                  是否自适应
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-01-13
         */
        virtual void setAllInstanceVisible(bool visibility, bool isFitView = true);
        /**
         * @brief    高亮对象
         * @param[i] dataObjId                  对象id
         * @param[i] color                      高亮颜色
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-01-06
         */
        virtual void hightObject(int dataObjId, QColor color = QColor());
        /**
         * @brief    高亮集合对象
         * @param[i] dataObjId                  对象ID
         * @param[i] parentID                   partID、InstanceID、AssemblyID
         * @param[i] color                      高亮颜色
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-02-05
         */
        virtual void hightSetObject(int dataObjId, int parentID, QColor color = QColor());
        /**
         * @brief    取消高亮数据
         * @param[i] dataObjId                  对象id
         * @param[i] parentID                   父对象ID（默认缺省）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-01-06
         */
        virtual void disHightObject(int dataObjId, int parentID = -1);
        /**
         * @brief    清除所有高亮接口
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-01-14
         */
        virtual void clearAllHight();
        /**
         * @brief    刷新[虚函数]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        virtual void render();
        /**
         * @brief    自适应[虚函数]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        virtual void fitView();
        /**
         * @brief    更新部分颜色
         * @param[i] dataObjId                对象id
         * @param[i] color                    颜色
         * @param[i] otherPar                 其他参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-28
         */
        virtual void updateColor(int dataObjId, QColor color, QVariant otherPar = QVariant());
        /**
         * @brief    更新全部颜色
         * @param[i] dataObjId                对象id
         * @param[i] color                    颜色
         * @param[i] forceUpdate              是否强制刷新（默认否）
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-28
         */
        virtual void updateAllColor(int dataObjId, QColor color);
        /**
         * @brief    更新点大小
         * @param[i] dataObjId                对象id -1:全部几何对象 -2:全部网格对象
         * @param[i] pointSize                点大小
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updatePointSize(int dataObjId, int pointSize);
        /**
         * @brief    更新线宽
         * @param[i] dataObjId                对象id -1:全部几何对象 -2:全部网格对象
         * @param[i] lineWidth                线宽
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updateLineWidth(int dataObjId, int lineWidth);
        /**
         * @brief    更新SPH粒子大小
         * @param[i] dataObjId                对象id -1:全部几何对象 -2:全部网格对象
         * @param[i] size                     粒子大小
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updateSHPSize(int dataObjId, double size);
        /**
         * @brief    更新透明度
         * @param[i] dataObjId                对象id  -1:全部几何对象 -2:全部网格对象
         * @param[i] element                  元素类型 (0:点 1:线 2:面)
         * @param[i] opacity                  透明度
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updateOpacity(int dataObjId,int element, double opacity);
        /**
         * @brief    更新渲染模式
         * @param[i] dataObjId                对象id  -1:全部几何对象 -2:全部网格对象
         * @param[i] representation           显示表示 (0:点 1:线框 2:面)
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updateRepresentation(int dataObjId, int representation);

        /** post */

        /**
         * @brief    切换场量数据
         * @param[i] dataId                     数据id
         * @param[i] type                       场量类型（1：点 2：单元）
         * @param[i] fieldName                  场量名称
         * @param[i] compIndex                  分量（1：X 2：Y 3：Z）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-02-14
         */
        virtual void changeField(int dataId, int type, QString fieldName, int compIndex = 0);
        /**
         * @brief    更新标量色带的可见性，只显示指定ID对象的色带
         * @param[i] selectedDataId       被选中的数据对象ID
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-05-19
         */
        virtual void updateScalarBarVisibility(int selectedDataId);
        /**
         * @brief    更新文本信息
         * @param[i] dataId
         * @param[i] info
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-18
         */
        virtual void updateTextInfo(int dataId, QString info);
        /**
         * @brief    设置范围颜色
         * @param[i] isOpenOutRanger                 是否显示范围之外的颜色
         * @param[i] minRangeColor                   范围最小颜色
         * @param[i] maxRangeColor                   范围最大颜色
         * @param[i] transparency                    透明度（0-1）
         * @param[i] postId                          后处理对象id  -1:全局
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-09
         */
        virtual void setRangeColor(bool isOpenOutRanger, QColor minRangeColor, QColor maxRangeColor, double transparency, int postId = -1);
        /**
         * @brief    获取端点颜色
         * @param[i] isOutRanger       是否获取显示范围之外的颜色
         * @param[o] minRangeColor    maxRangeColor
         **/
        virtual void getRangeColor(bool isOutRanger, int postId, QColor& minRangeColor, QColor& maxRangeColor);
        /**
        * @brief    设置端点范围
        * @param[i] postId                          后处理对象id
        * @param[i] range[2]                 [0]最小值，[1]最大值
        **/
        virtual void setRange(int postId, double* range);
        /**
         * @brief    获取端点范围
         * @param[i] postId                          后处理对象id
         **/
        virtual double* getRange(int postId);
        /**
         * @brief    设置范围之外的颜色
         * @param[i] isOpenOutRanger                  是否开启范围
         * @param[i] aboveRange                       超出范围颜色[4]
         * @param[i] belowRange                       低于范围颜色[4]
         * @param[i] colorNum                         颜色数量（不包含范围之外的数量）
         * @param[i] postId                           后处理对象id -1:全局
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-09
         */
        virtual void setOutRangerColor(bool isOpenOutRanger, double* aboveRGBA, double* belowRGBA, int colorNum, int postId = -1);
        /**
        * @brief    设置全局颜色设置标志
        * @param[i] isSet
        * @return   void
        */
        virtual void setRangerColorSetFlag(bool isSet);
        virtual bool getRangerColorSetFlag(int postId);
        /**
         * @brief    设置颜色数量
         * @param[i] isOpenOutRanger                  是否开启范围
         * @param[i] colorNum                         颜色数量（不包含范围之外的数量）
         * @param[i] postId                           后处理对象id  -1:全局
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-07
         */
        virtual void setRangerColorNum(bool isOpenOutRanger, int colorNum, int postId = -1);
        /**
         * @brief    获取范围颜色数量
         * @param[i] postId                           后处理对象id -1:全局
         * @author   ZhouZhaoYi (
         * @date     2025-11-07
         */
        virtual int getRangerColorNum(int postId = -1);
        /**
         * @brief    清空全部窗口中的可视化对象（虚函数）
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-03-18
         */
        virtual void finalize();
    };
}   // namespace EventOper

#endif // __GRAPHEVENTOPERATOR_H__