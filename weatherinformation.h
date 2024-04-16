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

signals:
    void loadFinished(QNetworkReply *reply);

private:
    QJsonObject weatherData;
    QJsonObject currentWeatherData;
    QString url = "https://wttr.in/";
};

#endif // WEATHERINFORMATION_H
