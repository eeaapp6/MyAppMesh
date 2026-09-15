/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObject3DPostBase.h
 * @brief  后处理渲染基类
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-09-10
 *********************************************************************/
#ifndef _GRAPHVTKOBJECT3DPOSTBASE_H__
#define _GRAPHVTKOBJECT3DPOSTBASE_H__

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKObject3D.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKAbstractStruPostVTK.h"
class vtkLookupTable;
class vtkActor;
class vtkScalarBarActor;
class vtkTextActor;
class FITKGraphActor;

namespace Exchange
{
    /**
     * @brief  后处理渲染基类
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-09-10
     */
    class GraphDataVTKAdaptorAPI GraphVTKObject3DPostBase : public GraphVTKObject3D
    {
        Q_OBJECT;
        FITKGraphObjectRegist(GraphVTKObject3DPostBase, GraphVTKObject3D);
        FITKCLASS(Exchange, GraphVTKObject3DPostBase);
    public:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        GraphVTKObject3DPostBase(Core::FITKAbstractDataObject* dataObj);
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        virtual ~GraphVTKObject3DPostBase();
        /**
         * @brief  初始化
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        virtual void init();
        /**
         * @brief    初始化色带演员
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-18
         */
        virtual void initBarActor();
        /**
         * @brief    初始化文本演员
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-18
         */
        virtual void initTextActor();
        /**
         * @brief    通过演员获取对应的查找表
         * @param[i] actor 
         * @return   vtkLookupTable *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-10
         */
        vtkLookupTable* getLookupTableByActor(vtkActor* actor);
        /**
         * @brief    切换场量数据
         * @param[i] dataId                     数据id
         * @param[i] type                       场量类型（1：点 2：单元）
         * @param[i] fieldName                  场量名称
         * @param[i] compIndex                  分量（1：X 2：Y 3：Z）
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        void changeField(int type, QString fieldName, int compIndex = 0) override;
        /**
         * @brief    获取场量范围
         * @param[i] type 
         * @param[i]  fieldName
         * @param[i] compIndex 
         * @param[i] range 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        void getFieldRange(int type, QString fieldName, int compIndex, double* range);
        /**
         * @brief    更新文本信息
         * @param[i] info 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-18
         */
        void updateTextInfo(const QString &info = QString()) override;
        /**
         * @brief    设置选中状态
         * @param[i] isSelected 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-10
         */
        virtual void setSelected(bool isSelected);
        /**
         * @brief    获取选中状态
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-10
         */
        bool isSelected() const { return _isSelected; }
        /**
         * @brief    更新
         * @param[i] forceUpdate 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-10
         */
        virtual void update(bool forceUpdate = false) override;
        /**
         * @brief    更新可见性
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-10
         */
        virtual void updateVisibility() override;
        /**
         * @brief    更新色带
         * @param[i] range 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-11
         */
        void updateScalarBarRange(double* range);
        /**
         * @brief    更新色带显示状态
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-11
         */
        void updateScalarBarVisible();
        /**
         * @brief    设置范围颜色
         * @param[i] isOpenOutRanger                 是否显示范围之外的颜色
         * @param[i] minRangeColor                   范围最小颜色
         * @param[i] maxRangeColor                   范围最大颜色
         * @param[i] transparency                    透明度（0-1）
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-09
         */
        virtual void setRangeColor(bool isOpenOutRanger, QColor minRangeColor, QColor maxRangeColor, double transparency) override;
        /**
        * @brief    获取端点颜色
        * @param[i] isOutRanger       是否获取显示范围之外的颜色
        * @param[o] minRangeColor    maxRangeColor
        **/
        virtual void getRangeColor(bool isOutRanger, QColor& minRangeColor, QColor& maxRangeColor) override;
        /**
         * @brief    设置端点范围
         * @param[i] range[2]                 [0]最小值，[1]最大值
         **/
        virtual void setRange(double* range)override;
        /**
         * @brief    获取端点范围
         * @param[o] range[2]                 [0]最小值，[1]最大值
         **/
        virtual double * getRange()override;
        /**
         * @brief    设置范围之外的颜色
         * @param[i] isOpenOutRanger                  是否开启范围
         * @param[i] aboveRange                       超出范围颜色[4]
         * @param[i] belowRange                       低于范围颜色[4]
         * @param[i] colorNum                         颜色数量（不包含范围之外的数量）
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-09
         */
        virtual void setOutRangerColor(bool isOpenOutRanger, double* aboveRGBA, double* belowRGBA, int colorNum) override;
        /**
         * @brief    设置全局颜色设置标志
         * @param[i] isSet 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-21
         */
        virtual void setRangerColorSetFlag(bool isSet)override;
        virtual bool getRangerColorSetFlag()override { return _isRangeColorSet; }

    protected:
        /**
         * @brief  标量色带对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        vtkScalarBarActor* _scalarBarActor = nullptr;
        /**
         * @brief  后处理数据渲染对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        FITKGraphActor* _postActor = nullptr;
        /**
         * @brief  文本信息演员
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-10-21
         */
        vtkTextActor* _infoTextActor = nullptr;
        /**
         * @brief  色带是否可见
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        bool _isSelected = false;
        /**
         * @brief  是否设置了范围颜色
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-10-21
         */
        bool _isRangeColorSet = false;
        /**
         * @brief  文本信息
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-10-21
         */
        QString _currentText;
    };
}
#endif // !_GRAPHVTKOBJECT3DPOSTBASE_H__
