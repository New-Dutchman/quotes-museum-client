#ifndef __CHECKABLE_MENU__
#define __CHECKABLE_MENU__

#include <QWidget>

class CheckableMenu: public QWidget
{
    Q_OBJECT

    QMenu* _menu;
    
public:
    CheckableMenu(const QString& title, const QStringList* options, QWidget *parent);
    ~CheckableMenu();

signals:
    void actionChosen(const QString& action);
    void actionChosenOff(const QString& action);

private slots:
    void onClickedOption(bool);

public slots:
    void uncheck(const QString& title);

};

#endif // __CHECKABLE_MENU__