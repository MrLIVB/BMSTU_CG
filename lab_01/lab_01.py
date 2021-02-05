from math import sqrt, pi
from tkinter import *


class Dot:
    def __init__(self, x, y):
        self.x = x
        self.y = y


'''
def triangle_square(d1, d2, d3):
    a = dist(d1, d2)
    b = dist(d1, d3)
    c = dist(d2, d3)
    p = (a + b + c) / 2
    s = sqrt(p * (p - a) * (p - b) * (p - c))
    return s
'''

'''
def circle_square(d1, d2, d3, ts):
    a = dist(d1, d2)
    b = dist(d1, d3)
    c = dist(d2, d3)
    r = (a * b * c) / (4 * ts)
    return pi * r * r
'''


def dist(d1, d2):
    return sqrt((d1.x - d2.x) * (d1.x - d2.x) + (d1.y - d2.y) * (d1.y - d2.y))


def triangle_square_s(a, b, c):
    p = (a + b + c) / 2
    s = sqrt(p * (p - a) * (p - b) * (p - c))
    return s


def circle_square_s(a, b, c, ts):
    if ts == 0:
        return -1
    r = (a * b * c) / (4 * ts)

    return pi * r * r


def find_closest(dots):
    if len(dots) < 3:
        return -1, 0, 0, 0

    si = -1
    sj = 1
    sk = 2

    for i in range(len(dots) - 2):
        for j in range(i + 1, len(dots) - 1):
            a = dist(dots[i], dots[j])
            for k in range(j + 1, len(dots)):
                b = dist(dots[i], dots[k])
                c = dist(dots[j], dots[k])

                ts = triangle_square_s(a, b, c)
                if ts == 0:
                    continue
                cs = circle_square_s(a, b, c, ts)
                if cs != -1:
                    si = i
                    sj = j
                    sk = k
                    break
            if si > 0:
                break
        if si > 0:
            break

    if si == -1:
        return -2, 0, 0, 0

    dif = cs - ts

    ind1 = si
    ind2 = sj
    ind3 = sk

    for i in range(len(dots) - 2):
        for j in range(i + 1, len(dots) - 1):
            a = dist(dots[i], dots[j])
            for k in range(j + 1, len(dots)):
                b = dist(dots[i], dots[k])
                c = dist(dots[j], dots[k])

                ts = triangle_square_s(a, b, c)
                if ts == 0:
                    continue
                cs = circle_square_s(a, b, c, ts)

                if cs == -1:
                    continue

                t_dif = cs - ts
                if t_dif < dif:
                    dif = t_dif
                    ind1 = i
                    ind2 = j
                    ind3 = k

    return ind1, ind2, ind3, dif


def scroll(event):
    table_cnv.configure(scrollregion=table_cnv.bbox("all"), width=200, height=400)


def tmp():
    pass


def delete_table():
    for frm in table_fr.winfo_children():
        if type(frm) is type(in_frame) and frm is not head_frame:
            frm.destroy()
    return


def is_in(x, y):
    for point in dots:
        if point.x == x and point.y == y:
            return 1
    return 0


def dot_add():
    tx = x_cord.get()
    ty = y_cord.get()
    if tx == '' or ty == '':
        hint_lbl.config(text='Incorrect input')
        return
    tx = int(tx)
    ty = int(ty)
    if is_in(tx, ty):
        return

    dot = Dot(tx, ty)
    dots.append(dot)
    draw_table(dots)
    draw_res()
    return


def delete_dot_val():
    tx = x_cord.get()
    ty = y_cord.get()
    if tx == '' or ty == '':
        hint_lbl.config(text='Incorrect input')
        return

    dot = Dot(int(tx), int(ty))
    for dt in dots:
        if dt.x == dot.x and dt.y == dot.y:
            dots.remove(dt)
            for frm in table_fr.winfo_children():
                if frm is Label:
                    frm.destroy()
            delete_table()
            draw_table(dots)
            break
    draw_res()
    return


def delete_dot_ind():
    ti = ind_ent.get()
    if ti == '':
        return

    i = int(ti) - 1
    if i >= len(dots) or i == -1:
        return
    dots.pop(i)

    delete_table()
    draw_table(dots)
    draw_res()
    return


def draw_table_head():
    n_lbl = Label(head_frame, text="№", width=5, relief=GROOVE)
    x_lbl = Label(head_frame, text="X", width=10, relief=GROOVE)
    y_lbl = Label(head_frame, text="Y", width=10, relief=GROOVE)

    n_lbl.pack(side="left")
    x_lbl.pack(side="left")
    y_lbl.pack(side="left")
    head_frame.pack()

    return


def draw_table(dots):
    delete_table()

    i = 1
    for dot in dots:
        line_frame = Frame(table_fr)
        n_lbl = Label(line_frame, text=str(i), width=5, relief=GROOVE)
        x_lbl = Label(line_frame, text=str(dot.x), width=10, relief=GROOVE)
        y_lbl = Label(line_frame, text=str(dot.y), width=10, relief=GROOVE)

        line_frame.pack()
        n_lbl.pack(side="left")
        x_lbl.pack(side="left")
        y_lbl.pack(side="left")

        i += 1
    return


def validate(char, actt, parent=None):
    if actt == '1':
        return (char in '0123456789.-') and not (char in '.-' and char in parent.get())
    else:
        return True


def alt_val(char, actt):
    if actt == '1':
        return char in '0123456789'
    else:
        return True


def create_circle(cnv, x, y, r, **kwargs):
    return cnv.create_oval(x-r, y-r, x+r, y+r, **kwargs)


def find_center(id1, id2, id3):
    center = Dot(0, 0)

    d1 = Dot(id1.x, id1.y)
    d2 = Dot(id2.x, id2.y)
    d3 = Dot(id3.x, id3.y)

    if d3.x == d2.x:
        tx, ty = d1.x, d1.y
        d1.x, d1.y = d2.x, d2.y
        d2.x, d2.y = tx, ty
    elif d1.x == d2.x:
        tx, ty = d3.x, d3.y
        d3.x, d3.y = d2.x, d2.y
        d2.x, d2.y = tx, ty
    #print(d1.x, d1.y, d2.x, d2.y)

    m1 = (d2.y - d1.y) / (d2.x - d1.x)
    m2 = (d3.y - d2.y) / (d3.x - d2.x)

    x = (m1 * m2 * (d1.y - d3.y) + m2 * (d1.x + d2.x) - m1 * (d2.x + d3.x)) / (2 * (m2 - m1))
    try:
        y = -1 * (x - (d1.x + d2.x) / 2) / m1 + (d1.y + d2.y) / 2
    except ZeroDivisionError:
        y = -1 * (x - (d2.x + d3.x) / 2) / m2 + (d2.y + d3.y) / 2

    center.x = x
    center.y = y

    return center


def scale(dot, k, xc, yc):
    dot.x = dot.x * k + (1 - k) * xc
    dot.y = dot.y * k + (1 - k) * yc
    return dot


def out_of_screen(center, r, w, h):
    dx = 0
    dy = 0
    if center.x - r <= 0:
        dx = r - center.x + 10
    elif center.x + r >= w:
        dx = w - center.x - r - 10

    if center.y - r <= 0:
        dy = r - center.y + 10
    elif center.y + r >= h:
        dy = h - center.y - r - 10
    return dx, dy


def draw_res():
    i1, i2, i3, dif = find_closest(dots)
    if i1 is -1:
        hint_lbl.config(text='! Not enough points')
        return
    elif i1 is -2:
        hint_lbl.config(text='! All points are lined up')
        return
    else:
        hint_lbl.config(text='minimal difference =%f'%(dif), )

    draw.delete("all")

    d1 = Dot(dots[i1].x, dots[i1].y)
    d1x = d1.x
    d1y = d1.y

    d2 = Dot(dots[i2].x, dots[i2].y)
    d2x = d2.x
    d2y = d2.y

    d3 = Dot(dots[i3].x, dots[i3].y)
    d3x = d3.x
    d3y = d3.y

    center = find_center(d1, d2, d3)
    r = dist(center, d1)

    if r < 25:
        k = 20
    elif r < 50:
        k = 4
    elif r < 100:
        k = 4
    elif r > 300:
        k = 0.25
    elif r > 150:
        k = 0.5

    if r < 100:
        d1 = scale(d1, k, center.x, center.y)
        d2 = scale(d2, k, center.x, center.y)
        d3 = scale(d3, k, center.x, center.y)
        r *= k

    dx, dy = out_of_screen(center, r, 400, 400)

    d1.x += dx
    d2.x += dx
    d3.x += dx
    d1.y = 400 - d1.y - dy
    d2.y = 400 - d2.y - dy
    d3.y = 400 - d3.y - dy

    draw.create_line(d1.x, d1.y, d2.x, d2.y, width=1)
    draw.create_line(d1.x, d1.y, d3.x, d3.y, width=1)
    draw.create_line(d2.x, d2.y, d3.x, d3.y, width=1)

    o1 = "%d(%d, %d)" % (i1 + 1, d1x, d1y)
    o2 = "%d(%d, %d)" % (i2 + 1, d2x, d2y)
    o3 = "%d(%d, %d)" % (i3 + 1, d3x, d3y)

    draw.create_text(d1.x, d1.y, text=o1)
    draw.create_text(d2.x, d2.y, text=o2)
    draw.create_text(d3.x, d3.y, text=o3)

    create_circle(draw, center.x + dx, 400 - center.y - dy, r)

    return


dots = []
'''
for i in range(20):
    x = randint(0, 400)
    y = randint(0, 400)
    dots.append(Dot(x, y))
'''
root = Tk()
root.wm_geometry("650x450+300+0")
root.wm_title("Smallest difference")

in_frame = Frame(root)

vcmd_x = in_frame.register(lambda char, actt: validate(char, actt, x_cord))
vcmd_y = in_frame.register(lambda char, actt: validate(char, actt, y_cord))

alt_vcmd = in_frame.register(alt_val)

xh_lbl = Label(in_frame, text="X = ")
x_cord = Entry(in_frame, validate='key', validatecommand=(vcmd_x, '%S', '%d'),)

yh_lbl = Label(in_frame, text="  Y = ")
y_cord = Entry(in_frame, validate='key', validatecommand=(vcmd_y, '%S', '%d'))

add = Button(in_frame, text="Add point",  command=dot_add)
rem = Button(in_frame, text="Delete point(value)", command=delete_dot_val)

space = Label(in_frame, text="", width=3)

ind_ent = Entry(in_frame, width=6, validate='key', validatecommand=(alt_vcmd, '%S', '%d'))
del_ind = Button(in_frame, text="Del point(index)", command=delete_dot_ind)

#
in_frame.pack()

xh_lbl.pack(side="left")
x_cord.pack(side="left")

yh_lbl.pack(side="left")
y_cord.pack(side="left")

add.pack(side="left")
rem.pack(side="left")

space.pack(side="left")

ind_ent.pack(side="left")
del_ind.pack(side="left")

# ----------------------------------------
out_frame = Frame(root)
out_frame.pack()


t = Frame(out_frame)
table_cnv = Canvas(t)

table_fr = Frame(table_cnv)
myscrollbar = Scrollbar(table_fr, orient="vertical", command=table_cnv.yview)
table_cnv.configure(yscrollcommand=myscrollbar.set)

myscrollbar.pack(side="right", fill="y")

table_cnv.pack(side="left")
table_cnv.create_window((0, 0), window=table_fr, anchor='nw')
table_fr.bind("<Configure>", scroll)


head_frame = Frame(table_fr)

draw_table_head()
draw_table(dots)

t.pack(side="left")

#
draw = Canvas(out_frame, relief=GROOVE, width=400, height=400, bd=1)

draw.pack()

hint_lbl = Label(out_frame, text="", font=(None, 14), fg="red")
hint_lbl.pack(side="bottom")

root.mainloop()
