#include "FITKAbstractMaterial.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"
FITKLIBINFOREGISTER(FITKInterfacePhysics, FITKInterfacePhysicsVersion);

Interface::FITKAbstractMaterial::FITKAbstractMaterial()
{
    //构造
    static int ABSMATERALID = 0;
    _materialID = ++ABSMATERALID;
}

Interface::FITKAbstractMaterial::~FITKAbstractMaterial()
{
    //析构
}

int Interface::FITKAbstractMaterial::getMaterialID() const
{
    //返回材料ID
    return _materialID;
}

QString Interface::FITKAbstractMaterial::getMaterialDescribe() const
{
    return _describe;
}

void Interface::FITKAbstractMaterial::setMaterialDescribe(const QString& des)
{
    _describe = des;
}
