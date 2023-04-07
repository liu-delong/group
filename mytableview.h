#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H

#include <QWidget>
#include<QTableView>

class MyTableView : public QTableView
{
    Q_OBJECT
public:
    explicit MyTableView(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    QVariant getData(int row,int column);
signals:

};

#endif // MYTABLEVIEW_H
