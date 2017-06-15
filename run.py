from tkinter import *
import subprocess
import platform

map_argument = [
    "",
    "dust",
]

map_pic = [
    "resources/pictures/default.gif",
    "resources/pictures/dust.gif",
]



def get_main_path(root_dir = ""):
    plat = platform.platform()
    p = root_dir
    if(plat[:3] == "Lin"):
        p += "./main"
    if(plat[:3] == "Dar"):
        p += "./main"
    if(plat[:3] == "Win"):
        p += "main"
    return p

def run_main():
    main_path = get_main_path()
    subprocess.call([main_path, map_argument[map_index.get()]])

def set_dust():
    cv.create_image((400,400),image=img)


def set_bg_img(*args):
    # img = PhotoImage(file=map_bmp[map_index.get()])
    cv.create_image((400,400), image=img[map_index.get()])

root = Tk()
root.title("Enjoy Your Road!")
root.geometry("800x600+200+200")


img = [PhotoImage(file=path) for path in map_pic]

cv = Canvas(root, bg="white", width="800", height = 600)
cv.create_image((400,400), image=img[0])
cv.place(x=0,y=0)

map_index = IntVar()
map_index.trace("w", set_bg_img)
R_ONE=Radiobutton(root, text="Dust Scene", variable=map_index, value=1).place(x=100,y=480)
R_TWO=Radiobutton(root, text="More Maps waiting to be developed...", variable=map_index, value=2, state="disabled").place(x=250,y=480)


button_run = Button(root, text="Run!", command = run_main)
button_run.place(x=100, y=550)
button_exit = Button(root, text="Exit",command = exit )
button_exit.place(x=700, y=550)
# button_exit.pack()

root.mainloop()