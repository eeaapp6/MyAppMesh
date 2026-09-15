#include "FITKInteractionLocation.h"
#include "FITKBoundaryLocation.h"

namespace Interface
{
    FITKInteractionLocation::FITKInteractionLocation()
    {
        //初始化，默认两个位置
        _locations.append(new FITKBoundaryLocation());
        _locations.append(new FITKBoundaryLocation());
    }

    FITKInteractionLocation::~FITKInteractionLocation()
    {
        //释放
        for (FITKBoundaryLocation* loc : _locations)
        {
            if(loc) delete loc;
        }
        _locations.clear();
    }

    FITKBoundaryLocation* FITKInteractionLocation::getFirstLocation()
    {
        //第一位置
        return this->getLocationAt(0);
    }

    FITKBoundaryLocation* FITKInteractionLocation::getSecondLocation()
    {
        //第二位置
        return this->getLocationAt(1);
    }

    void FITKInteractionLocation::swap()
    {
        //交换位置
        FITKBoundaryLocation* t1 = this->getLocationAt(0);
        FITKBoundaryLocation* t2 = this->getLocationAt(1);
        _locations[0] = t2;
        _locations[1] = t1;
    }

    FITKBoundaryLocation* FITKInteractionLocation::getLocationAt(const int index)
    {
        this->resize(index + 1);
        if (index < 0 || index >= getLocationCount()) return nullptr;
        return _locations[index];
    }

    int FITKInteractionLocation::getLocationCount() const
    {
        return _locations.size();
    }

    bool FITKInteractionLocation::copy(Core::FITKAbstractDataObject* obj)
    {
        FITKInteractionLocation* interactionLoc = dynamic_cast<FITKInteractionLocation*>(obj);
        if (interactionLoc == nullptr) return false; // 动态转换失败，返回false

        const int nloc = interactionLoc->getLocationCount(); // 获取当前对象中的位置计数
        for (int i = 0;i<nloc; ++i) // 遍历所有位置
        {
            FITKBoundaryLocation* oriLoc = interactionLoc->getLocationAt(i); // 获取源对象的位置
            if (oriLoc == nullptr)continue; // 如果源位置为空，则跳过当前迭代

            FITKBoundaryLocation* desloc = this-> getLocationAt(i); // 获取当前对象对应的位置
            *desloc = *oriLoc; // 将源位置信息复制到当前对象位置
        }
        return true; // 成功复制所有位置信息，返回true
    }

    void FITKInteractionLocation::removeLocationAt(int index)
    {
        if (index < 0 || index >= _locations.size()) return;
        FITKBoundaryLocation* bloc = _locations.at(index);
        this->removeLocation(bloc);
    }

    void FITKInteractionLocation::removeLocationAt(QList<int> indexs)
    {
        QList<FITKBoundaryLocation*> boundarys;
        //记录要删除的对象，避免index失效
        for (int index : indexs)
        {
            if (index < 0 || index >= _locations.size()) continue;
            FITKBoundaryLocation* bloc = _locations.at(index);
            boundarys.append(bloc);
        }
        for (FITKBoundaryLocation* loc : boundarys)
            this->removeLocation(loc);
    }

    void FITKInteractionLocation::removeLocationAt(int index1, int index2)
    {
        this->removeLocationAt(QList<int>{index1, index2});
    }

    void FITKInteractionLocation::removeLocation(FITKBoundaryLocation* loc)
    {
        if (!_locations.removeOne(loc)) return;
        _locations.removeOne(loc);
        if(loc) delete loc;
    }

    void FITKInteractionLocation::removeLastLocation()
    {
        FITKBoundaryLocation* bc = _locations.last();
        this->removeLocation(bc);
    }

    void FITKInteractionLocation::setLocationInvalid(const int index)
    {
        FITKBoundaryLocation* loc = this->getLocationAt(index);
        if (loc == nullptr) return;
        loc->setModel(-1);
        loc->setComponent(-1);
    }

    void FITKInteractionLocation::resize(const int nize)
    {
        while (_locations.size() < nize)
        {
            _locations.append(new FITKBoundaryLocation);
        }
    }

}
