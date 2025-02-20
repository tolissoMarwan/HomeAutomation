#include "dtos/SensorData.h"

namespace common::dtos {

SensorData::SensorData(double temp, double humidity, const QDateTime &timestamp)
    : m_temp(temp), m_humidity(humidity), m_timestamp(timestamp) {
  if (!isValid()) {
    qWarning() << "invalid SensorData initialized!";
  }
}

double SensorData::temp() const { return m_temp; }

double SensorData::humidity() const { return m_humidity; }

QDateTime SensorData::timestamp() const { return m_timestamp; }

void SensorData::setTemp(double temp) {
  if (temp >= -50.0 && temp <= 100.0) {
    m_temp = temp;
  } else {
    qWarning() << "Invalid temperature value: " << temp;
  }
}

void SensorData::setHumidity(double humidity) {
  if (humidity >= 0 && humidity <= 100.0) {
    m_humidity = humidity;
  } else {
    qWarning() << "Invalid humidity value: " << humidity;
  }
}

void SensorData::setTimestamp(const QDateTime &timestamp) {
  if (timestamp.isValid()) {
    m_timestamp = timestamp;
  } else {
    qWarning() << "Invalid timestamp provided for deviceId";
  }
}

QJsonObject SensorData::toJson() const {
  auto json = QJsonObject();

  json["temp"] = temp();
  json["humidity"] = humidity();
  json["timestamp"] = timestamp().toString(Qt::ISODate);

  return json;
}

SensorData SensorData::fromJson(const QJsonObject &json) {
  auto data = SensorData();

  if (json.contains("temp") && json["temp"].isDouble()) {
    data.setTemp(json["temp"].toDouble());
  }
  if (json.contains("humidity") && json["humidity"].isDouble()) {
    data.setHumidity(json["humidity"].toDouble());
  }
  if (json.contains("timestamp") && json["timestamp"].isString()) {
    data.setTimestamp(
        QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate));
  }

  return data;
}

bool SensorData::isValid() const {
  return (m_temp >= -50.0 && m_temp <= 100.0) &&
         (m_humidity >= 0.0 && m_humidity <= 100.0) && m_timestamp.isValid();
}

bool SensorData::operator==(const SensorData &other) const {
  return qFuzzyCompare(m_temp, other.m_temp) &&
         qFuzzyCompare(m_humidity, other.m_humidity) &&
         m_timestamp == other.m_timestamp;
}
} // namespace common::dtos

QDebug operator<<(QDebug debug, const common::dtos::SensorData &data) {
  QDebugStateSaver saver(debug);
  debug.nospace() << "timestamp=" << data.timestamp().toString(Qt::ISODate)
                  << ", SensorData(temperature=" << data.temp()
                  << ", humidity=" << data.humidity();
  return debug;
}
