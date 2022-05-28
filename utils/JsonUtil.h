#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <QVariant>
#include <QJsonObject>
#include <QString>

class JsonUtil
{
public:
    JsonUtil();
    static QVariant getValue(QJsonObject const &settings,
                  QString const &path,
                  QVariant const &defaultValue = QVariant());
};

#endif // JSONUTIL_H
