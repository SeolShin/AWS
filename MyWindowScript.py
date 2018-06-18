import math
import random
import sys
from PyQt5.QtCore import (pyqtSignal, QMutex, QMutexLocker, QPoint, QSize, Qt,
        QThread, QWaitCondition)
from PyQt5.QtGui import QColor, QImage, QPainter, QPixmap, qRgb
from PyQt5.QtWidgets import QApplication, QWidget
from MySensorWorker import SensorWorker

class Window(QWidget):
    
    def __init__(self, parent = None):
        
        QWidget.__init__(self, parent)
        
        self.thread = SensorWorker()
        
        label = QLabel(self.tr("FCN-based Reinforcement Learning"))
        self.spinBox = QSpinBox()
        self.spinBox.setMaximun(10000)
        self.spinBox.setValue(100)
        self.startButton = QPushButton(self.tr("&Start"))
        self.viewer = QLabel()
        self.viewer.setFixedSize(300, 300)

        self.connect(self.thread, SIGNAL("finished()"), self.updateUi)
        self.connect(self.thread, SIGNAL("terminated()"), self.updateUi)
        #self.connect(self.thread, SIGNAL("output(QRect, QImage)"), self.addImage)
        self.connect(self.startButton, SIGNAL("clicked()"), self.makePicture)

        layout = QGridLayout()
        layout.addWidget(label, 0, 0)
        layout.addWidget(self.spinBox, 0, 1)
        layout.addWidget(self.startButton, 0, 2)
        layout.addWidget(self.viewer, 1, 0, 1, 3)
        self.setLayout(layout)
        self.setwindowTitle(self.tr("Sensor Threading"))        

    def makePicture(self):

        self.spinBox.setReadOnly(True)
        self.startButton.setEnabled(False)
        pixmap = QPixmap(self.viewer.size())
        pixmap.fill(Qt.black)
        self.viewer.setPixmap(pixmap)
        self.thread.render(self.viewer.size(), self.spinBox.value())

    def addImage(self, rect, image):

        pixmap = self.viewer.pixmap()
        painter = QPainter()
        painter.begin(pixmap)
        painter.drawImage(rect, image)
        painter.end()
        self.viewer.update(rect)

    def updateUi(self):
        
        self.spinBox.setReadOnly(False)
        self.startButton.setEnabled(True)
        

    
