#include "SwarmSettingsWidget.h"
#include "ui_SwarmSettingsWidget.h"

#include <QTimer>

SwarmSettingsWidget::SwarmSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwarmSettingsWidget)
{
    ui->setupUi(this);
    QTimer::singleShot(10,this,SLOT(emitSettingsChanged()));
}

SwarmSettingsWidget::~SwarmSettingsWidget()
{
    delete ui;
}

void SwarmSettingsWidget::on_startResetButton_clicked()
{
    this->restartSignalled(this->ui->numAgentsSpinbox->value());
}

void SwarmSettingsWidget::on_zoneOfOrientationSpinbox_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    this->emitSettingsChanged();
}

void SwarmSettingsWidget::emitSettingsChanged()
{
    SwarmSettings settings;
    settings.zoneOfRepulsion = this->ui->zoneOfRepulsionSpinbox->value();
    settings.zoneOfAttraction = this->ui->zoneOfAttractionSpinbox->value();
    settings.zoneOfOrientation = this->ui->zoneOfOrientationSpinbox->value();
    settings.turningRateDegrees = this->ui->turningRateSpinbox->value() / 10.0;
    settings.speed = this->ui->speedSpinbox->value() / 10.0;
    settings.alpha = this->ui->alphaSpinbox->value();
    this->swarmSettingsChanged(settings);
}

void SwarmSettingsWidget::on_zoneOfRepulsionSpinbox_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    this->emitSettingsChanged();
}

void SwarmSettingsWidget::on_zoneOfAttractionSpinbox_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    this->emitSettingsChanged();
}

void SwarmSettingsWidget::on_turningRateSpinbox_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    this->emitSettingsChanged();
}

void SwarmSettingsWidget::on_speedSpinbox_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    this->emitSettingsChanged();
}

void SwarmSettingsWidget::on_alphaSpinbox_valueChanged(double arg1)
{
    Q_UNUSED(arg1)
    this->emitSettingsChanged();
}
