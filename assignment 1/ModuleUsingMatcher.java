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
import static linker.Linker.st;
import static linker.Linker.useList;

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
        readUseLine(m);
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
        
        int token = nextTokenSpec(m);

        if (isNumber(token)){
            this.numDefs = Integer.parseInt(token);
        } else{
            System.out.println("Parse Error line " + Linker.realLineNumber +" offset " + Linker.oldOffsetCounter + ": NUM_EXPECTED");
            System.exit(1);
            }
        if ((int) Integer.parseInt(token)>16){
            System.out.println("Parse Error line " + Linker.realLineNumber +" offset " + Linker.oldOffsetCounter + ": TO_MANY_DEF_IN_MODULE");
            System.exit(1);
        }
            for (int i=0; i<numDefs;i++){
                readDef(m);
            }
        }
        
        private void readDef(Matcher m) throws IOException{
            int token = nextTokenSpec(m);
            String symbolName = "";
            int symbolValue = -1;
            if (!isNumber(token)){
                symbolName = token;
                if (symbolName.length()>16){
                    System.out.println("Parse Error line  "+Linker.realLineNumber + " offset " + Linker.oldOffsetCounter + ": SYM_TOLONG"  );
                }
            } else{
                System.out.println("Parse Error line " + Linker.realLineNumber +" offset " + Linker.oldOffsetCounter+ ": SYM_EXPECTED");
                System.exit(1);
            }
            token  = nextTokenSpec(m);
            if (Linker.passNumber ==1){
            if (isNumber(token)){
                symbolValue = Integer.parseInt(token); 
            } else {
                System.out.println("Parse Error line " + Linker.realLineNumber +" offset " + Linker.oldOffsetCounter + ": NUM_EXPECTED");
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
         
        int token = nextTokenSpec(m);
        if (isNumber(token)){
            this.numUses = Integer.parseInt(token);
        } else{
            System.out.println("Parse Error line " + Linker.realLineNumber +" offset " + Linker.oldOffsetCounter + ": NUM_EXPECTED");
            System.exit(1);
        }
        if (this.numUses>16){
            System.out.println("Parse Error line " + Linker.realLineNumber +" offset " + Linker.oldOffsetCounter + ": TO_MANY_USE_IN_MODULE");
            System.exit(1);
        }
            for (int i=0; i<numUses;i++){
                readUse(m);
            }
        }
        
        private void readUse(Matcher m) throws IOException{
            int token = nextTokenSpec(st);
            String symbolName = "";
            if (Linker.passNumber == 1){
                if (token==StreamTokenizer.TT_WORD){
                symbolName = st.sval;
            } else{
                System.out.println("Parse Error line " + Linker.realLineNumber +" offset " + Linker.oldOffsetCounter + ": SYM_EXPECTED");
                System.exit(1);
            }
            useList.add(symbolName);
            if (!Linker.useList.contains(symbolName)){
            Linker.useList.add(symbolName);
            }
            }
            
            }
        
 public void readInstructLine(StreamTokenizer st) throws IOException {
         
        int token = nextTokenSpec(st);
        if (token==StreamTokenizer.TT_NUMBER){
            this.numInstructs = (int) st.nval;
            Linker.totalInstructions+=this.numInstructs;
        } else{
            System.out.println("Parse Error line " + Linker.realLineNumber +" offset " + Linker.oldOffsetCounter + ": NUM_EXPECTED");
            System.exit(1);
        }
        if (Linker.totalInstructions>=512){
            System.out.println("Parse Error line  "+Linker.realLineNumber + " offset " + Linker.oldOffsetCounter + ": TO_MANY_INSTR"  );
            System.exit(1);
        }
            for (int i=0; i<numInstructs;i++){
                readInstruct(st);
            }
            
              for (int i=0; i<numUses;i++){
                if (!this.instructList.contains(i)&&Linker.passNumber==2){
                    Linker.instructList.add("Warning: Module " + moduleNumber + ": " + useList.get(i)+" appeared in the uselist but was not actually used");
                    Linker.instructErrors.add("");
                }
            }
        }
     
        private void readInstruct(StreamTokenizer st) throws IOException{
            String instructType = "";
            int instructCode = -1;
            int token = nextTokenSpec(st);

            if  (token==StreamTokenizer.TT_WORD &&
                    (st.sval.contentEquals("A")|
                    st.sval.contentEquals("I")|
                    st.sval.contentEquals("E")|
                    st.sval.contentEquals("R"))){
                instructType = st.sval;
            } else{
                System.out.println("Parse Error line " + Linker.realLineNumber +" offset " + Linker.oldOffsetCounter +": ADDR_EXPECTED");
                System.exit(1);
                  }
            token = nextTokenSpec(st);
            if (token==StreamTokenizer.TT_NUMBER ){
                instructCode = (int) st.nval;
            } else{
                System.out.println("Parse Error line " + Linker.realLineNumber +" offset " + Linker.oldOffsetCounter + ": ADDR_EXPECTED");
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
        public static int nextTokenSpec(StreamTokenizer st) throws IOException{
            Linker.realLineNumber = st.lineno();
            Linker.oldOffsetCounter = Linker.offsetCounter;
            int token = st.nextToken();
            int count=0;
            while (token == StreamTokenizer.TT_EOL){
                count++;
                Linker.offsetCounter = 1;
                if (count>1){
                Linker.realLineNumber++;}
                token = st.nextToken();
            }
                if (token == StreamTokenizer.TT_WORD){
                Linker.offsetCounter += st.sval.length()+1;}
                if (token == StreamTokenizer.TT_NUMBER){
                Linker.offsetCounter += Integer.toString((int) st.nval).length()+1;
                }
            
            //System.out.println("true token: " + st.toString());
            return token;
        }
        public static boolean isNumber(String str){
            boolean result=str.matches("^-?\\d+$");
            return(result);
        }
} 