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

/**
 *
 * @author paul
 */
public class Linker {
    public static StreamTokenizer st = null;
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
    ArrayList<Module> modules= new ArrayList<Module>();
    public static BufferedReader dataReader = null;
    public static int realLineNumber;
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        int ch = -1;
        offsetCounter = 0;
        try {
            int moduleNumber = 1;
            String filename =args[0];
            //System.out.println("filename: " + filename);
            dataReader = new BufferedReader(new FileReader(filename));
            st = new StreamTokenizer(dataReader);
            
            ArrayList<Module> modules= new ArrayList<Module>();
           
            int baseAddress= 0;
            st.eolIsSignificant(true);

            //begin first pass

         do {
            Module m = new Module(st,baseAddress,moduleNumber);
            //System.out.println("module created");
            moduleNumber++;
            modules.add(m);
            baseAddress+=m.numInstructs;

            Module.nextTokenSpec(st);
            st.pushBack();
            

         } while (st.ttype!=StreamTokenizer.TT_EOF);
         
         // print symbol table
         
         System.out.println("Symbol Table");
         for (int i = 0 ; i<defKeys.size();i++){
             System.out.println(defKeys.get(i) + "="+defValues.get(i) + "  " + defErrors.get(i));
         }
         dataReader.close();
         passNumber = 2;

         // begin second pass
         dataReader = new BufferedReader(new FileReader(filename));
         st = new StreamTokenizer(dataReader);
         //System.out.println("pass two");
         for (int i=0;i<modules.size();i++){
             Module m = modules.get(i);
             m.readDefLine(st);
             m.readUseLine(st);
             m.readInstructLine(st);
         }

         //print out memory map
         
         System.out.println("");
         System.out.println("Memory Map");
         for (int i =0; i<instructList.size();i++){
             String formatted = String.format("%03d", i);
             System.out.println(formatted + ": " + instructList.get(i) + " " + instructErrors.get(i));
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