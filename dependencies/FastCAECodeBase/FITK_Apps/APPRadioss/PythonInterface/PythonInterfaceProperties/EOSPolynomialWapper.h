/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file EOSPolynomialWapper.h
 * @brief 线性多项式状态方程装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-26
 */
#ifndef _EOS_POLYNOMIAL_WAPPER_H___
#define _EOS_POLYNOMIAL_WAPPER_H___

#include <QObject>

#include "PyEquationOfStateBase.h"
#include "PythonInterfacePropertiesAPI.h"

namespace Radioss
{
    class FITKEquationOfStatePolynomial;
}

class PythonInterfacePropertiesAPI EOSPolynomial : public PyInterface::PyEquationOfStateBase
{
public:
    explicit EOSPolynomial(const QString& name);
    explicit EOSPolynomial();
    virtual ~EOSPolynomial() = default;

    Radioss::FITKEquationOfStatePolynomial* getEOSPolynomial();
};

class PythonInterfacePropertiesAPI EOSPolynomialWapper : public QObject
{
    Q_OBJECT

public slots:
    EOSPolynomial* new_EOSPolynomial();

    void delete_EOSPolynomial(EOSPolynomial* obj);

    EOSPolynomial* static_EOSPolynomial_GetEOSPolynomial(const QString& name);

    QString getName(EOSPolynomial* obj);

    QString setName(EOSPolynomial* obj, const QString& name);

    QString createEquationOfState(EOSPolynomial* obj, const QString& name = QString());

    QString addMaterial(EOSPolynomial* obj, const QString& materialName);

    QString removeMaterial(EOSPolynomial* obj, const QString& materialName);

    QString setP0(EOSPolynomial* obj, double value);

    QString setC0(EOSPolynomial* obj, double value);

    QString setC1(EOSPolynomial* obj, double value);

    QString setC2(EOSPolynomial* obj, double value);

    QString setC3(EOSPolynomial* obj, double value);

    QString setC4(EOSPolynomial* obj, double value);

    QString setC5(EOSPolynomial* obj, double value);

    QString setE0(EOSPolynomial* obj, double value);

    QString setPsh(EOSPolynomial* obj, double value);
};

#endif