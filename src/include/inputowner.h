#ifndef INPUTOWNER_H
#define INPUTOWNER_H

#include <QWidget>

namespace Ui {
class InputOwner;
}

class InputOwner : public QWidget
{
    Q_OBJECT

public:
    explicit InputOwner(QWidget *parent = nullptr);
    ~InputOwner();

public slots:
    void getReadyToSend();

public: signals:
    void sentOwner(std::pair<QString, QString>);

private:
    Ui::InputOwner *ui;
    std::pair<QString, QString> makeOwner;
};

#endif // INPUTOWNER_H
