#ifndef FACTMETADATA_H
#define FACTMETADATA_H

#include <QObject>
#include <QMap>
#include <QVariant>

#include <QDebug>

/// Holds the meta data associated with a Fact. This is kept in a seperate object from the Fact itself
/// since you may have multiple instances of the same Fact. But there is only ever one FactMetaData
/// instance of each Fact.

class FactMetaData : public QObject
{
    Q_OBJECT
public:
    ~FactMetaData();

    /// value type of metadata
    typedef enum{
        typeUint8,
        typeInt8,
        typeUint16,
        typeInt16,
        typeUint32,
        typeInt32,
        typeFloat,
        typeDouble
    } ValueType_t;

    explicit FactMetaData(ValueType_t type, QObject *parent = 0);

    /// convert type in parameter metadata xml file to ValueType_t type of metadata
    static ValueType_t stringToType(const QString &typeString, bool &unknownType);

    /// @return true: value converted to right type and is in available range
    bool convertAndValidateRaw(const QVariant &rawValue, bool convertOnly, QVariant &convertValue, QString &errorString);

    /// assign value of name,short description etc. parsed from parameter metadata file to this metadata object
    void setName(const QString &name) { _name = name;}
    void setShortDescription(const QString &shortDescription) {_shortDescription = shortDescription;}
    void setRawDefaultValue(const QVariant &rawDefaultValue);
    void setRawMin(const QVariant &rawMin);
    void setRawMax(const QVariant &rawMax);

    /// get value of type,name etc. of this metadata object
    ValueType_t type() const    {return _type;}
    QString name() const {return _name;}
    QString shortDescription() const {return _shortDescription;}
    QVariant rawDefaultValue() const;
    bool defaultValueAvailable() const {return _defaultValueAvailable;}
    QVariant rawMin() const {return _rawMin;}
    QVariant rawMax() const {return _rawMax;}

signals:

public slots:

private:
    QVariant _minForType() const; /// min value of this data type
    QVariant _maxForType() const; /// max value of this data type
    ValueType_t _type; /// data type of metadata

    bool     _defaultValueAvailable; //true when the default value we get is in range (_rawMin,_rawMax)
                                                                 //accepted and copied to _rawDefaultValue
    QString _name; /// name of metadata
    QString _shortDescription;
    QVariant _rawDefaultValue; /// default value of metadata
    QVariant _rawMax; /// max value of metadata; default: min value of this data type
    QVariant _rawMin; /// min value of metadata; default: max value of this data type
    };

#endif // FACTMETADATA_H
