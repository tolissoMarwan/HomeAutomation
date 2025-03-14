#include "parsers/MqttMessageParser.h"

Q_LOGGING_CATEGORY(MqttMessageParserCategory, "MqttMessageParser")

template <typename T>
bool MqttMessageParser::parseGeneric(const QByteArray &paylaod, T &outData,
                                     QString &error) {
  auto parseError = QJsonParseError();
  const auto doc = QJsonDocument::fromJson(paylaod, &parseError);

  if (parseError.error != QJsonParseError ::NoError) {
    error = QString("JSON parse error: %1").arg(parseError.errorString());
    qCCritical(MqttMessageParserCategory)
        << QString("MqttMessageParser: %1").arg(error);
    return false;
  }

  auto obj = doc.object();
  const auto data = outData.fromJson(obj);

  if (!data.isValid()) {
    qCWarning(MqttMessageParserCategory)
        << QString("validation failed: %1").arg(error);
  }

  return data.isValid();
}

bool MqttMessageParser::parserDeviceState(const QByteArray &payload,
                                          DeviceState &outData,
                                          QString &error) {
  auto parserInstance = MqttMessageParser();
  return parserInstance.parseGeneric(payload, outData, error);
}

bool MqttMessageParser::parseSensorData(const QByteArray &payload,
                                        SensorData &outData, QString &error) {
  auto parserInstance = MqttMessageParser();
  return parserInstance.parseGeneric(payload, outData, error);
}
