#ifndef WEATHERINFORMATION_H
#define WEATHERINFORMATION_H

#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QNetworkReply>


class WeatherInformation : public QObject
{
    Q_OBJECT

public:
    WeatherInformation();
    void LoadInformation();
    QJsonObject GetCurrentWeatherData() const;
   int current_temp_C() const;
   QString current_weatherDesc() const;
   QString current_weatherDescCN() const;
   QString current_weatherIconUrl() const;

signals:
    void loadFinished(QNetworkReply *reply);

private:
    QJsonObject weatherData;
    QJsonObject currentWeatherData;
    QString url = "https://wttr.in/";
    QMap<QString, QString> weatherIconID = {
        {"sunny", "01"},
        {"cloudy", "02"},
        {"overcast", "03"},
        {"rain", "10"},
        {"thundery", "11"},
        {"snow", "13"},
        {"unknown", "na"}
    };
    QStringList weatherNameKey = {
        "cloudy",
        "sunny",
        "rain",
        "thundery",
        "snow",
        "unknown",
        "overcast"
    };
};

#endif // WEATHERINFORMATION_H
