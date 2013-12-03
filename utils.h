#ifndef UTILS_H
#define UTILS_H

#include <QStringList>

class Utils {
public:
    Utils();

    static QStringList getArgumentList(const QString& args);
};

#endif // UTILS_H
