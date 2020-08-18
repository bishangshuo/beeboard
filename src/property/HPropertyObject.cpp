#include "HPropertyObject.h"

static HPropertyObject *gInstance = NULL;

HPropertyObject::HPropertyObject(QObject *parent)
    : QObject(parent)
    , m_nPenColor(Qt::red)
    , m_nBrushColor(Qt::transparent)
    , m_nBackgroundColor(QColor(36,66,66))
    , m_nPenWidth(4)
    , m_nEraserWidth(40)
    , m_nPenColorIndex(1)
    , m_nBrushColorIndex(0)
    , m_nPenWidthIndex(2)
    , m_nEraserWidthIndex(4)
{
}

HPropertyObject *HPropertyObject::getInstance(){
   if(!gInstance){
       gInstance = new HPropertyObject();
   }
   return gInstance;
}

QColor HPropertyObject::PenColor() const{
    return m_nPenColor;
}

QColor HPropertyObject::BrushColor() const{
    return m_nBrushColor;
}

int HPropertyObject::PenWidth() const{
    return m_nPenWidth;
}

int HPropertyObject::EraserWidth() const{
    return m_nEraserWidth;
}

QColor HPropertyObject::BackgroundColor() const{
    return m_nBackgroundColor;
}

int HPropertyObject::PenWidthIndex() const{
    return m_nPenWidthIndex;
}

int HPropertyObject::EraserWidthIndex() const{
    return m_nEraserWidthIndex;
}

int HPropertyObject::PenColorIndex() const{
    return m_nPenColorIndex;
}

int HPropertyObject::BrushColorIndex() const{
    return m_nBrushColorIndex;
}

int HPropertyObject::BackgroundColorIndex() const{
    return m_nBackgroundColorIndex;
}

void HPropertyObject::procPenColorChanged(const int &index, const QColor &color){
    m_nPenColorIndex = index;
    m_nPenColor = color;
}

void HPropertyObject::procBrushColorChanged(const int &index, const QColor &color){
    m_nBrushColorIndex = index;
    m_nBrushColor = color;
}

void HPropertyObject::procPenWidthChanged(const int &index, const int &width){
    m_nPenWidthIndex = index;
    m_nPenWidth = width;
}

void HPropertyObject::procEraserWidthChanged(const int &index, const int &width){
    m_nEraserWidthIndex = index;
    m_nEraserWidth = width;
}

void HPropertyObject::procBackgroundColorChanged(const int &index, const QColor &color){
    m_nBackgroundColorIndex = index;
    m_nBackgroundColor = color;
}

