/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObject3D.h
 * @brief  渲染对象基类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-12-18
 *********************************************************************/
#ifndef __GraphVTKObject3D_H__
#define __GraphVTKObject3D_H__

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKCommons.h"
#include "GUIWidget/GraphArea.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphObjectVTK.h"

// Forward declaration
class QVariant;

class vtkActor;
class vtkObject;
class vtkDataSet;
class vtkRenderer;
class vtkScalarBarActor;

class FITKGraphActor;

namespace Comp{
    class FITKGraphRender;
    class FITKGraph3DWindowVTK;
}

namespace Core{
    class FITKAbstractNDataObject;
}

namespace AbaqusData {
    class FITKDataCase;
}

namespace Exchange
{
    /**
     * @brief  渲染对象基类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-12-18
     */
    class GraphDataVTKAdaptorAPI GraphVTKObject3D : public Comp::FITKGraphObjectVTK
    {
        Q_OBJECT;
        FITKGraphObjectRegist(GraphVTKObject3D, Comp::FITKGraphObjectVTK);
        FITKCLASS(Exchange, GraphVTKObject3D);
    public:
        /**
         * @brief    构造函数
         * @param[i] dataCase                 算例对象
         * @param[i] dataObj                  数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        GraphVTKObject3D(Core::FITKAbstractDataObject* dataObj);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        virtual ~GraphVTKObject3D();
        /**
         * @brief    是否禁用透明度
         * @param[i] isOn              是否禁用
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        virtual void setTransparent(bool isOn);
        /**
         * @brief   Set the color.[virtual]
         * @param   color: The color
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        virtual void setColor(QColor color);
        /**
         * @brief    更新子模型颜色
         * @param[i] color                    颜色
         * @param[i] otherPar                 其他参数[缺省]
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-07-21
         */
        virtual void updateSubColor(QColor color, QVariant otherPar = QVariant());
        /**
         * @brief    更新模型颜色
         * @param[i] color                    颜色
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-07-21
         */
        virtual void updateModelColor(QColor color);
        /**
         * @brief    设置点大小
         * @param[i] size 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updatePointSize(int size);
        /**
         * @brief    设置线宽
         * @param[i] width 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updateLineWidth(int width);
        /**
         * @brief    设置SPH粒子大小
         * @param[i] size
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updateSHPSize(int size);
        /**
         * @brief    设置透明度
         * @param[i] element                  元素类型 0-点 1-线 2-面
         * @param[i] opacity                  透明度
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updateOpacity(int element, double opacity);
        /**
         * @brief    设置渲染模式
         * @param[i] arg 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updateRepresentation(int arg);
        /**
         * @brief    设置拾取模式
         * @param[i] mode                        拾取模式
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual void setPickMode(GraphVTKCommons::ShapePickMode mode);
        /**
         * @brief    根据给定的形状类型与VTK单元id获取形状ID
         * @param[i] vtkCellId                      VTK单元ID
         * @param[i] pickType                       拾取类型
         * @return   int                            形状ID
         * @return   otherInfo                      缺省参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual int getShapeIdByVTKCellId(int vtkCellId, GraphVTKCommons::ShapePickMode pickType, QVariant otherInfo = QVariant());
        /**
         * @brief    更具给定的形状类型与形状id，获取VTK单元id
         * @param[i] shapeId                        形状id
         * @param[i] pickType                       拾取类型
         * @return   const QVector<int>             单元id
         * @return   otherInfo                      缺省参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual const QVector<int> getVTKCellIdsByShapeId(int shapeId, GraphVTKCommons::ShapePickMode pickType, QVariant otherInfo = QVariant());
        /**
         * @brief    通过几何形状id获取虚拓扑索引
         * @param[i] shapeId                                 几何形状id（几何虚拓扑id）
         * @param[i] topAbsShapeType                         几何形状类型
         * @return   int                                     虚拓扑索引
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual int getVirTopoIndexByShapeId(int shapeId, GraphVTKCommons::ShapePickMode pickType);
        /**
         * @brief    根据形状网格类型获取网格数据
         * @param[i] type                           形状网格类型
         * @return   vtkDataSet*                    拾取类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual vtkDataSet* getMesh(GraphVTKCommons::ShapePickMode pickType);
        /**
         * @brief    获取可见性
         * @return   bool               是否可见
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-03
         */
        virtual bool getVisibility();
        /**
         * @brief    更新可见性
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-03
         */
        virtual void updateVisibility();
        /**
         * @brief    获取是否高亮
         * @return   bool            是否高亮
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        bool isHighlighting();
        /**
         * @brief    获取渲染图层
         * @return   int            渲染图层
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        int getRenderLayer();
        /**
         * @brief  设置渲染层级
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-21
         */
        void setRenderLayer(int layer);
        /**
         * @brief    获取渲染层
         * @return   vtkRenderer*                   渲染层
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-01
         */
        vtkRenderer* getRenderer();
        /**
         * @brief    设置渲染层
         * @param[i] render                         渲染层
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-01
         */
        void setRenderer(vtkRenderer* render);
        /**
         * @brief    获取高亮图层
         * @return   int            渲染图层
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-01-21
         */
        virtual int getHightlightRenderLayer();
        /**
         * @brief    预选高亮
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual void preHighlight();
        /**
         * @brief    高亮
         * @param[i] color           颜色
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual void highlight(QColor color = QColor());
        /**
         * @brief    取消高亮
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual void disHighlight();
        /**
         * @brief    设置渲染对象隐藏还是显示
         * @param[i] visibility            隐藏还是显示
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual void setVisible(bool visibility);
        /**
         * @brief    设置单元演员隐藏还是显示
         * @param[i] visibility 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-24
         */
        virtual void setCellVisible(bool visibility);
        /**
         * @brief    设置点演员隐藏还是显示
         * @param[i] visibility 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void setPointVisibility(bool visibility);
        /**
         * @brief    设置线演员隐藏还是显示
         * @param[i] visibility
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        virtual void setLineVisibility(bool visibility);
        /**
         * @brief    更新
         * @param[i] forceUpdate       是否强制刷新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual void update(bool forceUpdate = false) override;
        /**
         * @brief    获取数据id
         * @return   int             数据id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        int getDataId();
        /**
         * @brief    获取形状信息
         * @return   GraphVTKCommons::ShapeInfo      形状信息
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        GraphVTKCommons::ShapeInfo getShapeInfo();
        /**
         * @brief    添加附加渲染对象
         * @param[i] obj 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-18
         */
        void addAddinGraphObject(GraphVTKObject3D* obj);
        /**
         * @brief    获取附加渲染对象
         * @return   QList<GraphVTKObject3D*>
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-18
         */
        virtual QList<GraphVTKObject3D*> getAddinGraphObjs();
        /**
         * @brief    从显示窗口中移除
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-18
         */
        virtual void removeFromGraphWidget() override;
        /**
         * @brief   Get the user data by the given port( role key ).[template]
         * @param   role: The data role key
         * @return  The user data
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        template<class T>
        T getUserDataAs(int role)
        {
            if (!m_userInfo.contains(role))
            {
                return QVariant().value<T>();
            }

            return m_userInfo[role].value<T>();
        }
        /**
         * @brief    检查是否包含当前演员
         * @param[i] actor            演员
         * @return   bool             是否包含
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual bool contains(vtkProp* actor);
        /**
         * @brief    设置算例对象ID
         * @param[i] dataCaseId                     算例对象id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        void setDataCase(int dataCaseId);
        /**
         * @brief    设置分析步id
         * @param[i] stepID                         分析步id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        void setStepId(int stepID);

        /******************************   Post   *********************************/
        /**
         * @brief    获取标量条演员
         * @return   vtkScalarBarActor *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-09
         */
        virtual vtkScalarBarActor* getScalarBarActor();
        /**
         * @brief    切换场量数据
         * @param[i] type                       场量类型（1：点 2：单元）
         * @param[i] fieldName                  场量名称
         * @param[i] compIndex                  分量（1：X 2：Y 3：Z）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-02-14
         */
        virtual void changeField(int type, QString fieldName, int compIndex = 0);
        /**
         * @brief    更新文本信息
         * @param[i] info 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-18
         */
        virtual  void updateTextInfo(const QString &info = QString());
        /**
         * @brief    设置范围颜色
         * @param[i] isOpenOutRanger                 是否显示范围之外的颜色
         * @param[i] minRangeColor                   范围最小颜色
         * @param[i] maxRangeColor                   范围最大颜色
         * @param[i] transparency                    透明度（0-1）
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-09
         */
        virtual void setRangeColor(bool isOpenOutRanger, QColor minRangeColor, QColor maxRangeColor, double transparency);
        /**
         * @brief    获取端点颜色
         * @param[i] isOutRanger       是否获取显示范围之外的颜色
         * @param[o] minRangeColor    maxRangeColor      
         **/
        virtual void getRangeColor(bool isOutRanger, QColor& minRangeColor, QColor& maxRangeColor);
        /**
        * @brief    设置端点范围
        * @param[i] range[2]                 [0]最小值，[1]最大值
        **/
        virtual void setRange(double* range);
        /**
         * @brief    获取端点范围
         **/
        virtual double * getRange();
        /**
         * @brief    设置范围之外的颜色
         * @param[i] isOpenOutRanger                  是否开启范围
         * @param[i] aboveRange                       超出范围颜色[4]
         * @param[i] belowRange                       低于范围颜色[4]
         * @param[i] colorNum                         颜色数量（不包含范围之外的数量）
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-09
         */
        virtual void setOutRangerColor(bool isOpenOutRanger, double* aboveRGBA, double* belowRGBA, int colorNum);
        /**
         * @brief    设置全局颜色设置标志
         * @param[i] isSet
         * @return   void
         */
        virtual void setRangerColorSetFlag(bool isSet);
        virtual bool getRangerColorSetFlag();
        /************************************************************************/
    signals:
        /**
         * @brief   Signal - Need to be deleted while the data object has been deleted.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        void sig_needToBeDeleted();

    protected:
        /**
         * @brief   Constructor.( For copying data )
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        GraphVTKObject3D();
        /**
         * @brief    初始化演员属性
         * @param[i] actor              演员
         * @param[i] details            参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual void initActorProperties(vtkProp* actor, QVariant details = QVariant());
        /**
         * @brief    获取对象的可见性
         * @return   bool             是否可见
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual bool getDataVisibility();
        /**
         * @brief    设置表面灯光属性
         * @param[i] fActor                         演员对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-27
         */
        void setLightPropertiesForSurface(vtkActor* fActor);
        /**
         * @brief    删除VTK对象
         * @param[i] obj             VTK对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        template <class T>
        void deleteVtkObj(T* & obj){
            if (obj){
                obj->Delete();
            }
            obj = nullptr;
        }
        /**
         * @brief    重置VTK数据对象
         * @param[i] obj        VTK数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        template <class T>
        void resetVtkObj(T* obj){
            if (obj){
                obj->Reset();
            }
        }


        template <class T>
        void deleteObjs(QList<T*>& objs)
        {
            for (T* obj : objs)
            {
                if (obj)
                {
                    delete obj;
                }
            }

            objs.clear();
        }
    protected:
        /**
         * @brief  是否处于高亮状态
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-01-21
         */
        bool _isHighlighting = false;
        /**
         * @brief  形状信息结构体
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-12-26
         */
        GraphVTKCommons::ShapeInfo _shapeInfo;
        /**
         * @brief  拾取模式
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-12-26
         */
        GraphVTKCommons::ShapePickMode _pickMode = GraphVTKCommons::PickNone;
        /**
         * @brief  算例对象id
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-11
         */
        int _dataCaseId = -1;
        /**
         * @brief  分析步ID
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-11
         */
        int _stepID = -1;
        /**
         * @brief  渲染层级
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-01
         */
        int _renderLayer = 0;
        /**
         * @brief  渲染层
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-01
         */
        vtkRenderer* _render = nullptr;
        /**
         * @brief  附加渲染对象列表
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-25
         */
        QList<GraphVTKObject3D*> m_addinGraphObjList;
    };
}

#endif