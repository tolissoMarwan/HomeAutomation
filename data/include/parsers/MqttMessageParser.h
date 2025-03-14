#include <QJsonDocument>
#include <QJsonObject>

#include "dtos/DeviceState.h"
#include "dtos/SensorData.h"
#include "utils/Logger.h"

Q_DECLARE_LOGGING_CATEGORY(MqttMessageParserCategory)

class MqttMessageParser {
public:
  MqttMessageParser() = default;
  ~MqttMessageParser() = default;

  static bool parseSensorData(const QByteArray &payload, SensorData &outData,
                              QString &error);
  static bool parserDeviceState(const QByteArray &payload, DeviceState &outData,
                                QString &error);

private:
  template <typename T>
  bool parseGeneric(const QByteArray &payload, T &outData, QString &error);
  std::shared_ptr<Logger> m_logger;
  QString categoryName;
};
