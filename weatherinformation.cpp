#include "weatherinformation.h"

#include <QJsonDocument>
#include <QJsonArray>

#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

WeatherInformation::WeatherInformation()
{

}

void WeatherInformation::LoadInformation()
{
    QUrl url(this->url);
    QUrlQuery query;
    query.addQueryItem("format", "j1");
    url.setQuery(query.toString(QUrl::FullyEncoded));
    qDebug() << url;
    QNetworkRequest request(url);
    QNetworkAccessManager *m_http = new QNetworkAccessManager(this);
    QNetworkReply *reply = m_http->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, m_http](){
        QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
        QByteArray data = reply->readAll();
        qDebug() << data;
        qDebug() << reply->error();
        this->weatherData = QJsonDocument::fromJson(data).object();
        this->currentWeatherData = GetCurrentWeatherData();
        emit loadFinished(reply);
    });
}

QJsonObject WeatherInformation::GetCurrentWeatherData() const
{
    return weatherData.value("current_condition").toArray().at(0).toObject();
}

int WeatherInformation::current_temp_C() const
{
    // "temp_C": "5",
    return currentWeatherData.value("temp_C").toInt();
}

QString WeatherInformation::current_weatherDesc() const
{
    /*
    "weatherDesc": [
        {
            "value": "Overcast"
        }
    ],
    */
    return currentWeatherData.value("weatherDesc").toArray().at(0).toObject().value("value").toString();
}

QString WeatherInformation::current_weatherDescCN() const
{
    /*
     * "lang_zh": [
                {
                    "value": "阴天"
                }
            ],
    */
    // 如果能获取到中文的话
    if (!currentWeatherData.contains("lang_zh")) {
        // 如果没有，则
        return NULL;
    }
    return currentWeatherData.value("lang_zh").toArray().at(0).toObject().value("value").toString();
}

QString WeatherInformation::current_weatherIconUrl() const
{
    QString weatherName = this->current_weatherDesc();
    QString weatherNameKey = "unknown";
    foreach (QString name, this->weatherNameKey) {
        if (weatherName.contains(name)) {
            weatherNameKey = name;
            break;
        }
    }
    QString weatherID = this->weatherIconID.value(weatherNameKey);
    return ":/icon/Simple/" + weatherID + "d.png";
}
