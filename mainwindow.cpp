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

//Button - open file
void MainWindow::on_b_selectFile_clicked()
{
    //Get filename from OpenFileDialog
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Select source file"),
        "E:\\QT\\Projects\\IdentifierTable\\input",
        tr("Text files(*.txt)"));


    ClearBuffers();
    bool fileIsRead = ReadFile(&filename);

    //if file was read successfully
    if (fileIsRead)
    {
        //write the selected file name
        MainWindow::ui->tb_selectedFile->append(QFileInfo(filename).fileName());
        //update identifier list
        UpdateIdentifierDisplay();
    }
    //if error occured
    else
    {
        //Display error message
        QMessageBox mb;
        mb.setText("Ошибка при открытии файла.");
        mb.setStandardButtons(QMessageBox::Ok);
        mb.exec();
    }
}

//Reads the provided file and fills the list and the hash table
bool MainWindow::ReadFile(QString *filename)
{
    //input file
    QFile inputFile(*filename);
    //if the file is opened
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        //read the file
        while (!in.atEnd())
        {
            //current line
            QString qLine = in.readLine();
            std::string line = qLine.toStdString();

            //Добавление в структуры
            _sortedList.add(line);
            _hashTable.add(&line);

            //Список идентификаторов - для отображения в окне
            _identifiers.append(qLine);
        }
        //ToggleControls(false);
        return true;
    }
    //if file could not be opened
    else
    {
        return false;
    }
}

//Update the identifier list
void MainWindow::UpdateIdentifierDisplay()
{
    //for each indentifier in list
    for (unsigned int i = 0; i < _identifiers.length(); i++ )
    {
        QString line = _identifiers[i];
        //add the identifier in the list
        MainWindow::ui->tb_identifiers->append(line);
    }
}

//Search the structures for identfier
bool MainWindow::Query(QString *identifier)
{    
    std::string ideStr = identifier->toStdString();
    //clear structure statistics
    _hashTable.resetStats();
    _sortedList.resetStats();

    _searchCount++;
    //query ht
    bool r_ht = _hashTable.contains(&ideStr);
    //query sl
    bool r_sl = _sortedList.contains(ideStr);

    //if found in both, return true
    //maybe throw if found in only one?
    return r_ht && r_sl;
}

//Updates statistics
void MainWindow::UpdateStats()
{
    //get compare count from ht
    _ht_lastValComp = _hashTable.getValCompares();
    //get compare count from sl
    _sl_lastValComp = _sortedList.getValCompares();

    //sum the compare values
    _ht_valSum+=_ht_lastValComp;
    _sl_sum+=_sl_lastValComp;

    //calculate average
    _ht_valMean = _ht_valSum/_searchCount;
    _sl_mean = _sl_sum/_searchCount;
}

//Clear statistics
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

//Update statistics fields
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

//Button - search
void MainWindow::on_b_search_clicked()
{
    //get identfier from text box
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

//Clear form fields
void MainWindow::ClearBuffers()
{
    MainWindow::ui->tb_identifiers->clear();
    MainWindow::ui->tb_selectedFile->clear();
    _identifiers.clear();
}

//Button - reset
void MainWindow::on_b_statReset_clicked()
{
    ResetStats();
    DisplayStats();
}

//Disable Search and Reset buttons, identifier search input
void MainWindow::ToggleControls(bool disable)
{
    MainWindow::ui->tb_identifierQuery->setDisabled(disable);
    MainWindow::ui->b_search->setDisabled(disable);
    MainWindow::ui->b_statReset->setDisabled(disable);
}

//Highlight the form fields
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

//Reset identifier search field colour
void MainWindow::on_tb_identifierQuery_textChanged(const QString &arg1)
{
    HighLightIdentifierSearch(false);
}

