/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "RadWriteInformationMapper.h"

namespace Radioss
{

    void RadWriteInformationMapper::insertMaterialIDMap(const int materialID, const int radID)
    {
        _materialIDMap.insert(materialID, radID);
    }
    int RadWriteInformationMapper::getRadIDByMaterialID(const int materialID)
    {
        return _materialIDMap.value(materialID, 0);
    }
    int RadWriteInformationMapper::getMaxRadMaterialID()
    {
        if(_materialIDMap.isEmpty()) return 0;
        return std::max_element(_materialIDMap.begin(), _materialIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertPropertyIDMap(const int propertyID, const int radID)
    {
        _propertyIDMap.insert(propertyID, radID);
    }
    int RadWriteInformationMapper::getRadIDByPropertyID(const int propertyID)
    {
        return _propertyIDMap.value(propertyID, 0);
    }
    int RadWriteInformationMapper::getMaxRadPropertyID()
    {
        if (_propertyIDMap.isEmpty()) return 0;
        return std::max_element(_propertyIDMap.begin(), _propertyIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertCurveIDMap(const int curveID, const int radID)
    {
        _curveIDMap.insert(curveID, radID);
    }
    int RadWriteInformationMapper::getRadIDByCurveID(const int curveID)
    {
        return _curveIDMap.value(curveID, -1);
    }
    int RadWriteInformationMapper::getMaxRadCurveID()
    {
        if (_curveIDMap.isEmpty()) return 0;
        return std::max_element(_curveIDMap.begin(), _curveIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertRWallIDMap(const int rWallID, const int radID)
    {
        _rWallIDMap.insert(rWallID, radID);
    }
    int RadWriteInformationMapper::getRadIDByRWallID(const int rWallID)
    {
        return _rWallIDMap.value(rWallID, 0);
    }
    int RadWriteInformationMapper::getMaxRadRWallID()
    {
        if (_rWallIDMap.isEmpty()) return 0;
        return std::max_element(_rWallIDMap.begin(), _rWallIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertGravityIDMap(const int gravityID, const int radID)
    {
        _gravityIDMap.insert(gravityID, radID);
    }
    int RadWriteInformationMapper::getRadIDByGravityID(const int gravityID)
    {
        return _gravityIDMap.value(gravityID, 0);
    }
    int RadWriteInformationMapper::getMaxRadGravityID()
    {
        if (_gravityIDMap.isEmpty()) return 0;
        return std::max_element(_gravityIDMap.begin(), _gravityIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertNodeSetIDMap(const int setID, const int radID)
    {
        _nodeSetIDMap.insert(setID, radID);
    }
    int RadWriteInformationMapper::getRadIDByNodeSetID(const int setID)
    {
        return _nodeSetIDMap.value(setID, 0);
    }
    int RadWriteInformationMapper::getMaxRadNodeSetID()
    {
        if (_nodeSetIDMap.isEmpty()) return 0;
        return std::max_element(_nodeSetIDMap.begin(), _nodeSetIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertEleSetIDMap(const int setID, const int radID)
    {
        _eleSetIDMap.insert(setID, radID);
    }
    int RadWriteInformationMapper::getRadIDByEleSetID(const int setID)
    {
        return _eleSetIDMap.value(setID, 0);
    }
    int RadWriteInformationMapper::getMaxRadEleSetID()
    {
        if (_eleSetIDMap.isEmpty()) return 0;
        return std::max_element(_eleSetIDMap.begin(), _eleSetIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertInitialFieldIDMap(const int initialFieldID, const int radID)
    {
        _initialFieldIDMap.insert(initialFieldID, radID);
    }
    int RadWriteInformationMapper::getRadIDByInitialFieldID(const int initialFieldID)
    {
        return _initialFieldIDMap.value(initialFieldID, 0);
    }
    int RadWriteInformationMapper::getMaxRadInitialFieldID()
    {
        if (_initialFieldIDMap.isEmpty()) return 0;
        return std::max_element(_initialFieldIDMap.begin(), _initialFieldIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertBcIDMap(const int bcID, const int radID)
    {
        _bcIDMap.insert(bcID, radID);
    }
    int RadWriteInformationMapper::getRadIDByBcID(const int bcID)
    {
        return _bcIDMap.value(bcID, 0);
    }
    int RadWriteInformationMapper::getMaxRadBcID()
    {
        if (_bcIDMap.isEmpty()) return 0;
        return std::max_element(_bcIDMap.begin(), _bcIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertPartIDMap(const int partID, const int radID)
    {
        _partIDMap.insert(partID, radID);
    }
    int RadWriteInformationMapper::getRadIDByPartID(const int partID)
    {
        return _partIDMap.value(partID, 0);
    }
    int RadWriteInformationMapper::getMaxRadPartID()
    {
        if (_partIDMap.isEmpty()) return 0;
        return std::max_element(_partIDMap.begin(), _partIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertSubsetIDMap(const int subsetID, const int radID)
    {
        _subsetIDMap.insert(subsetID, radID);
    }
    int RadWriteInformationMapper::getRadIDBySubsetID(const int subsetID)
    {
        return _subsetIDMap.value(subsetID, 0);
    }
    int RadWriteInformationMapper::getMaxRadSubsetID()
    {
        if (_subsetIDMap.isEmpty()) return 0;
        return std::max_element(_subsetIDMap.begin(), _subsetIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertSurfaceIDMap(const int surfaceID, const int radID)
    {
        _surfaceIDMap.insert(surfaceID, radID);
    }
    int RadWriteInformationMapper::getRadIDBySurfaceID(const int surfaceID)
    {
        return _surfaceIDMap.value(surfaceID, -1);
    }
    int RadWriteInformationMapper::getMaxRadSurfaceID()
    {
        if (_surfaceIDMap.isEmpty()) return 0;
        return std::max_element(_surfaceIDMap.begin(), _surfaceIDMap.end()).value();
    }

    int RadWriteInformationMapper::getNextSegSurfaceID()
    {
        return ++_segSurfaceID;
    }

    void RadWriteInformationMapper::insertInteractionIDMap(const int interactionID, const int radID)
    {
        _interactionIDMap.insert(interactionID, radID);
    }
    int RadWriteInformationMapper::getRadIDByInteractionID(const int interactionID)
    {
        return _interactionIDMap.value(interactionID, 0);
    }
    int RadWriteInformationMapper::getMaxRadInteractionID()
    {
        if (_interactionIDMap.isEmpty()) return 0;
        return std::max_element(_interactionIDMap.begin(), _interactionIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertAccelIDMap(const int accelID, const int radID)
    {
        _accelIDMap.insert(accelID, radID);
    }
    int RadWriteInformationMapper::getRadIDByAccelID(const int accelID)
    {
        return _accelIDMap.value(accelID, 0);
    }
    int RadWriteInformationMapper::getMaxRadAccelID()
    {
        if (_accelIDMap.isEmpty()) return 0;
        return std::max_element(_accelIDMap.begin(), _accelIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertSectionIDMap(const int sectionID, const int radID)
    {
        _sectionIDMap.insert(sectionID, radID);
    }

    int RadWriteInformationMapper::getRadIDBySectionID(const int sectionID)
    {
        return _sectionIDMap.value(sectionID, 0);
    }

    int RadWriteInformationMapper::getMaxRadSectionID()
    {
        if (_sectionIDMap.isEmpty()) return 0;
        return std::max_element(_sectionIDMap.begin(), _sectionIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertConnectionIDMap(const int connectionID, const int radID)
    {
        _connectionIDMap.insert(connectionID, radID);
    }
    int RadWriteInformationMapper::getRadIDByConnectionID(const int connectionID)
    {
        return _connectionIDMap.value(connectionID, 0);
    }
    int RadWriteInformationMapper::getMaxRadConnectionID()
    {
        if (_connectionIDMap.isEmpty()) return 0;
        return std::max_element(_connectionIDMap.begin(), _connectionIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertTimeHIDMap(const int thID, const int radID)
    {
        _timeHID.insert(thID, radID);
    }
    int RadWriteInformationMapper::getRadIDByTimeHID(const int thID)
    {
        return _timeHID.value(thID, 0);
    }
    int RadWriteInformationMapper::getMaxRadTimeHID()
    {
        if (_timeHID.isEmpty()) return 0;
        return std::max_element(_timeHID.begin(), _timeHID.end()).value();
    }

    void RadWriteInformationMapper::insertFailureIDMap(const int failID, const int radID)
    {
        _failureIDMap.insert(failID, radID);
    }
    int RadWriteInformationMapper::getRadIDByFailureID(const int failID)
    {
        return _failureIDMap.value(failID, -1);
    }
    int RadWriteInformationMapper::getMaxRadFailureID()
    {
        if (_failureIDMap.isEmpty()) return 0;
        return std::max_element(_failureIDMap.begin(), _failureIDMap.end()).value();
    }

    void RadWriteInformationMapper::insertBEMIDMap(const int bemID, const int radID)
    {
        _bemIDMap.insert(bemID, radID);
    }
    int RadWriteInformationMapper::getRadIDByBEMID(const int bemID)
    {
        return _bemIDMap.value(bemID, -1);
    }
    int RadWriteInformationMapper::getMaxRadBEMID()
    {
        if (_bemIDMap.isEmpty()) return 0;
        return std::max_element(_bemIDMap.begin(), _bemIDMap.end()).value();
    }
}