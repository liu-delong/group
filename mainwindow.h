#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<vector>
#include<person.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(int currentMutex READ currentMutex WRITE setcurrentMutex NOTIFY currentMutexChanged)
    Q_PROPERTY(int currentTogether READ currentTogether WRITE setcurrentTogether NOTIFY currentTogetherChanged)
    int _currentMutex=0;
    int _currentTogether=0;
    vector<int> mutex_nums;
    vector<int> togher_nums;
    int get_table_index(QString type,int type_index);
    bool load_ming_dan(QString file_name);
    bool save_ming_dan(QString file_name);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    vector<Person> getPersons();
    int currentMutex(){return _currentMutex;}
    void setcurrentMutex(int value){if(value!=_currentMutex){_currentMutex=value;emit currentMutexChanged(value);}}
    int currentTogether(){return _currentTogether;}
    void setcurrentTogether(int value){if(value!=_currentTogether){_currentTogether=value;emit currentTogetherChanged(value);}}


private slots:
    void on_currentMutexChanged(int newValue);
    void on_currentTogetherChanged(int newValue);
    void on_pushButton_clicked();
    void on_pushButton_6_clicked();

    void on_mutexIndexSpinBox_valueChanged(int arg1);

    void on_pushButton_5_clicked();

    void on_togetherIndexspinBox_valueChanged(int arg1);

    void on_mutexNumSpinBox_valueChanged(int arg1);

    void on_togetherNumspinBox_valueChanged(int arg1);

    void on_choose_table_item(const QModelIndex &index);

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_2_clicked();

    void on_toolButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

signals:
    void currentMutexChanged(int newValue);
    void currentTogetherChanged(int newValue);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
