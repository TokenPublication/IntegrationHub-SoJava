/**
 * @file Main.java
 * @brief Entry point for the TokenX Connect (Wired) sample application
 * 
 * This class provides the main entry point for the TokenX Connect (Wired) sample application.
 * It demonstrates how to initialize and use the IntegrationHub class to communicate with
 * Token Financial Technologies X30TR and 300TR POS devices. The sample showcases connection
 * establishment, callback registration, and various transaction operations.
 * 
 * Platform: Java sample application for TokenX Connect SDK on MacOS/Linux
 */
package com.sdksotestjava;

import java.util.Scanner;

public class Main {
    /**
     * Main entry point for the application
     * Demonstrates a complete workflow for POS device integration
     * 
     * @param args Command line arguments (not used)
     */
    public static void main(String[] args) throws InterruptedException {
        IntegrationHub communication = new IntegrationHub();
        communication.createCommunication("TokenLinuxTestJava");

        communication.setSerialInCallback();
        communication.setDeviceStateCallback();

        try {
            Thread.sleep(3000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        Scanner scanner = new Scanner(System.in);
        
        while (true) {
            System.out.println("Press [0-3] to execute the actions below");
            System.out.println("0: Get Active Device");
            System.out.println("1: Send Example Basket");
            System.out.println("2: Send Example Payment");
            System.out.println("3: Get Fiscal Info");

            int input = scanner.nextInt();
            int activeDevice = communication.getActiveDeviceIndex();

            switch (input) {
                case 0: // Get Active Device
                    System.out.println("Active Device Index: " + activeDevice);
                    break;
                case 1: // Send Example Basket
                    IntegrationHubTest.sendBasketTest(communication, activeDevice);
                    break;
                case 2: // Send Example Payment (300TR Only)
                    IntegrationHubTest.sendPaymentTest(communication, activeDevice);
                    break;
                case 3: // Get Fiscal Info
                    IntegrationHubTest.getFiscalInfoTest(communication);
                    break;
                default:
                    System.out.println("Invalid input. Try again.");
                    break;
            }

            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
