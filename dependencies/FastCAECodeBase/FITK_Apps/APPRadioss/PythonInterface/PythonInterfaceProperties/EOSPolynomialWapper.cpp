/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "EOSPolynomialWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractEquationOfState.h"
#include "FITK_Component/FITKRadiossData/FITKEquationOfStatePolynomial.h"

namespace
{
    QString sendMessageToServer(const QString& message, bool isToServer = true)
    {
        if (isToServer)
        {
            HttpPython::FITKHttpCommons::sendMessageToServer(message);
        }
        return message;
    }
}

EOSPolynomial::EOSPolynomial(const QString& name) :
    PyInterface::PyEquationOfStateBase(name)
{
}

EOSPolynomial::EOSPolynomial() :
    PyInterface::PyEquationOfStateBase()
{
}

Radioss::FITKEquationOfStatePolynomial* EOSPolynomial::getEOSPolynomial()
{
    return dynamic_cast<Radioss::FITKEquationOfStatePolynomial*>(
        PyInterface::PyEquationOfStateBase::getEquationOfState());
}

EOSPolynomial* EOSPolynomialWapper::new_EOSPolynomial()
{
    return new EOSPolynomial();
}

void EOSPolynomialWapper::delete_EOSPolynomial(EOSPolynomial* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

EOSPolynomial* EOSPolynomialWapper::static_EOSPolynomial_GetEOSPolynomial(const QString& name)
{
    return new EOSPolynomial(name);
}

QString EOSPolynomialWapper::getName(EOSPolynomial* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString EOSPolynomialWapper::setName(EOSPolynomial* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, EOS polynomial with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString EOSPolynomialWapper::createEquationOfState(EOSPolynomial* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createEquationOfState(name, static_cast<int>(Radioss::FITKAbstractEquationOfState::Polynomial)))
    {
        return sendMessageToServer(tr("Creation failed! EOS polynomial with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, EOS polynomial created with name %1").arg(obj->getName()));
}

QString EOSPolynomialWapper::addMaterial(EOSPolynomial* obj, const QString& materialName)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->addMaterial(materialName))
    {
        return sendMessageToServer(tr("Failed !Material with name %1 not found or EOS is invalid!").arg(materialName));
    }

    return sendMessageToServer(tr("Success, Add material %1 to EOS polynomial").arg(materialName));
}

QString EOSPolynomialWapper::removeMaterial(EOSPolynomial* obj, const QString& materialName)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->removeMaterial(materialName))
    {
        return sendMessageToServer(tr("Failed !Material with name %1 not found or EOS is invalid!").arg(materialName));
    }

    return sendMessageToServer(tr("Success, Remove material %1 from EOS polynomial").arg(materialName));
}

QString EOSPolynomialWapper::setP0(EOSPolynomial* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setP0(value))
    {
        return sendMessageToServer(tr("Failed !No EOS polynomial found!"));
    }

    return sendMessageToServer(tr("Success, Set EOS polynomial P0 to %1").arg(value));
}

QString EOSPolynomialWapper::setC0(EOSPolynomial* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKEquationOfStatePolynomial* eos = obj->getEOSPolynomial();
    if (eos == nullptr)
    {
        return sendMessageToServer(tr("Failed !No EOS polynomial found!"));
    }

    eos->setC0(value);

    return sendMessageToServer(tr("Success, Set EOS polynomial C0 to %1").arg(value));
}

QString EOSPolynomialWapper::setC1(EOSPolynomial* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKEquationOfStatePolynomial* eos = obj->getEOSPolynomial();
    if (eos == nullptr)
    {
        return sendMessageToServer(tr("Failed !No EOS polynomial found!"));
    }

    eos->setC1(value);

    return sendMessageToServer(tr("Success, Set EOS polynomial C1 to %1").arg(value));
}

QString EOSPolynomialWapper::setC2(EOSPolynomial* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKEquationOfStatePolynomial* eos = obj->getEOSPolynomial();
    if (eos == nullptr)
    {
        return sendMessageToServer(tr("Failed !No EOS polynomial found!"));
    }

    eos->setC2(value);

    return sendMessageToServer(tr("Success, Set EOS polynomial C2 to %1").arg(value));
}

QString EOSPolynomialWapper::setC3(EOSPolynomial* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKEquationOfStatePolynomial* eos = obj->getEOSPolynomial();
    if (eos == nullptr)
    {
        return sendMessageToServer(tr("Failed !No EOS polynomial found!"));
    }

    eos->setC3(value);

    return sendMessageToServer(tr("Success, Set EOS polynomial C3 to %1").arg(value));
}

QString EOSPolynomialWapper::setC4(EOSPolynomial* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKEquationOfStatePolynomial* eos = obj->getEOSPolynomial();
    if (eos == nullptr)
    {
        return sendMessageToServer(tr("Failed !No EOS polynomial found!"));
    }

    eos->setC4(value);

    return sendMessageToServer(tr("Success, Set EOS polynomial C4 to %1").arg(value));
}

QString EOSPolynomialWapper::setC5(EOSPolynomial* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKEquationOfStatePolynomial* eos = obj->getEOSPolynomial();
    if (eos == nullptr)
    {
        return sendMessageToServer(tr("Failed !No EOS polynomial found!"));
    }

    eos->setC5(value);

    return sendMessageToServer(tr("Success, Set EOS polynomial C5 to %1").arg(value));
}

QString EOSPolynomialWapper::setE0(EOSPolynomial* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKEquationOfStatePolynomial* eos = obj->getEOSPolynomial();
    if (eos == nullptr)
    {
        return sendMessageToServer(tr("Failed !No EOS polynomial found!"));
    }

    eos->setE0(value);

    return sendMessageToServer(tr("Success, Set EOS polynomial E0 to %1").arg(value));
}

QString EOSPolynomialWapper::setPsh(EOSPolynomial* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKEquationOfStatePolynomial* eos = obj->getEOSPolynomial();
    if (eos == nullptr)
    {
        return sendMessageToServer(tr("Failed !No EOS polynomial found!"));
    }

    eos->setPsh(value);

    return sendMessageToServer(tr("Success, Set EOS polynomial Psh to %1").arg(value));
}