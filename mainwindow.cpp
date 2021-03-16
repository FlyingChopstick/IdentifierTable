#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ResetStats();

    ui->setupUi(this);
    ToggleControls(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_b_selectFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Select source file"),
        "E:\\QT\\Projects\\IdentifierTable\\input",
        tr("Text files(*.txt)"));

    ClearBuffers();
    MainWindow::ui->tb_selectedFile->append(QFileInfo(filename).fileName());
    ReadFile(&filename);    
    UpdateIdentifierDisplay();
}

void MainWindow::ReadFile(QString *filename)
{
    QFile inputFile(*filename);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString qLine = in.readLine();
            std::string line = qLine.toStdString();

            _sortedList.add(line);
            _hashTable.add(&line);
            _identifiers.append(qLine);
        }
        ToggleControls(false);
    }
    else
    {
        //TODO
        //Display error message
        return;
    }
}

void MainWindow::UpdateIdentifierDisplay()
{
    for (unsigned int i = 0; i < _identifiers.length(); i++ )
    {
        QString line = _identifiers[i];
        MainWindow::ui->tb_identifiers->append(line);
    }
}

void MainWindow::Query(QString *identifier)
{
    std::string ideStr = identifier->toStdString();
    _hashTable.resetStats();
    _sortedList.resetStats();

    _searchCount++;
    _hashTable.contains(&ideStr);
    _sortedList.contains(ideStr);
}

void MainWindow::UpdateStats()
{
    _ht_lastHashComp = _hashTable.getKeyCompares();
    _ht_lastValComp = _hashTable.getValCompares();

    _sl_lastValComp = _sortedList.getValCompares();

    _ht_hashSum+=_ht_lastHashComp;
    _ht_valSum+=_ht_lastValComp;
    _sl_sum+=_sl_lastValComp;

    _ht_hashMean = _ht_hashSum/_searchCount;
    _ht_valMean = _ht_valSum/_searchCount;

    _sl_mean = (float)_sl_sum/_searchCount;
}

void MainWindow::ResetStats()
{
    _searchCount = 0;

    _ht_lastHashComp = 0;
    _ht_lastValComp = 0;

    _sl_lastValComp = 0;

    _ht_hashSum = 0.0f;
    _ht_valSum = 0.0f;
    _sl_sum = 0.0f;

    _ht_hashMean = 0.0f;
    _ht_valMean = 0.0f;

    _sl_mean = 0.0f;
}

void MainWindow::DisplayStats()
{
    MainWindow::ui->l_ht_hashComp->setText("Сравнений хэша: " + QString::number(_ht_lastHashComp));
    MainWindow::ui->l_ht_valComp->setText("Сравнений значения: " + QString::number(_ht_lastValComp));
    MainWindow::ui->l_ht_mean_hash->setText("Хэш: " + QString::number(_ht_hashMean));
    MainWindow::ui->l_ht_mean_val->setText("Значение: " + QString::number(_ht_valMean));

    MainWindow::ui->l_sl_valComp->setText("Сравнений значения: " + QString::number(_sl_lastValComp));
    MainWindow::ui->l_sl_mean_val->setText("Значение: " + QString::number(_sl_mean));
}

void MainWindow::on_b_search_clicked()
{
    QString identifier = MainWindow::ui->tb_identifierQuery->text();
    if (identifier == "")
    {
        //TODO
        //Display error message
        return;
    }
    Query(&identifier);
    UpdateStats();
    DisplayStats();
}

void MainWindow::ClearBuffers()
{
    MainWindow::ui->tb_identifiers->clear();
    MainWindow::ui->tb_selectedFile->clear();
    _identifiers.clear();
}

void MainWindow::on_b_statReset_clicked()
{
    ResetStats();
    DisplayStats();
}

void MainWindow::ToggleControls(bool disable)
{
    MainWindow::ui->tb_identifierQuery->setDisabled(disable);
    MainWindow::ui->b_search->setDisabled(disable);
    MainWindow::ui->b_statReset->setDisabled(disable);
}
