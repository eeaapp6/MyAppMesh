/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPropSolid.h"

namespace Radioss
{

FITKPropSolid::FITKPropSolid()
{
    _value = new FITKPropSolidValue();
}

FITKPropSolid::~FITKPropSolid()
{
    FITKPropSolidValue* value = _value.getObjectPtr();
    if (value) delete value;
}

FITKPropAbstract::RadPropType FITKPropSolid::getRadPropType()
{
    return FITKPropAbstract::RadPropType::Solid;
}

FITKPropSolidValue* FITKPropSolid::getValue()
{
    return _value.getObjectPtr();
}

// FITKPropSolidValue getter/setter implementations
int FITKPropSolidValue::getIsolid() const { return m_Isolid; }
void FITKPropSolidValue::setIsolid(int val) { m_Isolid = val; }

int FITKPropSolidValue::getIsmstr() const { return m_Ismstr; }
void FITKPropSolidValue::setIsmstr(int val) { m_Ismstr = val; }

int FITKPropSolidValue::getIcpre() const { return m_Icpre; }
void FITKPropSolidValue::setIcpre(int val) { m_Icpre = val; }

int FITKPropSolidValue::getItetra4() const { return m_Itetra4; }
void FITKPropSolidValue::setItetra4(int val) { m_Itetra4 = val; }

int FITKPropSolidValue::getItetra10() const { return m_Itetra10; }
void FITKPropSolidValue::setItetra10(int val) { m_Itetra10 = val; }

int FITKPropSolidValue::getImas() const { return m_Imas; }
void FITKPropSolidValue::setImas(int val) { m_Imas = val; }

int FITKPropSolidValue::getIFrame() const { return m_IFrame; }
void FITKPropSolidValue::setIFrame(int val) { m_IFrame = val; }

// FITKPropSolid getter/setter implementations
double FITKPropSolid::getDn() const { return m_dn; }
void FITKPropSolid::setDn(double val) { m_dn = val; }

double FITKPropSolid::getQa() const { return m_qa; }
void FITKPropSolid::setQa(double val) { m_qa = val; }

double FITKPropSolid::getQb() const { return m_qb; }
void FITKPropSolid::setQb(double val) { m_qb = val; }

double FITKPropSolid::getH() const { return m_h; }
void FITKPropSolid::setH(double val) { m_h = val; }

double FITKPropSolid::getLambdaV() const { return m_lambdaV; }
void FITKPropSolid::setLambdaV(double val) { m_lambdaV = val; }

double FITKPropSolid::getMuV() const { return m_muV; }
void FITKPropSolid::setMuV(double val) { m_muV = val; }

double FITKPropSolid::getMinTimeStep() const { return m_minTimeStep; }
void FITKPropSolid::setMinTimeStep(double val) { m_minTimeStep = val; }

int FITKPropSolid::getNdir() const { return m_Ndir; }
void FITKPropSolid::setNdir(int val) { m_Ndir = val; }

int FITKPropSolid::getSPHPartID() const { return m_SPHPartID; }
void FITKPropSolid::setSPHPartID(int val) { m_SPHPartID = val; }

} // namespace Radioss
