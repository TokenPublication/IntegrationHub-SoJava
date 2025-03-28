
# **IntegrationHub Test Project - Build & Run Guide**  

## **📌 Prerequisites**  
Before building and running the project, ensure you have the necessary dependencies installed and your environment configured.  

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
