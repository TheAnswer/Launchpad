#include "configparser.h"
#include <QFile>
#include <QDebug>
#include <QStringList>

ConfigParser::ConfigParser() {
    registerTokens();
}

void ConfigParser::registerTokens() {
//INVALID = -1, EMPTY, COMMENT, EQUAL, BRACKET_LEFT, BRACKET_RIGHT

    tokens['#'] = ConfigToken::COMMENT;
    tokens['='] = ConfigToken::EQUAL;
    tokens['['] = ConfigToken::BRACKET_LEFT;
    tokens[']'] = ConfigToken::BRACKET_RIGHT;
    tokens['\n'] = ConfigToken::NEW_LINE;
    //tokens['\t'] = ConfigToken::TAB;
    tokens['.'] = ConfigToken::DOT;
    //tokens[' '] = ConfigToken::SPACE;
}

int ConfigParser::loadFile(const QString& folder, const QString& configFileName) {
    if (parsedFiles.contains(configFileName.toLower())) {
        qDebug() << "error circular include for file: " << configFileName << " from: " << currentFile;

        return -1;
    } else
        parsedFiles.append(configFileName.toLower());

    QString oldFile = currentFile;
    QString oldFolder = currentFolder;

    //qDebug() << folder + "\\" + configFileName;
#ifdef Q_OS_WIN32
    QFile fileObject(folder + "\\" + configFileName);
#else
    QFile fileObject(folder + "/" + configFileName);
#endif

    if (!fileObject.exists()) {
        //qDebug() << configFileName << " doesnt exist";
        errorMessage(configFileName + " doesnt exist", -1);

        return -1;
    }

    if (!fileObject.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //qDebug() << configFileName << " failed to open";
        errorMessage(configFileName + " failed to open", -1);

        return -1;
    }

    currentFolder = folder;
    currentFile = configFileName;

    QTextStream in(&fileObject);

    int lineNumber = 0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        QTextStream lineStream(&line);
        processLine(&lineStream, ++lineNumber);
    }

    currentFolder = oldFolder;
    currentFile = oldFile;

    //qDebug() << configValues;

    return 0;
}

void ConfigParser::errorMessage(const QString& errorMsg, int lineNumber) {
    QString msg;
    QTextStream stream(&msg);

    if (lineNumber >= 0)
        stream << currentFolder + "/" + currentFile << ":" << lineNumber << " error: " << errorMsg;
    else
        stream << currentFolder + "/" + currentFile << ": error: " << errorMsg;

    qDebug() << msg;
    emit errorSignal(msg);
}

int ConfigParser::parseDotExpression(QTextStream* stream, int lineNumber) {
    //stream->
    QString expression = stream->readAll();

    if (!expression.contains(' ')) {
        //qDebug() << "error in dot expression no space delimiter";

        errorMessage("error in dot expression no space delimiter", lineNumber);

        return -1;
    }

    QStringList values = expression.split(' ');

    if (values.size() < 2) {
        errorMessage("expected 2 values in expression: " + expression, lineNumber);

        return -1;
    }

    if (values.at(0) != "include") {
        errorMessage("unkown dot expression: " + values.at(0), lineNumber);

        return -1;
    }

    QString includeFile = values.at(1);

    if (includeFile.length() < 3) {
        errorMessage("expected \"filename\" found instead:" + includeFile, lineNumber);

        return -1;
    }

    QString fileName = includeFile.mid(1, includeFile.length() - 2);

    qDebug() << "including: " << fileName;

    return loadFile(currentFolder, fileName);
}

int ConfigParser::processLine(QTextStream* stream, int currentLineNumber) {
    bool comment = false;
    QString variableName;
    QString variableValue;
    bool classNameSet = false;

    QString currentLiteral;
    QTextStream currentLiteralStream(&currentLiteral);

    while (!stream->atEnd()) {
        ConfigToken token = readToken(stream);

        //qDebug() << token.value << " " << token.type;

        if (comment)
            return 0;

        switch (token.type) {
        case ConfigToken::CHAR_LITERAL:
            currentLiteralStream << token.value;
            break;
        case ConfigToken::EQUAL:
            //qDebug() << "assignment";

            if (currentLiteral.isEmpty()) {
                errorMessage("no variable name before assignment operator!", currentLineNumber);
            }

            variableName = currentLiteral;
            currentLiteral.clear();
            break;
        case ConfigToken::COMMENT:
            //qDebug() << "comment";
            comment = true;
            break;
        case ConfigToken::BRACKET_LEFT:
            //qDebug() << "class definition start";

            currentClassName.clear();
            break;
        case ConfigToken::BRACKET_RIGHT:
            //qDebug() << "class definition end";

            currentClassName = currentLiteral;
            currentLiteral.clear();

            classNameSet = true;
            break;
        case ConfigToken::DOT:
            //qDebug() << "dot literal";
            if (!variableName.isEmpty() || !variableValue.isEmpty())
                currentLiteralStream << token.value;
            else {
                return parseDotExpression(stream, currentLineNumber);
            }

            break;
        default:
            //qDebug() << "unknown literal found in config:" << token.value;
            errorMessage("unknown literal found in config:" + QString(token.value), currentLineNumber);
            break;
        }

        if (stream->atEnd()) {
            variableValue = currentLiteral;

            if (variableName.isEmpty()) {
                //qDebug() << "variable name is empty on assignment!";
                if (!classNameSet)
                    errorMessage("variable name is empty on assignment!", currentLineNumber);
            } else {
                //qDebug() << "current class:" << currentClassName << " " << "variable:" << variableName << " set to: " << variableValue;

                setVariable(variableName, variableValue);
            }
        }
    }

    return 0;
}

void ConfigParser::setVariable(const QString& variableName, const QString& variableValue) {
    if (currentClassName.isEmpty()) {
        qDebug() << "error assigning a variable to an empty class";
    }

    if (configValues.contains(currentClassName)) {
        QVector<ConfigValue > classValues = configValues.value(currentClassName);
        classValues.append(ConfigValue(variableName, variableValue, currentFile));
        //ConfigValue(const QString& varName, const QString& value, const QString& fileName = "")

        configValues.remove(currentClassName);
        configValues.insert(currentClassName, classValues);
    } else {
        QVector<ConfigValue > classValues;
        //classValues.append(QPair<QString, QString>(variableName, variableValue));
        classValues.append(ConfigValue(variableName, variableValue, currentFile));
        configValues.insert(currentClassName, classValues);
    }
}

ConfigToken ConfigParser::readToken(QTextStream *stream) {
    ConfigToken token;

    *stream >> token.value;

    token.type = tokens.value(token.value, ConfigToken::CHAR_LITERAL);

    return token;
}

int ConfigParser::getConfigValue(const QString& className, const QString& variable, ConfigValue& returnValue) {
    QVector<ConfigValue> values = getValues(className);

    for (int i = values.size() - 1; i >= 0; --i) {
        const ConfigValue val = values.at(i);

        if (val.variableName.toLower() == variable.toLower()) {
            returnValue = val;
            return 0;
        }
    }

    return -1;
}

bool ConfigParser::getBooleanConfigValue(const QString& className, const QString& variable, bool defaultValue) {
    ConfigValue value;
    int ret = getConfigValue(className, variable, value);

    if (ret != 0)
        return defaultValue;

    if (value.variableValue == "0" || value.variableValue.toLower() == "false")
        return false;
    else
        return true;
}

int ConfigParser::getIntConfigValue(const QString& className, const QString& variable, int defaultValue) {
    ConfigValue value;
    int ret = getConfigValue(className, variable, value);

    if (ret != 0)
        return defaultValue;

    int result = value.variableValue.toInt();

    return result;
}

QVector<ConfigValue> ConfigParser::getConfigValues(const QString& className, const QString& variable) {
    QVector<ConfigValue> values = getValues(className);

    QVector<ConfigValue> returnValues;

    for (int i = values.size() - 1; i >= 0; --i) {
        const ConfigValue val = values.at(i);

        if (val.variableName.toLower() == variable.toLower()) {
            returnValues.append(val);
        }
    }

    return returnValues;
}

QString ConfigParser::getStringConfigValue(const QString& className, const QString& variable, const QString& defaultValue) {
    ConfigValue value;
    int ret = getConfigValue(className, variable, value);

    if (ret != 0)
        return defaultValue;

    return value.variableValue;
}
