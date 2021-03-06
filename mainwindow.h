#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hashTable.h"
#include "sortedDynArray.h"
#include "analyser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_b_selectFile_clicked();

    void on_b_search_clicked();

    void on_b_statReset_clicked();

    void on_tb_identifierQuery_textChanged(const QString &arg1);

    void on_b_selectFile_2_clicked();

private:
    void HighLightIdentifierSearch(bool highlight, bool isFound = false);
    void ToggleControls(bool enable);
    void ClearBuffers();
    bool ReadFile(QString *filename);
    void UpdateIdentifierDisplay();
    bool Query(QString *identifier);
    void UpdateStats();
    void ResetStats();
    void DisplayStats();

    std::string ReadSourceLexemes(QString *filename);
    void Analyse(std::string sourceString);
    void AddNewRow(QString lexeme, QString value);
    void DisplayResults();

    Ui::MainWindow *ui;
    std::string _filename;
    QVector<QString> _identifiers;
    SortedList<std::string> _sortedList;
    HashTable _hashTable;
    Analyser _analyser;

    unsigned int _searchCount;
    //float _ht_hashMean;
    float _ht_valMean;
    //float _ht_hashSum;
    float _ht_valSum;
    //unsigned int _ht_lastHashComp;
    unsigned int _ht_lastValComp;
    float _sl_mean;
    float _sl_sum;
    unsigned int _sl_lastValComp;
};
#endif // MAINWINDOW_H
