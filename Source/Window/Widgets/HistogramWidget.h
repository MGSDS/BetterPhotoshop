#pragma once

#include "Core/Image/ColorModel/ColorModelConverter.hpp"
#include "Core/Image/Image.hpp"
#include <QSplitter>
#include <QWidget>
#include <QtCharts/QChartView>

class HistogramWidget : public QWidget
{
    Q_OBJECT

public:
    HistogramWidget(QWidget* parent = nullptr);
    ~HistogramWidget() = default;

    void SetImage(const Image* image, ActiveChannel activeChannel);

private:

    void SetGrayScaleMode();
    void SetRgbMode();
    void SetCharts(std::vector<std::vector<int>>& histograms);
    void SetRedChart(std::vector<int>& histogram);
    void SetGreenChart(std::vector<int>& histogram);
    void SetBlueChart(std::vector<int>& histogram);
    void SetGrayscaleChart(std::vector<int>& histogram);

    static void SetChart(std::vector<int>& histogram, QChartView* chartView, QColor color, std::string name);

private:
    std::unique_ptr<QChartView> m_RedChartView = nullptr;
    std::unique_ptr<QChartView> m_GreenChartView = nullptr;
    std::unique_ptr<QChartView> m_BlueChartView = nullptr;
    std::unique_ptr<QChartView> m_GrayscaleChartView = nullptr;

    std::unique_ptr<QSplitter> m_Splitter = nullptr;

    bool m_IsGrayScale = false;
};