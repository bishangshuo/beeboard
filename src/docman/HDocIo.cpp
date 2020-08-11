#include "HDocIo.h"
#include <QApplication>
#include <QPainter>
#include <QDesktopWidget>
#include <QFile>
#include <QBuffer>
#include <string.h>

HDocIo::HDocIo(QObject *parent) : QObject(parent)
{

}

void HDocIo::slotWriteSceneImage(const QPixmap &pixmap, const QString &fileName){
    //pixmap.save(fileName);

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    pixmap.save(&file, "jpg");
    file.close();

    emit sigWriteSceneImageCompleted(fileName);
}

void HDocIo::slotWriteSceneObject(const char *data, int size, const QString &fileName){
    bool res = false;

   QFile file(fileName);
   if(file.open(QIODevice::WriteOnly)){
       qint64 writeBytes = file.write(data, size);
       res = writeBytes > 0;
       file.close();
   }
   emit sigWriteSceneObjectCompleted(res, data, fileName);
}

void HDocIo::slotSavePixmapAsBuffer(qlonglong itemKey, const QPixmap &pixmap){
    QByteArray ba = QByteArray();
    QBuffer buff(&ba);
    pixmap.save(&buff, "png", 100);
    int size = ba.size();
    char *pBuffer = new char[size];
    memcpy(pBuffer, ba.data(), size);
    emit sigSavePixmapAsBufferCompleted(itemKey, pBuffer, size);
}
