from PySide6.QtWidgets import *
from PySide6.QtGui import *


class myEditor(QMainWindow):
    def __init__(self):
        super().__init__()
        self.filePath = None  # 파일 경로
        self.menu_bar = self.menuBar()  # 메뉴바
        self.setWindowTitle("My editor")  # 타이틀 설정
        self.menu_file = self.menu_bar.addMenu("file")  # 파일 메뉴 추가
        self.menu_help = self.menu_bar.addMenu("help")  # help 메뉴 추가

        # 파일 엑션 추가
        self.actionNew = QAction("New", self)  
        self.actionNew.setShortcut(QKeySequence("Ctrl+N")) # 단축키 추가
        self.actionOpen = QAction("Open", self)
        self.actionOpen.setShortcut(QKeySequence("Ctrl+O"))
        self.actionSave = QAction("Save", self)
        self.actionSave.setShortcut(QKeySequence("Ctrl+S"))
        self.actionSave_as = QAction("Save as", self)
        self.actionSave_as.setShortcut(QKeySequence("Ctrl+Shift+A"))
        self.actionInfo = QAction("Info", self)
        self.actionInfo.setShortcut(QKeySequence("Ctrl+I"))

        # 메뉴에 추가
        self.menu_file.addAction(self.actionNew)
        self.menu_file.addAction(self.actionOpen)
        self.menu_file.addAction(self.actionSave)
        self.menu_file.addAction(self.actionSave_as)
        self.menu_help.addAction(self.actionInfo)

        # 트리거 연결
        self.actionNew.triggered.connect(self.fileNew)
        self.actionOpen.triggered.connect(self.fileOpen)
        self.actionSave.triggered.connect(self.fileSave)
        self.actionSave_as.triggered.connect(self.fileSaveAs)
        self.actionInfo.triggered.connect(self.showInfo)

        # main 위젯 생성
        self.main = QWidget()
        self.setCentralWidget(self.main)
        self.plainTextEdit = QPlainTextEdit(self.main) # 텍스트 위젯 생성
        self.plainTextEdit.setGeometry(0, 0, 400, 300)
        self.resize(400, 320)

    # 새 메모장 열기
    def fileNew(self): 
        if self.plainTextEdit.document().isModified(): # 수정된 경우 저장할지 물어봄
            answer = QMessageBox.question(self, None, "Save or Not",
                                          QMessageBox.Save |
                                          QMessageBox.Discard |
                                          QMessageBox.Cancel)
            if answer & QMessageBox.Save:
                if not self.fileSaveAs():
                    return
            elif answer & QMessageBox.Cancel:
                return
        self.plainTextEdit.setPlainText("")
        self.filePath = None
        self.setTitle()

    # 파일 열기
    def fileOpen(self):
        self.filePath = QFileDialog.getOpenFileName(self, "Open File")[0]
        if self.filePath:
            self.plainTextEdit.setPlainText(open(self.filePath).read())
            self.setTitle()
    
    # 파일 저장
    def fileSave(self):
        if not self.filePath: # 경로가 없을 경우 저장할 이름 선택
            self.filePath = QFileDialog.getSaveFileName(self, "Save")[0]
        if self.filePath: # 이미 경로가 있으면 그대로 저장
            with open(self.filePath, "w") as f:
                f.write(self.plainTextEdit.toPlainText())
                self.setTitle()
                return True
        else :
            return False

    # 다른 이름으로 저장
    def fileSaveAs(self):
        self.filePath = QFileDialog.getSaveFileName(self, "Save As")[0]
        if self.filePath:
            with open(self.filePath, "w") as f:
                f.write(self.plainTextEdit.toPlainText())
                self.setTitle()
                return True
        else :
            return False

    # 타이틀 변경하기
    def setTitle(self):
        if self.filePath is None:  # 타이틀 없으면 디폴트 타이틀
            self.setWindowTitle("My editor")
        else:  # 경로 있으면 파일명 + 타이틀
            self.setWindowTitle("{} My Editor".format(self.filePath.split("/")[-1]))

    # 정보 출력
    def showInfo(self):
        text = "<center>" \
                "<h1>Text Editor</h1>" \
                "</center>" \
                "<p>Version 1.0.0<br>" \
                "Copyright dlworms32</p>"
        QMessageBox.about(self, "About", text)

    # 종료 이벤트 재정의
    def closeEvent(self, event):
        if not self.plainTextEdit.document().isModified():
            return
        answer = QMessageBox.question(self, None, "Save or Not",
                                      QMessageBox.Save |
                                      QMessageBox.Discard |
                                      QMessageBox.Cancel)
        if answer & QMessageBox.Save:
            if not self.fileSave():  # 저장이 이상하게 종료된 경우
                event.ignore()  # 종료 이벤트 무시
        elif answer & QMessageBox.Cancel:
            event.ignore()


if __name__ == '__main__':
    app = QApplication()
    win = myEditor()

    win.show()
    app.exec()