#pragma once

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QObject>
#include <memory>

namespace common::utils {
class Logger : public QObject {
  Q_OBJECT

public:
  explicit Logger(const QString &filename = "", QObject *parent = nullptr);
  ~Logger();

  enum class LogLevel { Debug, Info, Warning, Error, Critical, Fatal };
  Q_ENUM(LogLevel)

  static void init(const QString &filename = "");
  static void cleanup();

  static void log(LogLevel level, const QString &msg,
                  const QMessageLogContext &context = QMessageLogContext());

  // Convenience methods (optional, but often useful):
  static void setLogLevel(LogLevel level);
  static LogLevel loglevel();

private:
  static Logger *instance();
  void writeToFile(const QString &message);

  QFile m_logFile;
  QMutex m_mutex;
  LogLevel m_currentLevel;

  static std::unique_ptr<Logger> s_instance;
};
} // namespace common::utils
