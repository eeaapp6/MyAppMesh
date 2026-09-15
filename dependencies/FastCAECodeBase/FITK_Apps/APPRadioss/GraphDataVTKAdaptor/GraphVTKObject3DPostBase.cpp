/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObject3DPostBase.h"

#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

#include "FITK_Interface/FITKInterfaceStructuralPost/FITKAbstractStruPostVTK.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStruPostMessageText.h"

#include <vtkSmartPointer.h>
#include <vtkLookupTable.h>
#include <vtkMapper.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkScalarBarActor.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkTextProperty.h>
#include <vtkDataSetAttributes.h>
#include <vtkTextActor.h>

namespace Exchange
{
    GraphVTKObject3DPostBase::GraphVTKObject3DPostBase(Core::FITKAbstractDataObject* dataObj):
        GraphVTKObject3D(dataObj)
    {
    }
    GraphVTKObject3DPostBase::~GraphVTKObject3DPostBase()
    {
    }
    void GraphVTKObject3DPostBase::init()
    {
       //创建颜色映射器
        vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
        lut->SetHueRange(0.67, 0);
        lut->SetNumberOfColors(10);
        lut->Build();
        //渲染对象初始化
        _postActor = FITKGraphActor::New();
        _postActor->SetVisibility(true);
        _postActor->SetPickable(true);
        _postActor->setGraphObject(this);
        _postActor->GetProperty()->SetPointSize(5);

        vtkMapper* mapper = _postActor->GetMapper();
        if (mapper)
        {
            mapper->SetScalarVisibility(false);
            mapper->SetLookupTable(lut);
            mapper->SetInterpolateScalarsBeforeMapping(true);
        }
        addActor(_postActor);

        initBarActor();
        initTextActor();
    }
    void GraphVTKObject3DPostBase::initBarActor()
    {
        vtkMapper* mapper = _postActor->GetMapper();
        if (mapper == nullptr) {
            return;
        }
        vtkLookupTable* lut = vtkLookupTable::SafeDownCast(mapper->GetLookupTable());

        //色标对象初始化
        _scalarBarActor = vtkScalarBarActor::New();
        _scalarBarActor->SetLookupTable(lut);
        _scalarBarActor->SetPickable(false);

        //将色带默认设置为不可见,只有选中时才显示
        _scalarBarActor->SetVisibility(false);
        //设置标题
        _scalarBarActor->SetTitle("NONE");
        //设置标题大小
        _scalarBarActor->GetFrameProperty()->SetLineWidth(2);
        //设置标题不透明度
        _scalarBarActor->GetFrameProperty()->SetOpacity(0.7);
        //设置标题颜色
        _scalarBarActor->GetFrameProperty()->SetColor(1, 1, 1);
        //设置标题属性是否粗体
        _scalarBarActor->GetTitleTextProperty()->SetBold(false);
        //设置标题属性是否斜体
        _scalarBarActor->GetTitleTextProperty()->SetItalic(false);
        //设置标题属性字体大小
        _scalarBarActor->GetTitleTextProperty()->SetFontSize(15);
        //设置标题属性字体大小
        _scalarBarActor->GetTitleTextProperty()->SetColor(1., 1., 1.);
        //设置标题属性是否使用紧密约束框
        _scalarBarActor->GetTitleTextProperty()->SetUseTightBoundingBox(true);
        //设置标题属性左对齐
        _scalarBarActor->GetTitleTextProperty()->SetJustificationToLeft();
        //设置标题属性垂直对齐到顶部
        _scalarBarActor->GetTitleTextProperty()->SetVerticalJustificationToTop();
        //设置标签属性是否粗体
        _scalarBarActor->GetLabelTextProperty()->SetBold(false);
        //设置标签属性是否斜体
        _scalarBarActor->GetLabelTextProperty()->SetItalic(false);
        //设置标签属性字体大小
        _scalarBarActor->GetLabelTextProperty()->SetFontSize(14);
        //设置标签属性字体颜色
        _scalarBarActor->GetLabelTextProperty()->SetColor(1., 1., 1.);
        //设置注释属性是否粗体
        _scalarBarActor->GetAnnotationTextProperty()->SetBold(false);
        //设置注释属性是否粗体
        _scalarBarActor->GetAnnotationTextProperty()->SetItalic(false);
        //设置注释属性字体大小
        _scalarBarActor->GetAnnotationTextProperty()->SetFontSize(14);
        //设置注释属性字体颜色
        _scalarBarActor->GetAnnotationTextProperty()->SetColor(1., 1., 1.);
        //设置标题标签字体大小受约束
        _scalarBarActor->SetUnconstrainedFontSize(true);
        //设置标题与工具条之间的距离
        _scalarBarActor->SetVerticalTitleSeparation(8);
        //设置标题高度与刻度标签高度的比率
        _scalarBarActor->SetTitleRatio(1);
        //设置获取相对于小部件框架的颜色条的厚度。
        _scalarBarActor->SetBarRatio(0.2);
        ////设置位置。
        //_scalarBarActor->SetPosition(0.83 - offsetX, 0.0);
        //设置最大高度（像素点）
        _scalarBarActor->SetMaximumHeightInPixels(250);
        //设置最大宽度（像素点）
        _scalarBarActor->SetMaximumWidthInPixels(100);
        // Update the scalar bar's properties.
        _scalarBarActor->Modified();
        //添加演员对象
        addActor(_scalarBarActor);
    }
    void GraphVTKObject3DPostBase::initTextActor()
    {
        _infoTextActor = vtkTextActor::New();
        addActor(_infoTextActor);

        Interface::FITKAbstractStructuralPostVTK* postData = dynamic_cast<Interface::FITKAbstractStructuralPostVTK*>(_dataObj);
        if (!postData) return;
        Interface::FITKStruPostMessageText* textProp = postData->getMessageTextData();
        if (!textProp) return;

        updateTextInfo();
    }
    vtkLookupTable* GraphVTKObject3DPostBase::getLookupTableByActor(vtkActor* actor)
    {
        if (actor == nullptr) return nullptr;
        vtkMapper* mapper = actor->GetMapper();
        if (mapper == nullptr) return nullptr;
        vtkLookupTable* lut = vtkLookupTable::SafeDownCast(mapper->GetLookupTable());
        return lut;
    }
    void GraphVTKObject3DPostBase::changeField(int type, QString fieldName, int compIndex)
    {
        if (_isRangeColorSet) return;
        double range[2] = { 0,0 };
        getFieldRange(type, fieldName, compIndex, range);

        QByteArray name = fieldName.toLocal8Bit();
        char* cName = name.data();

        vtkMapper* mapper = _postActor->GetMapper();
        if (mapper == nullptr) {
            return;
        }
        switch (type) {
        case 0: {
            mapper->SetScalarVisibility(false);
        }
        case 1: {
            mapper->SetScalarVisibility(true);
            mapper->SetScalarModeToUsePointFieldData();
            break;
        }
        case 2: {
            mapper->SetScalarVisibility(true);
            mapper->SetScalarModeToUseCellFieldData();
            break;
        }
        }
        
        mapper->ColorByArrayComponent(cName, compIndex);
        mapper->SetScalarRange(range);
        mapper->Update();

        //设置向量映射的模式。
        vtkLookupTable* lookupTable = vtkLookupTable::SafeDownCast(mapper->GetLookupTable());
        if (lookupTable) {
            if (compIndex == -1) {
                //设置向量映射的模式为大小
                lookupTable->SetVectorModeToMagnitude();
            }
            else {
                //设置向量映射的模式为分量
                lookupTable->SetVectorModeToComponent();
            }
            //设置分量索引
            lookupTable->SetVectorComponent(compIndex);
            lookupTable->SetNanColor(0.5, 0.5, 0.5, 1.0);//灰色
            lookupTable->SetRange(range);
            lookupTable->Build();
        }

        _scalarBarActor->SetTitle(cName);
        switch (type) {
        case 0: setSelected(false); break;
        case 1:
        case 2:setSelected(true); break;
        }
    }
    void GraphVTKObject3DPostBase::getFieldRange(int type, QString fieldName, int compIndex, double * range)
    {
        Interface::FITKAbstractStructuralPostVTK* postVTKObj = dynamic_cast<Interface::FITKAbstractStructuralPostVTK*>(_dataObj);
        if (postVTKObj == nullptr) {
            return;
        }

        vtkDataSet* dataSet = postVTKObj->getOutput();
        if (dataSet == nullptr) {
            return;
        }

        //获取单元或点数据
        vtkDataSetAttributes* data = nullptr;
        switch (type) {
        case 0:
        case 1: data = dataSet->GetPointData(); break;
        case 2: data = dataSet->GetCellData(); break;
        }
        if (data == nullptr) {
            return;
        }

        //获取数据组
        vtkDataArray* dataArray = nullptr;
        for (int i = 0; i < data->GetNumberOfArrays(); i++) {
            QString arrName = data->GetArrayName(i);
            if (arrName == fieldName) {
                dataArray = data->GetArray(i);
                break;
            }
        }
        if (dataArray == nullptr) {
            return;
        }
        dataArray->GetRange(range, compIndex);

        //判断数值范围是否接近
        if (fabs(range[0] - range[1]) < 1e-6) {
            if (fabs(range[0]) < 1e-6) {
                range[0] = -1e-5;
                range[1] = 1e-5;
            }
            else {
                range[0] -= range[0] * 0.0001;
                range[1] += range[1] * 0.0001;
            }

        }
    }
    void GraphVTKObject3DPostBase::updateTextInfo(const QString & info)
    {
        if (!_infoTextActor)
            return;
        Interface::FITKAbstractStructuralPostVTK* postData = dynamic_cast<Interface::FITKAbstractStructuralPostVTK*>(_dataObj);
        if (!postData) return;
        Interface::FITKStruPostMessageText* textProp = postData->getMessageTextData();
        if (!textProp) return;
        QColor color = textProp->getColor();
        double colorF[3] = { 1,0,0 };
        bool isVisible = textProp->getIsEnable();

        _currentText.clear();
        QString textName = textProp->getText();
        int curStep = postData->getCurFrame();
        QString stepInfo = QString("Step: %1").arg(curStep);
        _currentText = QString("%1\n%2").arg(textName).arg(stepInfo);
        if (!info.isEmpty())
        {
            _currentText = QString("%1\n%2").arg(_currentText).arg(info);
        }

        int fontSize = textProp->getTextSize();
        bool isEnable = textProp->getIsEnable();
        _infoTextActor->SetInput(_currentText.toUtf8().data());

        vtkCoordinate* coord = _infoTextActor->GetPositionCoordinate();
        coord->SetCoordinateSystemToNormalizedViewport();
        coord->SetValue(0.05, 0.98);
        // 设置文本对齐方式为左上角
        _infoTextActor->GetTextProperty()->SetJustificationToLeft();
        _infoTextActor->GetTextProperty()->SetVerticalJustificationToTop();

        _infoTextActor->GetTextProperty()->SetColor(colorF);
        _infoTextActor->GetTextProperty()->SetFontSize(fontSize);
        _infoTextActor->SetVisibility(isEnable);
        _infoTextActor->Modified();
    }
    void GraphVTKObject3DPostBase::setSelected(bool isSelected)
    {
        _isSelected = isSelected;
        if (_scalarBarActor)
        {
            _scalarBarActor->SetVisibility(isSelected);
            _scalarBarActor->Modified();
        }
    }
    void GraphVTKObject3DPostBase::update(bool forceUpdate)
    {
        GraphVTKObject3D::update(forceUpdate);

        //更新可见性
        updateVisibility();
    }
    void GraphVTKObject3DPostBase::updateVisibility()
    {
        if (_dataObj == nullptr) {
            return;
        }

        bool isEnable = _dataObj->isEnable();
        _postActor->SetVisibility(isEnable);
    }
    void GraphVTKObject3DPostBase::updateScalarBarRange(double* range)
    {
        if (!_scalarBarActor || !range)
        {
            return;
        }

        // Set th range to lookup table.
        vtkLookupTable* lutBar = vtkLookupTable::SafeDownCast(_scalarBarActor->GetLookupTable());
        if (lutBar)
        {
            lutBar->SetTableRange(range);
            lutBar->Build();
        }

        _scalarBarActor->Modified();
    }
    void GraphVTKObject3DPostBase::updateScalarBarVisible()
    {
        if (!_scalarBarActor)
        {
            return;
        }
        // Set the visibility of scalar bar.
        if (_isSelected)
        {
            _scalarBarActor->SetVisibility(true);
        }
        else
        {
            _scalarBarActor->SetVisibility(false);
        }
        _scalarBarActor->Modified();
    }
    void GraphVTKObject3DPostBase::setRangeColor(bool isOpenOutRanger, QColor minRangeColor, QColor maxRangeColor, double transparency)
    {
        if (_postActor == nullptr) {
            return;
        }

        vtkLookupTable* lookupTable = vtkLookupTable::SafeDownCast(_postActor->GetMapper()->GetLookupTable());
        if (lookupTable == nullptr) {
            return;
        }

        int colorNum = lookupTable->GetNumberOfColors();

        double minRGBA[4] = { 0,0,0,0 }, maxRGBA[4] = { 0,0,0,0 };
        minRGBA[0] = minRangeColor.redF();
        minRGBA[1] = minRangeColor.greenF();
        minRGBA[2] = minRangeColor.blueF();
        minRGBA[3] = transparency;

        maxRGBA[0] = maxRangeColor.redF();
        maxRGBA[1] = maxRangeColor.greenF();
        maxRGBA[2] = maxRangeColor.blueF();
        maxRGBA[3] = transparency;

        lookupTable->SetTableValue(0, minRGBA);
        lookupTable->SetTableValue(colorNum - 1, maxRGBA);

        lookupTable->Build();
    }

    void GraphVTKObject3DPostBase::getRangeColor(bool isOutRanger, QColor & minRangeColor, QColor & maxRangeColor)
    {
        if (_postActor == nullptr) {
            return;
        }

        vtkLookupTable* lookupTable = vtkLookupTable::SafeDownCast(_postActor->GetMapper()->GetLookupTable());
        if (lookupTable == nullptr) {
            return;
        }

        int colorNum = lookupTable->GetNumberOfColors();
        double minRGBA[4] = { 0,0,0,0 }, maxRGBA[4] = { 0,0,0,0 };
        if (isOutRanger) {
            lookupTable->GetBelowRangeColor(minRGBA);
            lookupTable->GetAboveRangeColor(maxRGBA);
        }
        else {
            lookupTable->GetTableValue(0, minRGBA);
            lookupTable->GetTableValue(colorNum - 1, maxRGBA);
        }
        minRangeColor = QColor(minRGBA[0] * 255, minRGBA[1] * 255, minRGBA[2] * 255, minRGBA[3] * 255);
        maxRangeColor = QColor(maxRGBA[0] * 255, maxRGBA[1] * 255, maxRGBA[2] * 255, maxRGBA[3] * 255);
    }

    void GraphVTKObject3DPostBase::setRange(double * range)
    {
        if (_postActor == nullptr) {
            return;
        }

        vtkLookupTable* lookupTable = vtkLookupTable::SafeDownCast(_postActor->GetMapper()->GetLookupTable());
        if (lookupTable == nullptr) {
            return;
        }
        vtkMapper* mapper = _postActor->GetMapper();
        if (mapper == nullptr) {
            return;
        }
        mapper->SetScalarRange(range);
        mapper->Update();

        lookupTable->SetRange(range);
        lookupTable->Build();
    }

    double * GraphVTKObject3DPostBase::getRange()
    {
        if (_postActor == nullptr) {
            return nullptr;
        }

        vtkLookupTable* lookupTable = vtkLookupTable::SafeDownCast(_postActor->GetMapper()->GetLookupTable());
        if (lookupTable == nullptr) {
            return nullptr;
        }
        return lookupTable->GetRange();

    }

    void GraphVTKObject3DPostBase::setOutRangerColor(bool isOpenOutRanger, double* aboveRGBA, double* belowRGBA, int colorNum)
    {
        if (_postActor == nullptr) {
            return;
        }

        vtkLookupTable* lookupTable = vtkLookupTable::SafeDownCast(_postActor->GetMapper()->GetLookupTable());
        if (lookupTable == nullptr) {
            return;
        }
        double minRGBA[4] = { 0,0,0,0 }, maxRGBA[4] = { 0,0,0,0 };
        int currentColorNum = lookupTable->GetNumberOfColors();
        lookupTable->GetTableValue(0, minRGBA);
        lookupTable->GetTableValue(currentColorNum - 1, maxRGBA);
        //获取线性差值颜色
        std::vector<RGBA> colors = GraphVTKCommons::calculateColorTransition(colorNum-2, minRGBA, maxRGBA);
        if (colors.size() == 0) {
            return;
        }

        if (colorNum > 0) {
            lookupTable->SetNumberOfTableValues(colorNum);

            // 1. 设置第一个颜色
            lookupTable->SetTableValue(0, minRGBA);

            // 2. 设置中间的过渡颜色
            for (int i = 0; i < colors.size(); ++i) {
                double color[4] = { colors[i].r, colors[i].g, colors[i].b, colors[i].a };
                lookupTable->SetTableValue(i + 1, color);
            }

            // 3. 设置最后一个颜色
            lookupTable->SetTableValue(colorNum - 1, maxRGBA);
        }

        lookupTable->SetUseAboveRangeColor(isOpenOutRanger);
        lookupTable->SetUseBelowRangeColor(isOpenOutRanger);

        lookupTable->SetAboveRangeColor(aboveRGBA);
        lookupTable->SetBelowRangeColor(belowRGBA);

        lookupTable->Build();
    }
    void GraphVTKObject3DPostBase::setRangerColorSetFlag(bool isSet)
    {
        _isRangeColorSet = isSet;
        if (!isSet)
        {
            if (_postActor == nullptr) {
                return;
            }

            vtkLookupTable* lookupTable = vtkLookupTable::SafeDownCast(_postActor->GetMapper()->GetLookupTable());
            if (lookupTable == nullptr) {
                return;
            }

            // 重置 setOutRangerColor 的设置
            lookupTable->SetUseAboveRangeColor(false);
            lookupTable->SetUseBelowRangeColor(false);
            _scalarBarActor->SetNumberOfLabels(5 );
            // 重置 setRangeColor 的设置，恢复为默认的色相范围
            lookupTable->SetNumberOfTableValues(0);
            lookupTable->SetNumberOfColors(10);
            lookupTable->SetHueRange(0.67, 0);
            lookupTable->Build();
            _scalarBarActor->Modified();
        }

    }
}
