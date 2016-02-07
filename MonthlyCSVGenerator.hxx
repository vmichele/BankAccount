#ifndef MONTHLYCSVGENERATOR_HXX
#define MONTHLYCSVGENERATOR_HXX

#include <exception>
#include <string>

#include <QChar>
#include <QDate>

class QString;

class MonthlyCSVGenerator {
public:
  MonthlyCSVGenerator();

  static void convertRawCSVToMonthlyCSV(const QDate& p_date, const QChar& p_delim = ';',
    const QString& p_outputFileName = "operations.csv", const QString& p_rawFileName = "raw.csv");
  static void saveCategory(int p_row, QString const& p_category, QString const& p_inFileName);
  static void updateRawCSV(QDate const& p_date, QString const& p_inFileName, QChar const& p_delim = ';', bool p_hasHeader = true);

protected:
  static void CleanOperations(const QString& p_fileName);

private:
  static QString getOperationType(QString const& p_operationType);
};

class open_failure: public std::exception {
public:
  open_failure(std::string const& fileName, bool p_readMode = true):
    m_fileName(fileName),
    m_readMode(p_readMode) {
  }

private:
  virtual const char* what() const throw() {
    auto message = std::string("Cannot open "+m_fileName+" in "+(m_readMode?"read":"write")+" mode.");
    return message.c_str();
  }

  std::string m_fileName;
  bool m_readMode;
};

#endif // MONTHLYCSVGENERATOR_HXX
