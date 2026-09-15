/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAnalyticalExpressField.h"


namespace Interface
{
    FITKAbstractFiled::FITKFieldType FITKFieldAanlyticalExpress::getFieldType()
    {
        //解析场
        return FITKAbstractFiled::FITKFieldType::FFTAnalytical;
    }


    void FITKFieldAanlyticalExpress::setExpression(const QString& express)
    {
        //表达式
        _expression = express;
    }

    QString FITKFieldAanlyticalExpress::getExpression() const
    {
        //表达式
        return _expression;
    }

    bool FITKFieldAanlyticalExpress::copy(FITKAbstractDataObject* obj)
    {
        FITKFieldAanlyticalExpress* fe = dynamic_cast<FITKFieldAanlyticalExpress*>(obj);
        if (fe == nullptr) return false;
        _expression = fe->getExpression();
        return FITKAbstractFiled::copy(obj);
    }

}



