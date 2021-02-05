from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from math import sin, cos, sqrt, pi
import sys


def rad(grad):
    return grad * pi / 180


def dist(d1, d2):
    return sqrt((d1.x - d2.x) * (d1.x - d2.x) + (d1.y - d2.y) * (d1.y - d2.y))


class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def move(self, dx, dy):
        self.x += dx
        self.y += dy

    def scale(self, kx, ky, center):
        self.x = self.x * kx + (1 - kx) * center.x
        self.y = self.y * ky + (1 - ky) * center.y

    def rotate(self, angle, center):
        angle = rad(angle)
        x = center.x + (self.x - center.x) * cos(angle) - (self.y - center.y) * sin(angle)
        y = center.y + (self.x - center.x) * sin(angle) + (self.y - center.y) * cos(angle)

        self.x = x
        self.y = y


class Figure(object):
    def __init__(self, cent):
        self.base_center = cent
        self.center = cent
        self.circle_r = 15
        self.circles_c = []
        self.main_parts = ['base', 'top', 'top_oval', 'duct']
        self.points = {'base': [], 'top': [], 'top_oval': [], 'duct': [], 'circles': []}
        self.construct()

    def reset(self):
        self.center = self.base_center
        self.circle_r = 15
        self.circles_c = []
        self.main_parts = ['base', 'top', 'top_oval', 'duct']
        self.points = {'base': [], 'top': [], 'top_oval': [], 'duct': [], 'circles': []}
        self.construct()

    def construct(self):
        self.construct_base()
        self.construct_top()
        self.construct_top_oval()
        self.construct_duct()
        self.construct_duct()
        self.construct_circles()

    def construct_base(self):
        self.points['base'].append(Point(self.center.x - 150, self.center.y + 50))
        self.points['base'].append(Point(self.center.x + 150, self.center.y + 50))

        for i in range(540, 180, -1):
            self.points['base'].append(Point(25 * cos(rad(i / 2)) + self.center.x - 150, 25 * sin(rad(i / 2)) + self.center.y + 75))

        self.points['base'].append(Point(self.center.x - 150, self.center.y + 100))
        self.points['base'].append(Point(self.center.x + 150, self.center.y + 100))

        for i in range(180, -180, -1):
            self.points['base'].append(Point(25 * cos(rad(i / 2)) + self.center.x + 150, 25 * sin(rad(i / 2)) + self.center.y + 75))

    def construct_top(self):
        self.points['top'].append(Point(self.center.x - 100, self.center.y + 50))
        self.points['top'].append(Point(self.center.x - 100, self.center.y - 25))
        self.points['top'].append(Point(self.center.x + 100, self.center.y - 25))
        self.points['top'].append(Point(self.center.x + 100, self.center.y + 50))

    def construct_top_oval(self):
        for i in range(360, 720):
            self.points['top_oval'].append(Point(100 * cos(rad(i / 2)) + self.center.x, 25 * sin(rad(i / 2)) + self.center.y - 25))

    def construct_duct(self):
        lx = 55
        rx = 45
        r1s = 100 * 100
        r2s = 25 * 25
        ly = -sqrt(abs((r2s - lx * lx * r2s / r1s))) + self.center.y - 25
        ry = -sqrt(abs((r2s - rx * rx * r2s / r1s))) + self.center.y - 25

        self.points['duct'].append(Point(self.center.x - 55, ly))
        self.points['duct'].append(Point(self.center.x - 55, ly - 35))
        self.points['duct'].append(Point(self.center.x - 30, ly - 35))
        self.points['duct'].append(Point(self.center.x - 30, ly - 20))
        self.points['duct'].append(Point(self.center.x - 45, ly - 20))
        self.points['duct'].append(Point(self.center.x - 45, ry))

    def chose_centers(self):
        self.circles_c.append(Point(self.center.x - 150, self.center.y + 75))
        self.circles_c.append(Point(self.center.x - 75, self.center.y + 75))
        self.circles_c.append(Point(self.center.x, self.center.y + 75))
        self.circles_c.append(Point(self.center.x + 75, self.center.y + 75))
        self.circles_c.append(Point(self.center.x + 150, self.center.y + 75))

    def construct_circle(self, center_p):
        circle = []
        r = self.circle_r
        for i in range(360 * r):
            circle.append(Point(r * cos(i / r) + center_p.x, r * sin(i / r) + center_p.y))
        return circle

    def construct_circles(self):
        self.chose_centers()
        for i in self.circles_c:
            self.points['circles'].append(self.construct_circle(i))

    def move(self, dx, dy):
        for part in self.main_parts:
            for point in self.points[part]:
                point.move(dx, dy)

        for circle in self.points['circles']:
            for point in circle:    
                point.move(dx, dy)

        self.center.move(dx, dy)

    def rotate(self, angle, rotation_center):
        for part in self.main_parts:
            for point in self.points[part]:
                point.rotate(-angle, rotation_center)

        for circle in self.points['circles']:
            for point in circle:
                point.rotate(-angle, rotation_center)

        self.center.rotate(-angle, rotation_center)

    def scale(self, kx, ky, scale_center):
        for part in self.main_parts:
            for point in self.points[part]:
                point.scale(kx, ky, scale_center)

        for circle in self.points['circles']:
            for point in circle:
                point.scale(kx, ky, scale_center)

        self.center.scale(kx, ky, scale_center)

    def draw(self, pixmap: QPixmap):
        pixmap.fill(Qt.white)
        canvas = QPainter(pixmap)
        pen = QPen()

        pen.setWidth(10)
        pen.setColor(QColor('red'))
        canvas.setPen(pen)
        canvas.drawPoint(self.center.x, self.center.y)

        pen.setWidth(2)
        pen.setColor(QColor('black'))
        canvas.setPen(pen)

        for part in self.main_parts:
            for i in range(len(self.points[part]) - 1):
                canvas.drawLine(self.points[part][i].x, self.points[part][i].y, self.points[part][i + 1].x, self.points[part][i + 1].y)

        for circle in self.points['circles']:
            for i in range(len(circle) - 1):
                canvas.drawLine(circle[i].x, circle[i].y, circle[i + 1].x, circle[i + 1].y)


class Movement:
    def __init__(self, dx, dy):
        self.dx = dx
        self.dy = dy


class Scale:
    def __init__(self, kx, ky, center_scl):
        self.kx = kx
        self.ky = ky
        self.center = center_scl


class Rotation:
    def __init__(self, angle, center_rot):
        self.angle = angle
        self.center = center_rot


class Transformations(QApplication):
    def __init__(self):
        super(Transformations, self).__init__(sys.argv)
        self.main_window = QMainWindow()
        self.main_window.resize(1080, 720)
        self.picture = Figure(Point(390, 360))
        self.setupUi()
        self.actions = []

    def move_func(self):
        try:
            dx = float(self.dx_ent.text())
            dy = float(self.dx_ent.text())
        except:
            self.last_action_lbl.setText("Центр объекта:\n x=%.2f\n y=%.2f\n"
                                         "Ошибка: одно или несколько полей пусты!"
                                         % (self.picture.center.x, self.picture.center.y))
            return

        self.picture.move(dx, dy)
        self.picture.draw(self.painter())

        self.main_window.update()

        self.last_action_lbl.setText("Центр объекта:\n x=%.2f\n y=%.2f\n"
                                     "Объект был перемещён:\n dx=%.2f\n dy=%.2f"
                                     % (self.picture.center.x, self.picture.center.y, dx, dy))

        self.actions.append(Movement(dx, dy))

    def scale_func(self):
        try:
            kx = float(self.kx_ent.text())
            ky = float(self.ky_ent.text())
            xc = float(self.xc_scl_ent.text())
            yc = float(self.yc_scl_ent.text())
        except:
            self.last_action_lbl.setText("Центр объекта:\n x=%.2f\n y=%.2f\n"
                                         "Ошибка: одно или несколько полей пусты!"
                                         %(self.picture.center.x, self.picture.center.y))
            return
        if kx == 0 or ky == 0:
            self.last_action_lbl.setText("Центр объекта:\n x=%.2f\n y=%.2f\n"
                                         "Ошибка: один из коеффициентов \nравен 0!"
                                         % (self.picture.center.x, self.picture.center.y))
            return

        self.picture.scale(kx, ky, Point(xc, yc))
        self.picture.draw(self.painter())

        self.last_action_lbl.setText("Центр объекта:\n x=%.2f\n y=%.2f\n"
                                     "Обхъект был увеличен:"
                                     "\n kx=%.2f\n ky=%.2f\n вокруг точки:\n xc=%.2f\n yc=%.2f"
                                     % (self.picture.center.x, self.picture.center.y, kx, ky, xc, yc))

        self.main_window.update()
        self.actions.append(Scale(kx, ky, Point(xc, yc)))

    def rotate_func(self):
        try:
            angle = float(self.angle_ent.text())
            xc = float(self.xc_rot_ent.text())
            yc = float(self.yc_rot_ent.text())
        except:
            self.last_action_lbl.setText("Центр объекта:\n x=%.2f\n y=%.2f\n"
                                         "Ошибка: одно или несколько полей пусты!"
                                         % (self.picture.center.x, self.picture.center.y))
            return

        self.picture.rotate(angle, Point(xc, yc))
        self.picture.draw(self.painter())

        self.last_action_lbl.setText("Центр объекта:\n x=%.2f\n y=%.2f\n"
                                     "Объект был повернут\n угол=%d\n вокруг точки:\n xc=%.2f\n yc=%.2f"
                                     % (self.picture.center.x, self.picture.center.y, angle, xc, yc))

        self.main_window.update()
        self.actions.append(Rotation(angle, Point(xc, yc)))

    def reset_func(self):
        self.picture.reset()
        self.last_action_lbl.setText("Центр объекта:\n x=%.2f\n y=%.2f\n"
                                     "Объект был возвращен в исходное\nсостояние"
                                     % (self.picture.center.x, self.picture.center.y))
        self.picture.draw(self.painter())
        self.main_window.update()

    def undo_func(self):
        if not len(self.actions):
            return
        last_action = self.actions.pop()
        if type(last_action) is Movement:
            self.picture.move(-last_action.dx, -last_action.dy)
            self.last_action_lbl.setText("Центр объекта:\n x=%.2f\n y=%.2f\n"
                                         "Объект был смещено:\n dx=%.2f\n dy=%.2f"
                                         % (self.picture.center.x, self.picture.center.y, -last_action.dx, -last_action.dy))
        elif type(last_action) is Scale:
            self.picture.scale(1 / last_action.kx, 1 / last_action.ky, last_action.center)
            self.last_action_lbl.setText("Центр объекта:\n x=%.2f\n y=%.2f\n"
                                         "Объект был масштабирован:"
                                         "\n kx=%.2f\n ky=%.2f\n центр преобразования:\n xc=%.2f\n yc=%.2f"
                                         % (self.picture.center.x, self.picture.center.y, 1 / last_action.kx, 1 / last_action.ky,
                                            last_action.center.x, last_action.center.y))
        elif type(last_action) is Rotation:
            self.picture.rotate(-last_action.angle, last_action.center)
            self.last_action_lbl.setText("Центр объекта:\n x=%.2f\n y=%.2f\n"
                                         "Объект был повернут\n угол=%d\n вокруг точки:\n xc=%.2f\n yc=%.2f"
                                         % (self.picture.center.x, self.picture.center.y, -last_action.angle, last_action.center.x, last_action.center.y))

        self.picture.draw(self.painter())
        self.main_window.update()

    def setupUi(self):
        font = QFont()
        font.setFamily("Calibri")
        font.setPointSize(14)
        self.main_window.setFont(font)
        self.centralwidget = QWidget(self.main_window)

        val_cmd_flt = QRegExpValidator(QRegExp("-?[0-9]+[.][0-9]+"))
        val_cmd_int = QRegExpValidator(QRegExp("-?[0-9]+"))

        self.output_wdg = QWidget(self.centralwidget)
        self.output_wdg.setGeometry(QRect(300, 0, 780, 720))
        self.paint_lbl = QLabel(self.output_wdg)
        self.paint_lbl.setGeometry(QRect(0, 0, 780, 720))

        self.input_wdg = QWidget(self.centralwidget)
        self.input_wdg.setGeometry(QRect(0, 0, 300, 720))

        palette = QPalette()
        brush = QBrush(QColor(217, 217, 217))
        brush.setStyle(Qt.SolidPattern)
        palette.setBrush(QPalette.All, QPalette.Window, brush)

        self.input_wdg.setPalette(palette)
        self.input_wdg.setAutoFillBackground(True)

        self.tab_wdg = QTabWidget(self.input_wdg)
        self.tab_wdg.setGeometry(QRect(0, 0, 300, 211))
        self.tab_wdg.setFont(font)

        #
        # Вкладка Сдвига
        self.move_tb = QWidget()
        self.move_tb.setFont(font)
        self.formLayoutWidget = QWidget(self.move_tb)
        self.formLayoutWidget.setGeometry(QRect(0, 0, 291, 141))
        self.formLayout = QFormLayout(self.formLayoutWidget)
        self.formLayout.setContentsMargins(0, 0, 0, 0)

        self.dx_lbl = QLabel(self.formLayoutWidget, text='dX')

        self.formLayout.setWidget(0, QFormLayout.LabelRole, self.dx_lbl)
        self.dx_ent = QLineEdit(self.formLayoutWidget)
        self.dx_ent.setValidator(val_cmd_flt)
        self.formLayout.setWidget(0, QFormLayout.FieldRole, self.dx_ent)

        self.dy_lbl = QLabel(self.formLayoutWidget, text='dY')
        self.formLayout.setWidget(1, QFormLayout.LabelRole, self.dy_lbl)
        self.dy_ent = QLineEdit(self.formLayoutWidget)
        self.dy_ent.setValidator(val_cmd_flt)
        self.formLayout.setWidget(1, QFormLayout.FieldRole, self.dy_ent)

        self.move_btn = QPushButton(self.move_tb, text='Переместить')
        self.move_btn.setGeometry(QRect(0, 150, 291, 25))
        self.move_btn.clicked.connect(self.move_func)

        self.tab_wdg.addTab(self.move_tb, "")

        #
        # Вкладка Масштабирования
        self.scale_tb = QWidget()
        self.scale_tb.setFont(font)

        self.formLayoutWidget_2 = QWidget(self.scale_tb)
        self.formLayoutWidget_2.setGeometry(QRect(0, 0, 291, 141))
        self.formLayout_2 = QFormLayout(self.formLayoutWidget_2)
        self.formLayout_2.setContentsMargins(0, 0, 0, 0)

        self.kx_lbl = QLabel(self.formLayoutWidget_2, text='kX')
        self.formLayout_2.setWidget(0, QFormLayout.LabelRole, self.kx_lbl)
        self.kx_ent = QLineEdit(self.formLayoutWidget_2)
        self.kx_ent.setValidator(val_cmd_flt)
        self.formLayout_2.setWidget(0, QFormLayout.FieldRole, self.kx_ent)

        self.ky_lbl = QLabel(self.formLayoutWidget_2, text='kY')
        self.formLayout_2.setWidget(1, QFormLayout.LabelRole, self.ky_lbl)
        self.ky_ent = QLineEdit(self.formLayoutWidget_2)
        self.ky_ent.setValidator(val_cmd_flt)
        self.formLayout_2.setWidget(1, QFormLayout.FieldRole, self.ky_ent)

        self.xc_scl_lbl = QLabel(self.formLayoutWidget_2, text='Xc')
        self.formLayout_2.setWidget(2, QFormLayout.LabelRole, self.xc_scl_lbl)
        self.xc_scl_ent = QLineEdit(self.formLayoutWidget_2)
        self.xc_scl_ent.setValidator(val_cmd_flt)
        self.formLayout_2.setWidget(2, QFormLayout.FieldRole, self.xc_scl_ent)

        self.yc_scl_lbl = QLabel(self.formLayoutWidget_2, text='Yc')
        self.formLayout_2.setWidget(3, QFormLayout.LabelRole, self.yc_scl_lbl)
        self.yc_scl_ent = QLineEdit(self.formLayoutWidget_2)
        self.yc_scl_ent.setValidator(val_cmd_flt)
        self.formLayout_2.setWidget(3, QFormLayout.FieldRole, self.yc_scl_ent)

        self.scale_btn = QPushButton(self.scale_tb, text='Масштабировать')
        self.scale_btn.setGeometry(QRect(0, 150, 291, 25))
        self.scale_btn.clicked.connect(self.scale_func)

        self.tab_wdg.addTab(self.scale_tb, "")

        # Вкладка Поворота
        self.rot = QWidget()
        self.rot.setFont(font)
        self.formLayoutWidget_3 = QWidget(self.rot)
        self.formLayoutWidget_3.setGeometry(QRect(0, 0, 291, 141))
        self.formLayout_rot = QFormLayout(self.formLayoutWidget_3)
        self.formLayout_rot.setFieldGrowthPolicy(QFormLayout.AllNonFixedFieldsGrow)
        self.formLayout_rot.setContentsMargins(0, 0, 0, 0)

        self.angle_lbl = QLabel(self.formLayoutWidget_3, text='Угол')
        self.formLayout_rot.setWidget(0, QFormLayout.LabelRole, self.angle_lbl)
        self.angle_ent = QLineEdit(self.formLayoutWidget_3)
        self.angle_ent.setValidator(val_cmd_int)
        self.formLayout_rot.setWidget(0, QFormLayout.FieldRole, self.angle_ent)

        self.xc_rot_lbl = QLabel(self.formLayoutWidget_3, text='Xc')
        self.formLayout_rot.setWidget(1, QFormLayout.LabelRole, self.xc_rot_lbl)
        self.xc_rot_ent = QLineEdit(self.formLayoutWidget_3)
        self.xc_rot_ent.setValidator(val_cmd_flt)
        self.formLayout_rot.setWidget(1, QFormLayout.FieldRole, self.xc_rot_ent)

        self.yc_rot_lbl = QLabel(self.formLayoutWidget_3, text='Yc')
        self.formLayout_rot.setWidget(2, QFormLayout.LabelRole, self.yc_rot_lbl)
        self.yc_rot_ent = QLineEdit(self.formLayoutWidget_3)
        self.yc_rot_ent.setValidator(val_cmd_flt)
        self.formLayout_rot.setWidget(2, QFormLayout.FieldRole, self.yc_rot_ent)

        self.rotate_btn = QPushButton(self.rot, text='Повернуть')
        self.rotate_btn.setGeometry(QRect(0, 150, 291, 25))
        self.rotate_btn.clicked.connect(self.rotate_func)

        self.tab_wdg.addTab(self.rot, "")
        self.tab_wdg.setTabText(0, "Сдвиг")
        self.tab_wdg.setTabText(1, "Масштаб")
        self.tab_wdg.setTabText(2, "Поворот")
        self.tab_wdg.setCurrentIndex(0)

        #
        self.last_action_lbl = QLabel(self.input_wdg, text='Отменить последнее действие')
        self.last_action_lbl.setGeometry(QRect(10, 330, 281, 372))
        self.last_action_lbl.setFont(font)
        self.last_action_lbl.setAlignment(Qt.AlignLeading|Qt.AlignLeft|Qt.AlignTop)
        self.last_action_lbl.setText("Текущий центр объекта:\n x=%.2f\n y=%.2f\n"
                                     % (self.picture.center.x, self.picture.center.y))

        self.undo_btn = QPushButton(self.input_wdg, text='Отменить последнее действие')
        self.undo_btn.setGeometry(QRect(0, 240, 298, 25))
        self.undo_btn.setFont(font)
        self.undo_btn.clicked.connect(self.undo_func)

        self.reset_btn = QPushButton(self.input_wdg, text='Сбросить все изменения')
        self.reset_btn.setGeometry(QRect(0, 266, 298, 25))
        self.reset_btn.setFont(font)
        self.reset_btn.clicked.connect(self.reset_func)

        self.paint_lbl.setPixmap(QPixmap(780, 720))

        self.main_window.setCentralWidget(self.centralwidget)

    def painter(self):
        return self.paint_lbl.pixmap()


class Canvas(QWidget):
    def __init__(self):
        self.r = 15
        self.points = []
        self.circles = []
        QWidget.__init__(self)
        self.setGeometry(0, 0, 780, 720)


    def paintEvent(self, event):
        painter = QPainter(self)
        painter.begin()
        painter.fillRect(event.rect(), QBrush(QColor(10, 10, 10, 0)))
        painter.setRenderHint(QPainter.Antialiasing)
        painter.setPen(QPen(Qt.black))


        painter.end()


if __name__ == "__main__":
    app = Transformations()
    app.main_window.show()
    app.picture.draw(app.painter())
    app.picture.draw(app.painter())
    sys.exit(app.exec_())
