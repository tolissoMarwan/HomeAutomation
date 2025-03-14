#pragma concept

#include "SensorRepository.h"
#include "dtos/SensorData.h"

#include <memory>

class SensorDataProcessor : public QObject {
  Q_OBJECT
public:
  SensorDataProcessor(std::shared_ptr<SensorRepository> repository,
                      QObject *parent = nullptr);

  virtual ~SensorDataProcessor() = default;

  void setAggregationWindow(int minutes);
  void setThresholds(float maxTemp, float minTemp, float maxHumdidity);

public slots:
  void processRawData(const SensorData &rawData);

signals:
  void processedDataReady(const SensorData &processedData);
  void temperatureAlert(const QString &message);
  void humidityAlert(const QString &message);

private:
  void checkThershold(const SensorData &data);
  void aggregateData();

  std::shared_ptr<SensorRepository> m_repository;
  QVector<SensorData> m_rawDataBuffer;
  mutable QMutex m_mutex;

  int m_aggergationWindow;
  float m_maxTemp;
  float m_minTemp;
  float m_maxHumidity;
};
