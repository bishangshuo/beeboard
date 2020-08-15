#include "ItemCtrl.h"

ItemCtrl::ItemCtrl()
    : m_pCBRemove(NULL)
    , m_pCBRotate(NULL)
    , m_pCBItemChanged(NULL)
{

}

ItemCtrl::~ItemCtrl(){
    for(StGeo::iterator it = m_stUndo.begin(); it != m_stUndo.end(); ){
        BASEITEM_GEO *geo = *it;
        delete geo;
        it = m_stUndo.erase(it);
    }
    m_stUndo.clear();

    for(StGeo::iterator it = m_stRedo.begin(); it != m_stRedo.end(); ){
        BASEITEM_GEO *geo = *it;
        delete geo;
        it = m_stRedo.erase(it);
    }
    m_stRedo.clear();

}
