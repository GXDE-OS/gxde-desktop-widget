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
    QString current_temp_C() const;

signals:
    void loadFinished(QNetworkReply *reply);

private:
    QJsonObject weatherData;
    QJsonObject currentWeatherData;
    QString url = "https://wttr.in/?format=j1";
};

#endif // WEATHERINFORMATION_H
