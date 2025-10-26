import customtkinter as ctk
import socket

HOST = '127.0.0.1' 
PORT = 6769 

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
s.connect((HOST, PORT))

ctk.set_appearance_mode("dark")
ctk.set_default_color_theme("blue")

app = ctk.CTk()
app.title("HNVoidHeart Trainer")
app.geometry("480x300")
app.resizable(False, False)

frame = ctk.CTkFrame(app)
frame.pack(padx=16, pady=16, fill="both", expand=True)

title = ctk.CTkLabel(
    frame, text="HNVoidHeart Trainer",
    font=("Montserrat ExtraBold", 20)
)
title.pack(pady=(0, 15))

status_var = ctk.StringVar(value="Ready.")
status_lbl = ctk.CTkLabel(
    frame, textvariable=status_var,
    font=("Montserrat", 12),
    text_color="lightgreen"
)
status_lbl.pack(pady=(0, 12))

def set_status(msg: str):
    status_var.set(msg)
    if "Success" in msg or "Changed" in msg:
        status_lbl.configure(text_color="lightgreen")
    elif "Error" in msg or "Invalid" in msg:
        status_lbl.configure(text_color="red")
    else:
        status_lbl.configure(text_color="orange")

def change_health():
    value = health_entry.get()
    if not value.isdigit():
        set_status("Error: invalid health value.")
        return
    s.send(f"1 {value}".encode('ascii'))

    set_status(f"✅ Health changed to {value} (stub)")

def change_geo():
    value = geo_entry.get()
    if not value.isdigit():
        set_status("Error: invalid Geo value.")
        return
    s.send(f"2 {value}".encode('ascii'))

    set_status(f"✅ Geo changed to {value} (stub)")

health_label = ctk.CTkLabel(frame, text="Health", font=("Montserrat SemiBold", 14))
health_label.pack(anchor="w", pady=(10, 0))

health_frame = ctk.CTkFrame(frame, fg_color="transparent")
health_frame.pack(fill="x", pady=6)

health_entry = ctk.CTkEntry(health_frame, placeholder_text="Enter new health value...")
health_entry.pack(side="left", fill="x", expand=True, padx=(0, 8))

ctk.CTkButton(health_frame, text="Apply", width=80, command=change_health).pack(side="right")

geo_label = ctk.CTkLabel(frame, text="Geo", font=("Montserrat SemiBold", 14))
geo_label.pack(anchor="w", pady=(12, 0))

geo_frame = ctk.CTkFrame(frame, fg_color="transparent")
geo_frame.pack(fill="x", pady=6)

geo_entry = ctk.CTkEntry(geo_frame, placeholder_text="Enter new Geo value...")
geo_entry.pack(side="left", fill="x", expand=True, padx=(0, 8))

ctk.CTkButton(geo_frame, text="Apply", width=80, command=change_geo).pack(side="right")

ctk.CTkLabel(
    frame,
    text="Tip: Enter numeric values and press Apply.",
    font=("Montserrat", 10),
    text_color="#9aa"
).pack(side="bottom", pady=8)


app.mainloop()

