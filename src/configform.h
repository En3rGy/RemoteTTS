#ifndef CONFIGFORM_H
#define CONFIGFORM_H

#include <QWidget>
#include <QSettings>

class QComboBox;

namespace Ui {
class ConfigForm;
}

static const QString g_sConfig_TTSVoice = "default/TTSVoice";
static const QString g_sConfig_clipDir  = "default/ClipDirectory";
static const QString g_sConfig_sepTimer_ms  = "default/SeperationTimer_ms";
static const QString g_sConfig_ttsInPort = "default/TTSInPort";
static const QString g_sConfig_receiverPort = "default/ReceiverPort";
static const QString g_sConfig_receiverIP = "default/ReceiverIP";
static const QString g_sConfig_intoFile = "default/IntroFile";

class ConfigForm : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigForm(QWidget *parent = 0);
    ~ConfigForm();

    QComboBox * getVoiceComboBox( void );

    void loadConfig( void );
    QVariant getValue(const QString &p_sValue );

    void logEvent(const QString & p_sLog );

signals:
    void signal_saySample( void );
    void signal_quitApp( void );

public slots:
    void slot_safeConfig( void );
    void slot_selectClipDir( void );
    void slot_selectIntroFile( void );

private:
    Ui::ConfigForm *ui;
    QSettings       m_grSettings;
};

#endif // CONFIGFORM_H
