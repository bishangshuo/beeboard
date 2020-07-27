#include "PropObj.h"
#include <QColor>

PropObj *PropObj::g_pProp = nullptr;
PropObj::PropObj(QObject *parent) : QObject(parent)
{

}

PropObj *PropObj::GetInstance(){
    if(nullptr == PropObj::g_pProp)
        PropObj::g_pProp = new PropObj();
    return PropObj::g_pProp;
}

int PropObj::PenWidth() const{
    return 4;
}

QColor PropObj::PenColor() const{
    return  Qt::red;
}

QColor PropObj::BrushColor() const{
    return Qt::blue;
}
