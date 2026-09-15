/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITKParamLabel_H
#define FITKParamLabel_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParam.h"

namespace Core
{
    class FITKEasyParamAPI FITKParamLabel : public FITKAbstractEasyParam
    {
        Q_OBJECT;
    public:
        FITKParamLabel();
        virtual ~FITKParamLabel();

        virtual FITKAbstractEasyParam::FITKEasyParamType getParamType() override;
        virtual void copyParaData(FITKAbstractEasyParam* data) override;

        void setValue(QStringList value);
        void appendValue(QString value);
        bool isExist(QString v);
        QStringList getValue();

    private:
        QStringList _value{};
    };
}

#endif
