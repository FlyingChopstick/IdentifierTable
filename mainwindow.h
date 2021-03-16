#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hashTable.h"
#include "sortedDynArray.h"

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

private:
    void ToggleControls(bool enable);
    void ClearBuffers();
    void ReadFile(QString *filename);
    void UpdateIdentifierDisplay();
    void Query(QString *identifier);
    void UpdateStats();
    void ResetStats();
    void DisplayStats();

    Ui::MainWindow *ui;
    std::string _filename;
    QVector<QString> _identifiers;
    SortedList<std::string> _sortedList;
    HashTable _hashTable;

    unsigned int _searchCount;
    float _ht_hashMean;
    float _ht_valMean;
    float _ht_hashSum;
    float _ht_valSum;
    unsigned int _ht_lastHashComp;
    unsigned int _ht_lastValComp;
    float _sl_mean;
    float _sl_sum;
    unsigned int _sl_lastValComp;
};
#endif // MAINWINDOW_H
