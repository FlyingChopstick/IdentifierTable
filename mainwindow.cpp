#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include <QFileDialog>
#include <QMessageBox>

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
    bool fileIsRead = ReadFile(&filename);

    if (fileIsRead)
    {
        MainWindow::ui->tb_selectedFile->append(QFileInfo(filename).fileName());
        UpdateIdentifierDisplay();
    }
    else
    {
        QMessageBox mb;
        mb.setText("Ошибка при открытии файла.");
        mb.setStandardButtons(QMessageBox::Ok);
        mb.exec();
    }
}

bool MainWindow::ReadFile(QString *filename)
{
    QFile inputFile(*filename);
    //if the file is opened
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
        return true;
    }
    //if file could not be opened
    else
    {
        return false;
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

bool MainWindow::Query(QString *identifier)
{
    std::string ideStr = identifier->toStdString();
    _hashTable.resetStats();
    _sortedList.resetStats();

    _searchCount++;
    bool r_ht = _hashTable.contains(&ideStr);
    bool r_sl = _sortedList.contains(ideStr);

    return r_ht && r_sl;
}

void MainWindow::UpdateStats()
{
    //_ht_lastHashComp = _hashTable.getKeyCompares();
    _ht_lastValComp = _hashTable.getValCompares();

    _sl_lastValComp = _sortedList.getValCompares();

    //_ht_hashSum+=_ht_lastHashComp;
    _ht_valSum+=_ht_lastValComp;
    _sl_sum+=_sl_lastValComp;

    //_ht_hashMean = _ht_hashSum/_searchCount;
    _ht_valMean = _ht_valSum/_searchCount;
    _sl_mean = _sl_sum/_searchCount;
}

void MainWindow::ResetStats()
{
    _searchCount = 0;
    _ht_lastValComp = 0;
    _sl_lastValComp = 0;
    _ht_valSum = 0.0f;
    _sl_sum = 0.0f;
    _ht_valMean = 0.0f;
    _sl_mean = 0.0f;
}

void MainWindow::DisplayStats()
{
    MainWindow::ui->l_ht_valComp->setText("Сравнений: " + QString::number(_ht_lastValComp));
    MainWindow::ui->l_ht_mean->setText("В среднем сравнений на элемент: " + QString::number(_ht_valMean));
    MainWindow::ui->l_ht_totalComp->setText("Всего сравнений: " + QString::number(_ht_valSum));


    MainWindow::ui->l_sl_valComp->setText("Сравнений: " + QString::number(_sl_lastValComp));
    MainWindow::ui->l_sl_mean->setText("В среднем сравнений на элемент: " + QString::number(_sl_mean));
    MainWindow::ui->l_sl_totalComp->setText("Всего сравнений: " + QString::number(_sl_sum));

    MainWindow::ui->l_searchCount->setText("Поиск проводлися "+ QString::number(_searchCount) +" раз");
}

void MainWindow::on_b_search_clicked()
{
    QString identifier = MainWindow::ui->tb_identifierQuery->text();
    //empty string check
    if (identifier == "")
    {
        QMessageBox mb;
        mb.setText("Введите идентификатор.");
        mb.setStandardButtons(QMessageBox::Ok);
        mb.exec();
        return;
    }
    bool exists = Query(&identifier);
    //if the identifier was not found, show message and return
    if (!exists)
    {
        HighLightIdentifierSearch(true, false);
        QMessageBox mb;
        mb.setText("Идентификатор не найден.");
        mb.setStandardButtons(QMessageBox::Ok);
        mb.exec();
        return;
    }
    HighLightIdentifierSearch(true, true);
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

void MainWindow::HighLightIdentifierSearch(bool highlight, bool isFound)
{
    if (highlight)
    {
        if (isFound)
        {
            //green background
            MainWindow::ui->tb_identifierQuery->setStyleSheet(
                        "QLineEdit { background: rgb(133, 240, 127); selection-background-color: rgb(233, 99, 0); }");
            MainWindow::ui->tb_identifierQuery->setToolTip("Идентификатор найден");
        }
        else
        {
            //red background
            MainWindow::ui->tb_identifierQuery->setStyleSheet(
                        "QLineEdit { background: rgb(240, 144, 127); selection-background-color: rgb(233, 99, 0); }");
            MainWindow::ui->tb_identifierQuery->setToolTip("Идентификатор не найден");
        }
    }
    else
    {
        //remove highlight and tooltip
        MainWindow::ui->tb_identifierQuery->setStyleSheet(
                    "QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(233, 99, 0); }");
        MainWindow::ui->tb_identifierQuery->setToolTip("");
    }
}

void MainWindow::on_tb_identifierQuery_textChanged(const QString &arg1)
{
    HighLightIdentifierSearch(false);
}

