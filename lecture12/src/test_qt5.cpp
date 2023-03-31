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

/** QT_* and Q_* are mandatory Qt macros */

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
    Q_OBJECT
public:
    explicit ChartView(QWidget* parent = nullptr);
};

#include "test_qt5.moc"

ChartView::ChartView(QWidget* parent): QChartView{new QChart{}, parent}
{
    QScatterSeries* series0 = new QScatterSeries();
    series0->setName("scatter1");

    QScatterSeries* series1 = new QScatterSeries();
    series1->setName("scatter2");

    QScatterSeries* series2 = new QScatterSeries();
    series2->setName("scatter3");

    QLineSeries* line = new QLineSeries();
    line->setName("line");
    QBoxPlotSeries* boxes = new QBoxPlotSeries();
    boxes->setName("boxes");

    series0->append(0, 6);
    series0->append(2, 4);
    series0->append(3, 8);
    series0->append(7, 4);
    series0->append(10, 5);

    *series1 << QPointF(1, 1) << QPointF(3, 3) << QPointF(7, 6) << QPointF(8, 3) << QPointF(10, 2);
    *series2 << QPointF(1, 5) << QPointF(4, 6) << QPointF(6, 3) << QPointF(9, 5);

    for (auto x = 0.0; x < 10.0; x += 0.3)
        line->append(x, std::sin(x) * 4 + 5);

    static auto gen = std::mt19937{std::random_device{}()};
    static auto dist = std::uniform_real_distribution<double>{1, 9};
    static auto data = std::vector<double>(5);
    for (auto x = 0.0; x < 10.0; x += 1) {
        std::generate(std::begin(data), std::end(data), [] { return dist(gen); });
        auto mm = std::minmax_element(data.begin(), data.end());
        auto mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
        auto lq = (mean + *mm.first) / 2;
        auto uq = (mean + *mm.second) / 2;
        boxes->append(new QBoxSet(*mm.first, lq, mean, uq, *mm.second));
    }

    setRenderHint(QPainter::Antialiasing);
    chart()->addSeries(series0);
    chart()->addSeries(series1);
    chart()->addSeries(series2);
    chart()->addSeries(line);
    chart()->addSeries(boxes);

    chart()->setTitle("Simple scatterchart example");
    chart()->createDefaultAxes();
    chart()->setDropShadowEnabled(false);
    // chart()->setAnimationOptions(QChart::SeriesAnimations); // cool but distracting

    chart()->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);
    chart()->legend()->setAlignment(Qt::AlignBottom);
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);  // make sure to initialize it first before any other widget

    ChartView* chartView = new ChartView();
    auto window = QMainWindow{};
    window.setWindowTitle("Qt5 Scatterplot Test");
    window.setCentralWidget(chartView);
    window.resize(800, 600);
    window.show();

    return a.exec();
}
