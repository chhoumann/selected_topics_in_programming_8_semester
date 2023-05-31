#ifndef PLOT_HPP
#define PLOT_HPP

#include <memory>
#include <vector>
#include <concepts>

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

class plot_t
{
    std::string title;
    struct app_t;
    struct chart_t;
    std::unique_ptr<app_t> app;      // pimpl of the application
    std::unique_ptr<chart_t> chart;  // pimpl of the chart
    std::string x_axis_label;
    std::string y_axis_label;

public:
    plot_t(std::string title, std::string x_axis_label, std::string y_axis_label, int width, int height);
    plot_t(const plot_t&) = delete;
    plot_t& operator=(const plot_t&) = delete;
    plot_t(plot_t&&) noexcept = default;
    plot_t& operator=(plot_t&&) = default;
    ~plot_t() noexcept;
    void save_to_png(const std::string &filename);
    void scatter(const std::string& name, const std::vector<double>& x, const std::vector<double>& y);
    void lines(const std::string& name, const std::vector<double>& x, const std::vector<double>& y);

    void process();
};

#endif
