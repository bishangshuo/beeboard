#ifndef HDOCIO_H
#define HDOCIO_H

#include <QObject>
#include <QPixmap>
#include "src/shape/pbshape.pb.h"

class HDocIo : public QObject
{
    Q_OBJECT
public:
    explicit HDocIo(QObject *parent = nullptr);

signals:
    void sigWriteSceneImageCompleted(const QString &fileName);
    void sigWriteSceneObjectCompleted(bool res, const char *data, const QString &fileName);
    void sigSavePixmapAsBufferCompleted(qlonglong itemKey, const char *buffer, int size);
public slots:
    void slotWriteSceneImage(const QPixmap &pixmap, const QString &fileName);
    void slotWriteSceneObject(const char *data, int size, const QString &fileName);
    void slotSavePixmapAsBuffer(qlonglong itemKey, const QPixmap &pixmap);
};

#endif // HDOCIO_H
