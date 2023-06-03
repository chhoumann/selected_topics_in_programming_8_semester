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
#include <QValueAxis>

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

plot_t::plot_t(std::string title, std::string x_axis_label, std::string y_axis_label, int width, int height):
        title{std::move(title)}, app{std::make_unique<app_t>()}, chart{std::make_unique<chart_t>()}, x_axis_label{std::move(x_axis_label)}, y_axis_label{std::move(y_axis_label)}
{
    app->window.setCentralWidget(chart.get());
    app->window.setWindowTitle(this->title.c_str());
    app->window.resize(width, height);
    chart->setRenderHint(QPainter::Antialiasing);
}

plot_t::~plot_t() noexcept = default;

void plot_t::save_to_png(const std::string& filename)
{
    auto* ch = chart->chart();
    QPixmap pixmap(chart->size());
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    chart->render(&painter);
    pixmap.save(QString::fromStdString(filename), "PNG");
}

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

    if (!ch->axes(Qt::Vertical).isEmpty())
        ch->axes(Qt::Vertical).first()->setTitleText(y_axis_label.c_str());
    if (!ch->axes(Qt::Horizontal).isEmpty())
        ch->axes(Qt::Horizontal).first()->setTitleText(x_axis_label.c_str());

    QFont font = ch->titleFont();
    font.setPointSize(34); // Set the font size you want for the title
    ch->setTitleFont(font);

    QValueAxis *axisX = qobject_cast<QValueAxis *>(ch->axes(Qt::Horizontal).first());
    font = axisX->labelsFont();
    font.setPointSize(16); // Set the font size you want for the x axis
    axisX->setLabelsFont(font);

    QValueAxis *axisY = qobject_cast<QValueAxis *>(ch->axes(Qt::Vertical).first());
    font = axisY->labelsFont();
    font.setPointSize(16); // Set the font size you want for the y axis
    axisY->setLabelsFont(font);

    font = ch->legend()->font();
    font.setPointSize(22); // Set the font size you want for the legend
    ch->legend()->setFont(font);

    auto* yAxis = ch->axes(Qt::Vertical).first();
    yAxis->setTitleText(y_axis_label.c_str());
    QFont fontY = yAxis->titleFont();
    fontY.setPointSize(22); // change this to increase or decrease the size
    yAxis->setTitleFont(fontY);

    auto* xAxis = ch->axes(Qt::Horizontal).first();
    xAxis->setTitleText(x_axis_label.c_str());
    QFont fontX = xAxis->titleFont();
    fontX.setPointSize(22); // change this to increase or decrease the size
    xAxis->setTitleFont(fontX);

    ch->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);
    ch->legend()->setAlignment(Qt::AlignBottom);

    app->window.show();
    app->app.exec();
}
