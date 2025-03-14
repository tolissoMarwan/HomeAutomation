#pragma once

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QLoggingCategory>
#include <QMutex>
#include <QObject>
#include <atomic>
#include <memory>

Q_DECLARE_LOGGING_CATEGORY(loggerCategory)

class Logger : public QObject {
  Q_OBJECT

public:
  enum class LogLevel { Debug, Info, Warning, Critical, Fatal, Unknown };

  explicit Logger(QObject *parent = nullptr);
  virtual ~Logger();

  std::shared_ptr<Logger> getInstance() const;
  static void setInstance(std::shared_ptr<Logger> instance);

private:
  static void messageHandler(QtMsgType type, const QMessageLogContext &context,
                             const QString &msg);

  void setLogLevel(LogLevel level);
  void writeToFile(const QString &message) const;
  void init();

  QString log(const QString &categoryName, const QString &msg) const;
  QString loglevelToStr(const Logger::LogLevel &logLevel) const;
  QString formatMessage(const QString &categoryName, const QString &msg) const;

  mutable QFile m_logFile;
  mutable QMutex m_mutex;
  Logger::LogLevel m_logLevel;

  static std::shared_ptr<Logger> m_instance;
};
