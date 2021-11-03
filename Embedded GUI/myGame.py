from PySide6.QtCore import *
from PySide6.QtWidgets import *
from day3.game import Ui_Form
import random


class myGame(QWidget, Ui_Form):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.main()

    def main(self):
        self.intervalTime = 1000  # 위치 변경 주기
        self.cntClick = 0
        self.label.adjustSize()
        self.timer = QBasicTimer()
        self.timer.start(self.intervalTime, self)
        self.setTime()
        self.setCount()

    # 주기 설정
    def setInterval(self):
        self.timer.start(self.intervalTime, self)

    # 성공 횟수 설정
    def setCount(self):
        self.label_cnt.setText("성공 횟수 : {}".format(self.cntClick))

    # 위치 변경 주기 설정
    def setTime(self):
        self.label_time.setText("주기(ms) : {}".format(self.intervalTime))

    # 라벨 위치 변경 타이머 이벤트
    def timerEvent(self, event:QTimerEvent) -> None:
        x = random.randint(0, self.frame.size().width() - self.label.width())
        y = random.randint(0, self.frame.size().height() - self.label.height())
        self.label.move(x, y)  # 랜덤 위치 변경

    # 위치 변경 주기 초기화
    def initTime(self):
        self.intervalTime = 1000
        self.timer.start(self.intervalTime, self)
        self.cntClick = 0
        self.setCount()
        self.setTime()
        
    def changeColor(self):
        color = QColorDialog.getColor()
        if color.isValid():
            self.frame.setStyleSheet("background-color:{}".format(color.name()))

    def changeName(self):
        name, ok = QInputDialog.getText(self, "이름 변경", "이름을 입력하세요")
        if ok:
            self.label.setText(name)
            self.label.adjustSize()

    def showDiagram(self):
        sender = self.sender()
        if sender.text() == "이름변경":
            self.changeName()
        else:
            self.changeColor()

    # 마우스 클릭 이벤트
    def mousePressEvent(self, event):
        x = event.x()
        y = event.y()

        tx = self.label.x()
        ty = self.label.y()
        # 라벨을 클릭한 경우
        if tx <= x <= tx + self.label.width() and ty <= y <= ty + self.label.height():
            self.cntClick += 1  # 성공 횟수 증가
            self.setCount()
            self.intervalTime = int(self.intervalTime * 0.9)  # 위치 변경 시간 빨라지게 하기
            self.setInterval()
            self.setTime()


if __name__ == '__main__':
    app = QApplication()
    win = myGame()
    win.show()
    app.exec()