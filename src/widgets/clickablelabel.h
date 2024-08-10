#ifndef __CLICKABLE_LABEL__
#define __CLICKABLE_LABEL__

#include <QLabel>
#include <QFont>

template <typename QClass>
class ClickableLabel: public QLabel
{
public:
    explicit ClickableLabel(const QString& label, void (QClass::*onPressAction)(void), QClass* parent)
    : QLabel(parent)
    , _parent(parent)
    , func(onPressAction)
    {
        QFont defaultFont("Segou UI", 24);
        this->setText(label);
        this->setFont(defaultFont);
    }
    ~ClickableLabel() { }

private:
    void (QClass::*func)(void);
    QClass* _parent;

protected:
    void mousePressEvent(QMouseEvent* event) override 
    {
        if (event->button() == Qt::LeftButton && _parent && func) 
        {
            (_parent->*func)();
        }
        QLabel::mousePressEvent(event);
    }
};

#endif // __CLICKABLE_LABEL__