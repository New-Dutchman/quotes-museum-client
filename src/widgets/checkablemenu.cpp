#include "checkablemenu.h"

#include <QVBoxLayout>
#include <QMenu>
#include <QToolButton>

CheckableMenu::CheckableMenu(const QString& title, const QStringList* options, QWidget *parent) 
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QToolButton *toolButton = new QToolButton(this);
    toolButton->setText(title);
    toolButton->setPopupMode(QToolButton::InstantPopup);

    _menu = new QMenu(this);

    for(auto option: *options)
    {
        QAction *option1 = _menu->addAction(option);
        option1->setCheckable(true);
        QObject::connect(option1, QAction::toggled, this, &CheckableMenu::onClickedOption);
    }

    toolButton->setMenu(_menu);

    layout->addWidget(toolButton);
}

CheckableMenu::~CheckableMenu() 
{
    delete _menu;
}

void CheckableMenu::onClickedOption(bool checked)
{
    if (checked)
    {
        QAction *action = qobject_cast<QAction*>(sender());
        emit actionChosen(action->text());
        return;
    }
    QAction *action = qobject_cast<QAction*>(sender());
    emit actionChosenOff(action->text());
}

void CheckableMenu::uncheck(const QString& title)
{
    foreach (QAction* a, _menu->actions())
    {
        if (a->text() == title)
        {
            a->setChecked(false);
            return;
        }
    }
}