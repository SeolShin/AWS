import sys
import random
import math
from PyQt5.QtCore import (pyqtSignal, QMutex, QMutexLocker, QPoint, QSize, Qt,
        QThread, QWaitCondition)
from PyQt5.QtGui import QColor, QImage, QPainter, QPixmap, qRgb
from PyQt5.QtWidgets import QApplication, QWidget

class SensorWorker(QThread):

    def __init__(self, Parent = None):

        QThread.__init__(self, parent)
        self.exiting = False
        self.size = QSize(0, 0)
        self.path = QPainterPath()
        angle = 2*math.pi/5
        self.outerRadius = 20
        self.innerRadius = 8
        self.path.moveTo(self.outerRadius, 0)
        for step in range(1, 6):
            self.path.lineTo(
                self.innerRadius * math.cos((step - 0.5) * angle),
                self.innerRadius * math.sin((step - 0.5) * angle)
                )
            self.path.lineTo(
                self.outerRadius * math.cos(step * angle),
                self.outerRadius * math.sin(step * angle)
                )
        self.path.closeSubpath()
        
    def __del__(self):

        self.exiting = True
        self.wait()
        
    def render(self, size, stars):

        self.size = size
        self.stars = stars
        self.start()        
        
    def run(self):

        random.seed()
        n = self.stars
        width = self.size.width()
        height = self.size.height()

        while not self.exiting and n > 0:
        
            image = QImage(self.outerRadius * 2, self.outerRadius * 2, QImage.Format_ARGB32)
            image.fill(qRgba(0, 0, 0, 0))
            
            x = random.randrange(0, width)
            y = random.randrange(0, height)
            angle = random.randrange(0, 360)
            red = random.randrange(0, 256)
            green = random.randrange(0, 256)
            blue = random.randrange(0, 256)
            alpha = random.randrange(0, 256)
            
            painter = QPainter()
            painter.begin(image)
            painter.setRenderHint(QPainter.Antialiasing)
            painter.setPen(Qt.NoPen)
            painter.setBrush(QColor(red, green, blue, alpha))
            painter.translate(self.outerRadius, self.outerRadius)
            painter.rotate(angle)
            painter.drawPath(self.path)
            painter.end()
            self.emit(SIGNAL("output(QRect, QImage)"),QRect(x - self.outerRadius, y - self.outerRadius, self.outerRadius * 2, self.outerRadius * 2), image)
            n -= 1
        

    

    

        
