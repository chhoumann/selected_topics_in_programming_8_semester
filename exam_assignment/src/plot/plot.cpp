#include "plot.hpp"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBoxPlotSeries>
#include <QtCharts/QLegendMarker>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QtMath>

#include <random>
#include <algorithm>

QT_CHARTS_USE_NAMESPACE

struct plot_t::app_t
{
    int argc{0};
    char** args{nullptr};
    QApplication app{argc, args};
    QMainWindow window{nullptr};
};

class plot_t::chart_t : public QChartView
{
public:
    chart_t(): QChartView{new QChart{}, nullptr} {}
    ~chart_t() noexcept {}
};

plot_t::plot_t(std::string title, int width, int height):
        title{std::move(title)}, app{std::make_unique<app_t>()}, chart{std::make_unique<chart_t>()}
{
    app->window.setCentralWidget(chart.get());
    app->window.setWindowTitle(this->title.c_str());
    app->window.resize(width, height);
    chart->setRenderHint(QPainter::Antialiasing);
}

plot_t::~plot_t() noexcept = default;

// Helper template function for DRY code
template <typename Series, typename Chart>
void add_xy(Chart& chart, const std::string& name, const std::vector<double>& x, const std::vector<double>& y)
{
    assert(x.size() <= y.size());
    auto* series = new Series();
    series->setName(name.c_str());
    for (auto i = 0; i < x.size(); ++i)
        series->append(x[i], y[i]);
    chart.addSeries(series);
}

void plot_t::scatter(const std::string& name, const std::vector<double>& x, const std::vector<double>& y)
{
    add_xy<QScatterSeries>(*chart->chart(), name, x, y);
}

void plot_t::lines(const std::string& name, const std::vector<double>& x, const std::vector<double>& y)
{
    add_xy<QLineSeries>(*chart->chart(), name, x, y);
}

void plot_t::process()
{
    auto* ch = chart->chart();
    ch->setTitle(title.c_str());
    ch->createDefaultAxes();
    ch->setDropShadowEnabled(false);
    // chart()->setAnimationOptions(QChart::SeriesAnimations); // cool but distracting

    ch->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);
    ch->legend()->setAlignment(Qt::AlignBottom);

    app->window.show();
    app->app.exec();
}
