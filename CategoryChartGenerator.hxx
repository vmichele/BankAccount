#ifndef CATEGORYCHARTGENERATOR_HXX
#define CATEGORYCHARTGENERATOR_HXX

#include <QObject>
#include <QtCharts/QChartView>
#include <QDate>
#include <QString>

class CategoryChartGenerator: public QObject {
public:
  CategoryChartGenerator(QString const& p_category, QDate const& p_beginDate, QDate const& p_endDate, QObject* p_parent = nullptr);

  QtCharts::QChartView* createChartView();

  inline void setCategory(QString const& p_category) {m_category = p_category;}

protected:
  float getCategoryAmount(QDate const p_date) const;
  QString getCurrentCSVFileName(const QDate& p_date) const;

private:
  QString m_category;
  QDate m_beginDate;
  QDate m_endDate;
};

#endif // CATEGORYCHARTGENERATOR_HXX