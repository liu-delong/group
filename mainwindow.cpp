#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "person.h"
#include "mytableview.h"
#include "mainwindow.h"
#include<QToolTip>
#include<QDebug>
#include"defaultseperator.h"
#include<QVBoxLayout>
#include<QTextBrowser>
#include<QFileDialog>
#include<QDateTime>
#include<QTextStream>
#include<QStandardItemModel>
#include<QDir>
int MainWindow::get_table_index(QString type, int type_index)
{
    int cnt=0;
    auto table=ui->widget;
    auto model=table->model();
    for(int i=0;i<model->columnCount();i++)
    {
        if(model->headerData(i,Qt::Horizontal).toString().contains(type)) cnt++;
        if(cnt==type_index) return i;
    }
    return 0;
}

bool MainWindow::load_ming_dan(QString file_name)
{
    QFile file(file_name);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream fin(&file);
        int temp;
        QString temps;
        fin>>temp;
        ui->groupCntSpinBox->setValue(temp);

        fin>>temp;
        mutex_nums.clear();
        for(int i=0;i<temp;i++)
        {
            int t;
            fin>>t;
            mutex_nums.push_back(t);
        }
        fin>>temp;
        togher_nums.clear();
        for(int i=0;i<temp;i++)
        {
            int t;
            fin>>t;
            togher_nums.push_back(t);
        }
        int rowC,colC;
        fin>>rowC>>colC;
        auto table=ui->widget;
        auto model=(QStandardItemModel*)table->model();
        model->setRowCount(rowC);
        model->setColumnCount(colC);
        model->setHeaderData(0,Qt::Horizontal,"姓名");
        for(int i=1;i<colC;i++)
        {
            int t;
            fin>>t;
            if(t==1)
            {
                model->setHeaderData(i,Qt::Horizontal,"互斥");
            }
            else if(t==2)
            {
                model->setHeaderData(i,Qt::Horizontal,"绑定");
            }
        }
        for(int i=0;i<rowC;i++)
        {
            QString ts;
            fin>>ts;
            model->setData(model->index(i,0),ts);
            for(int j=1;j<rowC;j++)
            {
                int tn;
                fin>>tn;
                if(tn!=INT32_MAX) model->setData(model->index(i,j),tn);
            }
        }
        setcurrentMutex(0);
        setcurrentTogether(0);
        ui->mutexIndexSpinBox->setMaximum(mutex_nums.size());
        ui->togetherIndexspinBox->setMaximum(togher_nums.size());
    }
}

bool MainWindow::save_ming_dan(QString file_name)
{
    QFile file(file_name);

    // 打开文件
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
       QTextStream fout(&file);
       fout<<ui->groupCntSpinBox->value()<<endl;
       fout<<mutex_nums.size()<<endl;
       for(int i=0;i<mutex_nums.size();i++)
       {
           fout<<mutex_nums[i]<<" ";
       }
       fout<<endl;
       fout<<togher_nums.size()<<endl;
       for(int i=0;i<togher_nums.size();i++)
       {
           fout<<togher_nums[i]<<" ";
       }
       fout<<endl;
       auto table=ui->widget;
       auto model=table->model();
       fout<<model->rowCount()<<" "<<model->columnCount()<<endl;
       for(int i=1;i<model->columnCount();i++)
       {
           if(model->headerData(i,Qt::Horizontal).toString().contains("互斥"))
           {
               fout<<1<<" ";
           }
           else if(model->headerData(i,Qt::Horizontal).toString().contains("绑定"))
           {
               fout<<2<<" ";
           }

       }
       fout<<endl;
       for(int i=0;i<model->rowCount();i++)
       {
           fout<<model->data(model->index(i,0)).toString()<<"\t";
           for(int j=1;j<model->columnCount();j++)
           {
               if(table->getData(i,j).isNull())
               {
                   fout<<INT32_MAX<<"\t";
               }
               else
               {
                   fout<<table->getData(i,j).toInt()<<"\t";
               }
           }
           fout<<endl;
       }

    } else {
        // 文件打开失败
        qDebug() << "Failed to open file:" << file_name;

    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    connect(this,SIGNAL(currentMutexChanged(int)),this,SLOT(on_currentMutexChanged(int)));
    connect(this,SIGNAL(currentTogetherChanged(int)),this,SLOT(on_currentTogetherChanged(int)));
    connect(ui->widget,&QTableView::clicked,this,&MainWindow::on_choose_table_item);
    ui->togetherIndexspinBox->setMaximum(0);
    ui->togetherIndexspinBox->setMinimum(0);
    ui->mutexIndexSpinBox->setMaximum(0);
    ui->mutexIndexSpinBox->setMinimum(0);
    ui->togetherNumspinBox->setDisabled(true);
    ui->mutexNumSpinBox->setDisabled(true);
    //ui->label_mutex->setToolTip("允许在同一个互斥组的人被分到同一个组的人数");
    ui->groupCntSpinBox->setMinimum(1);
    QFileInfo info(QDir::currentPath()+"/默认名单.wfs");
    if(info.exists())
    {
        load_ming_dan(QDir::currentPath()+"/默认名单.wfs");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

vector<Person> MainWindow::getPersons()
{
    auto table=ui->widget;
    vector<Person> ret;
    for(int i=0;i<table->model()->rowCount();i++)
    {
        auto name=table->getData(i,0).toString();
        qDebug()<<name;
        vector<int> mutex_group;
        vector<int> together_group;
        for(int j=1;j<table->model()->columnCount();j++)
        {
            if(table->model()->headerData(j,Qt::Horizontal,Qt::DisplayRole).toString().contains("互斥"))
            {
                if(table->getData(i,j).isNull())
                {
                    mutex_group.push_back(INT32_MAX);
                }
                else
                {
                    mutex_group.push_back(table->getData(i,j).toInt());
                }
            }
            if(table->model()->headerData(j,Qt::Horizontal,Qt::DisplayRole).toString().contains("绑定"))
            {
                if(table->getData(i,j).isNull())
                {
                    together_group.push_back(INT32_MAX);
                }
                else
                {
                    together_group.push_back(table->getData(i,j).toInt());
                }
            }
        }
        ret.emplace_back(name,mutex_group,together_group);
    }
    return ret;

}

void MainWindow::on_currentMutexChanged(int newValue)
{
    ui->mutexIndexSpinBox->setValue(newValue);
    if(newValue==0)
    {
        ui->mutexNumSpinBox->setDisabled(true);
    }
    else
    {
        ui->mutexNumSpinBox->setDisabled(false);
        ui->mutexNumSpinBox->setValue(mutex_nums[newValue-1]);
        ui->mutexNumSpinBox->setMinimum(1);
     }
}

void MainWindow::on_currentTogetherChanged(int newValue)
{
    ui->togetherIndexspinBox->setValue(newValue);
    if(newValue==0)
    {
        ui->togetherNumspinBox->setDisabled(true);
    }
    else
    {
        ui->togetherNumspinBox->setDisabled(false);
        ui->togetherNumspinBox->setValue(togher_nums[newValue-1]);
        ui->togetherNumspinBox->setMinimum(0);
    }
}


void MainWindow::on_pushButton_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("*.txt");
    dialog.setDefaultSuffix("txt");
    dialog.selectFile("分组名单");
    if(dialog.exec())
    {
        auto file_name=dialog.selectedFiles().at(0);
        QFile file(file_name);

        // 打开文件
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            file.write((QString("记录于:")+QDateTime::currentDateTime().toString()+"\n").toUtf8());
            auto layout=ui->groupbox->layout();
            for(int i=0;i<layout->count();i++)
            {
                auto item=layout->itemAt(i);
                auto tb=(QTextBrowser*)item->widget();
                auto text=tb->toPlainText().replace("\n","\t")+"\n\n";
                file.write(text.toUtf8());
            }
        } else {
            // 文件打开失败
            qDebug() << "Failed to open file:" << file_name;
        }

    }
}


void MainWindow::on_pushButton_6_clicked()
{
    mutex_nums.push_back(1);
    ui->mutexIndexSpinBox->setMaximum(mutex_nums.size());
    ui->mutexIndexSpinBox->setValue(mutex_nums.size());
    auto table=ui->widget;
    auto model=table->model();
    model->insertColumn(model->columnCount());
    model->setHeaderData(model->columnCount()-1,Qt::Horizontal,"互斥");
}


void MainWindow::on_mutexIndexSpinBox_valueChanged(int arg1)
{
    setcurrentMutex(arg1);
}


void MainWindow::on_pushButton_5_clicked()
{
    togher_nums.push_back(0);
    ui->togetherIndexspinBox->setMaximum(togher_nums.size());
    ui->togetherIndexspinBox->setValue(togher_nums.size());
    auto table=ui->widget;
    auto model=table->model();
    model->insertColumn(model->columnCount());
    model->setHeaderData(model->columnCount()-1,Qt::Horizontal,"绑定");
}


void MainWindow::on_togetherIndexspinBox_valueChanged(int arg1)
{
    setcurrentTogether(arg1);
}


void MainWindow::on_mutexNumSpinBox_valueChanged(int arg1)
{
    mutex_nums[currentMutex()-1]=arg1;
}


void MainWindow::on_togetherNumspinBox_valueChanged(int arg1)
{
    togher_nums[currentTogether()-1]=arg1;
}

void MainWindow::on_choose_table_item(const QModelIndex &index)
{

    int column=index.column();
    auto table=ui->widget;
    int mutex_cnt=0;
    int together_cnt=0;
    for(int i=0;i<=column;i++)
    {
        auto column_name=table->model()->headerData(i,Qt::Horizontal).toString();
        if(column_name.contains("互斥")) mutex_cnt++;
        else if(column_name.contains("绑定")) together_cnt++;
    }
    auto column_name=table->model()->headerData(column,Qt::Horizontal).toString();
    if(column_name.contains("互斥"))
    {
        setcurrentMutex(mutex_cnt);
    }
    else if(column_name.contains("绑定"))
    {
        setcurrentTogether(together_cnt);
    }
}



void MainWindow::on_pushButton_7_clicked()
{
    auto table=ui->widget;
    auto model=table->model();
    int mutex_index=currentMutex();
    int table_index=get_table_index("互斥",mutex_index);
    if(table_index>0)
    {
        model->removeColumn(table_index);
    }
    mutex_nums.erase(mutex_nums.begin()+mutex_index-1);
    if(currentMutex()>mutex_nums.size())
    {
        setcurrentMutex(mutex_nums.size());
    }
    ui->mutexIndexSpinBox->setMaximum(mutex_nums.size());

}


void MainWindow::on_pushButton_8_clicked()
{
    auto table=ui->widget;
    auto model=table->model();
    int together_index=currentTogether();
    int table_index=get_table_index("绑定",together_index);
    if(table_index>0)
    {
        model->removeColumn(table_index);
    }
    togher_nums.erase(togher_nums.begin()+together_index-1);
    if(currentMutex()>togher_nums.size())
    {
        setcurrentMutex(togher_nums.size());
    }
    ui->mutexIndexSpinBox->setMaximum(togher_nums.size());
}


void MainWindow::on_pushButton_2_clicked()
{
    auto personList=getPersons();
    auto serpeator=DefaultSeperator(mutex_nums,togher_nums);
    auto groups=serpeator.seperate(personList,ui->groupCntSpinBox->value());
    auto layout=new QVBoxLayout();
    for(auto& group:groups)
    {
        qDebug()<<"--------------------";
        QString namelist;
        int i=0;
        for(auto& person:group)
        {
            qDebug()<<person._name;
            namelist+=person._name;
            if(i%4==3) namelist+="\n";
            else namelist+="\t";
            i++;
        }
        auto tb=new QTextBrowser();
        tb->setText(namelist);
        QFont font("Arial", 20);
        tb->setFont(font);
        layout->addWidget(tb);
    }

    auto groupBox=ui->groupbox;
    auto l=groupBox->layout();
    delete l;

    groupBox->setLayout(layout);


}


void MainWindow::on_toolButton_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("*.wfs");
    dialog.setDefaultSuffix("wfs");
    dialog.selectFile("待分组人员名单");
    if(dialog.exec())
    {
        auto file_name=dialog.selectedFiles().at(0);
        save_ming_dan(file_name);
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("*.wfs");
    if(dialog.exec())
    {
        auto file_name=dialog.selectedFiles().at(0);
        load_ming_dan(file_name);
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    save_ming_dan(QDir::currentPath()+"/"+"默认名单.wfs");
}

