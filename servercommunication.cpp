#include "servercommunication.h"

ServerCommucation::ServerCommucation()
{
    m_manager = new QNetworkAccessManager(this);
    networkManager = new QNetworkAccessManager(this);

    lastDataIndex = 0;
    lastSightingIndex = 0;

    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
         this, SLOT(replyFinished(QNetworkReply*)));
    connect(networkManager, SIGNAL(finished(QNetworkReply*)),
         this, SLOT(speciesReplyFinished(QNetworkReply*)));
}

bool ServerCommucation::fetch()
{
    //reply_ = m_manager->get(QNetworkRequest(QUrl("http://88.193.159.113:8081/sightings")));
    reply_ = m_manager->get(QNetworkRequest(QUrl("http://88.193.159.113:8081/sightings")));
    return true;
}

void ServerCommucation::fetchSpecies()
{
    networkManager->get(QNetworkRequest(QUrl("http://88.193.159.113:8081/species")));
}


void ServerCommucation::replyFinished(QNetworkReply* pReply) {

    //Reading the data from the server
    QByteArray data = pReply->readAll();
    QString str(data);

    QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());
    QJsonArray json_array = jsonResponse.array();

    // Saving the values from JSON file
    foreach (const QJsonValue &value, json_array) {
        QJsonObject json_obj = value.toObject();
        sighting_struct sighting;

        QString id = json_obj["id"].toString();
        sighting.id = id;

        QString species = json_obj["species"].toString();
        sighting.species = species;

        QString description = json_obj["description"].toString();
        sighting.description = description;

        int count = json_obj["count"].toInt();
        QString countString = QString::number(count);
        sighting.count = countString;

        QDateTime dateTime = QDateTime::fromString(json_obj["dateTime"].toString(), Qt::ISODate);
        QString dateTimeString = dateTime.toString(Qt::SystemLocaleShortDate);
        sighting.dateTime = dateTimeString;

        sighting_vector.push_back(sighting);
    }

    // Sorting the sightings by time to display them  chronologically by default
    sortEarliestFirst();

}

void ServerCommucation::speciesReplyFinished(QNetworkReply* reply) {

    QByteArray data = reply->readAll();
    QString str(data);

    QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());
    QJsonArray json_array = jsonResponse.array();

    foreach (const QJsonValue &value, json_array) {
        QJsonObject json_obj = value.toObject();

        QString speciesName = json_obj["name"].toString();
        species_vector.push_back(speciesName);
    }
}

int ServerCommucation::giveVectorSize() {

    // If reply from the server is not finished, blocking the run until it's done
    if (!reply_->isFinished()) {
        QEventLoop pause;
        connect(m_manager, SIGNAL(finished(QNetworkReply*)), &pause, SLOT(quit()));
        pause.exec();
    }

    return sighting_vector.size();
}

QString ServerCommucation::giveNextData() {

    // Blocking the run until server has replayed
    if (!reply_->isFinished()) {
        QEventLoop pause;
        connect(m_manager, SIGNAL(finished(QNetworkReply*)), &pause, SLOT(quit()));
        pause.exec();
    }

    QString newData;

    if (lastDataIndex == 0) {
        newData = sighting_vector.at(lastSightingIndex).id;
        ++lastDataIndex;
    }

    else if (lastDataIndex == 1) {
        newData = sighting_vector.at(lastSightingIndex).species;
        ++lastDataIndex;
    }

    else if (lastDataIndex == 2) {
        newData = sighting_vector.at(lastSightingIndex).description;
        ++lastDataIndex;
    }

    else if (lastDataIndex == 3) {
        newData = sighting_vector.at(lastSightingIndex).dateTime;
        ++lastDataIndex;
    }

    else if (lastDataIndex == 4) {
        newData = sighting_vector.at(lastSightingIndex).count;
        ++lastSightingIndex;
        lastDataIndex = 0;
    }

    return newData;
}

QString ServerCommucation::giveNextSpecies() {

    QString species = species_vector.at(lastSpeciesIndex);

    ++lastSpeciesIndex;
    return species;
}

int ServerCommucation::giveSpeciesVectorSize() {
    return species_vector.size();
}

bool ServerCommucation::postSighting(const QString &species, const QString &description, int count) {

    // Establishing network connection
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QUrl url = QUrl("http://88.193.159.113:8081/sightings");

    // Establishing network requet
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QDateTime currentTime = QDateTime::currentDateTime();
    QString dateTimeString = currentTime.toString(Qt::ISODate);

    QJsonObject object;
    object["species"] = species;
    object["description"] = description;
    object["count"] = count;
    object["dateTime"] = dateTimeString;

    QJsonDocument doc(object);
    QByteArray jsonString = doc.toJson();

    // Posting a new sighting to server
    manager->post(request, jsonString);

    return true;
}

bool ServerCommucation::compareByTime(const sighting_struct &a, const sighting_struct &b) {

    QDateTime dateTime_a = QDateTime::fromString(a.dateTime, "d.M.yyyy h.mm");
    QDateTime dateTime_b = QDateTime::fromString(b.dateTime, "d.M.yyyy h.mm");

    return dateTime_a > dateTime_b;
}


bool ServerCommucation::sortEarliestFirst() {

    std::sort(sighting_vector.begin(), sighting_vector.end(), compareByTime);
    return true;
}

