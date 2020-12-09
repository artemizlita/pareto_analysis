#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Qt>

QVector<int> faults_raiting, number;
QVector<float> percents_raiting, sum_percents_raiting;
QVector<QString> name;
int i20, colums;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    k = 0;
    new_table();
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(slotAction()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(slotAction()));
    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(slotAction()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(slotAction()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotAction() {
    if (sender() == ui->actionNew) {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Pareto"), tr("Введите число причин сбоя:"), QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
            k = text.toInt();
        }
        new_table();
    }
    if (sender() == ui->actionSave) {
        QString fName = QFileDialog::getSaveFileName(this, tr("Save File"), "./", tr("matr(*.txt)"));
        QString suf = QFileInfo(fName).suffix();
        if (suf.isEmpty()) fName.append(".txt");
        QFile file(fName);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug()<<"File not open";
        } else {
            QTextStream out(&file);
            out<<k;
            for (int i = 0; i < k; ++i) {
                QString reason = ui->tableWidget->item(i, 0)->text();
                reason.remove("\n");
                QString count = ui->tableWidget->item(i, 1)->text();
                count.remove("\n");
                out<<endl<<reason<<endl<<count;
            }
            file.close ();
        }
    }
    if (sender() == ui->actionLoad) {
        QString fName = QFileDialog::getOpenFileName(this, tr("Open File"), "./", tr("matr(*.txt)"));
        if (fName != "") {
            QString suf = QFileInfo(fName).suffix();
            if (suf.isEmpty()) fName.append(".txt");
            QFile file(fName);
            if (!file.open(QIODevice::ReadOnly)) {
                qDebug()<<"File not open";
            } else {
                QString s = file.readLine();
                k = s.toInt();
                new_table();
                int c = 0;
                while (!file.atEnd()) {
                    QString reason, count;
                    QTableWidgetItem *item1, *item2;

                    reason = file.readLine();
                    item1 = new QTableWidgetItem;
                    item1->setText(reason);
                    ui->tableWidget->setItem(c, 0, item1);

                    count = file.readLine();
                    item2 = new QTableWidgetItem;
                    item2->setText(count);
                    ui->tableWidget->setItem(c, 1, item2);

                    ++c;
                }
                file.close ();
            }
        }
    }
    if (sender() == ui->actionExit) {
        qApp->quit();
    }
}

void MainWindow::new_table() {
    ui->tableWidget->clear();
    ui->tableWidget_2->clear();
    ui->tableWidget_3->clear();
    ui->tableWidget_3->setColumnCount(0);
    ui->tableWidget_3->setRowCount(0);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(k);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Fault"<<"Count");
    ui->tableWidget_2->setColumnCount(4);
    ui->tableWidget_2->setRowCount(k);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()<<"Fault"<<"Count"<<"%"<<"E%");
    QStringList empty;
    for (int i = 0; i < k; ++i) {
        empty<<"";
        QTableWidgetItem *item1, *item2;
        item1 = new QTableWidgetItem;
        item1->setText("");
        ui->tableWidget->setItem(i, 0, item1);

        item2 = new QTableWidgetItem;
        item2->setText("");
        ui->tableWidget->setItem(i, 1, item2);
    }
    ui->tableWidget->setColumnWidth(1, 45);
    ui->tableWidget_2->setVerticalHeaderLabels(empty);
    ui->tableWidget_2->setColumnWidth(1, 45);
    ui->tableWidget_2->setColumnWidth(2, 40);
    ui->tableWidget_2->setColumnWidth(3, 40);
    ui->pushButton_2->setEnabled(0);
    ui->pushButton_3->setEnabled(0);
    ui->pushButton_4->setEnabled(0);
}

void MainWindow::on_pushButton_clicked()
{
    bool ok = 1;
    for (int i = 0; i < k; ++i) {
        bool local_ok = 1;
        ui->tableWidget->item(i, 1)->text().toInt(&local_ok);
        if (!local_ok) {
            ok = 0;
        } else {
            int test_int = ui->tableWidget->item(i, 1)->text().toInt(&local_ok);
            if (test_int <= 0) {
                ok = 0;
            }
        }
    }
    if ((ok) && (k > 0)) {
    faults_raiting.clear();
    number.clear();
    percents_raiting.clear();
    sum_percents_raiting.clear();
    name.clear();
    int sum_count = 0, c = 0;
    QVector<int> number_of_reason, count_of_faults;
    QVector<QString> name_of_number;
    for (int i = 0; i < k; ++i) {
        number_of_reason.push_back(i + 1);
        name_of_number.push_back(ui->tableWidget->item(i, 0)->text());
        count_of_faults.push_back(ui->tableWidget->item(i, 1)->text().toInt());
    }
    while (number_of_reason.size() > 0) {
        int max = 0, id = 0;
        for (int i = 0; i < number_of_reason.size(); ++i) {
            if (count_of_faults[i] > max) {
                max = count_of_faults[i];
                id = i;
            }
        }

        sum_count += count_of_faults[id];
        QTableWidgetItem *item1, *item2, *item3;

        item1 = new QTableWidgetItem;
        item1->setText(QString().setNum(number_of_reason[id]));
        ui->tableWidget_2->setVerticalHeaderItem(c, item1);
        number.push_back(number_of_reason[id]);

        item2 = new QTableWidgetItem;
        item2->setText(name_of_number[id]);
        ui->tableWidget_2->setItem(c, 0, item2);
        name.push_back(name_of_number[id]);

        item3 = new QTableWidgetItem;
        item3->setText(QString().setNum(count_of_faults[id]));
        ui->tableWidget_2->setItem(c, 1, item3);
        faults_raiting.push_back(count_of_faults[id]);

        ++c;
        number_of_reason.erase(number_of_reason.begin() + id);
        name_of_number.erase(name_of_number.begin() + id);
        count_of_faults.erase(count_of_faults.begin() + id);
    }

    bool ok_i20 = 0;
    for (int i = 0; i < k; ++i) {
        QTableWidgetItem *item3, *item4;
        float percent = ui->tableWidget_2->item(i, 1)->text().toFloat() / sum_count * 100;

        item3 = new QTableWidgetItem;
        item3->setText(QString().setNum(percent, 'f', 2));
        ui->tableWidget_2->setItem(i, 2, item3);
        percents_raiting.push_back(percent);

        item4 = new QTableWidgetItem;
        item4->setText(QString().setNum((i == 0) ? percent : (i == k - 1) ? 100 : ui->tableWidget_2->item(i - 1, 3)->text().toFloat() + percent, 'f', 2));
        ui->tableWidget_2->setItem(i, 3, item4);
        sum_percents_raiting.push_back((i == 0) ? percent : ui->tableWidget_2->item(i - 1, 3)->text().toFloat() + percent);
        if (sum_percents_raiting[i] > 80 && !ok_i20) {
            i20 = i;
            ok_i20 = 1;
        }
    }
    ui->pushButton_2->setEnabled(1);
    ui->pushButton_4->setEnabled(1);
    } else {
        QMessageBox::critical(NULL,QObject::tr("Неправильно введены данные"),tr("-Количество сбоев - целое число > 0\n-Количество причин сбоя - также целое число > 0"));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if (name.size() < 10) {
        colums = name.size() - 1;
    } else {
        colums = i20;
    }
    MainWindow2 d;
    d.setWindowTitle("Graph");
    d.setFixedSize(std::min((colums + 1) * 100 + 75, 975), 650);
    d.exec();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->tableWidget_3->setColumnCount(2);
    ui->tableWidget_3->setHorizontalHeaderLabels(QStringList()<<"Fault"<<"Count");

    int i = 0;
    while (sum_percents_raiting[i] < 80) {
        ui->tableWidget_3->setRowCount(i + 1);
        QTableWidgetItem *item1, *item2, *item3;

        item1 = new QTableWidgetItem;
        item1->setText(QString().setNum(number[i]));
        ui->tableWidget_3->setVerticalHeaderItem(i, item1);

        item2 = new QTableWidgetItem;
        item2->setText(name[i]);
        ui->tableWidget_3->setItem(i, 0, item2);

        item3 = new QTableWidgetItem;
        item3->setText(QString().setNum(faults_raiting[i]));
        ui->tableWidget_3->setItem(i, 1, item3);

        ++i;
    }
    ui->pushButton_3->setEnabled(1);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "./", tr("matr(*.csv)"));
    QString suf = QFileInfo(filename).suffix();
    if (suf.isEmpty()) filename.append(".csv");
    QFile f(filename);
    if(f.open(QIODevice::WriteOnly)) {
            QTextStream ts( &f );
            QStringList strList;

            strList<<"\" \"";

            for(int c = 0; c < ui->tableWidget->horizontalHeader()->count(); ++c)
                strList<<"=\"" + ui->tableWidget->model()->headerData(c, Qt::Horizontal).toString()+"\"";

            strList<<"\" \"";
            strList<<"\" \"";

            for(int c = 0; c < ui->tableWidget_2->horizontalHeader()->count(); ++c)
                strList<<"=\"" + ui->tableWidget_2->model()->headerData(c, Qt::Horizontal).toString()+"\"";

            strList<<"\" \"";
            strList<<"\" \"";

            for(int c = 0; c < ui->tableWidget_3->horizontalHeader()->count(); ++c)
                strList<<"=\"" + ui->tableWidget_3->model()->headerData(c, Qt::Horizontal).toString()+"\"";

            ts<<strList.join( ";" )+"\n";
            for(int r = 0; r < ui->tableWidget->verticalHeader()->count(); ++r)
            {
                strList.clear();
                strList<<"=\"" + ui->tableWidget->model()->headerData(r, Qt::Vertical).toString() + "\"";
                for( int c = 0; c < ui->tableWidget->horizontalHeader()->count(); ++c )
                {
                    strList<<"=\"" + ui->tableWidget->model()->data(ui->tableWidget->model()->index(r, c), Qt::DisplayRole).toString() + "\"";
                }

                strList<<"\" \"";

                strList<<"=\"" + ui->tableWidget_2->model()->headerData(r, Qt::Vertical).toString() + "\"";
                for( int c = 0; c < ui->tableWidget_2->horizontalHeader()->count(); ++c )
                {
                    strList<<"=\"" + ui->tableWidget_2->model()->data(ui->tableWidget_2->model()->index(r, c), Qt::DisplayRole).toString() + "\"";
                }

                strList<<"\" \"";

                if (r < ui->tableWidget_3->rowCount()) {
                    strList<<"=\"" + ui->tableWidget_3->model()->headerData(r, Qt::Vertical).toString() + "\"";
                    for( int c = 0; c < ui->tableWidget_3->horizontalHeader()->count(); ++c )
                    {
                        strList<<"=\"" + ui->tableWidget_3->model()->data(ui->tableWidget_3->model()->index(r, c), Qt::DisplayRole).toString() + "\"";
                    }
                }

                ts<<strList.join(";") + "\n";
            }

            strList<<"\n\n\" \"";

            f.close();
    }
}

MainWindow2::MainWindow2(QWidget *parent) :
    QDialog(parent)
{
    setGeometry(100, 100, std::min((colums + 1) * 100 + 75, 975), 650);
    if (colums > 8) {
        shift = 900 / (colums + 1);
    } else {
        shift = 100;
    }
    if (i20 == colums) {
        for (int i = i20 + 2; i < percents_raiting.size(); ++i) {
            percents_raiting[i20] += percents_raiting[i];
        }
        sum_percents_raiting[i20] = 100;
    }

    MainWindow3 *d2 = new MainWindow3(this);
    d2->setWindowTitle("Legend");
    d2->setFixedSize(150, 300);
    d2->show();

    repaint();
    QLabel *graph_name;
    graph_name = new QLabel(this);
    graph_name->setGeometry(std::min((colums + 1) * 100 + 100, 1000) / 2 - 100, 30, 200, 30);
    graph_name->setStyleSheet("font-size: 30px");
    graph_name->setText("Pareto chart");
    for (int i = 0; i < 11; ++i) {
        QLabel *L_percent;
        L_percent = new QLabel(this);
        L_percents.push_back(L_percent);
        L_percents[i]->setGeometry((i == 10) ? 5 : (i == 0) ? 24 : 14, 590 - 50 * i, 50, 20);
        L_percents[i]->setStyleSheet("font-size: 16px; color: gray");
        L_percents[i]->setText(QString().setNum(i * 10)+"%");
    }
    for (int i = 0; i < colums + 1; ++i) {
        QLabel *L_fault;
        L_fault = new QLabel(this);
        L_faults.push_back(L_fault);
        L_faults[i]->setGeometry(50 + (i + 1) * shift - shift * 3 / 4, 605, 100, 25);
        L_faults[i]->setStyleSheet("font-size: 16px");
        L_faults[i]->setText((i == i20 && colums == i20) ? "misc" : QString().setNum(i + 1));
    }
    for (int i = 0; i < colums + 1; ++i) {
        QLabel *L_perc;
        L_perc = new QLabel(this);
        L_percs.push_back(L_perc);
        L_percs[i]->setGeometry(50 + (i + 1) * shift - shift * 3 / 4, 580 - percents_raiting[i] * 5, 50, 20);
        L_percs[i]->setStyleSheet("font-size: 12px; color: darkRed");
        L_percs[i]->setText(QString().setNum(percents_raiting[i], 'f', 2)+"%");
    }
    for (int i = 0; i < colums; ++i) {
        QLabel *L_sum_perc;
        L_sum_perc = new QLabel(this);
        L_sum_percs.push_back(L_sum_perc);
        L_sum_percs[i]->setGeometry((sum_percents_raiting[i + 1] < 10 ? 15 : 10) + (i + 2) * shift - shift * 1 / 2, 575 - sum_percents_raiting[i + 1] * 5, 50, 20);
        L_sum_percs[i]->setText(QString().setNum(sum_percents_raiting[i + 1], 'f', 2)+"%");
    }
}

void MainWindow2::paintEvent(QPaintEvent *event) {
    begin(this);
    setPen(QPen(Qt::lightGray));
    setBrush(QBrush(Qt::white));
    drawRect(-1, -1, (colums + 1) * 100 + 101, 651);
    for (int i = -1; i < 10; ++i) {
        drawLine(50, 600 - 50 * (i + 1), std::min((colums + 1) * 100 + 50, 950), 600 - 50 * (i + 1));
    }
    bool p80 = 0;
    for (int i = 0; i < colums + 1; ++i) {
        if (i >= i20) {
            setPen(QPen(Qt::lightGray));
            setBrush(QBrush(Qt::lightGray));
        } else {
            setPen(QPen(Qt::blue));
            setBrush(QBrush(Qt::blue));
        }
        drawRect(50 + (i + 1) * shift - shift * 3 / 4, 600, shift / 2, -percents_raiting[i] * 5);
        if (i > 0) {
            setPen(QPen(QColor(240, 240, 0), 3));
            drawLine(50 + i * shift - shift / 2, 600 - sum_percents_raiting[i - 1] * 5, 50 + (i + 1) * shift - shift / 2, 600-sum_percents_raiting[i] * 5);
            setPen(QPen(Qt::black, 2));
            drawLine(50 + (i + 1) * shift - shift / 2, 600 - sum_percents_raiting[i] * 5, 45 + (i + 1) * shift - shift / 2, 595 - sum_percents_raiting[i] * 5);
        }
        if (sum_percents_raiting[i] > 80 && !p80) {
            setPen(QPen(Qt::red, 1, Qt::DashLine));
            int delta = 50 + i * shift - shift / 2 +
                    (80 - sum_percents_raiting[i - 1]) / (sum_percents_raiting[i] - sum_percents_raiting[i - 1]) * shift;
            drawLine(50, 200, delta - 1, 200);
            drawLine(delta - 1, 200, delta - 1, 600);
            p80 = 1;
        }
    }
    end();
}

MainWindow2::~MainWindow2()
{

}

MainWindow3::MainWindow3(QWidget *parent) :
    QDialog(parent)
{
    setGeometry(1100, 300, 150, 300);
    Tab_legend = new QTableWidget(name.size(), 1, this);
    Tab_legend->setGeometry(0, 0, 150, 300);
    Tab_legend->setEditTriggers(QAbstractItemView::EditTriggers(0));
    Tab_legend->setHorizontalHeaderLabels(QStringList()<<"Fault");
    for (int i = 0; i < name.size(); ++i) {
        QTableWidgetItem *item;

        item = new QTableWidgetItem;
        item->setText(name[i]);
        Tab_legend->setItem(i, 0, item);
        if (i < i20) {
            Tab_legend->item(i, 0)->setTextColor(Qt::blue);
        } else {
            Tab_legend->item(i, 0)->setTextColor(Qt::darkGray);
        }
    }
}

MainWindow3::~MainWindow3()
{

}
