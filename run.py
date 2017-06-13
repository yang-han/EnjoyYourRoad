from tkinter import *
import subprocess
import platform

map_argument = [
    "",
    "dust",
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
    print("sdasdddddddddddddddddddddddddddddddd", map_index.get())
    print("sdasdddddddddddddddddddddddddddddddd", map_index.get())
    main_path = get_main_path()
    subprocess.call([main_path, map_argument[map_index.get()]])



root = Tk()
root.title("Enjoy Your Road!")
root.geometry("800x600+200+200")

label = Label(root, bg="red", width=800, height=600, anchor="center").pack()

button_run = Button(root, text="Run!", command = run_main)
button_run.place(x=100, y=550)
button_exit = Button(root, text="Exit",command = exit )
button_exit.place(x=700, y=550)
# button_exit.pack()

map_index = IntVar()
R_ONE=Radiobutton(root, text="Dust Scene", variable=map_index, value=1).place(x=100,y=480)
R_TWO=Radiobutton(root, text="More Maps waiting to be developed...", variable=map_index, value=2, state="disabled").place(x=250,y=480)

root.mainloop()