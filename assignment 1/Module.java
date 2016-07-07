/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package linker;
import java.util.ArrayList;
import java.util.HashMap;
import java.io.*;
import static linker.Linker.defKeys;
import static linker.Linker.realLineNumber;
import static linker.Linker.oldLineNumber;
import static linker.Linker.matcher;
import static linker.Linker.useList;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
/**
 *
 * @author paul
 */
public class Module {
    
    private int numUses;
    private int numDefs;
    public int numInstructs;
    public int moduleNumber;
    public ArrayList<String> extraDefs = new ArrayList<>();
    public ArrayList<String> useList = new ArrayList<>();
    public ArrayList<String> defKeys = new ArrayList<>();
    public ArrayList<Integer> instructList = new ArrayList<>();
    private final int base_address;
        public Module(Matcher m, int baseAddress,int moduleNumber) throws IOException{
        this.base_address = baseAddress;
        this.moduleNumber = moduleNumber;

        readDefLine(m);
        //System.out.println("done reading def line");
        readUseLine(m);
        //System.out.println("done reading use line");
        readInstructLine(m);

        if (Linker.passNumber == 1){
            for (int i=0; i<this.numDefs;i++){
                String symbolName = Linker.defKeys.get(i);
                int symbolValue = Integer.parseInt(Linker.defValues.get(i))-this.base_address;
                if (symbolValue>=this.numInstructs){
                System.out.println("Warning: Module " + moduleNumber + ": " + symbolName+" to big " +symbolValue+ " (max="+(this.numInstructs-1)+") assume zero relative");
                Linker.defValues.set(i,Integer.toString(this.base_address));
                }   
            }
        }
        
                //end parsing instruction string
        
        }
        public void readDefLine(Matcher m) throws IOException {
        
        String token = nextTokenSpec(m);
        //System.out.println("numDefs token: " + token);
        if (isNumber(token)){
            this.numDefs = Integer.parseInt(token);
        } else{
            System.out.println("Parse Error line " + Linker.oldLineNumber +" offset " + Linker.offsetCounter + ": NUM_EXPECTED");
            System.exit(1);
            }
        if ((int) Integer.parseInt(token)>16){
            System.out.println("Parse Error line " + Linker.realLineNumber +" offset " + Linker.offsetCounter + ": TO_MANY_DEF_IN_MODULE");
            System.exit(1);
        }
            for (int i=0; i<numDefs;i++){
                readDef(m);
            }
        }
        
        private void readDef(Matcher m) throws IOException{
            String token = nextTokenSpec(m);
            String symbolName = "";
            int symbolValue = -1;

            if (isAlpha(token)){
                symbolName = token;
                if (symbolName.length()>16){
                    System.out.println("Parse Error line  "+Linker.realLineNumber + " offset " + Linker.offsetCounter + ": SYM_TOLONG"  );
                }
            } else {
                System.out.println("Parse Error line " + Linker.oldLineNumber +" offset " + Linker.offsetCounter+ ": SYM_EXPECTED");
                System.exit(1);
            }
            token  = nextTokenSpec(m);

            if (Linker.passNumber ==1){
            if (isNumber(token)){
                symbolValue = Integer.parseInt(token); 
            } else {
                System.out.println("Parse Error line " + Linker.oldLineNumber +" offset " + Linker.offsetCounter + ": NUM_EXPECTED");
                System.exit(1);
            }
            if (Linker.defKeys.contains(symbolName)){
                int ind = Linker.defKeys.indexOf(symbolName);
                Linker.defErrors.set(ind, "Error: This variable is multiple times defined; first value used");
            }
            else{
            Linker.defKeys.add(symbolName);
            Linker.defValues.add( Integer.toString(symbolValue+this.base_address));
            Linker.defErrors.add("");
            }
            this.defKeys.add(symbolName);

            }
            if (Linker.passNumber == 2){
             if (!Linker.useList.contains(symbolName)){              
                  Linker.warnings.add("Warning: Module " + moduleNumber + ": " + symbolName+" was defined but never used");
             }
            }
            }
        
 public void readUseLine(Matcher m) throws IOException {
         
        String token = nextTokenSpec(m);
        if (isNumber(token)){
            this.numUses = Integer.parseInt(token);
        } else{
            System.out.println("Parse Error line " + Linker.oldLineNumber +" offset " + Linker.offsetCounter + ": NUM_EXPECTED");
            System.exit(1);
        }
        if (this.numUses>16){
            System.out.println("Parse Error line " + Linker.realLineNumber +" offset " + Linker.offsetCounter + ": TO_MANY_USE_IN_MODULE");
            System.exit(1);
        }
            for (int i=0; i<numUses;i++){
                readUse(m);
            }
        }
        
        private void readUse(Matcher m) throws IOException{
            String token = nextTokenSpec(m);
            String symbolName = "";
            if (Linker.passNumber == 1){
                if (isAlpha(token)){
                symbolName = token;
            } else{
                System.out.println("Parse Error line " + Linker.realLineNumber +" offset " + Linker.offsetCounter + ": SYM_EXPECTED");
                System.exit(1);
            }
            useList.add(symbolName);
            if (!Linker.useList.contains(symbolName)){
            Linker.useList.add(symbolName);
            }
            }
            
            }
        
 public void readInstructLine(Matcher m) throws IOException {
         
        String token = nextTokenSpec(m);
        if (isNumber(token)){
            this.numInstructs = Integer.parseInt(token);
            Linker.totalInstructions+=this.numInstructs;
        } else{
            System.out.println("Parse Error line " + Linker.oldLineNumber +" offset " + Linker.offsetCounter + ": NUM_EXPECTED");
            System.exit(1);
        }
        if (Linker.totalInstructions>=512){
            System.out.println("Parse Error line "+Linker.realLineNumber + " offset " + Linker.offsetCounter + ": TO_MANY_INSTR"  );
            System.exit(1);
        }
            for (int i=0; i<numInstructs;i++){
                readInstruct(m);
            }
            
              for (int i=0; i<numUses;i++){
                if (!this.instructList.contains(i)&&Linker.passNumber==2){
                    Linker.instructList.add("Warning: Module " + moduleNumber + ": " + useList.get(i)+" appeared in the uselist but was not actually used");
                    Linker.instructErrors.add("");
                }
            }
        }
     
        private void readInstruct(Matcher m) throws IOException{
            String instructType = "";
            int instructCode = -1;
            String token = nextTokenSpec(m);

            if  (isAlpha(token) &&
                    (token.contentEquals("A")|
                    token.contentEquals("I")|
                    token.contentEquals("E")|
                    token.contentEquals("R"))){
                instructType = token;
            } else{
                System.out.println("Parse Error line " + Linker.oldLineNumber +" offset " + Linker.offsetCounter +": ADDR_EXPECTED");
                System.exit(1);
                  }
            token = nextTokenSpec(m);
            if (isNumber(token) ){
                instructCode = Integer.parseInt(token);
            } else{
                System.out.println("Parse Error line " + Linker.oldLineNumber +" offset " + Linker.offsetCounter + ": ADDR_EXPECTED");
                System.exit(1);
            }

            if (instructCode>9999 && Linker.passNumber == 1){
                if (!instructType.contentEquals("I")){
                    Linker.instructList.add(Integer.toString(9999));
                    Linker.instructErrors.add("Error: Illegal opcode; treated as 9999");
                }
                else{
                    Linker.instructList.add(Integer.toString(9999));
                    Linker.instructErrors.add("Error: Illegal immediate value; treated as 9999");
                }
            }
            if (Linker.passNumber == 2&&instructCode<=9999){
          
               
            if (instructType.contentEquals("I")){
                Linker.instructList.add(Integer.toString(instructCode));
                Linker.instructErrors.add("");
            }
            if (instructType.contentEquals("A")){
                int operand  = Integer.parseInt(Integer.toString(instructCode).substring(1));
                int baseOperand = instructCode-operand;
                if (operand<=512){
                Linker.instructList.add(Integer.toString(instructCode));
                Linker.instructErrors.add("");
                }
                else{
                Linker.instructList.add(Integer.toString(baseOperand));
                Linker.instructErrors.add("Error: Absolute address exceeds machine size; zero used");
                }
            }
            if (instructType.contentEquals("R")){
                int operand  = Integer.parseInt(Integer.toString(instructCode).substring(1));
                int baseOperand = instructCode-operand;
                //System.out.println("instructCode "+ instructCode+", operand: "+ operand);
                if (operand<=this.numInstructs){ 
                Linker.instructList.add(Integer.toString(instructCode + this.base_address));
                Linker.instructErrors.add("");
                }
                else{
                Linker.instructList.add(Integer.toString(baseOperand));
                Linker.instructErrors.add("Error: Relative address exceeds module size; zero used");
                }
            }
            if (instructType.contentEquals("E")){
                int operand  = Integer.parseInt(Integer.toString(instructCode).substring(1));
                int baseOperand = instructCode-operand;
                if (operand<numUses){
                String sym= this.useList.get(operand);

                if (!this.defKeys.contains(sym)&&!this.extraDefs.contains(sym)){
                this.extraDefs.add(sym);
                }

                int ind = Linker.defKeys.indexOf(sym);
                this.instructList.add(operand);

                if (ind!=-1){
                Linker.instructList.add(Integer.toString(Integer.parseInt(Linker.defValues.get(ind))+(baseOperand)));
                Linker.instructErrors.add("");
                }
                else{
                Linker.instructList.add(Integer.toString(baseOperand));
                Linker.instructErrors.add("Error: " + sym + " is not defined; zero used");
                }
                }
                
                else{
                Linker.instructList.add(Integer.toString(instructCode));
                Linker.instructErrors.add("Error: External address exceeds length of uselist; treated as immediate");
                }

            }
            }
            
        }
        public static String nextTokenSpec(Matcher m) throws IOException{
            boolean flag = false;
            m.find();
            String token = m.group();
            System.out.println("token: *"+token+ "*, start: "+m.start()+ ", end: " + m.end() + ", line: " + Linker.realLineNumber + ", bol index: "+Linker.bolIndex);

            //System.out.println("token in nextTokenSpec: " + token);
            Linker.oldOffsetCounter = Linker.offsetCounter;
            Linker.oldLineNumber = Linker.realLineNumber;
            if (isEOL(token)){
                Linker.realLineNumber++;
            }
            int count=0;

            while (isEOL(token)){

                if (m.end() == Linker.eofIndex){
                    break;
                }

                count++;
                flag = true;
                if (count>1){
                Linker.realLineNumber++;
            }
                m.find();
                if (flag == true){
                    Linker.bolIndex = m.start();
                }

                token = m.group();

                System.out.println("token: *"+token+ "*, start: "+m.start()+ ", end: " + m.end()+ ", line: " + Linker.realLineNumber+ ", bol index: "+Linker.bolIndex);
            }
            System.out.println("mstart: " + m.start() + ", bolIndex: " + Linker.bolIndex);
            Linker.offsetCounter  = m.start()-Linker.bolIndex+1;
             
                //System.out.println("new offset: " + Linker.offsetCounter);
                
            
            //System.out.println("true token: " + st.toString());
            return token;
        }
        public static boolean isNumber(String str){
            boolean result=str.matches("^-?\\d+$");
            return(result);
        }
        public static boolean isEOL(String str){
            boolean result = false;
            if (str.trim().length()==0){
                result = true;
            } else {result =false;}
            return(result);
        }
        public static boolean isAlpha(String str){
            return(!isNumber(str)&&!isEOL(str));
        }

} 