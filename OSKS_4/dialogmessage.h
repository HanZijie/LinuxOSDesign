#ifndef DIALOGMESSAGE_H
#define DIALOGMESSAGE_H

#include <QDialog>

namespace Ui {
class DialogMessage;
}

class DialogMessage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMessage(QWidget *parent = 0);
    ~DialogMessage();

private:
    Ui::DialogMessage *ui;
};

#endif // DIALOGMESSAGE_H
