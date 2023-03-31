#include "plot.hpp"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>

#include <iostream>

QT_CHARTS_USE_NAMESPACE

class Plot::Impl : public QObject
{
    Q_OBJECT
public:
    Impl();
    ~Impl() = default;

    void scatter(const std::vector<std::pair<double, double>>& data);
    void lines(const std::vector<std::pair<double, double>>& data);
    void process();

private:
    int argc = 0;
    char** argv = nullptr;
    std::unique_ptr<QApplication> app;
    std::unique_ptr<QMainWindow> window;
    QChartView* chartView = nullptr;
    std::unique_ptr<QChart> chart;
    std::unique_ptr<QScatterSeries> scatterSeries;
    std::unique_ptr<QLineSeries> lineSeries;

    void adjustAxisRanges();
};

Plot::Impl::Impl():
    app(std::make_unique<QApplication>(argc, argv)), window(std::make_unique<QMainWindow>()),
    chart(std::make_unique<QChart>()), scatterSeries(std::make_unique<QScatterSeries>()),
    lineSeries(std::make_unique<QLineSeries>())
{
    chartView = new QChartView(window.get());

    // print all pairs in scatterSeries
    for (const auto& point : scatterSeries->points()) {
        std::cout << point.x() << ", " << point.y() << std::endl;
    }

    chart->addSeries(scatterSeries.get());
    chart->addSeries(lineSeries.get());
    chart->createDefaultAxes();

    chart->setTitle("Scatter and line chart");

    chart->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView->setChart(chart.get());
    window->setCentralWidget(chartView);
    window->resize(800, 600);
}

void Plot::Impl::scatter(const std::vector<std::pair<double, double>>& data)
{
    scatterSeries->clear();
    for (const auto& point : data) {
        std::cout << point.first << ", " << point.second << std::endl;
        scatterSeries->append(point.first, point.second);
    }
    adjustAxisRanges();
}

void Plot::Impl::lines(const std::vector<std::pair<double, double>>& data)
{
    lineSeries->clear();
    for (const auto& point : data) {
        lineSeries->append(point.first, point.second);
    }
    adjustAxisRanges();
}

void Plot::Impl::adjustAxisRanges()
{
    if (scatterSeries->count() > 0 || lineSeries->count() > 0) {
        qreal minX = std::numeric_limits<qreal>::max();
        qreal maxX = std::numeric_limits<qreal>::min();
        qreal minY = std::numeric_limits<qreal>::max();
        qreal maxY = std::numeric_limits<qreal>::min();

        for (const QPointF& point : scatterSeries->points()) {
            minX = std::min(minX, point.x());
            maxX = std::max(maxX, point.x());
            minY = std::min(minY, point.y());
            maxY = std::max(maxY, point.y());
        }

        for (const QPointF& point : lineSeries->points()) {
            minX = std::min(minX, point.x());
            maxX = std::max(maxX, point.x());
            minY = std::min(minY, point.y());
            maxY = std::max(maxY, point.y());
        }

        chart->axisX()->setRange(minX, maxX);
        chart->axisY()->setRange(minY, maxY);
    }
}

void Plot::Impl::process()
{
    window->show();
    app->exec();
}

Plot::Plot(): impl(std::make_unique<Impl>()) {}

Plot::~Plot() = default;

void Plot::scatter(const std::vector<std::pair<double, double>>& data) { impl->scatter(data); }

void Plot::lines(const std::vector<std::pair<double, double>>& data) { impl->lines(data); }

void Plot::process() { impl->process(); }

#include "plot.moc"
