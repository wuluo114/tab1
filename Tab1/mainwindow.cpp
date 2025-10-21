#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    digitBtns = {{Qt::Key_0,ui->btnNum0},
               {Qt::Key_1,ui->btnNum1},
               {Qt::Key_2,ui->btnNum2},
               {Qt::Key_3,ui->btnNum3},
               {Qt::Key_4,ui->btnNum4},
               {Qt::Key_5,ui->btnNum5},
               {Qt::Key_6,ui->btnNum6},
               {Qt::Key_7,ui->btnNum7},
               {Qt::Key_8,ui->btnNum8},
               {Qt::Key_9,ui->btnNum9},
               };
    foreach(auto btn,digitBtns)connect(btn,SIGNAL(clicked()),this,SLOT(btnNumClicked()));


    opBtns = {{Qt::Key_Plus,ui->btnPlus},
              {Qt::Key_Minus,ui->btnMinus},
              {Qt::Key_Asterisk,ui->btnMult},
              {Qt::Key_Slash,ui->btnDivide},
              {Qt::Key_Enter,ui->btnEqual},
              {Qt::Key_Period,ui->btnPeriod},
              {Qt::Key_Backspace,ui->btnDel}
              };

    // connect(ui->btnNum0,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum1,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum2,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum3,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum4,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum5,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum6,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum7,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum8,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum9,SIGNAL(clicked()),this,SLOT(btnNumClicked()));

    connect(ui->btnMult,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivide,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnPlus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMinus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));

    connect(ui->btnPer,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnInverse,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnPow,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::btnNumClicked()
{
    QString digit = qobject_cast<QPushButton*>(sender())->text();

    if(digit == "0" && operand == "0")digit = "";

    if(operand == "0" && digit != "0")operand ="";

    operand += digit;

    ui->display->setText(operand);
}


void MainWindow::on_btnPeriod_clicked()
{
    if(!operand.contains("."))
    {
        if(operand == "")
        {
            operand = "0";
            operand += qobject_cast<QPushButton*>(sender())->text();
        }
        else operand += qobject_cast<QPushButton*>(sender())->text();
    }
    ui->display->setText(operand);
}


void MainWindow::on_btnDel_clicked()
{
    operand = operand.left(operand.length()-1);
    ui->display->setText(operand);
}


void MainWindow::on_btnClearAll_clicked()
{
    operand.clear();
    operands.clear();
    opcodes.clear();
    ui->display->setText(operand);
}


QString MainWindow::calculation()
{
    if(operands.size() == 2 && opcodes.size() > 0)
    {
        double result = 0;
        double operand1 = operands.front().toDouble();
        operands.pop_front();
        double operand2 = operands.front().toDouble();
        operands.pop_front();
        QString op = opcodes.front();
        opcodes.pop_front();

        if(operand2 == 0 && op == "÷")
        {
            operand = "";
            operands.clear();
            opcodes.clear();
            return "无效的输入";
        }

        if(op == "+")result = operand1 + operand2;
        else if(op == "-")result = operand1 - operand2;
        else if(op == "×")result = operand1 * operand2;
        else if(op == "÷")result = operand1 / operand2;

        operands.push_back(QString::number(result));

        operand = "";
        return QString::number(result);
    }
    else
    {
        QString result = operands.front();

        operand = "";
        return result;
    }
}


void MainWindow::btnBinaryOperatorClicked()
{
    QString opcode = qobject_cast<QPushButton*>(sender())->text();

    if(operand != "")
    {
        opcodes.push_back(opcode);
        operands.push_back(operand);
        operand = "";

        QString result =  calculation();
        ui->display->setText(result);
    }
    else if(opcodes.size() == 0 && operands.size() ==1)
    {
        opcodes.push_back(opcode);
    }
}


void MainWindow::btnUnaryOperatorClicked()
{
    if(operand != "")
    {
        double result = operand.toDouble();
        operand = "";

        QString op = qobject_cast<QPushButton *>(sender())->text();

        if(op == "1/𝑥")
        {
            if(result == 0)return;
            else result = 1/result;
        }
        else if(op == "𝑥²")result *= result;
        else if(op == "√")result = sqrt(result);
        else if(op == "%")result /= 100.0;

        ui->display->setText(QString::number(result));
        operands.push_back(QString::number(result));
    }
    else if(opcodes.size() == 0 && operands.size() ==1)
    {
        double result = operands.front().toDouble();
        operand = "";

        QString op = qobject_cast<QPushButton *>(sender())->text();

        if(op == "1/𝑥")result = 1/result;
        else if(op == "𝑥²")result *= result;
        else if(op == "√")result = sqrt(result);
        else if(op == "%")result /= 100.0;

        ui->display->setText(QString::number(result));
        operands.pop_back();
        operands.push_back(QString::number(result));
    }
}


void MainWindow::on_btnEqual_clicked()
{
    if(opcodes.size() == 0 && operands.size() ==1)btnBinaryOperatorClicked();
    else if(operand != "")
    {
        operands.push_back(operand);
        operand = "";

        QString result = calculation();

        ui->display->setText(result);
    }


}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach(auto btnKey,digitBtns.keys())
    {
        if(event->key() == btnKey)
        {
            digitBtns[btnKey]->animateClick();
            break;
        }
    }
    foreach (auto btnKey,opBtns.keys())
    {
        if(event->key() == btnKey)
        {
            opBtns[btnKey]->animateClick();
            break;
        }
    }
}


void MainWindow::on_btnClear_clicked()
{
    operand = "";
    ui->display->setText("0");
}


void MainWindow::on_btnSign_clicked()
{
    if(operand != "")
    {
        double result = operand.toDouble();
        result -= 2*result;

        operand = QString::number(result);
        ui->display->setText(QString::number(result));

    }
    else
    {
        double result = operands.front().toDouble();
        operands.pop_back();
        result -= 2*result;
        operands.push_back(QString::number(result));
        ui->display->setText(QString::number(result));
    }
}

