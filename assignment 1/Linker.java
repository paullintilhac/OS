/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package linker;

/**
 *
 * @author paul
 */

import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

/**
 *
 * @author paul
 */

public class Linker {
    public static Matcher matcher=null;
    public static ArrayList<String> defKeys = new ArrayList<>();
    public static ArrayList<String> defValues = new ArrayList<>();
    public static ArrayList<String> useList = new ArrayList<>();
    public static ArrayList<String> useErrors = new ArrayList<>();
    public static ArrayList<String> instructList= new ArrayList<>();
    public static ArrayList<String> instructErrors = new ArrayList<>();
    public static ArrayList<String> defErrors = new ArrayList<>();
    public static ArrayList<String> warnings = new ArrayList<>();
    public static int passNumber =1;
    public static int offsetCounter;
    public static int oldOffsetCounter;
    public static int endOffset;
    public static int bolIndex;
    ArrayList<Module> modules;
    public static BufferedReader dataReader = null;
    public static int realLineNumber=1;
    public static int oldLineNumber=1;
    public static int totalInstructions=0;
    public static int eofIndex;

    /**
    * @param args the command line arguments
    */

    public static void main(String[] args) {
        int ch = -1;
        offsetCounter = 1;
        oldOffsetCounter=1;
        endOffset = 1;
        String wholeFile="";
        try {

            int moduleNumber = 1;
            String filename = args[0];
            //System.out.println("filename: " + filename);
            dataReader = new BufferedReader(new FileReader(filename));

            while (dataReader.ready()) {
                wholeFile += (char) dataReader.read();
            }

            eofIndex = wholeFile.length();
            matcher = Pattern.compile("\\S+|(?m)$").matcher(wholeFile);
            ArrayList<Module> modules= new ArrayList<Module>();
            
            int baseAddress=0;
            //begin first pass
         do {
            Module m = new Module(matcher,baseAddress,moduleNumber);
            //System.out.println("module created");
            moduleNumber++;
            modules.add(m);
            baseAddress+=m.numInstructs;
            if (matcher.end()+1 == eofIndex){
                break;
            }
         } while (true);
         
         // print symbol table
         
         System.out.println("Symbol Table");
         for (int i = 0 ; i<defKeys.size();i++){
             System.out.println(defKeys.get(i) + "="+defValues.get(i) + "  " + defErrors.get(i));
         }
         dataReader.close();
         passNumber = 2;

         // begin second pass
         matcher = Pattern.compile("\\S+|(?m)$").matcher(wholeFile);
         //System.out.println("pass two");

         for (int i=0;i<modules.size();i++){
             Module m = modules.get(i);
             m.readDefLine(matcher);
             m.readUseLine(matcher);
             m.readInstructLine(matcher);
         }

         //print out memory map
         
         System.out.println("");
         System.out.println("Memory Map");
         int  count = 0;
         for (int i =0; i<instructList.size();i++){
             String formatted = String.format("%03d", count);
             //System.out.println("first character: "+instructList.get(i).substring(0,1));
             if (instructList.get(i).substring(0,1).contentEquals("W")){
                //System.out.println("encountered warning");
                System.out.println(instructList.get(i));
             } else{
                count++;
                System.out.println(formatted + ": " + instructList.get(i) + " " + instructErrors.get(i));
             }
         }
     
             System.out.println("");
             for (int i=0;i<warnings.size();i++){
                 System.out.println(warnings.get(i));
             }
         
       
        } catch (IOException ioe) {
            System.out.println("message: " + ioe.getMessage());
        }
        

    }
   
    
}