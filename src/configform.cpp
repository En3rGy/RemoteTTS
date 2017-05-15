#include "configform.h"
#include "ui_configform.h"
#include <QFileDialog>
#include <QDebug>
#include <QTime>

ConfigForm::ConfigForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigForm)
{
    ui->setupUi(this);

    connect( ui->pushButton_save, SIGNAL( released() ), this, SLOT( slot_safeConfig()) );
    connect( ui->toolButton_clipDir, SIGNAL( released() ), this, SLOT( slot_selectClipDir()) );
    connect( ui->toolButton_introSound, SIGNAL( released() ), this, SLOT( slot_selectIntroFile()) );
    connect( ui->pushButton_quit, SIGNAL( released()), this, SIGNAL( signal_quitApp()) );
    connect( ui->toolButton_playSample, SIGNAL( released()), this, SIGNAL( signal_saySample()) );
}

ConfigForm::~ConfigForm()
{
    delete ui;
}

QComboBox * ConfigForm::getVoiceComboBox()
{
    return ui->comboBox_voice;
}

void ConfigForm::loadConfig()
{
    QVariant grVoice = m_grSettings.value( g_sConfig_TTSVoice );
    ui->comboBox_voice->setCurrentText( grVoice.toString() );

    QVariant grClipDir = m_grSettings.value( g_sConfig_clipDir, "../etc" );
    ui->lineEdit_clipDirectory->setText( grClipDir.toString() );

    ui->spinBox_sepTimer->setValue( m_grSettings.value( g_sConfig_sepTimer_ms, 1000 ).toInt() );
    ui->spinBox_ttsInPort->setValue( m_grSettings.value( g_sConfig_ttsInPort, 1000 ).toInt() );
    ui->spinBox_receiverPort->setValue( m_grSettings.value( g_sConfig_receiverPort, 2001 ).toInt() );
    ui->lineEdit_receiverIP->setText( m_grSettings.value( g_sConfig_receiverIP, "127.0.0.1" ).toString() );
    ui->lineEdit_introSound->setText( m_grSettings.value( g_sConfig_intoFile, QString() ).toString() );
}

QVariant ConfigForm::getValue( const QString & p_sValue )
{
    if( p_sValue == g_sConfig_TTSVoice )
        return ui->comboBox_voice->currentText();

    else if ( p_sValue == g_sConfig_clipDir )
        return ui->lineEdit_clipDirectory->text();

    else if ( p_sValue == g_sConfig_sepTimer_ms )
        return ui->spinBox_sepTimer->value();

    else if ( p_sValue == g_sConfig_ttsInPort )
        return ui->spinBox_ttsInPort->value();

    else if ( p_sValue == g_sConfig_receiverIP )
        return ui->lineEdit_receiverIP->text();

    else if ( p_sValue == g_sConfig_receiverPort )
        return ui->spinBox_receiverPort->value();

    else if ( p_sValue == g_sConfig_intoFile )
        return ui->lineEdit_introSound->text();

    return QVariant();
}

void ConfigForm::logEvent(const QString & p_sLog)
{
    QString sTime = QTime::currentTime().toString( "hh:mm:ss,zzz" );
    ui->plainTextEdit_log->appendPlainText( sTime + " - " + p_sLog );
}

void ConfigForm::slot_safeConfig()
{
    m_grSettings.setValue( g_sConfig_TTSVoice, ui->comboBox_voice->currentText() );
    m_grSettings.setValue( g_sConfig_clipDir, ui->lineEdit_clipDirectory->text() );
    m_grSettings.setValue( g_sConfig_sepTimer_ms, ui->spinBox_sepTimer->value() );
    m_grSettings.setValue( g_sConfig_receiverIP, ui->lineEdit_receiverIP->text() );
    m_grSettings.setValue( g_sConfig_receiverPort, ui->spinBox_receiverPort->value() );
    m_grSettings.setValue( g_sConfig_ttsInPort, ui->spinBox_ttsInPort->value() );
    m_grSettings.setValue( g_sConfig_intoFile, ui->lineEdit_introSound->text() );

    m_grSettings.sync();
}

void ConfigForm::slot_selectClipDir()
{
    QString sDir = ui->lineEdit_clipDirectory->text();
    if ( sDir.isEmpty() ) {
        sDir = "../etc";
    }
    sDir = QFileDialog::getExistingDirectory( nullptr, tr("Select Clip Directory"), sDir );
    ui->lineEdit_clipDirectory->setText( sDir );
}

void ConfigForm::slot_selectIntroFile()
{
    QString sFile = ui->lineEdit_introSound->text();

    sFile = QFileDialog::getOpenFileName( nullptr, tr("Select Into File"), sFile );
    ui->lineEdit_introSound->setText( sFile );
}
