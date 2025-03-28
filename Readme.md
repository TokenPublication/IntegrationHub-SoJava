
# **IntegrationHub Test Project - Build & Run Guide**  

## **📌 Prerequisites**  
Before building and running the project, ensure you have the necessary dependencies installed and your environment configured.  

---

## **⚙️ Installing Necessary Libraries**

```sh
sudo apt install g++
sudo apt install zlib1g-dev
sudo apt install libusb-1.0-0-dev
sudo apt install libssl-dev
wget https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
sudo mkdir /usr/include/nlohmann/
sudo mv json.hpp /usr/include/nlohmann/
```
---

## **⚙️ Installing Drivers for 300TR**

```sh
wget https://ftdichip.com/wp-content/uploads/2025/02/libftd2xx-linux-x86_64-1.4.32.tgz
tar -xvf libftd2xx-linux-x86_64-1.4.32.tgz
download_dir="$(pwd)/linux-x86_64"
cd linux-x86_64
sudo cp lib* /usr/local/lib
cd /usr/local/lib
sudo ln -s libftd2xx.so libftd2xx.so.1.4.32
sudo chmod 0755 libftd2xx.so.1.4.32
sudo rmmod ftdi_sio
sudo rmmod usbserial
cd "$download_dir"
sudo cp ftd2xx.h /usr/local/include
sudo cp WinTypes.h /usr/local/include
sudo ldconfig -v
```

- After the installation you may delete libftd2xx-linux-x86_64-1.4.32.tgz file and linux-x86_64 folder.

---

## **⚙️ Installing Drivers for X30TR**

Most of the linux distributions already have libusb drivers as default. You don't need further driver installation for X30TR device connection.

---

## **⚙️ Setting Up USB Connection Permissions**

This is needed for data transferring between devices. Also, this is the last part of initial installation process for linux computer. You should restart your computer after this step.

```sh
sudo usermod -aG plugdev $USER
echo -e "SUBSYSTEM==\"usb\", ATTR{idVendor}==\"18d1\", MODE=\"0666\"\nKERNEL==\"ttyUSB*\", SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"0403\", ATTRS{idProduct}==\"6001\", MODE=\"0666\", GROUP=\"plugdev\"" | sudo tee -a /etc/udev/rules.d/99-usb.rules
sudo udevadm control --reload-rules
sudo systemctl restart systemd-udevd
```

---

## **⚙️ Setting Up the Environment**  

Before compiling, add the path of the `libIntegrationHub.so` file to `LD_LIBRARY_PATH`:  

```sh
export LD_LIBRARY_PATH=$(pwd)/src/main/resources:$LD_LIBRARY_PATH
```

This ensures that the shared library can be found at runtime.

---

## **🚀 Building the Project**  

To compile the project, simply run:  

```sh
make
```

This will:  
✅ Compile the Java and JNI code  
✅ Generate the necessary JNI headers  
✅ Build the native shared library  

---

## **▶️ Running the Project**  

Once built, execute the project using:  

```sh
make run
```

This will launch the Java application with the correct library path.

---

## **🔄 Cleaning the Build**  

To remove compiled files and reset the project, run:  

```sh
make clean
```

---

## **📢 Notes**  

- Ensure you have a compatible **Java Development Kit (JDK)** installed.
- Ensure you have JAVA_HOME environment variable is defined.
- If encountering shared library errors, double-check the `LD_LIBRARY_PATH` variable.  
- If making changes to native code, **rebuild** using `make clean` followed by `make`.  
