#include "SensorDataProcessor.h"

SensorDataProcessor::SensorDataProcessor(
    std::shared_ptr<SensorRepository> repository, QObject *parent)
    : QObject(parent), m_repository((repository)), m_rawDataBuffer(),
      m_mutex(), m_aggergationWindow(5), m_maxTemp(35.0f), m_minTemp(10.0f),
      m_maxHumidity(80.0f) {}

void SensorDataProcessor::setAggregationWindow(int minutes) {
  auto locker = QMutexLocker(&m_mutex);

  m_aggergationWindow = qBound(1, minutes, 60);
  // Log
}

void SensorDataProcessor::setThresholds(float maxTemp, float minTemp,
                                        float maxHumdidity) {
  auto locker = QMutexLocker(&m_mutex);

  m_maxTemp = maxTemp;
  m_minTemp = minTemp;
  m_maxHumidity = maxHumdidity;

  // Log
}

void SensorDataProcessor::processRawData(const SensorData &rawData) {
  auto locker = QMutexLocker(&m_mutex);

  if (!rawData.isValid()) {
    // Log
    return;
  }

  m_rawDataBuffer.append(rawData);
  checkThershold(rawData);

  // Remove old data
  auto cutoff = QDateTime::currentDateTime().addSecs(-m_aggergationWindow * 60);
  while (!m_rawDataBuffer.isEmpty() &&
         m_rawDataBuffer.first().timestamp() < cutoff) {
    m_rawDataBuffer.removeFirst();

    // save to repository
    if (m_repository) {
      m_repository->saveSensorData(rawData);
    }

    // Aggregate if windows is full
    if (m_rawDataBuffer.size() >= 10) {
      aggregateData();
    }
  }
}

void SensorDataProcessor::checkThershold(const SensorData &data) {
  if (data.getTemp() > m_maxTemp) {
    const auto msg =
        QString("High temperature alert: %1°C").arg(data.getTemp());

    // Log
    emit temperatureAlert(msg);
  }

  if (data.getTemp() < m_minTemp) {
    const auto msg = QString("Low temperature alert: %1°C").arg(data.getTemp());

    // Log
    emit temperatureAlert(msg);
  }

  if (data.getHumidity() > m_maxHumidity) {
    const auto msg = QString("High Humidity alert: %1").arg(data.getHumidity());

    // Log
    emit humidityAlert(msg);
  }
}

void SensorDataProcessor::aggregateData() {
  if (m_rawDataBuffer.isEmpty())
    return;

  auto aggregated = SensorData();
  double tempSum = 0.0;
  double humiditySum = 0.0;
  const int count = m_rawDataBuffer.size();

  for (const auto &data : std::as_const(m_rawDataBuffer)) {
    tempSum += data.getTemp();
    humiditySum += data.getHumidity();
  }

  aggregated.setTemp(tempSum / count);
  aggregated.setHumidity(humiditySum / count);
  aggregated.setTimestamp(QDateTime::currentDateTime());

  if (aggregated.isValid()) {
    // Log
    emit processedDataReady(aggregated);
  }
}
