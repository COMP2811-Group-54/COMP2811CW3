#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include "POPs.hpp"
#include "stats.hpp"

PersistentOrganicPollutants::PersistentOrganicPollutants(): QWidget()
{
  createChart();
  createButtons();
  createBoxes();
  createFilters();
  arrangeWidgets();

  setWindowTitle("Persistent Organic Pollutants");
}

void PersistentOrganicPollutants::createChart()
{
  auto popChart = new QChart();

  auto series = new QLineSeries();
  series->append(0,0);
  series->append(10,10);
  popChart->addSeries(series);

  popChart->setTitle("POPs Chart");

  auto xAxis = new QValueAxis();
  xAxis->setTitleText("Time");
  xAxis->setRange(0,10);
  popChart->addAxis(xAxis, Qt::AlignBottom);
  series->attachAxis(xAxis);

  auto yAxis = new QValueAxis();
  yAxis->setTitleText("Level");
  yAxis->setRange(0,10);
  popChart->addAxis(yAxis, Qt::AlignLeft);
  series->attachAxis(xAxis);

  popChartView = new QChartView(popChart);
}

void PersistentOrganicPollutants::createButtons()
{
  viewList = new QPushButton("View List");
  moreInfo = new QPushButton("More Info");
}

void PersistentOrganicPollutants::createBoxes()
{
  pcbs = new QLabel("<h2>PCBs (Polychlorinated Byphenyls)<h2>"
                            "<p>PCBs are a group of man-made organic chemicals consisting of carbon, hydrogen and chloreine atoms<p>");
  otherPops = new QLabel("<h2>Other POPs<h2>"
                                "Examples include DDT, chlordane and dioxins. These substances have various origins and effects<p>");
  pcbs->setWordWrap(true);
  otherPops->setWordWrap(true);
}

void PersistentOrganicPollutants::createFilters()
{
  QStringList locationOptions;
  locationOptions << "All locations" << "1" << "2" << "3" << "4";
  location = new QComboBox();
  location->addItems(locationOptions);

  locationLabel = new QLabel("&Location:");
  locationLabel->setBuddy(location);

  QStringList timeRangeOptions;
  timeRangeOptions << "All time" << "day" << "week" << "month" << "year";
  timeRange = new QComboBox();
  timeRange->addItems(timeRangeOptions);

  timeRangeLabel = new QLabel("&Time Range:");
  timeRangeLabel->setBuddy(timeRange);
}

void PersistentOrganicPollutants::arrangeWidgets()
{
  QHBoxLayout* filters = new QHBoxLayout();
  filters->addWidget(locationLabel);
  filters->addWidget(location);
  filters->addWidget(timeRangeLabel);
  filters->addWidget(timeRange);
  filters->addStretch();

  QVBoxLayout* chart = new QVBoxLayout();
  chart->addWidget(popChartView);
  chart->addLayout(filters);
  chart->addStretch();

  QVBoxLayout* info = new QVBoxLayout();
  info->addWidget(pcbs);
  info->addWidget(moreInfo);
  info->addWidget(otherPops);
  info->addWidget(viewList);
  info->addStretch();

  QHBoxLayout* layout = new QHBoxLayout();
  layout->addLayout(chart);
  layout->addLayout(info);
  layout->addStretch();

  setLayout(layout);
}