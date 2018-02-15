#ifndef SERVERCOMMUNICATION_H
#define SERVERCOMMUNICATION_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QString>
#include <QDateTime>
#include <QUrlQuery>
#include <algorithm>

struct sighting_struct {
    QString id;
    QString species;
    QString description;
    QString count;

    QString dateTime;
};

class ServerCommucation : public QObject
{
    Q_OBJECT

    // Making the methods accessible from qml
    Q_PROPERTY(QString giveNextData READ giveNextData NOTIFY nextDataChanged)
    Q_PROPERTY(int giveVectorSize READ giveVectorSize CONSTANT)
    Q_PROPERTY(int giveSpeciesVectorSize READ giveSpeciesVectorSize CONSTANT)
    Q_PROPERTY(QString giveNextSpecies READ giveNextSpecies CONSTANT)
    Q_PROPERTY(bool sortEarliestFirst READ sortEarliestFirst CONSTANT)

signals:
    void nextDataChanged();

public:
    ServerCommucation();
    // Posts sighting to server
    Q_INVOKABLE bool postSighting(const QString &species , const QString &description, int count);
    // Fetches the sighting data from server
    Q_INVOKABLE bool fetch();

    // Fetches the available duck species from server
    void fetchSpecies ();

    // Returns the next sighting data
    QString giveNextData();
    // Returns the next species name
    QString giveNextSpecies();
    // Returns the count of sightings
    int giveVectorSize();
    // Returns the count of known species
    int giveSpeciesVectorSize();

    // Comparison function for sorting sightings by time
    static bool compareByTime(const sighting_struct& a, const sighting_struct& b);
    // A little dirty workaround to put the function bool even though not needed to
    // return anything, but Q_PROPERTY requires a type where you can create pointer to
    bool sortEarliestFirst();


public slots:
    // Called when the network connection established
    void replyFinished(QNetworkReply*);
    void speciesReplyFinished(QNetworkReply*);

private:
    QNetworkAccessManager* m_manager;
    QNetworkAccessManager* networkManager;
    QNetworkReply* reply_;

    int lastDataIndex;
    int lastSpeciesIndex = 0;
    int lastSightingIndex;

    std::vector<sighting_struct> sighting_vector;
    std::vector<QString> species_vector;

    QString postSpecies;
    QString postDescription;
    int postCount;
};



#endif // SERVERCOMMUNICATION_H
