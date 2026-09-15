/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphObjectAgent.h"
#include "PostGraphObjectBase.h"

#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"

#include <vtkDataSetMapper.h>
#include <vtkActor.h>

namespace Interface
{
    PostGraphObjectAgent::PostGraphObjectAgent(PostGraphObjectBase* postData) :
        _postData(postData)
    {

    }

    PostGraphObjectAgent::~PostGraphObjectAgent()
    {
        if (_postData) {
            delete _postData;
            _postData = nullptr;
        }
    }
    PostGraphObjectBase * PostGraphObjectAgent::getGraphObject()
    {
        return _postData;
    }
}

