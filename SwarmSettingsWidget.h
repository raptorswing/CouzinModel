#ifndef SWARMSETTINGS_H
#define SWARMSETTINGS_H

#include <QWidget>

namespace Ui {
    class SwarmSettingsWidget;
}

class SwarmSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    struct SwarmSettings
    {
        qreal zoneOfRepulsion;
        qreal zoneOfAttraction;
        qreal zoneOfOrientation;
        qreal turningRateDegrees;
        qreal speed;
        qreal alpha;
    };

public:
    explicit SwarmSettingsWidget(QWidget *parent = 0);
    ~SwarmSettingsWidget();

private slots:
    void on_startResetButton_clicked();

    void on_zoneOfOrientationSpinbox_valueChanged(double arg1);

    void on_zoneOfRepulsionSpinbox_valueChanged(double arg1);

    void on_zoneOfAttractionSpinbox_valueChanged(double arg1);

    void emitSettingsChanged();

    void on_turningRateSpinbox_valueChanged(double arg1);

    void on_speedSpinbox_valueChanged(double arg1);

    void on_alphaSpinbox_valueChanged(double arg1);

signals:
    void restartSignalled(int numberOfAgents);
    void swarmSettingsChanged(SwarmSettingsWidget::SwarmSettings);

private:
    Ui::SwarmSettingsWidget *ui;


};

#endif // SWARMSETTINGS_H
