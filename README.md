[README.md](https://github.com/user-attachments/files/23398537/README.md)
# 🔗 Network File Sharing System (C++ - Linux)

A simple **client-server based file sharing system** built in **C++** using **TCP sockets**.  
This project simulates a distributed network where files can be uploaded and downloaded between clients and a shared database on the server.

## 🧩 Features

✅ Establishes TCP connection between client and server  
✅ Lists all available files in the **Database**  
✅ Uploads files from **Local** → **Database**  
✅ Downloads files from **Database** → **Downloads**  
✅ Maintains consistent file organization on both sides  
✅ Clear and interactive menu interface  

---

## 🗂️ Folder Structure

```
NetworkFileSharing/
├── server.cpp
├── client.cpp
├── Database/      # Central repository for all shared files
├── Local/         # User’s local folder for uploading
├── Downloads/     # Files downloaded from Database
```

---

## 🧱 Requirements

- **OS:** Linux / WSL (for socket headers like `<sys/socket.h>`)
- **Compiler:** g++ (C++11 or newer)
- **Networking:** TCP/IP stack enabled

---

## ⚙️ Compilation Instructions

Open two terminals — one for the **server** and one for the **client**.

### 🖥️ Compile:
```bash
g++ server.cpp -o server
g++ client.cpp -o client
```

### 🚀 Run Server:
```bash
./server
```

### 💻 Run Client:
```bash
./client
```

---

## 🧭 Menu Options

When you run the client, you’ll see:

```
==== File Sharing Menu ====
1. List files in Database
2. Download file (to Downloads)
3. Upload file (from Local)
4. Exit
```

### 1️⃣ List Files in Database
Displays all files stored in the server’s **Database** directory.

### 2️⃣ Download File
Downloads a selected file from the **Database** (server) to the client’s **Downloads** folder.

### 3️⃣ Upload File
Uploads a selected file from the client’s **Local** folder to the server’s **Database** folder.

### 4️⃣ Exit
Closes the connection gracefully.

---

## 🧠 Example Flow

1. **Run server**  
   Server waits for a client connection.

2. **Run client**  
   Connects to the server and displays the menu.

3. Choose options like:
   - Upload `test.txt` from `Local` to `Database`
   - List files in the database
   - Download any file to `Downloads`

4. **Check folders:**
   - Uploaded files appear in `Database/`
   - Downloaded files appear in `Downloads/`

---

## 🧰 Future Enhancements

- User authentication (username/password)
- File metadata tracking (file size, uploader, timestamp)
- Concurrent connections for multiple clients
- File version control and logging

---

## 🧑‍💻 Author

**Anku Kumar**   
📧 Email: *ankukr2444777@gmail.com*

---

## 📜 License

This project is open-source and available under the **MIT License**.
