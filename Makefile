# Compiler and flags
CC = gcc
CFLAGS = -shared -fPIC -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux

# Paths
JAVA_SRC = \
	src/main/java/com/sdksotestjava/Main.java \
	src/main/java/com/sdksotestjava/IntegrationHub.java \
	src/main/java/com/sdksotestjava/IntegrationHubTest.java
JNI_SRC = src/main/jni/IntegrationHubJNI.cpp
JNI_HEADER = src/main/jni/com_sdksotestjava_IntegrationHub.h
LIB_NATIVE = src/main/resources/libIntegrationHubJNI.so
LIB_EXTERNAL = src/main/resources/libIntegrationHub.so
BUILD_DIR = build
CLASS_DIR = $(BUILD_DIR)/classes

# Target: Compile Java and Generate JNI Header
$(JNI_HEADER): $(JAVA_SRC)
	mkdir -p $(BUILD_DIR) $(CLASS_DIR)
	javac -h src/main/jni -d $(CLASS_DIR) $(JAVA_SRC)

# Target: Compile Native C Library and Link External .so
$(LIB_NATIVE): $(JNI_SRC) $(JNI_HEADER) $(LIB_EXTERNAL)
	$(CC) $(CFLAGS) -o $(LIB_NATIVE) $(JNI_SRC) -Lsrc/main/resources -lIntegrationHub

# Target: Run Java program
run: $(LIB_NATIVE)
	java -Djava.library.path=src/main/resources -cp $(CLASS_DIR) com.sdksotestjava.Main

# Clean build
clean:
	rm -rf $(BUILD_DIR) $(JNI_HEADER) $(LIB_NATIVE)

