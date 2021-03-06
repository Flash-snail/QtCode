/*******************************************************
* @brief        查找文件并打开文件
* @author       xiaolei
* @copyright    -
* @version      V1.0
* @data         2019-10-28
* @note         1、通过打开文件对话框选择文件目录
* @note         2、选择文件格式，查找文件，目前只添加简单的几种格式
* @note         3、未添加功能：选择文件名，打开文件
* @note         4、未添加供能：若要手动输入目录，需要检测目录是否存在
*******************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopServices>
#include <QDateTime>
#include <QProcess>
#include <QMenu>
#include <QAction>
#include <QGuiApplication>
#include <QDesktopServices>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(720,560);
    setWindowTitle("查找文件");

    this->loadStyleSheet(":/style.qss");

    //创建中央小部件
    centralWidget=new QWidget(this);
    this->setCentralWidget(centralWidget);
    centralWidget->setGeometry(0,0,720,560);

    dirEdit=new QLineEdit(centralWidget);
    dirEdit->setGeometry(30,30,580,30);
    dirEdit->setReadOnly(true);

    browseBtn=new QPushButton(centralWidget);
    browseBtn->setText("浏览");
    browseBtn->setGeometry(620,30,70,30);
    connect(browseBtn,&QPushButton::clicked,this,&MainWindow::browseDir);

    findBtn=new QPushButton(centralWidget);
    findBtn->setText("查找");
    findBtn->setGeometry(620,100,70,30);
    connect(findBtn,&QPushButton::clicked,this,&MainWindow::findFiles);

    openBtn=new QPushButton(centralWidget);
    openBtn->setText(tr("打开"));
    openBtn->setGeometry(620,140,70,30);
    connect(openBtn,&QPushButton::clicked,this,&MainWindow::openFile);

    createFileFormat(); //创建文件格式组合
    createFileTable();  //创建文件显示表格
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*******************************************************
* @brief        创建文件格式
* @param        []
* @return       void
* @author       xiaolei
* @data         2019-10-28
* @note         -
*******************************************************/
void MainWindow::createFileFormat()
{
    group=new QGroupBox(centralWidget);
    group->setGeometry(30,70,580,150);
    group->setTitle("文件格式");

    QVBoxLayout *pictureLayout=new QVBoxLayout();

    allCheckBox=new QCheckBox();
    allCheckBox->setText(tr("*"));
    allCheckBox->setChecked(true);

    jpgCheckBox=new QCheckBox();
    jpgCheckBox->setText(tr("jpg"));

    pngCheckBox=new QCheckBox();
    pngCheckBox->setText(tr("png"));

    bmpCheckBox=new QCheckBox();
    bmpCheckBox->setText(tr("bmp"));

    gifCheckBox=new QCheckBox();
    gifCheckBox->setText(tr("gif"));

    tiffCheckBox=new QCheckBox();
    tiffCheckBox->setText(tr("tiff"));

    swfCheckBox=new QCheckBox();
    swfCheckBox->setText(tr("swf"));

    pictureLayout->addWidget(jpgCheckBox);
    pictureLayout->addWidget(pngCheckBox);
    pictureLayout->addWidget(bmpCheckBox);
    pictureLayout->addWidget(gifCheckBox);
    pictureLayout->addWidget(tiffCheckBox);
    pictureLayout->addWidget(swfCheckBox);

    QVBoxLayout *textLayout=new QVBoxLayout();

    txtCheckBox=new QCheckBox();
    txtCheckBox->setText(tr("txt"));

    xmlCheckBox=new QCheckBox();
    xmlCheckBox->setText(tr("xml"));

    docCheckBox=new QCheckBox();
    docCheckBox->setText(tr("doc"));

    xlsCheckBox=new QCheckBox();
    xlsCheckBox->setText(tr("xls"));

    pdfCheckBox=new QCheckBox();
    pdfCheckBox->setText(tr("pdf"));

    textLayout->addWidget(txtCheckBox);
    textLayout->addWidget(xmlCheckBox);
    textLayout->addWidget(docCheckBox);
    textLayout->addWidget(xlsCheckBox);
    textLayout->addWidget(pdfCheckBox);

    QVBoxLayout *voiceLayout=new QVBoxLayout();

    mp3CheckBox=new QCheckBox();
    mp3CheckBox->setText(tr("mp3"));

    wavCheckBox=new QCheckBox();
    wavCheckBox->setText(tr("wav"));

    voiceLayout->addWidget(mp3CheckBox);
    voiceLayout->addWidget(wavCheckBox);

    QVBoxLayout *videoLayout=new QVBoxLayout();

    aviCheckBox=new QCheckBox();
    aviCheckBox->setText(tr("avi"));

    mp4CheckBox=new QCheckBox();
    mp4CheckBox->setText(tr("mp4"));

    flvCheckBox=new QCheckBox();
    flvCheckBox->setText(tr("flv"));

    mkvCheckBox=new QCheckBox();
    mkvCheckBox->setText(tr("mkv"));

    wmvCheckBox=new QCheckBox();
    wmvCheckBox->setText(tr("wmv"));

    videoLayout->addWidget(aviCheckBox);
    videoLayout->addWidget(mp4CheckBox);
    videoLayout->addWidget(flvCheckBox);
    videoLayout->addWidget(mkvCheckBox);
    videoLayout->addWidget(wmvCheckBox);

    QVBoxLayout *compressLayout=new QVBoxLayout();

    zipCheckBox=new QCheckBox();
    zipCheckBox->setText(tr("zip"));

    rarCheckBox=new QCheckBox();
    rarCheckBox->setText(tr("rar"));

    zip7CheckBox=new QCheckBox();
    zip7CheckBox->setText(tr("7z"));

    compressLayout->addWidget(zipCheckBox);
    compressLayout->addWidget(rarCheckBox);
    compressLayout->addWidget(zip7CheckBox);

    QHBoxLayout *fileformatLayout=new QHBoxLayout(group);

    fileformatLayout->addWidget(allCheckBox);
    fileformatLayout->addLayout(pictureLayout);
    fileformatLayout->addLayout(textLayout);
    fileformatLayout->addLayout(voiceLayout);
    fileformatLayout->addLayout(videoLayout);
    fileformatLayout->addLayout(compressLayout);
}

/*******************************************************
* @brief        创建文件表格
* @param        []
* @return       void
* @author       xiaolei
* @data         2019-10-28
* @note         -
*******************************************************/
void MainWindow::createFileTable()
{
    table = new QTableWidget(1000, 3,centralWidget);
    table->setGeometry(30,230,660,270);
    table->setSelectionBehavior(QAbstractItemView::SelectRows); //选择整行

    QStringList labels;
    labels << tr("文件名") << tr("大小")<<tr("创建时间");

    table->setHorizontalHeaderLabels(labels);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->verticalHeader()->hide();
    table->setShowGrid(false);                                                  //隐藏网格
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);                  //不可编辑
    table->setStyleSheet("selection-background-color:rgb(150,190,60)");         //选中颜色
}

/*******************************************************
* @brief        浏览文件目录
* @param        []
* @return       void
* @author       xiaolei
* @data         2019-10-25
* @note         -
*******************************************************/
void MainWindow::browseDir()
{
    QString directory =QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("浏览目录"), QDir::currentPath()));

    if (!directory.isEmpty())
    {
        dirEdit->setText(directory);
        dirEdit->setReadOnly(true);
    }
}

/*******************************************************
* @brief        查找文件
* @param        []
* @return       void
* @author       xiaolei
* @data         2019-10-25
* @note         -
*******************************************************/
void MainWindow::findFiles()
{
    QStringList files;
    QString path=QDir::cleanPath(dirEdit->text());
    qDebug()<<path;
    QDir *dir=new QDir(path);

    QStringList filter; //筛选器
    if(allCheckBox->isChecked())
    {
        filter.push_back(tr("*"));
    }
    if(jpgCheckBox->isChecked())
    {
        filter.push_back(tr("*.jpg"));
    }
    if(pngCheckBox->isChecked())
    {
        filter.push_back(tr("*.png"));
    }
    if(txtCheckBox->isChecked())
    {
        filter.push_back(tr("*.txt"));
    }
    if(xmlCheckBox->isChecked())
    {
        filter.push_back(tr("*.xml"));
    }

    fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));

    qDebug()<<"------------- 打印文件信息 --------------";
    qDebug()<<fileInfo->count();
    for(int i=0;i<fileInfo->count();++i)
    {
        qDebug()<<fileInfo->at(i).fileName();   //文件名
        qDebug()<<fileInfo->at(i).filePath();   //文件路径
        qDebug()<<fileInfo->at(i).size();       //文件大小
        QDateTime data_time=fileInfo->at(i).created();
        qDebug()<<data_time;    //创建时间
    }
    //table->setItem(0,0,new QTableWidgetItem(fileInfo->at(0).fileName()));
    showFileInfo();
}

/*******************************************************
* @brief        打开文件
* @param        []
* @return       void
* @author       xiaolei
* @data         2019-10-28
* @note         -
*******************************************************/
void MainWindow::openFile()
{
    int row=table->currentRow();
    qDebug()<<"row:"<<row;
    QString path = fileInfo->at(row).filePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));   //通过桌面公共服务打开文件
}

/*******************************************************
* @brief        表格显示文件信息
* @param        []
* @return       void
* @author       xiaolei
* @data         2019-10-28
* @note         -
*******************************************************/
void MainWindow::showFileInfo()
{
    for(int row=0;row<fileInfo->count();++row)
    {
        for(int column=0;column<3;++column)
        {
            switch (column){
            case 0:
                table->setItem(row,column,new QTableWidgetItem(fileInfo->at(row).fileName()));
                break;
            case 1:{
                QString strSize=QString::number(fileInfo->at(row).size());
                table->setItem(row,column,new QTableWidgetItem(strSize));
                break;
                }
            case 2:{
                QString strDataTime;
                QDateTime time=fileInfo->at(row).created();
                strDataTime=time.toString("yyyy-MM-dd hh:mm:ss");   //QDateTime转QString
                table->setItem(row,column,new QTableWidgetItem(strDataTime));
                break;
                }
            default:
                break;
            }
        }
    }
}

/*******************************************************
* @brief        加载样式表
* @param        []
* @return       void
* @author       xiaolei
* @data         2019-10-29
* @note         -
*******************************************************/
void MainWindow::loadStyleSheet(const QString &sheetName)
{
    QFile file(sheetName);
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
    file.close();   //关闭文件
}


