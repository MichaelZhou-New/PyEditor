#include "JsonUtil.h"

JsonUtil::JsonUtil()
{

}

/**
 * @brief getValue given a json object and a path, retrieve the value of the key at path
 * @param settings source json object
 * @param path path to the key, where individual keys are separated by "/"
 * @param defaultValue value to return when the key / it's parents are not found in the settings object
 * @return value if found, else defaultValue
 */
QVariant JsonUtil::getValue(QJsonObject const &settings,
                  QString const &path,
                  QVariant const &defaultValue)
{
    QVariant result = defaultValue;

    QStringList pathComponents = path.split("/");
    QJsonObject parentObject = settings;

    while (pathComponents.size() > 0)
    {
        QString const &currentKey = pathComponents.takeFirst();

        if (parentObject.contains(currentKey))
        {
            if (pathComponents.size() == 0)
            {
                result = parentObject.value(currentKey);
            }
            else
            {
                parentObject = parentObject.value(currentKey).toObject();
            }
        }
        else
        {
            qWarning() << "Settings could not access unknown key" << currentKey << " in " << parentObject;
            break;
        }
    }

    return result;
}
